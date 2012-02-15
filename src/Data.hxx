//! \file Data.hxx
//! \brief Classes and functions relevant to data unpacking.
//! \details Defines the rb::Data class, a generic wrapper for user-defined
//!  data storage classes.
#ifndef DATA_HXX
#define DATA_HXX
#include <cassert>
#include <typeinfo>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <TROOT.h>
#include <TTree.h>
#include <TBranchElement.h>
#include <TError.h>
#include "Hist.hxx"

namespace rb
{
  //! Base class wrapping pointers to user data objects.

  //! The basic idea behind this class is to create a generic framework
  //! into which users can plug their own classes with minimal effort (or re-coding if their
  //! data storage and analysis routines are already written). By adding
  //! a couple of lines to a file, the user should get an instance of his/her class which
  //! he/she can use to code how event data are handled, as well as the ability to later
  //! access class member data in CINT, in a thread safe way.
  //!
  //! The way this is implemented is to have the Data class wrap a volatile void* pointer to a
  //! user data object. The data object is allocated using \c new when we create an rb::Data
  //! instance using rb::Data::New(), and it is de-allocated in the destructor. By storing the
  //! data as a void* internally, we are able to generically wrap any type of user object.
  //! Access to the object is controlled through member functions that ensure both thread-safety
  //! (since the data is marked \c volatile - access via LockingPointer) and type-safe (generally,
  //! only using template functions to access the raw pointer).
  //!
  //! Each object wrapped by rb::Data is stored along with a name (string) and it's class name.
  //! These values are used as keys in a variety of std::map objects that store things like the
  //! address of the data and the appropriate template functions to access or modify it. These
  //! maps are then accessed publically via static member functions. For users to have safe access
  //! to the basic data types encapsulated by the classes stored in rb::Data, the address of each
  //! data type is stored in a std::map, keyed by the full name of the data type and accessed
  //! for writing or reading by SetDataValue() and GetDataValue(). For example, given a simple class,
  //! \code
  //! struct UserClass {
  //!    double a;
  //!    struct UserSubClass {
  //!       double b;
  //!    } sub;
  //! } top;
  //! \endcode
  //! The user can read the values of basic data types as follows:
  //! \code
  //! rb::Data::GetValue("top.a");  // returns the value of UserClass::a for the instance 'top'.
  //! rbS::Data::GetValue("top.sub.b");  // returns the value of UserClass::sub::b for the instance 'top'.
  //! \endcode
  //! 
  class Data
  {
  protected:
    /// Name of the class instance.
    const std::string kName;

    /// For keeping track of whether or not MapData() needs to print the header lines.
    static Bool_t& PrintHeader() { static Bool_t* b = new Bool_t(kTRUE); return *b; }

  public:
    /// Sets kName.
    Data(const char* name) : kName(name) {};

    /// Nothing to do.
    virtual ~Data() {}

    /// Write the user class basic data members and their current values to a stream.
    static void SavePrimitive(std::ostream& ofs);

    /// Return the value of a user class data member.
    static Double_t GetValue(const char* name);

    /// Set the value of a user class data member.
    static void SetValue(const char* name, Double_t newvalue);

    /// Print the fill name and current value of every data member in every listed class.
    static void PrintAll();

  protected:
    /// Recurse through a class and construct an instance of MBasicData for each basic data type.
    virtual void MapClass(const char* name, const char* classname, volatile void* address);

    /// \brief Adds a specific element to fgObjectMap.
    //! \details For a specific element of a class, check if it's a basic data type. If so, construct
    //! an MBasicData.  If not, return false.
    virtual Bool_t MapData(const char* name, TStreamerElement* element, volatile void* base_address);
  };


  template <class T>
  class TData : public Data
  {
  private:
    /// String specifying the type of the user data class.
    std::string fClassName;

    /// Pointer to the user data class.
    //! \note Volatile so that we have to use a LockingPointer to get access, ensuring thread safety.
    volatile T* fData;

    /// Mutex to protect access to fData.
    TMutex fMutex;

    /// Does most of the work for the constructor.
    void Init(Bool_t makeVisible, const char* args = "");

  public:
    /// \details Allocates memory to the user data class and sets internal variables.
    //!
    //! \param [in] name Name of the user data class. This is how you will refer to it in the
    //! interactive CINT session, i.e. <tt>t->Draw("name.whatever");</tt>.
    //!
    //! \param [in] makeVisible Specifies whether or not you want to make the class visible
    //! in CINT.  If this is selected true, then you will be able to change or read the values of
    //! basic data types in your user data class using SetData() and GetData().  If selected false,
    //! you won't have direct access to the data (in CINT) and will only be able to view it in histograms.
    //! Generally, one wants to specify true for variables and false for parameters.
    //!
    //! \param [in] args Optional arguments to pass to the user data class's constructor. They should
    //! be in the literal format that would appear if writing the constructor directly in code. For
    //! example, consider that class
    //! \code
    //! struct MyClass {
    //!   MyClass(const char* str, int val);
    //!   // ... //
    //! };
    //! \endcode
    //! To call the constructor with str == "mine" and val == 37:
    //! \code
    //! rb::Data<MyClass>("myClassName", kFALSE /*(not visible)*/, "\"mine\", 37");
    //! \endcode
    //! If this argument is left as the default (empty string), the user class is constructed
    //! without any arguments, i.e. <tt>new MyClass()</tt>.
    TData(const char* name, Bool_t makeVisible = kFALSE, const char* args = "");

    //! \details Frees memory allocated to the user class.
    virtual ~TData();

    /// \brief Return a scoped and locked pointer to the user class.
    //! \details Returns an AutoLockingPointer wrapping the user class.
    //! This allows thread safe access to the data in the scope that it's
    //! needed while also ensuring that dynamic resources are freed properly.
    inline AutoLockingPointer<T> GetPointer();
  };
} // namespace rb

// Implementation of TData functions
#ifndef __MAKECINT__
#define IMPLEMENT_DATA_TEMPLATES
#include "Data.cxx"
#undef  IMPLEMENT_DATA_TEMPLATES
#endif

#endif
