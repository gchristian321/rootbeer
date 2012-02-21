//! \file Data.hxx
//! \brief Classes and functions relevant to data unpacking.
//! \details Defines the rb::Data class, a generic wrapper for user-defined
//! data storage classes.
#ifndef DATA_HXX
#define DATA_HXX
#include <string>
#include <iostream>
#include <sstream>
#include <TClass.h>
#include <TStreamerInfo.h>
#include <TStreamerElement.h>
#include <TError.h>
#include "Hist.hxx"


// Helper class for performing set and get operations on the
// basic data elements (e.g. ints, doubles, etc.) that are wrapped
// by the class pointed to by the fData pointer of rb::Data
// The basic idea is that rb::Data has a static map keyed by the complete
// string names of all basic data types (e.g. "top.sub.end"), and indexing
// a pointer to the appropriate derived (template) class of MBasicData.
// For example, if "top.sub.end" is a double, then the key "top.sub.end"
// would index a pointer to a BasicData<double>, which is derived from
// MBasic data.
class MBasicData
{
public:
  typedef std::map<std::string, MBasicData*> Map_t;
protected:
  static MBasicData::Map_t& fgAll();
  const char* kBasicClassName;
  const char* kBranchName;
public:
  MBasicData(const char* branch_name, const char* basic_type_name);
  virtual ~MBasicData() { };
  virtual Double_t GetValue() = 0;
  virtual void SetValue(Double_t newval) = 0;
  static MBasicData* Find(const char* name);
  static void SavePrimitive(std::ostream& strm);
  static void PrintAll();
  static MBasicData* New(const char* name, volatile void* addr, const char* basic_type_name, TStreamerElement* element);
 
#ifndef __MAKECINT__
  /// Recurse through a class and construct an instance of MBasicData for each basic data type.
  static void MapClass(const char* name, const char* classname, volatile void* address);
#endif

protected:
  /// \brief Adds a specific element to fgObjectMap.
  //! \details For a specific element of a class, check if it's a basic data type. If so, construct
  //! an MBasicData.  If not, return false.
  static Bool_t MapData(const char* name, TStreamerElement* element, volatile void* base_address);

  // This is just a quick and dirty class to reconstruct the original indices
  // of a multi-dimensional array that has been flattened by TStreamerElements.
  // All we really need out is just strings with the original indices in brackets.
  class ArrayConverter
  {
  private:
    TStreamerElement* fElement;
    std::vector<std::string> fIndices;
  public:
    ArrayConverter(TStreamerElement* element);
    // Return the original indices.
    std::string GetFullName(const char* baseName, UInt_t index);
  };

};
  
template <class ABasic>
class BasicData : public MBasicData
{
private:
  volatile ABasic* fAddress; // Memory address of the basic data

public:
  BasicData(const char* name, volatile void* addr, const char* basic_type, TStreamerElement* element);
  Double_t GetValue();
  void SetValue(Double_t newval);
  virtual ~BasicData() { }
};


#ifndef __MAKECINT__
  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  // void_pointer_add                                      //
  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  // Add an offset to a void* pointer
  extern void void_pointer_add(volatile void*& initial, Int_t offset);

  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  // remove_duplicate_spaces                               //
  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  // Remove adjacent whitespace from a \c std::string
  extern void remove_duplicate_spaces(std::string& str);

  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  // double2str                                            //
  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  // Convert a double to a std::string
  extern std::string double2str(Double_t d);

  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  // string_len_compare                                    //
  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  // Tell is a string is longer than the other
  extern bool string_len_compare (const std::string& lhs, const std::string& rhs);


  //////////////////////////////////////////
  ///// BasicData<ABasic> Implementaion ////
  //////////////////////////////////////////
  template <class ABasic>
  BasicData<ABasic>::BasicData(const char* name, volatile void* addr, const char* basic_type, TStreamerElement* element) :
    MBasicData(name, basic_type), fAddress(reinterpret_cast<volatile ABasic*>(addr)) {
    Int_t arrayLen = element ? element->GetArrayLength() : 0;
    if(arrayLen == 0) { // just a single element
      fgAll().insert(std::make_pair<std::string, MBasicData*>(name, this));
    }
    else { // an array
      if(element->GetArrayDim() > 4) { 
	Warning("MapData", "No support for arrays > 4 dimensions. The array %s is %d and will not be mapped!",
		name, element->GetArrayDim());
      }
      else {
	Int_t size = element->GetSize() / arrayLen;
	ArrayConverter arrayConvert(element);
	for(Int_t i=0; i< arrayLen; ++i) {
	  void_pointer_add(addr, size*(i>0));
	  MBasicData::New(arrayConvert.GetFullName(name, i).c_str(), addr, basic_type, 0);
	}
      }
    }
  }

  template <class ABasic>
  inline Double_t BasicData<ABasic>::GetValue() {
    LockingPointer<ABasic> p(fAddress, rb::Hist::GetMutex());
    return *p;
  }

  template <class ABasic>
  inline void BasicData<ABasic>::SetValue(Double_t newval) {
    LockingPointer<ABasic> p(fAddress, rb::Hist::GetMutex());
    *p = ABasic(newval);
  }

#endif // #ifndef __MAKECINT__


namespace rb
{
  //! \brief Prvides access to the user's data classes.
  //! \details Instances of the user data classes are wrapped by this class as a <tt>volatile T*</tt>
  //! pointer. In compiled code, the data classes are only accessable via the GetLockingPointer() method,
  //! which ensures thread safety. If the makeVisible flag is set to true in the constructor, the addresses
  //! of the basic data types are mapped by the parent class MData, which allows the user to access them in
  //! CINT using rb::data::GetValue() and rb::data::SetValue().
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
    if(1) { /////MData::PrintHeader()) {
      std::cout << "\nMapping the address of user data objects:\n"
		<< "      Name\t\t\tClass Name\n"
		<< "      ----\t\t\t----------\n";
      /////MData::PrintHeader() = kFALSE;
    }
    std::cout << "      " << kBranchName << "\t\t\t" << fClassName << "\n";

    MBasicData::MapClass(kBranchName, fClassName.c_str(), reinterpret_cast<volatile void*> (fData));
  }


  LockingPointer<T> pData (fData, fMutex);
  void* v = reinterpret_cast<void*>(pData.Get());
  rb::Hist::AddBranch(kBranchName, fClassName.c_str(), &v);
}
#endif // #ifndef __MAKECINT__

#endif // #ifndef DATA_HXX
