//! \file Data.hxx
//! \brief Classes and functions relevant to data unpacking.
//! \details Defines the rb::data::Wrapper class, a generic wrapper for user-defined
//! data storage classes. Also defines rb::data::Basic, which allows access to the
//! values of basic data members of user's classes in CINT.
#ifndef DATA_HXX
#define DATA_HXX
#include <iostream>
#include <sstream>
#include <cassert>
#include <vector>
#include <TROOT.h>
#include <TTree.h>
#include <TError.h>
#include <TClass.h>
#include <TDataMember.h>
#include "Event.hxx"
#include "utils/Error.hxx"
#include "utils/LockingPointer.hxx"
#ifndef __MAKECINT__
#include "boost/scoped_ptr.hpp"
#else
namspace boost { template <class T> class scoped_ptr; }
#endif

// #define RB_DATA_ON_STACK
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
	//! Maps the base name of each data class to a pointer to its corresponding
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
	//! Returns a vector containing the names of all variables.
	static std::vector<std::string> GetAll();
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
//! such values can be changed "on the fly" while data is coming in. Because of potential
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
//!    -# A pointer to each instance of data::Basic<T> is stored in the global std::map
//!       data::MBasic::fgAll, keyed by the "name" (leaf name) of its corresponding basic
//!       data member.
//!    -# The class allows (read and write) access to the values of its encapsulated basic data
//!       through SetValue() and GetValue() member functions. Note that each of these functions
//!       locks a mutex, ensuring thread safety.
//!
//! Instances of this class are created from the constructor of rb::data::Wrapper <T>, whenever the user
//! requests that his/her class be mapped by specifying the <i>makeVisible</i> argument of rb::data::Wrapper::Wrapper
//! to true. Individual fields of a class can be excluded from the mapping by including a special comment directly
//! after the field declaration (following the spirit of using comments to direct TStreamer construction in ROOT).
//! There are two options for exclusion comments: the first is <tt>//!</tt>, which will both exclude the field
//! from being mapped by this class, and also exclude it from having a TStreamer created in ROOT (or in other words,
//! it won't show up in any TTree branches or anywhere if the class is written to a ROOT file). Basically, the <tt>//!</tt>
//! works just as in normal ROOT.  The comment block that is specific to ROOTBEER it <tt>//#</tt>.  Use of this comment
//! will prevent the field in question (and any of its sbsequent members) from being mapped by rb::data::Basic <T>, but
//! it will still show up in TTrees, etc. The comments <tt>//!</tt> and <tt>//#</tt> can also be mixed, with the <tt>!</tt>
//! always coming first; this will allow fields to be accessable in CINT but not viewable in TTrees, etc.
//!
//! The intention behind introducing the <tt>//#</tt> option is to allow parameters and variables to be mixed within a class.
//! Typically, one will always want to view parameters event-by-event in hostograms, but will not want to be able to change their
//! values manually. Variables, on the other hand, are something we do want to change manually and may or may not care about
//! viewing in histograms. The present system gives any of these options. For example, say I've done the following:
//! \code
//! // Some external header //
//! struct Data {
//!   Int_t param; //# (parameter) Can't be viewed in CINT, can be put in Trees.
//!   Int_t var;   //  (varaible) Can be viewed in CINT or Trees.
//!   Int_t var2   //! (varaible) Can be viewed in CINT but not Trees.
//!   SomeClass internal; //!# Ignore this both in CINT and in Trees.
//! };
//! // ImportData.h //
//! #include "some_external_header.h"
//! RB_IMPORT_DATA(Data, fData, "data", true, "");
//! RB_IMPORT_DATA(Data, fData, "data2", false, "");
//! \endcode
//!
//! In this case the following would be allowed:
//! \code
//! rb::Hist::New("hparam","",100,0,100,"data.param"); 
//! rb::Hist::New("hvar","",100,0,100,"data.var");
//! // ^^(and likewise replacing "data." with "data2.") //
//!
//! rb::data::GetValue("data.var");    // [or rb::data::SetValue("data.var", 12);]
//! rb::data::GetValue("data.var2");   // [or rb::data::SetValue("data.var2", 123);]
//! \endcode
//!
//! The following would <i>not</i> be allowed:
//! \code
//! rb::Hist::New("hvar2","",100,0,100,"data.var2");
//! rb::Hist::New("hinternal","",100,0,100,"data.internal.....");
//! // ^^(and likewise replacing "data." with "data2.") //
//!
//! rb::data::GetValue("data.param"); //< [likewise for SetValue()]
//! rb::data::GetValue("data.internam...."); //< [likewise for SetValue()]
//! rb::Data::SetValue("data2.ANYTHING"); //< [likewise for SetValue()]
//! \endcode
//!
//! \note In case it isn't clear, the template argument is the actual type of the basic data, e.g. int, double, etc.
template <class T>
class Basic : public MBasic
{
private:
	//! Memory address of the basic data
	//! \note Marked volatile so that all access has to be through LockingPointers.
	volatile T * fAddress;
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
	//! Branch name used for this instance.
	const std::string kBranchName;
	//! Name of the class known to TClass.
	const std::string kClassName;
	//! Base memory address of the class.
	const Long_t kBase;
public:
	//! Set constants, call Message() is callMessage is false
	Mapper(const char* branchname, const char* classname, Long_t base_address, Bool_t call_message);
	//! Recurses through a class structure, and for each basic data member, creates a new
	//! instance of rb::data::Basic.
	void MapClass();
	//! Similar to MapClass(), except if isn't concerned with addresses and it fills an external vector.
	void ReadBranches(std::vector<std::string>& branches);
private:
	//! Handle a basic element, create a new instance of MBasic data for each array element.
	void HandleBasic(TDataMember* d, const char* name);
	//! Insert basic element names into a vector of strings, handles arrays similar to HandleBasic
	void InsertBasic(TDataMember* d, std::vector<std::string>& v_names, const char* name);
	//! Adds a message to rb::Rint::fMessage indicating that a class's basic data has been mapped out.
	void Message();
};
inline Mapper::Mapper(const char* branchname, const char* classname, Long_t base_address, Bool_t call_message) :
	kBranchName(branchname), kClassName(classname), kBase(base_address) {
	if(call_message) Message();
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
#ifdef RB_DATA_ON_STACK
	/// Instance of the user data class.
	T fData;
#else
	/// Pointer to the user data class.
	boost::scoped_ptr<T> fData;
#endif
	/// \brief Pointer-to-pointer of the data.
	//! \details Used for TBranch creation.
	//! \note The void** pointer passed to TTree::Branch needs to stay in scope for the lifetime
	//! of the Branch. This is why we include this here as a class member rather than just assigning
	//! a temporary when creating branches.
	void * fDataVoidPtr;

	/// Does most of the work for the constructor.
	void Init(Event* event, Bool_t makeVisible, const char* args, Int_t bufsize);
public:
	/// \details Allocates memory to the user data class and sets internal variables.
	//!
	//! \param [in] name Name of the user data class. This is how you will refer to it in the
	//! interactive CINT session, i.e. <tt>t->Draw("name.whatever");</tt>. Note that if you
	//! end this with a "<tt>.</tt>", full branch name specifications will be required to
	//! read data from a TTree, e.g. <tt>t->Draw("name.whatever")</tt> required over just
	//! <tt>t->Draw("whatever");</tt>
	//!
	//! \param [in] event Pointer to an rb::Event instance. If this is non-null, then
	//! \c this intance will be added as a branch in the Tree within the event. A null
	//! pointer is ignored and this instance won't be added as a branch in any trees.
	//!
	//! \param [in] makeVisible Specifies whether or not you want to make the class visible
	//! in CINT.  If this is selected true, then you will be able to change or read the values of
	//! basic data types in your user data class using SetData() and GetData().  If selected false,
	//! you won't have direct access to the data (in CINT) and will only be able to view it in histograms.
	//! Generally, one wants to specify true for variables and false for parameters (or add comment
	//! fields as explained in the rb::data::Basic <T> documentation.
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
	Wrapper(const char* name, Event* event = 0, Bool_t makeVisible = true, const char* args = "", Int_t bufsize = 32000);

	/// \brief Constructor version for when no default constructor is available for class \e T
	//! \details Overloads the constructor for cases where no default constructor is available
	//! for the wrapped class. This is done by adding a "dummy" bool as the second argument, but
	//! other than that the use is identical to the other constructor. The reason this is needed is
	//! to allow \e static (compile time) decisions about how to construct the object (new T() vs.
	//! gROOT->ProcessLineFast()). Previously this was only done at runtime which only will compile
	//! if a dfault construtor exists for the class \e T.
	Wrapper(const char* name, Bool_t, Event* event = 0, Bool_t makeVisible = true, const char* args = "", Int_t bufsize = 32000);

	/// \details Frees memory allocated to the user class.
	virtual ~Wrapper();

	/// Pointer access to the internal data.
	//! \returns Pointer to fData.
	T* Get();

	/// Identical to Get()
	//! Included for syntactic compatability with smart pointers (auto_ptr, etc.)
	T* get();

	/// Indirection operator
	//! \returns Pointer to fData.
	T* operator-> ();

	/// Dereference operator
	//! \returns reference to fData.
	T& operator* ();

	/// Pointer access to the internal data (const)
	//! \returns Pointer to fData.
	T* Get() const;

	/// Identical to Get() (const)
	//! Included for syntactic compatability with smart pointers (auto_ptr, etc.)
	T* get() const;

	/// Indirection operator (const)
	//! \returns Pointer to fData.
	T* operator-> () const;

	/// Dereference operator (const)
	//! \returns reference to fData.
	T& operator* () const;
};
} // namespace data
} // namespace rb


#ifndef __MAKECINT__
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Template Class                        //
// rb::data::Basic<T> Implementation     //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
rb::data::Basic<T>::Basic(const char* name, volatile void* addr, const TDataMember* d) :
  MBasic(d), fAddress(reinterpret_cast<volatile T*>(addr)) {
  fgAll().insert(std::make_pair<std::string, MBasic*>(name, this));
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destructor                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline rb::data::Basic<T>::~Basic() {
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Double_t rb::data::Basic<T>::GetValue()   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline Double_t rb::data::Basic<T>::GetValue() {
  LockingPointer<T> p(fAddress, gDataMutex);
  return *p;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::Basic<T>::SetValue()       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline void rb::data::Basic<T>::SetValue(Double_t newval) {
  LockingPointer<T> p(fAddress, gDataMutex);
  *p = T(newval);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Template Class                        //
// rb::data::Wrapper<T> Implementation   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace
{
#ifndef RB_DATA_ON_STACK
// Optional CINT contstruction
template<class T> T* Construct(const char* args) {
	// Allocate new instance of T
	T* data = 0;
	if(!strcmp(args, "")) data = new T(); // default constructor
	else { // non-default constructor, needs gROOT->ProcessLineFast()
		std::stringstream cmd;
		cmd << "new " << TClass::GetClass(typeid(T))->GetName() << "(" << args  << ");";
		data = reinterpret_cast<T*> (gROOT->ProcessLineFast(cmd.str().c_str()));
	}
	if (!data) {
		err::Error("Data::Init") <<
			"Couldn't create a new instance of the template class " << "(typeid.name(): "<< 
			typeid(T).name() << ", constructor arguments: " << args << ").";
		data = 0;
	}
	return data;
}
// Force CINT construction
template<class T> T* Construct(const char* args, Bool_t) {
	// Allocate new instance of T
	T* data = 0;
	// Force use of CINT to construct
	std::stringstream cmd;
	cmd << "new " << TClass::GetClass(typeid(T))->GetName() << "(" << args  << ");";
	data = reinterpret_cast<T*> (gROOT->ProcessLineFast(cmd.str().c_str()));

	if (!data) {
		err::Error("Data::Init") <<
			"Couldn't create a new instance of the template class " << "(typeid.name(): "<< 
			typeid(T).name() << ", constructor arguments: " << args << ").";
		data = 0;
	}
	return data;
}
#endif
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline rb::data::Wrapper<T>::Wrapper(const char* name, Event* event, Bool_t makeVisible,
																		 const char* args, Int_t bufsize):
  kBranchName(name),
#ifdef RB_DATA_ON_STACK
  fData(),  fDataVoidPtr(&fData)
#else
  fData(Construct<T>(args)),  fDataVoidPtr(fData.get())
#endif
{
  Init(event, makeVisible, args, bufsize);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor (v2)           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline rb::data::Wrapper<T>::Wrapper(const char* name, Bool_t, Event* event,
																		 Bool_t makeVisible, const char* args, Int_t bufsize):
	kBranchName(name),
#ifdef RB_DATA_ON_STACK
	fData(), fDataVoidPtr(&fData)
#else
	fData(Construct<T>(args, true)), fDataVoidPtr(fData.get())
#endif
{
#ifdef RB_DATA_ON_STACK
	assert(0 && "Dynamic constructor not available with stack data, recompile with RB_DATA_ON_STACK not defined.");
#endif
	Init(event, makeVisible, args, bufsize);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destructor                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline rb::data::Wrapper<T>::~Wrapper() {
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// T* rb::data::Wrapper<T>::Get()                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline T* rb::data::Wrapper<T>::Get() {
#ifdef RB_DATA_ON_STACK
  return &fData;
#else
  return fData.get();
#endif
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// T* rb::data::Wrapper<T>::operator-> ()                         //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline T* rb::data::Wrapper<T>::get() {
  return Get();
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// T* rb::data::Wrapper<T>::operator-> ()                         //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline T* rb::data::Wrapper<T>::operator->() {
  return Get();
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// T* rb::data::Wrapper<T>::operator() ()                         //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline T& rb::data::Wrapper<T>::operator* () {
#ifdef RB_DATA_ON_STACK
  return fData;
#else
  return *fData;
#endif
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// T* rb::data::Wrapper<T>::Get()                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline T* rb::data::Wrapper<T>::Get() const {
#ifdef RB_DATA_ON_STACK
  return &fData;
#else
  return fData.get();
#endif
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// T* rb::data::Wrapper<T>::operator-> ()                         //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline T* rb::data::Wrapper<T>::get() const {
  return Get();
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// T* rb::data::Wrapper<T>::operator-> ()                         //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline T* rb::data::Wrapper<T>::operator->() const {
  return Get();
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// T* rb::data::Wrapper<T>::operator() ()                         //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
inline T& rb::data::Wrapper<T>::operator* () const {
#ifdef RB_DATA_ON_STACK
  return fData;
#else
  return *fData;
#endif
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::Wrapper<T>::Init()                               //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
template <class T>
void rb::data::Wrapper<T>::Init(Event* event, Bool_t makeVisible, const char* args, Int_t bufsize) {
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
  // Map "visible" classes for CINT
  if (makeVisible) {
#ifdef RB_DATA_ON_STACK
    data::Mapper mapper (kBranchName, cl->GetName(), reinterpret_cast<Long_t>(&fData), true);
#else
    data::Mapper mapper (kBranchName, cl->GetName(), reinterpret_cast<Long_t>(fData.get()), true);
#endif
    mapper.MapClass();
  }
  // Add as a branch in the event's internal tree.
  if(event) {
    Bool_t add_success = Event::BranchAdd::Operate(event, kBranchName, cl->GetName(), &fDataVoidPtr, bufsize);
    if(!add_success) err::Error("data::Wrapper::Init") << "Unsuccessful attempt to add the branch " << kBranchName;
  }
  else err::Error("data::Wrapper::Init") << "Event pointer == 0";
}

#endif // #ifndef __MAKECINT__
#endif // #ifndef DATA_HXX
