//! \file Linkdef.h
//! \brief Tells CINT to generate dictionaries for our classes.
 
#ifdef __CINT__ 
#pragma link off all globals; 
#pragma link off all classes; 
#pragma link off all functions; 
#pragma link C++ nestedclasses; 
 
#pragma link C++ namespace rb; 
#pragma link C++ namespace rb::gui;

#pragma link C++ class rb::gui::Canvas+;
#pragma link C++ class rb::gui::MainFrame+;

#endif // #ifdef __CINT__
