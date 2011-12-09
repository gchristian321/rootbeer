/*! \file Data.hxx
 *  \brief Defines generic wrappers for user-defined data classes.
 */
#ifndef __DATA_HXX__
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include "TTree.h"
#include "TBranchElement.h"
#include "TError.h"
#include "Hist.hxx"


namespace rb
{
  /// Class wrapping pointers to user data objects.
  /*! The basic idea behind this class is to create a generic framework
   *  into which users can plug their own classes with minimal effort (or re-coding if their
   *  data storage and analysis routines are already written). By adding
   *  a couple of lines to a file, the user should get an instance of his/her class which
   *  he/she can use to code how event data are handled, as well as the ability to later
   *  access class member data in CINT, in a thread safe way.
   *
   *  The way this is implemented is to have the Data class wrap a \c void* pointer to a
   *  user data object that is created on the stack and has global scope.
   *  In addition to the pointer, it also stores some useful information, like the variable
   *  name and class name. It also contains a static \c std::map that maps the variable name to
   *  the corresponding Data pointer for every existing Data derived object.
   *  Finally, it implements some methods that are useful, like creating branches
   *  in the static rb::fgTree from which histograms are filled. Some of these are called at startup
   *  in main() so that everything is ready to go in terms of being able to parse strings referring to
   *  data members and fill them in histograms.
   */
  class Data
  {
  public:
    /// Function typedef void_cast <--> void func(void*, Double_t).
     typedef void (*void_cast)(void*, Double_t);

    /// Function typedef void_get <--> void func(const char*).
     typedef Double_t (*void_get)(const char*);

    /// Map typedef (string -> Data*).
    typedef std::map<std::string, Data*> Map_t;

    /// Map typedef (string -> pair<void*, string>).
    typedef std::map<std::string, std::pair<void*, std::string> > ObjectMap_t;

    /// Map typedef (string -> void_cast).
    typedef std::map<std::string, void_cast> SetMap_t;

    /// Map typedef (string -> void_get).
    typedef std::map<std::string, void_get> GetMap_t;

    /// Map iterator typedef (string -> Data*).
    typedef std::map<std::string, Data*>::iterator MapIterator_t;

    /// Map iterator typedef (string -> pair<void*, string>).
    typedef std::map<std::string, std::pair<void*, std::string> >::iterator ObjectMapIterator_t;

    /// Map iteratortypedef (string -> void_cast).
    typedef std::map<std::string, void_cast>::iterator SetMapIterator_t;

    /// Map iteratortypedef (string -> void_get).
    typedef std::map<std::string, void_get>::iterator GetMapIterator_t;

  protected:
    /// Tells whether we should have a pointer automatically created for use in CINT.
    const Bool_t kMapClass;

    /// Name of the class instance. Equivalent to the variable defined in Skeleton.hh
    const std::string kName;

    /// String specifying the class type.
    const std::string kClassName;

    /// \c void pointer to the user data class.
    void* fData;

    /// Maps \c kName to the base class address.
    static Map_t fgMap;

    /// Maps the full name of a data member of a user class to it's <address, class name>.
    static ObjectMap_t fgObjectMap;

    /// Map of string (type names, i.e. Double_t, etc) -> SetDataValue pointers 
    static SetMap_t fgSetFunctionMap;

    /// Map of string (type names, i.e. Double_t, etc) -> GetDataValue pointers 
    static GetMap_t fgGetFunctionMap;

    /// Recurse through a class and add each of it's basic data objects to fgObjectMap.
    static void MapClass(const char* name, const char* classname, void* address);

    /// Adds a specific element to fgObjectMap.
    //! For a specific element of a class, check if it's a basic data type. If so, add it to
    //! fgObjectMap. If not, return \c false.
    static Bool_t MapData(const char* name, TStreamerElement* element, void* base_address);

    /// Template function to set the value of data located at a generic address.
    //! The template argument should always be the same as the type of data pointed to by address.
    template<typename T>
    static void SetDataValue(void* address, Double_t newval) {
      rb::Hist::Lock();
      *reinterpret_cast<T*>(address) = T(newval);
      rb::Hist::Unlock();
    }

    /// Template function to get the value of data at a generic address.
    //! The template argument should always be the same as the type of data pointed to by address.
    //! Converts the data to \c Double_t before returning.
    template<typename T>
    static Double_t GetDataValue(const char* name) {
      ObjectMapIterator_t it = fgObjectMap.find(name);
      if(it == fgObjectMap.end()) {
	Error("Get", "%s not found.", name);
	return T(-1);
      }
      rb::Hist::Lock();
      void* address = it->second.first;
      return Double_t(*reinterpret_cast<T*>(address));
      rb::Hist::Unlock();
    }

  public:
    /// Constructor.
    /*! Sets the data pointer, name, class name, and create pointer members. */
    Data(const char* name, const char* class_name, void* data, Bool_t createPointer = kFALSE);

    /// Destructor
    /*! Remove from fgMap */
    virtual ~Data() {
      MapIterator_t it = fgMap.find(kName);
      if(it != fgMap.end()) fgMap.erase(it);
    }

    /// Add fData as a branch in a TTree.
    TBranch* AddBranch() {
      std::string brName = kName; brName += ".";
      return rb::Hist::AddBranch(brName.c_str(), kClassName.c_str(), &fData);
    }

    /// Write the fData data members and their current values to a stream.
    static void SavePrimitive(std::ostream& ofs);

    /// Call AddBranch() on everything in rb::Data::fgMap
    static void AddBranches();

    /// Call MapClass on all instances that ask for it (i.e. have set kMapClass true).
    static void MapClasses();

    /// Return the value of a user class data member. Thread safe.
    static Double_t Get(const char* name);

    /// Set the value of a user class data member. Thread safe.
    static void Set(const char* name, Double_t value);

    /// Print the fill name and current value of every data member in every listed class.
    static void PrintAll();

  };

}

#endif
