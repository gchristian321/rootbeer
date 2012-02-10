//! \file Data.cxx
//! \brief Implements Data.hxx
#ifndef IMPLEMENT_DATA_TEMPLATES
#include <algorithm>
#include <TROOT.h>
#include <TClass.h>
#include <TStreamerInfo.h>
#include <TStreamerElement.h>
#include <TTreeFormula.h>
#include "Data.hxx"
using namespace std;



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Helper Functions                                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void_pointer_add                                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Add an offset to a void* pointer
void void_pointer_add(volatile void*& initial, Int_t offset) {
  volatile char* temp =  reinterpret_cast<volatile char*>(initial);
  temp += offset;
  initial = reinterpret_cast<volatile void*>(temp);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// remove_duplicate_spaces                               //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Remove adjacent whitespace from a \c std::string
void remove_duplicate_spaces(string& str) {
  for(int i=str.size()-1; i > 0; --i) {
    if(str[i] == ' ' && str[i-1] == ' ')
      str.erase(i,1);
  }
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// double2str                                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Convert a double to a std::string
inline string double2str(Double_t d) {
  stringstream sstr;
  sstr << d;
  return sstr.str();
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// string_len_compare                                    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Tell is a string is longer than the other
bool string_len_compare (const string& lhs, const string& rhs) {
  return rhs.size() > lhs.size();
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Helper Classes                                        //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// This is just a quick and dirty class to reconstruct the original indices
// of a multi-dimensional array that has been flattened by TStreamerElements.
// All we really need out is just strings with the original indices in brackets.
class ArrayConverter
{
private:
  TStreamerElement* fElement;
  vector<string> fIndices;

public:
  ArrayConverter(TStreamerElement* element) :
    fElement(element) {
    stringstream sstr;
    Int_t ndim = fElement->GetArrayDim();
    vector<UInt_t> index(4, 0);

    // Lazy way of "flattening" the array, just use nested for loops.  TStreamerElement
    // only supports up to 4-dimensional arrays anyway so there's no loss. Though really this
    // should be done recursively withouth the for loops (or some other clever way).
    for(index[0] = 0; index[0] < fElement->GetMaxIndex(0); ++index[0]) {
      for(index[1] = 0; index[1] < (ndim > 1 ? fElement->GetMaxIndex(1) : 1) ; ++index[1]) {
	for(index[2] = 0; index[2] < (ndim > 2 ? fElement->GetMaxIndex(2) : 1) ; ++index[2]) {
	  for(index[3] = 0; index[3] < (ndim > 3 ? fElement->GetMaxIndex(3) : 1) ; ++index[3]) {
	    sstr.str("");
	    for(Int_t i=0; i< ndim; ++i) {
	      sstr << "[" << index[i] << "]";
	    }
	    fIndices.push_back(sstr.str());
	  }
	}
      }
    }
  }
  // Return the original indices.
  string GetFullName(const char* baseName, UInt_t index) {
    if(index < fIndices.size()) {
      string out = baseName;
      out += fIndices[index];
      return out;
    }
    else {
      fprintf(stderr, "Invalid index %d\n", index);
      return "";
    }
  }
};


// Helper class for performing set and get operations on the
// basic data elements (e.g. ints, doubles, etc.) that are wrapped
// by the class pointed to by the fData pointer of rb::Data
// The basic idea is that rb::Data has a static map keyed by the complete
// string names of all basic data types (e.g. "top.sub.end"), and indexing
// a pointer to the appropriate derived (template) class of MBasicData.
// For example, if "top.sub.end" is a double, then the key "top.sub.end"
// would index a pointer to a BasicData<double>, which is derived from
// MBasic data.
class MBasicData
{
protected:
  volatile void* fAddress; // copy of the rb::Data fData pointer
public:
  const std::string kBasicClassName;
  virtual Double_t GetValue() = 0;
  virtual void SetValue(Double_t newval) = 0;
  virtual ~MBasicData() { }
  static MBasicData* New(volatile void* addr, const char* basic_type_name);
  static MBasicData* New(volatile void* addr, const std::string& basic_type_name) {
    return MBasicData::New(addr, basic_type_name.c_str());
  }
protected:
  MBasicData(volatile void* addr, const char* basic_class_name) :
    fAddress(addr), kBasicClassName(basic_class_name) { }
};

template <class T>
class BasicData : public MBasicData
{
public:
  BasicData(volatile void* addr, const char* basic_type) :
    MBasicData(addr, basic_type) { }
  Double_t GetValue() {
    LockingPointer<T> pAddress(reinterpret_cast<volatile T*> (fAddress), rb::Hist::GetMutex());
    return *pAddress;
  }
  virtual void SetValue(Double_t newval) {
    LockingPointer<T> pAddress(reinterpret_cast<volatile T*> (fAddress), rb::Hist::GetMutex());
    *pAddress = T(newval);
  }
  virtual ~BasicData() { }
};

#define CHECK_TYPE(type)		    \
  else if (!strcmp(basic_type_name, #type)) \
    return new BasicData<type> (addr, basic_type_name)
MBasicData* MBasicData::New(volatile void* addr, const char* basic_type_name) {
  if(0);
  CHECK_TYPE(Double_t);
  CHECK_TYPE(Float_t);
  CHECK_TYPE(Long64_t);
  CHECK_TYPE(Long_t);
  CHECK_TYPE(Int_t);
  CHECK_TYPE(Short_t);
  CHECK_TYPE(Char_t);
  CHECK_TYPE(Bool_t);
  CHECK_TYPE(ULong64_t);
  CHECK_TYPE(ULong_t);
  CHECK_TYPE(UInt_t);
  CHECK_TYPE(UShort_t);
  CHECK_TYPE(double);
  CHECK_TYPE(float);
  CHECK_TYPE(long long);
  CHECK_TYPE(long);
  CHECK_TYPE(int);
  CHECK_TYPE(short);
  CHECK_TYPE(char);
  CHECK_TYPE(bool);
  CHECK_TYPE(unsigned long long);
  CHECK_TYPE(unsigned long);
  CHECK_TYPE(unsigned int);
  CHECK_TYPE(unsigned);
  CHECK_TYPE(unsigned short);
  else return 0;
}
#undef CHECK_TYPE
  



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                                 //
// rb::Data                                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::Data::Getvalue                                    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Double_t rb::Data::GetValue(const char* name) {
  BasicDataMap_t::iterator it = fgBasicDataMap().find(name);
  if(it == fgBasicDataMap().end()) {
    Error("GetValue", "%s not found.", name);
    return -1.;
  }
  MBasicData* b = it->second;
  return (Double_t)b->GetValue();
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// static rb::Data::SetValue                             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Data::SetValue(const char* name, Double_t newvalue) {
  BasicDataMap_t::iterator it = fgBasicDataMap().find(name);
  if(it == fgBasicDataMap().end()) {
    Error("SetData", "Data object: %s not found.", name);
    return;
  }
  MBasicData* b = it->second;
  b->SetValue(newvalue);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// static rb::Data::SavePrimitive                        //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Data::SavePrimitive(ostream& strm) {
  if(fgBasicDataMap().empty()) return;
  BasicDataMap_t::iterator it;
  for(it = fgBasicDataMap().begin(); it != fgBasicDataMap().end(); ++it) {
    strm << "  rb::Data::SetValue(\"" << it->first << "\", " << GetValue(it->first.c_str()) << ");\n";
  }
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// static rb::Data::MapData                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t rb::Data::MapData(const char* name, TStreamerElement* element, volatile void* base_address) {
  Bool_t ret = kFALSE; // return value, default to false

  string typeName = element->GetTypeName(); // the type of this specific element
  remove_duplicate_spaces(typeName); // in case someone did 'unsigned    short' or whatever

  volatile void* address = base_address; // address of base pointer to the whole class containing this element
  void_pointer_add(address, element->GetOffset()); // increment to address of this specific element
  MBasicData* basicData = MBasicData::New(address, typeName); // try to create an MBasicData derived instance

  if(basicData) { // this element is a basic data type, so process it
    ret = kTRUE;
    Int_t arrLen = element->GetArrayLength();
    if(!arrLen) { // just a single element
      fgBasicDataMap().insert(std::make_pair(name, basicData));
    }
    else { // an array
      if(element->GetArrayDim() > 4) {
	Warning("MapData", "No support for arrays > 4 dimensions. The array %s is %d and will not be mapped!",
		name, element->GetArrayDim());
      }
      else {
	Int_t size = element->GetSize() / arrLen;
	ArrayConverter arrayConvert(element);
	for(Int_t i=0; i< arrLen; ++i) {
	  void_pointer_add(address, size*(i>0));
	  fgBasicDataMap().insert(std::make_pair(arrayConvert.GetFullName(name, i), MBasicData::New(address, typeName)));
	}
      }
    }
  }
  return ret;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::Data::MapClass                                    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Data::MapClass(const char* name, const char* classname, volatile void* address) {

  TClass* cl = TClass::GetClass(classname);
  if(!cl) return;

  TStreamerInfo* sinfo = static_cast<TStreamerInfo*>(cl->GetStreamerInfo());
  TObjArray* elems = sinfo->GetElements();

  for( Int_t i=0; i< elems->GetEntries(); ++i) {
    TStreamerElement* element =
      reinterpret_cast <TStreamerElement*>(elems->At(i));
    stringstream ssName;
    ssName << name << "." << element->GetName();
    Bool_t basic = MapData(ssName.str().c_str(), element, address);
    if(!basic) {
      void_pointer_add(address, element->GetOffset());
      MapClass(ssName.str().c_str(),  element->GetTypeName(), address);
    }
  }
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// static rb::Data::PrintAll                             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Data::PrintAll() {
  if(fgBasicDataMap().empty()) return;
  vector<string> names, values, classes;

  BasicDataMap_t::iterator it = fgBasicDataMap().begin();
  while(it != fgBasicDataMap().end()) {
    names.push_back(it->first);
    values.push_back(double2str(GetValue(it->first.c_str())));
    classes.push_back(it->second->kBasicClassName);
    ++it;
  }
  Int_t maxName  = max_element(names.begin(), names.end(), string_len_compare)->size();
  maxName = maxName > 4 ? maxName : 4;

  printf("\n%-*s\t%s\n", maxName, "Name", "Value [class]");
  printf("%-*s\t%s\n", maxName, "----", "-------------");
  for(Int_t i=0; i< names.size(); ++i) {
    if(atoi(values.at(i).c_str()) < 0)
      printf("%-*s\t%s [%s]\n", maxName, names.at(i).c_str(), values.at(i).c_str(), classes.at(i).c_str());
    else
      printf("%-*s\t %s [%s]\n", maxName, names.at(i).c_str(), values.at(i).c_str(), classes.at(i).c_str());
  } printf("\n");
}




#else // Template class member function implementations

template <class T>
void rb::TData<T>::Init(Bool_t makeVisible, const char* args) {
  TClass* cl = TClass::GetClass(typeid(T));
  if(!cl) {
    Error("TData::Init",
	  "CINT Does not know about a class you asked it to create "
	  "(typeid.name(): %s, constructor arguments: %s). "
	  "Check UserLinkdef.h to make sure a dictionary is properly generated.",
	  typeid(T).name(), args);
    return;
  }
  fClassName = cl->GetName();

  T* data = 0;
  if(!strcmp(args, ""))  data = new T();
  else {
    std::stringstream cmd;
    cmd << "new " << fClassName << "(" << args  << ");";
    data = reinterpret_cast<T*> (gROOT->ProcessLineFast(cmd.str().c_str()));
  }
  if (!data) {
    Error("TData::Init",
	  "Couldn't create a new instance of the template class "
	  "(typeid.name(): %s, constructor arguments: %s).",
	  typeid(T).name(), args);
    return;
  }

  fData = data;
  fgMap().insert(std::make_pair<std::string, Data*>(kName, this));

  if (makeVisible) {
    if(PrintHeader()) {
      std::cout << "\nMapping the address of user data objects:\n"
		<< "      Name\t\t\tClass Name\n"
		<< "      ----\t\t\t----------\n";
      PrintHeader() = kFALSE;
    }
    std::cout << "      " << kName << "\t\t\t" << fClassName << "\n";

    MapClass(kName.c_str(), fClassName.c_str(), reinterpret_cast<volatile void*>(fData));

    LockingPointer<T> pData (fData, fMutex);
    void* v = reinterpret_cast<void*>(pData.Get());
    std::string brName = kName; brName += ".";
    rb::Hist::AddBranch(kName.c_str(), fClassName.c_str(), &v);
  }
}

template <class T>
rb::TData<T>::TData(const char* name, Bool_t makeVisible, const char* args) :
  Data(name) {
  Init(makeVisible, args);
}

template <class T>
rb::TData<T>::~TData() {
  LockingPointer<T> pData(fData, fMutex);
  delete pData.Get();
  fData = 0;

  Map_t::iterator it = fgMap().find(kName);
  if(it != fgMap().end()) {
    fgMap().erase(it);
  }
}

template <class T>
std::auto_ptr<LockingPointer<T> > rb::TData<T>::GetLockedData() {  
  std::auto_ptr<LockingPointer<T> > out (new LockingPointer<T> (fData, fMutex));
  return out;
}

#endif
