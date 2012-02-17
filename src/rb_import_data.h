//! \file rb_import_data.h
//! \brief Defines the RB_IMPORT_DATA macro.
//! \details The actual definition will depend on the location from which
//!  this file is #included, basically this allows the macro to be put in just
//!  one place (ImportData.h) but used differently in multiple others.


//! \def RB_IMPORT_DATA(CLASS, SYMBOL, NAME, VISIBLE, ARGS)
//! \brief Adds a global instance of a user's class to rootbeer.
//! \details Users should invoke this macro in ImportData.hh for every
//! instance of their classes they want to include globally in ROOTBEER. Global
//! inclusion is needed to make histograms or use class data in calculations.
//! The function of the macro is to create a global instance of rb::Data<T> wrapping
//! the user's class, and to ensure that it's visible in the source files where it's needed.
//! \param CLASS The typename of the user's class.
//! \param SYMBOL The c++ variable name to be used for the global instance.
//! \param NAME String specifying the top-level branch name you want to be used for this instance.
//! \param VISIBLE Boolean specifying whether or not the class should be visible in CINT (using
//! rb::MData::SetValue() and rb::Data::GetValue(). Generally, set this to true for variables and
//! false for parameters.
//! \param ARGS String literal specifying constructor arguments to be used for this instance of the
//! user's class. A null string (<tt>""</tt>) will call the default constructor.
#define RB_IMPORT_DATA(CLASS, SYMBOL, NAME, VISIBLE, ARGS)
#undef RB_IMPORT_DATA // Doxygen trick //

#if defined RB_ALLOCATE  // [rb::Rint::Rint()]
#define RB_IMPORT_DATA(CLASS, SYMBOL, NAME, VISIBLE, ARGS)	\
  SYMBOL = new rb::Data<CLASS> (NAME, VISIBLE, ARGS);
#undef RB_ALLOCATE

#elif defined RB_DEALLOCATE // [rb::Rint::Terminate()]
#define RB_IMPORT_DATA(CLASS, SYMBOL, NAME, VISIBLE, ARGS)	\
  delete SYMBOL;
#undef RB_DEALLOCATE

#elif defined RB_EXTERN // [Rootbeer.hxx]
#define RB_IMPORT_DATA(CLASS, SYMBOL, NAME, VISIBLE, ARGS)	\
  extern rb::Data<CLASS>* SYMBOL;
#undef RB_EXTERN

#elif defined RB_INIT // [Rootbeer.cxx]
#define RB_IMPORT_DATA(CLASS, SYMBOL, NAME, VISIBLE, ARGS)	\
  rb::Data<CLASS> * SYMBOL = 0;
#undef RB_INIT

#endif

#include "../user/ImportData.h"
#undef RB_IMPORT_DATA
