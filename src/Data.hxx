//! \file Data.hxx
//! \brief Classes and functions relevant to data unpacking.
//! \details Defines the rb::data::Wrapper class, a generic wrapper for user-defined
//! data storage classes. Also defines rb::data::Basic, which allows access to the
//! values of basic data members of user's classes in CINT.
#ifndef DATA_HXX
#define DATA_HXX
#include <iostream>
#include <sstream>
#include <TClass.h>
#include <TStreamerInfo.h>
#include <TStreamerElement.h>
#include <TDataMember.h>
#include "Hist.hxx"

namespace rb
{
  namespace data
  {
    /// \brief Abstract base class facilitating access to the values of basic data members of user's classes in CINT.
    //! \details See data::Basic <T> documentation for more info.
    class MBasic
    {
    public:
      typedef std::map<std::string, MBasic*> Map_t;
    protected:
      //! Maps the base name of each data class to a pointer to its correcponding
      //! data::MBasic instance
      static MBasic::Map_t& fgAll();
      //! The type of basic data (int, double, etc.)
      const TDataMember* fDataMember;
    public:
      //! Sets fDataMember
      MBasic(const TDataMember* d);
      //! Nothing to do
      virtual ~MBasic() {}
      //! Pure virtual, see rb::data::Basic
      virtual Double_t GetValue() = 0;
      //! Pure virtual, see rb::data::Basic
      virtual void SetValue(Double_t newval) = 0;
      //! Search for an instance of Basic*
      //! \param [in] leafName The name (how it would be referred to in TTree::Draw) of the class instance being searched for.
      static MBasic* Find(const char* leafName);
      //! Allocate a \c new instance of data::MBasic.
      //! \returns A heap allocated instance of a data::MBasic-derived class (i.e. some data::Basic template class). It is
      //! automatically caseted to the correct type based on the <i>basic_type_name</i> input parameter.
      static void New(const char* name, volatile void* addr, TDataMember* element);

      //! Prints or writes to a stream information on each entry in fgAll.
      class Printer
      {
      public:
	//! Save the constructor syntax for each element to a std::ostream
	void SavePrimitive(std::ostream& strm);
	//! Print the full name and value of each data::MBasic instance
	void PrintAll();
      };
    };
    inline rb::data::MBasic::MBasic(const TDataMember* d) :
      fDataMember(d) {}
    inline MBasic::Map_t& MBasic::fgAll() {
      static MBasic::Map_t* m = new MBasic::Map_t();
      return *m;
    }

    /// \brief Allows access to basic data members of user classes in CINT.
    //! \details The motivation behind this class is to allow users to have safe
    //! access to the basic data members of their classes in CINT. Typically this is
    //! desired for data representing, e.g. calibration variables and the like, so that
    //! such values can be changed "on the fly" while data is coming in. Because of potantial
    //! conflicts with the background thread performing data unpacking, calibration, etc. it is
    //! not safe to give CINT access to the user's classes directly. Instead, what we basically
    //! do is create a map of the memory address of each basic data type (e.g. int, double, float, etc)
    //! that is a member of the user's classes. This class then allows access (read or write) to the
    //! values of those basic data members, keyed by their "name."  Note that here the "name" of the
    //! basic data member essentially refers to it's name in the tree structure defined by the user's
    //! class. Or in other words, it's the leaf name that TTree would give to the member if you had
    //! called TTree::Branch using an instance of your class.
    //!
    //! The basic funtion of this template class is three-fold:
    //!    -# It encapsulates the memory address of the each basic data member.
    //!    -# A pointer to each instance of data::Basic<B> is stored in the global std::map
    //!       data::MBasic::fgAll, keyed by the "name" (leaf name) of its corresponding basic
    //!       data member.
    //!    -# The class allows (read and write) access to the values of its encapsulated basic data
    //!       through SetValue() and GetValue() member functions. Note that each of these functions
    //!       locks a mutex, ensuring thread safety.
    //!
    //! Instances of this class are created from the constructor of rb::data::Wrapper <T>, whenever the user
    //! requests that his/her class be mapped by specifying the <i>makeVisible</i> argument of rb::data::Wrapper::Wrapper
    //! to true. 
    //!
    //! In case it isn't clear, the template argument is the actual type of the basic data, e.g. int, double, etc.
    template <class B>
    class Basic : public MBasic
    {
    private:
      //! Memory address of the basic data
      //! \note Marked volatile so that all access has to be through LockingPointers.
      volatile B * fAddress;
    public:
      /// \brief Sets fAddress and inserts \c this into data::MBasic::fgAll.
      //! \details Also, in the case of an array, it iterates through the whole array and
      //! creates a new instance of data::Basic for each element.
      Basic(const char* name, volatile void* addr, const TDataMember* d);
      //! Return the value of the data stored at fAddress
      Double_t GetValue();
      //! Change the value of the data stored at fAddress
      void SetValue(Double_t newval);
      //! Nothing to do
      virtual ~Basic();
    };

    //! Helper class to perform the actual mapping of name -> address for basic data members of user classes.
    class Mapper
    {
    private:
      //! Base address of the class.
      Long_t fBase;
    public:
      //! Calls Message()
      Mapper(const char* branchName, const char* className, Long_t baseAddress);
      //! Recurses through a class structure, and for each basic data member, creates a new
      //! instance of rb::data::Basic.
      void MapClass(const char* name, const char* classname);
    private:
      //! Handle a basic element, create a new instance of MBasic data for each array element.
      void HandleBasic(TDataMember* d, const char* name);
      //! Adds a message to rb::Rint::fMessage indicating that a class's basic data has been mapped out.
      void Message(const char* brName, const char* clName);
    };
    inline Mapper::Mapper(const char* branchName, const char* className, Long_t baseAddress) :
      fBase(baseAddress) {
      Message(branchName, className);
    }

    //! \brief Provides access to the user's data classes in compiled code.
    //! \details Instances of the user data classes are wrapped by this class as a <tt>volatile T*</tt>
    //! pointer. In compiled code, the data classes are only accessable via the GetLockingPointer() method,
    //! which ensures thread safety. If the makeVisible flag is set to true in the constructor, the addresses
    //! of the basic data types are mapped by the parent class MData, which allows the user to access them in
    //! CINT using rb::data::GetValue() and rb::data::SetValue().
    template <class T>
    class Wrapper
    {
    private:
      /// \brief The name assigned to the TTree branch starting with this class instance.
      //! \details For example, if we have some class:
      //! \code
      //! class C {
      //!    int a;
      //!    // etc //
      //! };
      //! \endcode
      //! and set kBranchName to <tt>fC</tt>, then TTree branch names would look like
      //! <tt>fC.a</tt>, etc.
      const char* kBranchName;

      /// Pointer to the user data class.
      //! \note Volatile so that we have to use a LockingPointer to get access, ensuring thread safety.
      volatile T* fData;

      /// Mutex to protect access to fData.
      rb::Mutex fMutex;

      /// Does most of the work for the constructor.
      void Init(Bool_t makeVisible, const char* args = "");

    public:
      /// \details Allocates memory to the user data class and sets internal variables.
      //!
      //! \param [in] name Name of the user data class. This is how you will refer to it in the
      //! interactive CINT session, i.e. <tt>t->Draw("name.whatever");</tt>. Note that if you
      //! end this with a "<tt>.</tt>", full branch name specifications will be required to
      //! read data from a TTree, e.g. <tt>t->Draw("name.whatever")</tt> required over just
      //! <tt>t->Draw("whatever");</tt>
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
      //! rb::data::Wrapper<MyClass>("myClassName", kFALSE /*(not visible)*/, "\"mine\", 37");
      //! \endcode
      //! If this argument is left as the default (empty string), the user class is constructed
      //! without any arguments, i.e. <tt>new MyClass()</tt>.
      //!
      //! \note Typically in rootbeer, this constructor is not called explicitly. Instead, it is
      //! automatically invoked at the appropriate place once the user calls a corresponding
      //! instance of the RB_IMPORT_DATA macro.
      Wrapper(const char* name, Bool_t makeVisible = kFALSE, const char* args = "");

      //! \details Frees memory allocated to the user class.
      virtual ~Wrapper();

      /// \brief Return a scoped and locked pointer to the user class.
      //! \details Returns a CountedLockingPointer wrapping the user class.
      //! This allows thread safe access to the data in the scope that it's
      //! needed while also ensuring that dynamic resources are freed properly.
      //! To use (e.g. in your implementaion of rb::BufferSource::UnpackBuffer()):
      //! \code
      //! CountedLockingPointer<MyClass> p = gMyClass.GetPointer();
      //! p->DoSomething();
      //! \endcode
      //! \warning If you follow the example above (e.g. create a specific
      //! instance of CountedLockingPointer<MyClass>, the mutex will not be
      //! released until this pointer goes out of scope.  If you are compiling
      //! without DEBUG defined, this will result in a deadlock condition (basically
      //! the program will freeze).  Compiling with DEBUG defined will cause the program
      //! to exit with an error message.
      CountedLockingPointer<T> GetPointer();

      /// \brief Return a scoped and locked pointer to the user class.
      //! \details Same function as GetPointer(), but allowing for more pointer-like
      //! behavior (it's almost the same as normal dereferencing, but the \c * is needed
      //! twice). Example:
      //! \code
      //! (**gMyClass).DoSomething();
      //! // or //
      //! do_something (**gMyClass);
      //! \endcode
      CountedLockingPointer<T> operator* ();

      /// \brief Return a scoped and locked pointer to the user class.
      //! \details Alows a rb::data::Wrapper<T> reference to be used like a pointer, e.g.
      //! \code
      //! gMyClass->DoSomething();
      //! \endcode
      CountedLockingPointer<T> operator-> ();
    };
  } // namespace data
} // namespace rb




#ifndef __MAKECINT__
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Template Class                        //
// rb::data::Basic<B> Implementation     //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class B>
rb::data::Basic<B>::Basic(const char* name, volatile void* addr, const TDataMember* d) :
  MBasic(d), fAddress(reinterpret_cast<volatile B*>(addr)) {
  fgAll().insert(std::make_pair<std::string, MBasic*>(name, this));
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destructor                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class B>
inline rb::data::Basic<B>::~Basic() {}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Double_t rb::data::Basic<B>::GetValue()   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class B>
inline Double_t rb::data::Basic<B>::GetValue() {
  LockingPointer<B> p(fAddress, rb::Hist::GetMutex());
  return *p;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::Basic<B>::SetValue()       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class B>
inline void rb::data::Basic<B>::SetValue(Double_t newval) {
  LockingPointer<B> p(fAddress, rb::Hist::GetMutex());
  *p = B(newval);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Template Class                        //
// rb::data::Wrapper<T> Implementation   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline rb::data::Wrapper<T>::Wrapper(const char* name, Bool_t makeVisible, const char* args) :
  kBranchName(name) {
  Init(makeVisible, args);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destructor                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline rb::data::Wrapper<T>::~Wrapper() {
  LockingPointer<T> pData(fData, fMutex);
  delete pData.Get();
  fData = 0;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// AutoLockingPointer<T> rb::data::Wrapper<T>::GetPointer()       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline CountedLockingPointer<T> rb::data::Wrapper<T>::GetPointer() {
  CountedLockingPointer<T> out (fData, &fMutex);
  return out;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// AutoLockingPointer<T> rb::data::Wrapper<T>::operator()         //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline CountedLockingPointer<T> rb::data::Wrapper<T>::operator* () {
  CountedLockingPointer<T> out (fData, &fMutex);
  return out;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// AutoLockingPointer<T> rb::data::Wrapper<T>::operator-> ()      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline CountedLockingPointer<T> rb::data::Wrapper<T>::operator->() {
  CountedLockingPointer<T> out (fData, &fMutex);
  return out;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::Wrapper<T>::Init()                               //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
void rb::data::Wrapper<T>::Init(Bool_t makeVisible, const char* args) {
  // Figure out class name
  TClass* cl = TClass::GetClass(typeid(T));
  if(!cl) {
    Error("Data::Init",
	  "CINT Does not know about a class you asked it to create "
	  "(typeid.name(): %s, constructor arguments: %s). "
	  "Check UserLinkdef.h to make sure a dictionary is properly generated.",
	  typeid(T).name(), args);
    return;
  }
  std::string className = cl->GetName();

  // Allocate new instance of T
  T* data = 0;
  if(!strcmp(args, "")) data = new T(); // default constructor
  else { // non-default constructor, needs gROOT->ProcessLineFast()
    std::stringstream cmd;
    cmd << "new " << className << "(" << args  << ");";
    data = reinterpret_cast<T*> (gROOT->ProcessLineFast(cmd.str().c_str()));
  }
  if (!data) {
    Error("Data::Init",
	  "Couldn't create a new instance of the template class "
	  "(typeid.name(): %s, constructor arguments: %s).",
	  typeid(T).name(), args);
    return;
  }
  fData = data;

  // Map "visible" classes for CINT
  if (makeVisible) {
    data::Mapper m (kBranchName, className.c_str(), reinterpret_cast<Long_t>(fData));
    m.MapClass(kBranchName, className.c_str());
  }

  // Add branch to rb::Hist tree
  LockingPointer<T> pData (fData, fMutex);
  void* v = reinterpret_cast<void*>(pData.Get());
  rb::Hist::AddBranch(kBranchName, className.c_str(), &v);
}
#endif // #ifndef __MAKECINT__
#endif // #ifndef DATA_HXX
