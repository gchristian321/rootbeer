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

#pragma link C++ namespace raw;
#pragma link C++ namespace cal;
#pragma link C++ namespace var;
#pragma link C++ defined_in /user/gchristian/soft/n2analysis/src/mona.hh;

#pragma link C++ class raw::detector+;

// User templates
#pragma link C++ class ExampleData+;
#pragma link C++ class ExampleVariables+;
#pragma link C++ class InnerVariables+;


#endif // #ifdef __CINT__
