//! \file Linkdef.h
//! \brief Tells CINT to generate dictionaries for our classes.
 
#ifdef __MAKECINT__ 
#pragma link off all globals; 
#pragma link off all classes; 
#pragma link off all functions; 
#pragma link C++ nestedclasses; 
 
#pragma link C++ namespace rb; 
#pragma link C++ namespace rb::data;
#pragma link C++ namespace rb::hist;
#pragma link C++ namespace rb::canvas;
#pragma link C++ class rb::Rint+;
#pragma link C++ class rb::Signals+;
#pragma link C++ class rb::HistSignals+;
// #pragma link C++ class rb::Save+;
#pragma link C++ class rb::Event+;
#pragma link C++ class TGSelectDialog+;
#pragma link C++ class TGDivideSelect+;

#pragma link C++ defined_in ../src/ClassData.hxx;
#pragma link C++ class rb::ClassFormula+;
#pragma link C++ class rb::ClassMemberParser+;
#pragma link C++ class rb::hist::Base+;
#pragma link C++ class rb::hist::D1+;
#pragma link C++ class rb::hist::D2+;
#pragma link C++ class rb::hist::D3+;
#pragma link C++ class rb::hist::Summary+;
#pragma link C++ class rb::hist::Scaler+;
#pragma link C++ class rb::hist::Gamma+;
#pragma link C++ class rb::hist::Bit+;
#pragma link C++ class AxisIndices;

// #pragma link C++ class TMidasOnline+;


#pragma link C++ defined_in ../src/utils/Timer.hxx;
#pragma link C++ defined_in ../src/Attach.hxx;

#endif // #ifdef __CINT__
