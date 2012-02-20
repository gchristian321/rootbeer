//! \file Data.hxx
//! \brief Classes and functions relevant to data unpacking.
//! \details Defines the rb::Data class, a generic wrapper for user-defined
//! data storage classes.
#ifndef DATA_HXX
#define DATA_HXX
#include <string>
#include <iostream>
#include <sstream>
#include <TError.h>
#include "Hist.hxx"

namespace rb
{
  //! \brief Base class for allowing access to user data classes.
  //! \details Along with its template derived classes rb::Data<T>, this
  //! allows users access to their data classes both in compiled codes (e.g.
  //! rb::BufferSource::UnpackBuffer() implementations) and, optionally, in CINT.
  //! CINT access is available for basic data types (int, double, etc.), using the
  //! GetValue() and SetValue() methods. The memory adddress of each basic data type
  //! contained within a user's class is keyed and accessed  by it's full name (the
  //! same string that TTree::Branch() would generate for the corresponding leaf).
  class MData
  {
  protected:
    /// Name of the class instance.
    const std::string kName;

  public:
    /// For keeping track of whether or not MapData() needs to print the header lines.
    static Bool_t& PrintHeader() { static Bool_t* b = new Bool_t(kTRUE); return *b; }

    /// Sets kName.
    MData(const char* name) : kName(name) {};

    /// Nothing to do.
    virtual ~MData() {}

    /// Write the user class basic data members and their current values to a stream.
    static void SavePrimitive(std::ostream& ofs);

    /// Get the value of a user class data member.
    //! \param [in] name The full name (TTree leaf) of the data member. For example:
    //! \code
    //! struct AStruct {
    //!   int a;
    //! } mine;
    //! \endcode
    //! would be accessed by
    //! \code
    //! rb::MData::GetValue("mine.a");
    //! \endcode
    //! \returns The value of the basic data member keyed by <i>name</i>, casted
    //! to a Double_t.
    static Double_t GetValue(const char* name);

    /// Set the value of a user class data member.
    //! \param [in] name The full name (TTree leaf) of the data member (see GetValue() for
    //! an example).
    //! \param [in] newvalue What you want to set the data keyed by <i>name</i> to.
    static void SetValue(const char* name, Double_t newvalue);

    /// Print the fill name and current value of every data member in every listed class.
    static void PrintAll();

    /// Recurse through a class and construct an instance of MBasicData for each basic data type.
    virtual void MapClass(const char* name, const char* classname, volatile void* address);

  protected:
    /// \brief Adds a specific element to fgObjectMap.
    //! \details For a specific element of a class, check if it's a basic data type. If so, construct
    //! an MBasicData.  If not, return false.
    virtual Bool_t MapData(const char* name, TStreamerElement* element, volatile void* base_address);
  };


  //! \brief Prvides access to the user's data classes.
  //! \details Instances of the user data classes are wrapped by this class as a <tt>volatile T*</tt>
  //! pointer. In compiled code, the data classes are only accessable via the GetLockingPointer() method,
  //! which ensures thread safety. If the makeVisible flag is set to true in the constructor, the addresses
  //! of the basic data types are mapped by the parent class MData, which allows the user to access them in
  //! CINT using MData::GetValue() and MData::SetValue().
  template <class T>
  class Data
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

    /// String specifying the type of the user data class.
    std::string fClassName;

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
    //! rb::Data<MyClass>("myClassName", kFALSE /*(not visible)*/, "\"mine\", 37");
    //! \endcode
    //! If this argument is left as the default (empty string), the user class is constructed
    //! without any arguments, i.e. <tt>new MyClass()</tt>.
    //!
    //! \note Typically in rootbeer, this constructor is not called explicitly. Instead, it is
    //! automatically invoked at the appropriate place once the user calls a corresponding
    //! instance of the RB_IMPORT_DATA macro.
    Data(const char* name, Bool_t makeVisible = kFALSE, const char* args = "");

    //! \details Frees memory allocated to the user class.
    virtual ~Data();

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
    //! \details Alows a rb::Data<T> reference to be used like a pointer, e.g.
    //! \code
    //! gMyClass->DoSomething();
    //! \endcode
    CountedLockingPointer<T> operator-> ();

  };
} // namespace rb



#ifndef __MAKECINT__
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                                 //
// rb::Data<T> Implementation                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline rb::Data<T>::Data(const char* name, Bool_t makeVisible, const char* args) :
  kBranchName(name) {
  Init(makeVisible, args);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destructor                                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline rb::Data<T>::~Data() {
  LockingPointer<T> pData(fData, fMutex);
  delete pData.Get();
  fData = 0;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// AutoLockingPointer<T> rb::Data<T>::GetPointer()       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline CountedLockingPointer<T> rb::Data<T>::GetPointer() {
  CountedLockingPointer<T> out (fData, &fMutex);
  return out;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// AutoLockingPointer<T> rb::Data<T>::operator()         //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline CountedLockingPointer<T> rb::Data<T>::operator* () {
  CountedLockingPointer<T> out (fData, &fMutex);
  return out;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// AutoLockingPointer<T> rb::Data<T>::operator-> ()      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline CountedLockingPointer<T> rb::Data<T>::operator->() {
  CountedLockingPointer<T> out (fData, &fMutex);
  return out;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Data<T>::Init()                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
void rb::Data<T>::Init(Bool_t makeVisible, const char* args) {
  TClass* cl = TClass::GetClass(typeid(T));
  if(!cl) {
    Error("Data::Init",
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
    Error("Data::Init",
	  "Couldn't create a new instance of the template class "
	  "(typeid.name(): %s, constructor arguments: %s).",
	  typeid(T).name(), args);
    return;
  }

  fData = data;

  if (makeVisible) {
    if(MData::PrintHeader()) {
      std::cout << "\nMapping the address of user data objects:\n"
		<< "      Name\t\t\tClass Name\n"
		<< "      ----\t\t\t----------\n";
      MData::PrintHeader() = kFALSE;
    }
    std::cout << "      " << kBranchName << "\t\t\t" << fClassName << "\n";

    MData m(kBranchName);
    m.MapClass(kBranchName, fClassName.c_str(), reinterpret_cast<volatile void*>(fData));
  }


  LockingPointer<T> pData (fData, fMutex);
  void* v = reinterpret_cast<void*>(pData.Get());
  rb::Hist::AddBranch(kBranchName, fClassName.c_str(), &v);
}
#endif // #ifndef __MAKECINT__

#endif // #ifndef DATA_HXX
