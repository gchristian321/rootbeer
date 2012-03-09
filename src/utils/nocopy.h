//! \file nocopy.h
//! \brief Defines a simple macro to disallow copy and assignment of classes.
//! \note Sets scope to private automatically.
//! \note Using rootcint requires that every function be given an implementation.
//!  Thus this macro won't work on classes with const members or reference members.


#define RB_NOCOPY(class)			\
  private:					\
  class (const class& other) {};		\
  class& operator= (const class& other) { return *this; };
