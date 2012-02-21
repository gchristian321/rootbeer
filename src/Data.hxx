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
      const char* kBasicClassName;
    public:
      //! Sets kBasicClassName
      MBasic(const char* basicClass) : kBasicClassName(basicClass) {}
      //! Nothing to do
      virtual ~MBasic() {}
      //! Pure virtual, see rb::data::Basic
      virtual Double_t GetValue() = 0;
      //! Pure virtual, see rb::data::Basic
      virtual void SetValue(Double_t newval) = 0;
      //! Search for an instance of Basic*
      //! \param [in] branchName The name (how it would be referred to in TTree::Draw) of the class instance being searched for.
      static MBasic* Find(const char* branchName);
      //! Allocate a \c new instance of data::MBasic.
      //! \returns A heap allocated instance of a data::MBasic-derived class (i.e. some data::Basic template class). It is
      //! automatically caseted to the correct type based on the <i>basic_type_name</i> input parameter.
      static MBasic* New(const char* name, volatile void* addr, const char* basic_type_name, TStreamerElement* element);

      //! Prints or writes to a stream information on each entry in fgAll().
      class Printer
      {
      public:
	//! Save the constructor syntax for each element to a std::ostream
	void SavePrimitive(std::ostream& strm);
	//! Print the full name and value of each data::MBasic instance
	void PrintAll();
      };
    };

    /// \brief Takes care of the actual mapping of names to addresses of each basic data type.
    template <class B>
    class Basic : public MBasic
    {
    private:
      //! Memory address of the basic data
      //! \note Marked volatile so that all access has to be through LockingPointers.
      volatile B * fAddress;
    public:
      /// \brief Sets fAddress and inserts \c this into data::MBasic::fgAll().
      //! \details Also, in the case of an array, it iterates through the whole array and
      //! creates a new instance of data::Basic for each element.
      Basic(const char* name, volatile void* addr, const char* basic_type, TStreamerElement* element);
      //! Return the value of the data stored at fAddress
      Double_t GetValue();
      //! Change the value of the data stored at fAddress
      void SetValue(Double_t newval);
      //! Nothing to do
      virtual ~Basic();
    };

    //! Helper class to perform the mapping of name -> address for basic data members of user classes.
    class Mapper
    {
    public:
      //! Calls Message()
      Mapper(const char* branchName, const char* className);
      //! Recurses through a class structure, and for each basic data member, creates a new
      //! instance of rb::data::Basic.
      void MapClass(const char* name, const char* classname, volatile void* address);
    private:
      //! Helper function called by MapClass, checks if the element a basic data or not.
      Bool_t MapElement(const char* name, TStreamerElement* element, volatile void* base_address);
      //! Adds a message to rb::Rint::fMessage indicating that a class's basic data has been mapped out.
      void Message(const char* brName, const char* clName);
    };
    inline Mapper::Mapper(const char* branchName, const char* className) {
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
namespace
{
  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  //   Helper Functions & Classses  //
  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

  // Add an offset to a void* pointer //
  void void_pointer_add(volatile void*& initial, Int_t offset) {
    volatile char* temp =  reinterpret_cast<volatile char*>(initial);
    temp += offset;
    initial = reinterpret_cast<volatile void*>(temp);
  }
  // Remove adjacent whitespace from a \c std::string //
  void remove_duplicate_spaces(std::string& str) {
    for(int i=str.size()-1; i > 0; --i) {
      if(str[i] == ' ' && str[i-1] == ' ')
	str.erase(i,1);
    }
  }
  // Convert a double to a std::string //
  inline std::string double2str(Double_t d) {
    std::stringstream sstr;
    sstr << d;
    return sstr.str();
  }
  // Tell if a string is longer than the other //
  bool string_len_compare (const std::string& lhs, const std::string& rhs) {
    return rhs.size() > lhs.size();
  }
  // Class to reconstruct the original indices
  // of a multi-dimensional array that has been flattened by TStreamerElements.
  // Gives us strings with the original indices in brackets.
  class ArrayConverter
  {
  public:
    ArrayConverter(TStreamerElement* element) : fElement(element) {
      std::stringstream sstr;
      Int_t ndim = fElement->GetArrayDim();
      std::vector<UInt_t> index(4, 0);

      // Lazy way of "flattening" the array, just use nested for loops.  TStreamerElement
      // only supports up to 4-dimensional arrays anyway so there's no loss. Though really this
      // should be done recursively withouth the for loops (or some other clever way).
      for(index[0] = 0; index[0] < fElement->GetMaxIndex(0); ++index[0]) {
	for(index[1] = 0; index[1] < (ndim > 1 ? fElement->GetMaxIndex(1) : 1) ; ++index[1]) {
	  for(index[2] = 0; index[2] < (ndim > 2 ? fElement->GetMaxIndex(2) : 1) ; ++index[2]) {
	    for(index[3] = 0; index[3] < (ndim > 3 ? fElement->GetMaxIndex(3) : 1) ; ++index[3]) {
	      sstr.str("");
	      for(Int_t i=0; i< ndim; ++i) {
		sstr << "[" << index[i] << "]";
	      }
	      fIndices.push_back(sstr.str());
	    }
	  }
	}
      }
    }
    // Return the original indices.
    std::string GetFullName(const char* baseName, UInt_t index)  {
      if(index < fIndices.size()) {
	std::string out = baseName;
	out += fIndices[index];
	return out;
      }
      else {
	fprintf(stderr, "Invalid index %d\n", index);
	return "";
      }
    }
  private:
    TStreamerElement* fElement;
    std::vector<std::string> fIndices;
  }; // class ArrayConverter
} // namespace 


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Template Class                        //
// rb::data::Basic<B> Implementation     //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class B>
rb::data::Basic<B>::Basic(const char* name, volatile void* addr, const char* basic_type, TStreamerElement* element) :
  MBasic(basic_type), fAddress(reinterpret_cast<volatile B*>(addr)) {
  Int_t arrayLen = element ? element->GetArrayLength() : 0;
  if(arrayLen == 0) { // just a single element
    fgAll().insert(std::make_pair<std::string, MBasic*>(name, this));
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
	MBasic::New(arrayConvert.GetFullName(name, i).c_str(), addr, basic_type, 0);
      }
    }
  }
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
    data::Mapper m (kBranchName, className.c_str());
    m.MapClass(kBranchName, className.c_str(), reinterpret_cast<volatile void*> (fData));
  }

  // Add branch to rb::Hist tree
  LockingPointer<T> pData (fData, fMutex);
  void* v = reinterpret_cast<void*>(pData.Get());
  rb::Hist::AddBranch(kBranchName, className.c_str(), &v);
}
#endif // #ifndef __MAKECINT__
#endif // #ifndef DATA_HXX
