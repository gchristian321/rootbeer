//! \file MidasLinkdef.h
//! \brief Tells CINT to generate dictionaries for out midas classes.
 
#ifdef __MAKECINT__ 
#pragma link off all globals; 
#pragma link off all classes; 
#pragma link off all functions; 
#pragma link C++ nestedclasses; 

#pragma link C++ defined_in ../src/midas/MidasBuffer.hxx;
#pragma link C++ defined_in ../src/midas/TMidasEvent.h;
#pragma link C++ defined_in ../src/midas/TMidasFile.h;
#pragma link C++ defined_in ../src/midas/TMidasStructs.h;

#endif // #ifdef __CINT__
