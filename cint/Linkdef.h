/** \file Linkdef.h */
/*! \brief Tells CINT to generate dictionaries for our classes. */
 
 
#ifdef __CINT__ 
#pragma link off all globals; 
#pragma link off all classes; 
#pragma link off all functions; 
#pragma link C++ nestedclasses; 
 
#pragma link C++ namespace rb; 
#pragma link C++ namespace rb::canvas; 
 
#pragma link C++ class rb::Data+; 


// User templates
#pragma link C++ class ExampleData+;
#pragma link C++ class ExampleVariables+;
#pragma link C++ class InnerVariables+;


#endif // #ifdef __CINT__
