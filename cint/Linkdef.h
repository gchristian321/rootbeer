//! \file Linkdef.h
//! \brief Tells CINT to generate dictionaries for our classes.
 
 
#ifdef __CINT__ 
#pragma link off all globals; 
#pragma link off all classes; 
#pragma link off all functions; 
#pragma link C++ nestedclasses; 
 
#pragma link C++ namespace rb; 
#pragma link C++ namespace rb::data;
#pragma link C++ namespace rb::canvas;
#pragma link C++ class rb::Rint+;
#pragma link C++ class rb::MData+; 

#include "../user/UserLinkdef.h"

#endif // #ifdef __CINT__
