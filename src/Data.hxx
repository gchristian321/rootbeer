/*! \file Data.hxx
 *  \brief Defines generic wrappers for user-defined data classes.
 */
#ifndef __DATA_HXX__
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include "Hist.hxx"



/// Base class wrapping pointers to data objects.
/*! Since the type needs to be generic, we store the pointer as a void*
 *  The derived classes are templates and they case the void* pointer to
 *  the appropriate type.
 */
class UserDataABC
{
public:
  /// Map typedef to reduce typing/increase clarity.
  typedef std::map<std::string, UserDataABC*> Map_t;

  /// Map iterator typedef to reduce typing/increase clarity.
  typedef std::map<std::string, UserDataABC*>::iterator MapIterator_t;

protected:
  /// \c void pointer to the user data class.
  void* fData;

  /// Maps \c fName to the base class pointer.
  /*! Currently initialized in Skeleton.hh */
  static Map_t fgMap;

public:

  /// Destructor
  virtual ~UserDataABC() { }

  /// Name of the class instance.
  const std::string fName;

  /// Name of the class.
  const std::string fClassName;

  /// Tells whether we should have a pointer automatically created for use in CINT.
  Bool_t kCintPointer;

  /// Add in instance of the wrapped class as a branch parable by rb::Hist::fgTree
  TBranch* AddBranch() {
    return rb::Hist::CreateBranch(fName.c_str(), fClassName.c_str(), &fData);
  }

  /// Create a branch for every instance of classes derived from UserDataABC.
  static void AddBranches();

  /// Create a pointer int CINT for all instances of derived classes that ask for it.
  static void CreatePointers();

  /// Figure out class member names and print name = value; to stream
  static void ParseAllInstances(std::ostream& strm);

protected:
  /// Constructor. Protedted because users shouldn't call it.
  UserDataABC(const char* name, const char* class_name,
	      Bool_t createPointer = kFALSE) :
    fName(name), fClassName(class_name),
    kCintPointer(createPointer) { };

};



template <class Class>
class UserData : public UserDataABC
{
public:
  /// Constructor
  /*! Initialize void* pointer to point to the data object, add to static map of all
   *  UserDataABC derived classes. */
  UserData(const char* name, const char* class_name, Class& data, Bool_t createPointer = kFALSE) :
    UserDataABC(name, class_name, createPointer) {
    fData = static_cast<void*>(&data);
    UserDataABC::fgMap[fName] = this;
  };

  /// Return the pointer to the data object, casted to the appropriate type.
  Class* GetData() {
    return static_cast<Class*>(fData);
  }

  /// Return an appropriately-typed pointer to the data object, called by name.
  static Class* Get(const char* name) {
    return static_cast<UserData<Class>* >(fgMap[name])->GetData();
  }

  /// Destructor
  ~UserData() { }

};



#endif
