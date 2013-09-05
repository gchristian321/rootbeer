/// \file strlcpy.hxx
/// \brief Header file for strlcpy.cxx
/********************************************************************\

   Name:         strlcpy.h
   Created by:   Stefan Ritt

   Contents:     Header file for strlcpy.c

   $Id: strlcpy.h 62 2007-10-23 17:53:45Z sawada $

\********************************************************************/

/*
	Modified by G. Christian, 2013-08-29 to become part of the ROOTBEER package. Changes
	are as follows:
	 - Extension changes to .hxx (intended for C++ compilation).
	 - Functions placed under the `rb` namespace.
*/


#ifndef RB_STRLCPY_H_
#define RB_STRLCPY_H_



#ifndef EXPRT
#if defined(EXPORT_DLL)
#define EXPRT __declspec(dllexport)
#else
#define EXPRT
#endif
#endif

namespace rb {

size_t EXPRT strlcpy(char *dst, const char *src, size_t size);
size_t EXPRT strlcat(char *dst, const char *src, size_t size);

}

#endif /*_STRLCPY_H_ */
