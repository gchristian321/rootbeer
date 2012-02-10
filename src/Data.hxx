//! \file Data.hxx
//! \brief Classes and functions relevant to data unpacking.
//! \details Defines the rb::Data class, a generic wrapper for user-defined
//!  data storage classes.
#ifndef DATA_HXX
#define DATA_HXX
#include <assert.h>
#include <memory>
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
#include "midas/rbMidasEvent.h"
#include "midas/TMidasFile.h"




class MBasicData;
namespace rb
{
  class Rint; // forward declaration
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
    // Function pointers
    typedef void (*delete_function)(Data*);

    // Maps
    typedef std::map<std::string, Data*>           Map_t;
    typedef std::map<std::string, MBasicData*>     BasicDataMap_t;
    typedef std::map<std::string, delete_function> DeleteMap_t;

  protected: //private:
    /// Tells whether we should map the map a user class instance's data members or not.
    Bool_t kMapClass;

    /// Name of the class instance. Equivalent to the variable defined in Skeleton.hh
    std::string kName;

    /// String specifying the class type.
    std::string kClassName;

    /// <tt>void</tt> pointer to the user data class.
    volatile void* fData;

    /// Mutex to protect access to the data.
    TMutex fMutex;

    /// Maps \c kName to the base class address.
    static Map_t& fgMap() { static Map_t* m = new Map_t(); return *m; }

    /// Maps the class name to the appropraite delete method.
    static DeleteMap_t& fgDeleteMap() { static DeleteMap_t* m = new DeleteMap_t(); return *m; }

    /// Maps the full name of a basic data type to an MBasicDataType pointer wrapping it.
    static BasicDataMap_t& fgBasicDataMap() { static BasicDataMap_t* m = new BasicDataMap_t(); return *m; }

    /// Recurse through a class and add each of it's basic data objects to fgObjectMap.
    static void MapClass(const char* name, const char* classname, volatile void* address);

    /// Adds a specific element to fgObjectMap.
    //! For a specific element of a class, check if it's a basic data type. If so, add it to
    //! fgObjectMap. If not, return \c false.
    static Bool_t MapData(const char* name, TStreamerElement* element, volatile void* base_address);

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
      while(fgMap().size() != 0) {
	delete fgMap().begin()->second;
      }
    }

  public:
    /// Destructor
    //! Remove from fgMap, and call the appropriate destructor for fData.
    virtual ~Data() {
      DeleteMap_t::iterator itDelete = fgDeleteMap().find(kClassName);
      assert(itDelete != fgDeleteMap().end());
      itDelete->second(this);
      fData = 0;

      Map_t::iterator it = fgMap().find(kName);
      if(it != fgMap().end()) {
	fgMap().erase(it);
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
      fgDeleteMap()[class_name] = &FreeMemory<T>;
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

    /// Return a scoped and locked pointer to the data.
    //! Returns a std::auto_ptr to a LockedPointer wrapping the data.
    //! This allows thread safe access to the data in the scope that it's
    //! needed while also ensuring that dynamic resources are freed properly.
    //! \note Since the return is a "pointer like object" pointing to a "pointer like object",
    //! the samantics for use are a little strange, e.g.
    //! \code
    //! std::auto_ptr<LockingPointer<myClass> > p = someData.GetLockedData();
    //! *p->SomeMemberFunction();
    //! \endcode
    //! As a result, there is the option to use the GET_LOCKING_POINTER macro,
    //! which copies \c *p into a LockingPointer reference.
    template<typename T>
    std::auto_ptr<LockingPointer<T> > GetLockedData() {
      LockingPointer<T>* lockedData = new LockingPointer<T> (GetDataPointer<T>(), fMutex);
      std::auto_ptr<LockingPointer<T> > out (lockedData);
      return out;
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
    //    friend void rb::unpack::UnpackBuffer(BUFFER_TYPE&);
    // friend class Midas; //BufferSource;
    // friend class BufferSource;

    /*   CAN"T ACCESS POINTERS WITHOUT FRIENDSHIP!!!!!!!! <<<<<<<<<
#define GET_LOCKING_POINTER(SYMBOL, NAME, CLASS)                        \
  LockingPointer<CLASS> SYMBOL (NAME##_Data->GetDataPointer<CLASS>(), NAME##_Data->fMutex);

    template <class T>
    LockingPointer<T>& GetLockingPointer<T> () {
      LockingPointer<T> out = 
      return GetDataPointer<T
    */

  };

}





static const Int_t N_SOURCES = 3;
enum DataSources { ONLINE_, FILE_, LIST_ };
class BufferSource
{
private:
  //! Arguments for AttachFile()
  struct FileArguments {
    std::string fileName;
    Bool_t stopEnd;
    void Set(const char* fname, Bool_t stop) {
      fileName = fname;
      stopEnd = stop;
    }
  } fFileArgs;

  //! Arguments for AttachOnline()
  struct OnlineArguments {
    std::string source;
    std::string other;
    char** others;
    int nothers;
    void Set(const char* source_, const char* other_,
	     char** others_, int nothers_) {
      source = source_;
      other = other_;
      others = others_;
      nothers = nothers_;
    }
  } fOnlineArgs;

  //! Tells whether the source is attached to a data source.
  static Bool_t* Attached_() {
    static Bool_t firstTime = kTRUE;
    static Bool_t* isAttached = new Bool_t[N_SOURCES];
    if (firstTime) {
      firstTime = kFALSE;
      for(Int_t i=0; i< N_SOURCES; ++i)
	isAttached[i] = kFALSE;
    }
    return isAttached;
  }

public:
  std::vector<rb::Data*> fUserData;
  virtual void AddData() = 0;


  //! Constructor
  BufferSource() {
    fFileArgs.Set("", kTRUE);
    fOnlineArgs.Set("", "", 0, 0);
  }

  //! Destructor
  virtual ~BufferSource() {};

  //! Tells whether or not it's attached to a data source.
  //! \details Use the DataSources enum to denote the types.
  static Bool_t IsAttached(Int_t source) { return Attached_()[source]; }

  //! Sets appropriate Attached_ flag to true.
  static void SetAttached(Int_t source) { Attached_()[source] = kTRUE; }

  //! Sets appropriate Attached_ flag to false.
  static void SetNotAttached(Int_t source) { Attached_()[source] = kFALSE; }

  //! Return reference to file arguments
  FileArguments& FileArgs() { return fFileArgs; }

  //! Return reference to online arguments
  OnlineArguments& OnlineArgs() { return fOnlineArgs; }

  //! Open a data file
  //! \param [in] file_name Name (path) of the file to open.
  //! \param [in] other_args Any other arguments that might be needed.
  //! \param [in] n_others Number of other (tertiary) arguments.
  //! \returns true if file successfully opened, false otherwise.
  virtual Bool_t OpenFile(const char* file_name, char** other = 0, int nother = 0) = 0;

  //! Connect to an online data source.
  //! \param [in] host Name of the host from which the data are received.
  //! \param [in] other_arg Secondary argument specifying where the data come from (e.g. experiment for MIDAS).
  //! \param [in] other_args Any other arguments that might be needed.
  //! \param [in] n_others Number of other (tertiary) arguments.
  //! \returns true if connection is successfully made, false otherwise.
  virtual Bool_t ConnectOnline(const char* host, const char* other_arg = "", char** other_args = 0, int n_others = 0) = 0;

  //! Read an abstract buffer from an offline data source.
  //! \returns true if buffer is successfully read, false otherwise.
  virtual Bool_t ReadBufferOffline() = 0;

  //! Read an abstract buffer from an online data source.
  //! \returns true if buffer is successfully read, false otherwise.
  virtual Bool_t ReadBufferOnline() = 0;

  //! Unpack an abstract buffer into rb::Data classes.
  //! \returns Error code
  virtual Bool_t UnpackBuffer() = 0;
};

class Midas : public BufferSource
{
protected:
  Int_t fRequestId; //< Return code for online event requests.
  TMidasFile fFile; //< Offline MIDAS file.
  TMidasEvent fBuffer; //< Midas event buffer.
public:
  Midas() : fRequestId(-1) { AddData(); std::cout << "HI\n"; };
  virtual ~Midas();
  virtual Bool_t OpenFile(const char* file_name, char** other = 0, int nother = 0);
  virtual Bool_t ConnectOnline(const char* host, const char* other_arg = "", char** other_args = 0, int n_others = 0);
  virtual Bool_t ReadBufferOffline();
  virtual Bool_t ReadBufferOnline();
  virtual Bool_t UnpackBuffer();
  virtual void AddData();
protected:
  static void RunStop(int transition, int run_number, int trans_time);
  static void RunStart(int transition, int run_number, int trans_time);
  static void RunPause(int transition, int run_number, int trans_time);
  static void RunResume(int transition, int run_number, int trans_time);
};
typedef Midas USER_BUFFER_SOURCE;


#endif
