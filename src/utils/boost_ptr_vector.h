#ifndef __MAKECINT__
#include "boost/ptr_container/ptr_vector.hpp"
#else
namespace boost { template <class T> class ptr_vector<T>; }
#endif
