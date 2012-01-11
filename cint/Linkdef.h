/** \file Linkdef.h */
/*! \brief Tells CINT to generate dictionaries for our classes. */
 
 
#ifdef __CINT__ 
#pragma link off all globals; 
#pragma link off all classes; 
#pragma link off all functions; 
#pragma link C++ nestedclasses; 
 
#pragma link C++ namespace rb; 
#pragma link C++ namespace rb::Canvas;
#pragma link C++ namespace rb::CutG;
 
#pragma link C++ class rb::Data+; 

#include "../user/UserLinkdef.h"

#endif // #ifdef __CINT__
