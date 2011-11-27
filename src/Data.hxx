/*! \file Data.hxx
 *  \brief Defines generic wrappers for user-defined data classes.
 */
#ifndef __DATA_HXX__
#include <string>
#include <sstream>
#include <iostream>
#include <map>
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
   *  the corresponding MData pointer for every existing MData derived object.
   *  Finally, it implements some methods that are useful, like creating branches
   *  in the static rb::fgTree from which histograms are filled. Some of these are called at startup
   *  in main() so that everything is ready to go in terms of being able to parse strings referring to
   *  data members and fill them in histograms.
   *
   * In general, instances of this class should not be created directly. Instead, use the template
   * derived classes, which allow type-safe access to the wrapped data.
   */
  class MData
  {
  public:
    /// Map typedef to reduce typing/increase clarity.
    typedef std::map<std::string, MData*> Map_t;

    /// Map iterator typedef to reduce typing/increase clarity.
    typedef std::map<std::string, MData*>::iterator MapIterator_t;

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
    /*! Currently initialized in Skeleton.hh
     *  \todo Crashes if initialized in Data.cxx...try to figure out why and fix. */
    static Map_t fgMap;

    /// Constructor.
    /*! Sets the data pointer, name, class name, and create pointer members.
     *  \note This is protected because we shouldn't create instances explicitly, instead
     *  use the template derived classe. */
    MData(const char* name, const char* class_name, void* data, Bool_t createPointer = kFALSE);

  public:
    /// Destructor
    /*! Remove from fgMap */
    virtual ~MData() {
      MapIterator_t it = fgMap.find(kName);
      if(it != fgMap.end()) fgMap.erase(it);
    }

    /// Add in instance of the wrapped class as a branch parable by rb::Hist::fgTree
    TBranch* AddBranch() {
      return rb::Hist::CreateBranch(kName.c_str(), kClassName.c_str(), &fData);
    }

    /// Write the fData data members and their current values to a stream.
    /*! Makes use of a temporary TTree to parse the class into string named.
     *  \todo Try to implement this directly instead of relying on TTree. Implementing
     *  with TTree::Branch has some limitations. For example, if anything was prevented
     *  from being put into the tree with an \c //! comment, we wouldn't be able to write it.
     */
    void SavePrimitive(std::ostream& ofs);

    /// Run SavePrimitive() on every instance of MData that has kCintPointer == true.
    static void SaveAllPrimitive(std::ostream& strm);

    /// Create a rb::Hist::fgTree branch for every instance of classes derived from MData.
    static void AddBranches();

    /// Create a pointer in CINT for all instances of derived classes that ask for it (i.e. have set kCintPointer true).
    static void CreatePointers();

  };


  /// Template class derived from MData.
  /*! As noted in the MData documentation, instances of MData-derived classes should be created using this
   *  template class. It's main function is to provide type-safe access to the user data classes. */
  template <class UserClass>
  class Data : public MData
  {
  public:
    /// Constructor
    /*! Pass everything off to the MData constructor. */
    Data(const char* name, const char* class_name, UserClass& data, Bool_t createPointer = kFALSE) :
      MData(name, class_name, static_cast<void*>(&data), createPointer) { }

    /// Return a pointer to the data object, casted to the appropriate type.
    UserClass* GetData() {
      return static_cast<UserClass*>(fData);
    }

    /// Return an appropriately-typed pointer to the data object, called by name.
    /*! Searches MData::fgMap for an instance keyed with <tt>name</tt>. If it finds
     *  one, returns the corresponding pointer to the wrapped user data class.
     *  If called for the wrong template class, returns 0 and gives an error message. */
    static UserClass* Get(const char* name) {
      MapIterator_t it = fgMap.find(name);
      if(it == fgMap.end()) {
	Error("Data<T>::Get", "\"%s\" not found.", name);
	return 0;
      }
      MData* data = it->second;
      Data<UserClass>* cast_data = dynamic_cast <Data <UserClass>* >(data);
      if(!cast_data) {
	Error("rb::Data<T>::Get", "Invalid class specification. Try rb::Data<%s>::Get() instead",
	      data->kClassName.c_str());
	return 0;
      }
      return cast_data->GetData();
    }

    /// Destructor
    ~Data() { }

  };
}

#endif
