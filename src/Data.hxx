//! \file Data.hxx
//! \brief Classes and functions relevant to data unpacking.
//! \details Defines the rb::Data class, a generic wrapper for user-defined
//!  data storage classes.
#ifndef DATA_HXX
#define DATA_HXX
#include <assert.h>
#include <memory>
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
#include "midas/rbMidasEvent.h"
#include "midas/TMidasFile.h"


class MBasicData;
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
    typedef std::map<std::string, Data*> Map_t;
    typedef std::map<std::string, MBasicData*> BasicDataMap_t;

    /// Name of the class instance.
    const std::string kName;

    /// For keeping track of whether or not MapData() needs to print the header lines.
    static Bool_t& PrintHeader() { static Bool_t* b = new Bool_t(kTRUE); return *b; }

    /// Maps kName to the base class address.
    static Map_t& fgMap() { static Map_t* m = new Map_t(); return *m; }

    /// Maps the full name of a basic data type to an MBasicDataType pointer wrapping it.
    static BasicDataMap_t& fgBasicDataMap() { static BasicDataMap_t* m = new BasicDataMap_t(); return *m; }

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
    /// Recurse through a class and add each of it's basic data objects to fgBasicDataMap
    virtual void MapClass(const char* name, const char* classname, volatile void* address);

    /// \brief Adds a specific element to fgObjectMap.
    //! \details For a specific element of a class, check if it's a basic data type. If so, add it to
    //! fgBasicDataMap. If not, return false.
    //! \note Called by MapClass()
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
    //! without and arguments, i.e. <tt>new MyClass()</tt>.
    TData(const char* name, Bool_t makeVisible = kFALSE, const char* args = "");

    //! \details Frees memory allocated to the user class and removes \c this from fgMap.
    virtual ~TData();

    /// \brief Return a scoped and locked pointer to the user class.
    //! \details Returns a std::auto_ptr to a LockedPointer wrapping the user class.
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
    inline std::auto_ptr<LockingPointer<T> > GetLockedData();
  };
} // namespace rb

// Implementation of TData functions
#define IMPLEMENT_DATA_TEMPLATES
#include "Data.cxx"
#undef  IMPLEMENT_DATA_TEMPLATES



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
