/*! \file Data.hxx
 *  \brief Defines generic wrappers for user-defined data classes.
 */
#ifndef __DATA_HXX__
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include "Hist.hxx"



class UserDataABC
{
public:
  const std::string fName;
  const std::string fClassName;
  Bool_t kCreatePointer;

  typedef std::map<std::string, UserDataABC*> Map_t;
  typedef std::map<std::string, UserDataABC*>::iterator MapIterator_t;
  static Map_t Map;

  virtual TBranch* AddBranch() = 0;
  virtual ~UserDataABC() { }

  static void AddBranches() {
    MapIterator_t it = Map.begin();
    while(it != UserDataABC::Map.end())
      (*it++).second->AddBranch();
  }

  static void CreatePointers() {
    std::vector<std::string> vPrint;
    vPrint.push_back("\nCreating pointers to user data objects:\n");
    MapIterator_t it = Map.begin();
    while(it != Map.end()) {
      if(!it->second->kCreatePointer) {
	++it; continue;
      }
      std::string name = it->first;
      std::string className = it->second->fClassName;
      std::stringstream sPrint, sExecute;
      sPrint << "        " << className << "* " << name << std::endl;
      vPrint.push_back(sPrint.str());
      sExecute << className << "* " << name << " = "
	       << "UserData<" << className << ">::Get "
	       << "(\"" << name << "\")";
      gROOT->ProcessLine(sExecute.str().c_str());
      ++it;
    }
    if(vPrint.size() > 1) {
      for(UInt_t u=0; u< vPrint.size(); ++u)
	std::cout << vPrint[u];
      std::cout << std::endl << std::endl;
    }
  }

protected:
  UserDataABC(const char* name, const char* class_name,
	      Bool_t createPointer = kFALSE) :
    fName(name), fClassName(class_name),
    kCreatePointer(createPointer) { };

};



template <typename T>
class UserData : public UserDataABC
{
protected:
  T* pData;

public:
  UserData(const char* name, const char* class_name, T& data) :
    UserDataABC(name, class_name) {
    pData = &data;
    UserDataABC::Map[fName] = this;
  };

  TBranch* AddBranch() {
    return rb::Hist::CreateBranch(fName.c_str(), fClassName.c_str(), (void**)&pData);
  }

  T* Get() {
    return pData;
  }

  static T* Get(const char* name) {
    return static_cast<UserData<T>* >(Map[name])->Get();
  }

  ~UserData() { }

};


#endif
