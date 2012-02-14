//! \file Skeleton.hxx
//! \brief Defines some macros used in Skeleton.cxx

//! \brief Adds user-defined classes to ROOTBEER.
//! \details This macro creates a \c new instance of rb::Data, wrapping a user-defined class.
//! \param NAME The variable name that you want to assign to your class instance.
//! \param CLASS_NAME The name of the class.
//! \param CINT_ACCESS Boolean specifying whether or not end users should have access to the class
//!  data members from CINT.
//!
//!  Example:
//! \code
//! class MyClass {
//!   public:
//!   MyClass();
//!   // -- ETC -- //
//!   };
//!
//!   ADD_CLASS_INSTANCE(my_class_instance, MyClass, false);
//! \endcode
//!  See the documentation on rb::Data for more information.
#define ADD_CLASS_INSTANCE(NAME, CLASS_NAME, CINT_ACCESS)		\
  static rb::TData<CLASS_NAME>   NAME##_DataF (#NAME, CINT_ACCESS);	\
  rb::Data* NAME##_Data = &NAME##_DataF;

//! \brief Same as ADD_CLASS_INSTANCE, but for non-default constructors
//! \param ARGS Arguments to the user class's constructor, specified as a string.
//!
//!  Example:
//! \code
//! class MyClass {
//!   public:
//!   MyClass(int a, double b, string name);
//!   // -- ETC -- //
//!   };
//!
//!   ADD_CLASS_INSTANCE_ARGS(my_class_instance, MyClass, true, "12, 31.4, \"my_class_name\"");
//! \endcode
#define ADD_CLASS_INSTANCE_ARGS(NAME, CLASS_NAME, CINT_ACCESS, ARGS)	\
  static rb::TData<CLASS_NAME>   NAME##_DataF (#NAME, CINT_ACCESS, ARGS); \
  rb::Data* NAME##_Data = &NAME##_DataF;


//! \brief Creates an instance of LockingPointer with which you can safely access your data.
//! \param SYMBOL The pointer symbol.
//! \param NAME The variable name of the class instance you need to access.
//! \param CLASS The class you're getting a pointer to.
//! 
//! Example:
//! \code
//! ADD_CLASS_INSTANCE(my_class_instance, MyClass, false);
//! // -- ETC -- //
//! GET_LOCKING_POINTER(pMine, my_class_instance, MyClass);
//! pMine->MemberFunction();
//! \endcode
// #define GET_LOCKING_POINTER(SYMBOL, NAME, CLASS)			\
//   auto_ptr<LockingPointer<CLASS> > temp_12312312312 = ((rb::TData<CLASS>*)NAME##_Data)->GetLockedData(); \
//   LockingPointer<CLASS>& SYMBOL = *temp_12312312312;
