//! \file Data.hxx
//! \brief Classes and functions relevant to data unpacking.
//! \details Defines the rb::Data class, a generic wrapper for user-defined
//! data storage classes.
#ifndef _DATA_HXX_
#define _DATA_HXX_
#include <assert.h>
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
  class Rint; // forward declaration

  //! Type of buffer into which we copy our data.
  //! \note DATA_TYPE is \c #defined in user/Makefile.user It corresponds to the
  //! type of data packed into the event stream.
  typedef std::vector<DATA_TYPE> Buffer;

  //! Encloses functions relevant to reading and unpacking buffers.
  //! \details Functions are implemented in Skeleton.cxx
  namespace unpack
  {
    //! Defines how a single data buffer is extracted from a data stream.
    //! \details In principle, this needs to be filled by the end user in Skeleton.cxx.  However,
    //! appropriate implementations for the <a href = http://docs.nscl.msu.edu/daq/bluebook/html/> NSCL </a>
    //! and <a href = https://daq-plone.triumf.ca/SR/MIDAS/> MIDAS </a> sysems are already written.  These
    //! can be utilized by commenting the appropriate \c #define derectives as noted in the sources.
    //! \param[in] ifs stream from which the data is read.
    //! \param[out] buf buffer (std::vector<DATA_TYPE>) into which we copy the data.
    extern void ReadBuffer(std::istream& ifs, rb::Buffer& buf);

    //! Defines how raw data buffers are unpacked.
    //! \details The code of this function is filled in by users in Skeleton.cxx
    //! It should define how to handle the raw data buffers, i.e. how to take the
    //! packed data and disseminate it into the classes the users have written to
    //! store their data.
    //! \param[in] buf The data buffer that we want to unpack into our user classes.
    extern void UnpackBuffer(rb::Buffer& buf);
  }


  //! Class wrapping pointers to user data objects.

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
  //! rb::Data::GetValue("top.sub.b");  // returns the value of UserClass::sub::b for the instance 'top'.
  //! \endcode
  //! 
  class Data
  {
  public:
    // Typedefs
#ifndef __CINT__ // CINT doesn't like volatile void* apparently.
    typedef void (*void_cast)(volatile void*, Double_t);
    typedef std::map<std::string, std::pair<volatile void*, std::string> > ObjectMap_t;
    typedef std::map<std::string, std::pair<volatile void*, std::string> >::iterator ObjectMapIterator_t;
#else
    typedef void (*void_cast)(void*, Double_t);
    typedef std::map<std::string, std::pair<void*, std::string> > ObjectMap_t;
    typedef std::map<std::string, std::pair<void*, std::string> >::iterator ObjectMapIterator_t;
#endif

    // Function pointers
    typedef void (*delete_function)(Data*);
    typedef Double_t (*void_get)(const char*);

    // Maps
    typedef std::map<std::string, Data*>           Map_t;
    typedef std::map<std::string, void_cast>       SetMap_t;
    typedef std::map<std::string, void_get>        GetMap_t;
    typedef std::map<std::string, delete_function> DeleteMap_t;

    // Iterators
    typedef std::map<std::string, Data*>::iterator           MapIterator_t;
    typedef std::map<std::string, void_cast>::iterator       SetMapIterator_t;
    typedef std::map<std::string, void_get>::iterator        GetMapIterator_t;
    typedef std::map<std::string, delete_function>::iterator DeleteMapIterator_t;


  private:
    /// Tells whether we should map the map a user class instance's data members or not.
    const Bool_t kMapClass;

    /// Name of the class instance. Equivalent to the variable defined in Skeleton.hh
    const std::string kName;

    /// String specifying the class type.
    const std::string kClassName;

    /// <tt>void</tt> pointer to the user data class.
    volatile void* fData;

    /// Mutex to protect access to the data.
    TMutex fMutex;

    /// Maps \c kName to the base class address.
    static Map_t fgMap;

    /// Maps the full name of a data member of a user class to it's <address, class name>.
    static ObjectMap_t fgObjectMap;

    /// Maps type names to SetDataValue<T> function pointers.
    static SetMap_t fgSetFunctionMap;

    /// Maps type names to GetDataValue<T> function pointers.
    static GetMap_t fgGetFunctionMap;

    /// Maps the class name to the appropraite delete method.
    static DeleteMap_t fgDeleteMap;

    /// Recurse through a class and add each of it's basic data objects to fgObjectMap.
    static void MapClass(const char* name, const char* classname, volatile void* address);

    /// Adds a specific element to fgObjectMap.
    //! For a specific element of a class, check if it's a basic data type. If so, add it to
    //! fgObjectMap. If not, return \c false.
    static Bool_t MapData(const char* name, TStreamerElement* element, volatile void* base_address);

    /// Template function to set the value of data located at a generic address.
    //! The template argument should always be the same as the type of data pointed to by address.
    template<typename T>
    static void SetDataValue(volatile void* address, Double_t newval) {
      LockingPointer<T> pAddress(reinterpret_cast<volatile T*>(address), rb::Hist::GetMutex());
      *pAddress = T(newval);
    }

    /// Template function to get the value of data at a generic address.
    //! The template argument should always be the same as the type of data pointed to by address.
    //! Converts the data to \c Double_t before returning.
    template<typename T>
    static Double_t GetDataValue(const char* name) {
      ObjectMapIterator_t it = fgObjectMap.find(name);
      if(it == fgObjectMap.end()) {
	Error("GetDataValue", "%s not found.", name);
	return -1.;
      }
      LockingPointer<T> pAddress(reinterpret_cast<volatile T*>(it->second.first), rb::Hist::GetMutex());
      return Double_t(*pAddress);
    }

    /// Returns a T* pointer to the fData object.
    template<typename T>
    volatile T* GetDataPointer() {
      return reinterpret_cast<volatile T*>(fData);
    }

    /// Constructor.
    //! Sets data fields and fills internal std::maps.
    //! Private, users should call rb::Data::New<T>().
    Data(const char* name, const char* class_name, volatile void* data, Bool_t createPointer = kFALSE);

    /// Free memory allocated to fData.
    //! The template argument determines how the memory is freed, by casting the \c void
    //! fData pointer to the template argument type, then calling <tt>delete</tt>.  Pointers
    //! to the various flavors of this function are stored in \c fgDeleteMap, with the key
    //! being the corresponding class name (as a string).
    template <class T>
    static void FreeMemory(Data* _this) {
      LockingPointer<T> pData(reinterpret_cast<volatile T*> (_this->fData), _this->fMutex);
      delete pData.Get();
    }

    /// \c delete all instances of rb::Data.
    static void DeleteAll() {
      while(fgMap.size() != 0) {
	delete fgMap.begin()->second;
      }
    }

  public:
    /// Destructor
    //! Remove from fgMap, and call the appropriate destructor for fData.
    virtual ~Data() {
      DeleteMapIterator_t itDelete = fgDeleteMap.find(kClassName);
      assert(itDelete != fgDeleteMap.end());
      itDelete->second(this);
      fData = 0;

      MapIterator_t it = fgMap.find(kName);
      if(it != fgMap.end()) {
	fgMap.erase(it);
      }
    }

    /// Create a new rb::Data instance.
    //! Since this is a template function, we can create a \c new instance of the wrapped data
    //! directly inside instead of passing one to it. If the class T has a non-default constructor,
    //! it can be passed to rb::Data via the 'args' argument of this function, for example:
    //! \code
    //! rb::Data* newData = rb::Data::New("newData", "myClass", kTRUE, "ArgumentToMyClass, anotherArgumentToMyClass");
    //! \endcode
    //! The above will create a \c new instance of <tt>myClass</tt>, constructed as
    //! \code
    //! new myClass(ArgumentToMyClass, anotherArgumentToMyClass);
    //! \endcode
    //! (this is done using <tt>ROOT->ProcessLineFast()</tt>). Leaving the \c args arguemnt as default creates
    //! a \c new instance of the class, calling the default constructor.
    //! \code
    //! new myClass(); // note: done without using gROOT->ProcessLineFast();
    //! \endcode
    //! The \c makeVisible argument decides whether or not the user can access the wrapped class in CINT, using
    //! rb::Data::GetValue() and rb::Data::SetValue() functions. Typically, \c makeVisible should be \c false
    //! for parameters (data updated event-by-event) and \c true for variables (values used in calculating parameters.
    template <class T>
    static Data* New(const char* name, const char* class_name, Bool_t makeVisible = kFALSE, const char* args = "") {
      fgDeleteMap[class_name] = &FreeMemory<T>;
      T* data = 0;
      if(!strcmp(args, ""))  data = new T();
      else {
	std::stringstream cmd;
	cmd << "new " << class_name << "(" << args  << ");";
	data = reinterpret_cast<T*> (gROOT->ProcessLineFast(cmd.str().c_str()));
      }
      Data* _this = new Data(name, class_name, data, makeVisible);
      return _this;
    }

    /// Write the fData data members and their current values to a stream.
    static void SavePrimitive(std::ostream& ofs);

    /// Call AddBranch() on everything in rb::Data::fgMap
    static void AddBranches();

    /// Call MapClass on all instances that ask for it (i.e. have set kMapClass true).
    static void MapClasses();

    /// Return the value of a user class data member.
    static Double_t GetValue(const char* name);

    /// Set the value of a user class data member.
    static void SetValue(const char* name, Double_t newvalue);

    /// Print the fill name and current value of every data member in every listed class.
    static void PrintAll();

    /// Accesses DeleteAll()
    friend class rb::Rint;

    /// Accesses GetDataPointer() and fMutex
    friend void rb::unpack::UnpackBuffer(rb::Buffer&);
  };

}

#endif
