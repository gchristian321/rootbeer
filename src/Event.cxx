//! \file Event.cxx
//! \brief Implements Event.hxx
#include "Event.hxx"
#include "hist/Hist.hxx"



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Helper Functions & Classes                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace { struct HistFill { Int_t operator() (rb::hist::Base* const& hist) {
    return hist->FillUnlocked();
  } } fill_hist; }

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                                 //
// rb::Event                                             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::Event::Event():
  fTree(new TTree("tree", "Rootbeer event tree")),
  fHistManager() {
  LockingPointer<TTree>(fTree.get(), gDataMutex)->SetDirectory(0);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Event::Process()                             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Event::Process(void* event_address, Int_t nchar) {
  Bool_t success = false;
  {
    ScopedLock<rb::Mutex> lock(gDataMutex);
    success = DoProcess(event_address, nchar);
  }
  if(success) fHistManager.FillAll();
  else HandleBadEvent();
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Event::InitFormula::Operate()                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t rb::Event::InitFormula::Operate(rb::Event* const event, const char* formula_arg, FormulaPtr_t& ttf) {
  LockingPointer<TTree> pTree(event->fTree.get(), gDataMutex);
  ttf.reset(new TTreeFormula(formula_arg, formula_arg, pTree.Get()));
  return ttf.get() ? LockFreePointer<TTreeFormula>(ttf.get())->GetNdim() != 0 : false;
} 
