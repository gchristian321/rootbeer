//! \file boost_ptr_vector.h
//! \brief #includes boost/ptr_container/ptr_vector.hpp if not in rootcint,
//!  otherwise forward declares boost::ptr_vector<T>
#ifndef __MAKECINT__
#include "boost/ptr_container/ptr_vector.hpp"
#else
namespace boost { template <class T> class ptr_vector<T>; }
#endif
