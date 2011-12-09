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
  /// Base class wrapping pointers to data objects.
  /*! The basic idea behind this class and it's derivatives is to create a generic framework
   *  into which users can plug their own classes with minimal effort (or re-coding if their
   *  data storage and analysis routines are already written). By adding
   *  a couple of lines to a file, the user should get an instance of his/her class which
   *  he/she can use to code how event data are handled, as well as the ability to later
   *  access that class instance via a pointer in CINT (optionally, have this created automaticaly
   *  upon starting ROOTBEER).
   *
   *  The way this is implemented is to create a class that wraps a \c void* pointer to a
   *  user data object that is created on the stack and has global scope. That's what this class
   *  is. In addition to the pointer, it also stores some useful information, like the variable
   *  name and class name. It also contains a static \c std::map that maps the variable name to
   *  the corresponding Data pointer for every existing Data derived object.
   *  Finally, it implements some methods that are useful, like creating branches
   *  in the static rb::fgTree from which histograms are filled. Some of these are called at startup
   *  in main() so that everything is ready to go in terms of being able to parse strings referring to
   *  data members and fill them in histograms.
   *
   * In general, instances of this class should not be created directly. Instead, use the template
   * derived classes, which allow type-safe access to the wrapped data.
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
    const Bool_t kCintPointer;

    /// Name of the class instance. Equivalent to the variable defined in Skeleton.hh
    const std::string kName;

    /// String specifying the class type.
    const std::string kClassName;

    /// \c void pointer to the user data class.
    /// \note In general, this should \emph not be accessed directly. Instead use the
    /// Get() or GetData() methods of the template derived class, which ensure type
    /// safety. Exception is the CreateBranch() function which needs to pass a \c void*
    /// to TTree::Branch() anyway.
    void* fData;

    /// Maps \c kName to the base class pointer.
    static Map_t fgMap;

    /// Map of string (type names, i.e. Double_t, etc) -> SetDataValue pointers 
    static SetMap_t fgSetFunctionMap;

    /// Map of string (type names, i.e. Double_t, etc) -> GetDataValue pointers 
    static GetMap_t fgGetFunctionMap;


  public:
    /// Constructor.
    /*! Sets the data pointer, name, class name, and create pointer members.
     *  \note This is protected because we shouldn't create instances explicitly, instead
     *  use the template derived classe. */
    Data(const char* name, const char* class_name, void* data, Bool_t createPointer = kFALSE);

    /// Template function to set data values to the appropriate type.
    template<typename T>
    static void SetDataValue(void* address, Double_t newval) {
      *reinterpret_cast<T*>(address) = T(newval);
    }

    /// Template function to get data values to the appropriate type, returning double.
    template<typename T>
    static Double_t GetDataValue(const char* name) {
      ObjectMapIterator_t it = fgObjectMap.find(name);
      if(it == fgObjectMap.end()) {
	Error("Get", "%s not found.", name);
	return T(-1);
      }
      void* address = it->second.first;
      return Double_t(*reinterpret_cast<T*>(address));
    }

  public:
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

    /// Create a pointer in CINT for all instances of derived classes that ask for it (i.e. have set kCintPointer true).
    static void CreatePointers();

    static Double_t Get(const char* name);

    static void Set(const char* name, Double_t value);

    static ObjectMap_t fgObjectMap;

    static void list();

    static void ParseClass(const char* name, const char* classname, void* address);

    static Bool_t MapDataAddress(const char* name, TStreamerElement* element, void* base_address);

  };


//   /// Template class derived from Data.
//   /*! As noted in the Data documentation, instances of Data-derived classes should be created using this
//    *  template class. It's main function is to provide type-safe access to the user data classes. */
//   template <class UserClass>
//   class Data : public Data
//   {
//   public:
//     /// Constructor
//     /*! Pass everything off to the Data constructor. */
//     Data(const char* name, const char* class_name, UserClass& data, Bool_t createPointer = kFALSE) :
//       Data(name, class_name, static_cast<void*>(&data), createPointer) {
//       //      if(createPointer) Data::ParseClass(name, class_name);
//     }

//     /// Return a pointer to the data object, casted to the appropriate type.
//     UserClass* GetData() {
//       return static_cast<UserClass*>(fData);
//     }

//     /// Return an appropriately-typed pointer to the data object, called by name.
//     /*! Searches Data::fgMap for an instance keyed with <tt>name</tt>. If it finds
//      *  one, returns the corresponding pointer to the wrapped user data class.
//      *  If called for the wrong template class, returns 0 and gives an error message. */
//     static UserClass* Get(const char* name) {
//       MapIterator_t it = fgMap.find(name);
//       if(it == fgMap.end()) {
// 	Error("Data<T>::Get", "\"%s\" not found.", name);
// 	return 0;
//       }
//       Data* data = it->second;
//       Data<UserClass>* cast_data = dynamic_cast <Data <UserClass>* >(data);
//       if(!cast_data) {
// 	Error("rb::Data<T>::Get", "Invalid class specification. Try rb::Data<%s>::Get() instead",
// 	      data->kClassName.c_str());
// 	return 0;
//       }
//       return cast_data->GetData();
//     }

//     /// Destructor
//     ~Data() { }

//   };
}

#endif
