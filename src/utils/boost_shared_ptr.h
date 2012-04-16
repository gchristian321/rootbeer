//! \file boost_shared_ptr.h
//! \brief #includes \c boost/shared_ptr.hpp if not in rootcint,
//! otherwise just provides a forward declaration.
#ifndef __MAKECINT__
#include "boost/shared_ptr.hpp"
#else
namespace boost { template <class T> class shared_ptr<T>; }
#endif
