/** \file Linkdef.h */
/*! \brief Tells CINT to generate dictionaries for our classes. */


#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

#pragma link C++ namespace rb;
#pragma link C++ namespace rb::canvas;

#pragma link C++ class sData+;
#pragma link C++ class sVariables+;

#pragma link C++ class rb::Hist+;
#pragma link C++ class rb::H1D+;
#pragma link C++ class rb::H2D+;
#pragma link C++ class rb::H3D+;

#pragma link C++ class rb::MData+;
#pragma link C++ class rb::Data<sData>+;
#pragma link C++ class rb::Data<sVariables>+;


#endif // #ifdef __CINT__
