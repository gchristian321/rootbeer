/** \file HistLinkdef.h */
/*! \brief Tells CINT to generate dictionaries for our classes. */
 
 
#ifdef __CINT__ 
#pragma link off all globals; 
#pragma link off all classes; 
#pragma link off all functions; 
#pragma link C++ nestedclasses; 
 
#pragma link C++ namespace rb;
#pragma link C++ class rb::Hist+;
#pragma link C++ class AxisIndices;
/* #pragma link C++ class rb::SummaryHist+; */
/* #pragma link C++ class rb::GammaHist+; */
/* #pragma link C++ class rb::BitHist<1>+; */
/* #pragma link C++ class rb::BitHist<2>+; */
/* #pragma link C++ class rb::BitHist<4>+; */
/* #pragma link C++ class rb::BitHist<8>+; */
/* #pragma link C++ class rb::BitHist<16>+; */
/* #pragma link C++ class rb::BitHist<32>+; */
/* #pragma link C++ class rb::BitHist<64>+; */

#endif // #ifdef __CINT__
