//! \file boost_scoped_ptr.h
//! \brief #includes \c boost/scoped_ptr.hpp if not in rootcint,
//! otherwise just provides a forward declaration.
#ifndef __MAKECINT__
#include "boost/scoped_ptr.hpp"
#else
namespace boost { template <class T> class scoped_ptr<T>; }
#endif
