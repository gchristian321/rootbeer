/** \file HistLinkdef.h */
/*! \brief Tells CINT to generate dictionaries for our classes. */
 
 
#ifdef __CINT__ 
#pragma link off all globals; 
#pragma link off all classes; 
#pragma link off all functions; 
#pragma link C++ nestedclasses; 
 
#pragma link C++ namespace rb;
#pragma link C++ class rb::Hist+;
#pragma link C++ class rb::SummaryHist+;
#pragma link C++ class rb::GHist+;
#pragma link C++ class std::list<rb::Hist*>;
#pragma link C++ class std::list<rb::Hist*>::iterator;

#endif // #ifdef __CINT__
