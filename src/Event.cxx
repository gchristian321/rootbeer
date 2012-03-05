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
  LockingPointer<TTree> pTree(fTree, gDataMutex);
  //  pTree->SetDirectory(0);a
  pTree->SetCircular(1); // Allows storage of only one event <<<<<<<<<
  err::Info("Event") ;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Event::Process()                             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Event::Process(void* event_address, Int_t nchar) {
  Bool_t success = false;
  { // Artificial { } block to limit the scope of pTree
    LockingPointer<TTree> pTree(fTree, gDataMutex);
    success = DoProcess(event_address, nchar);
    if(success) {
      pTree->Fill();
      pTree->LoadTree(0);
    }
  }
  if(success) fHistManager.FillAll();
  else HandleBadEvent();
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Bool_t rb::Event::InitFormula::Operate()              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t rb::Event::InitFormula::Operate(rb::Event* const event, const char* formula_arg, FormulaPtr_t& ttf) {
  LockingPointer<TTree> pTree(event->fTree, gDataMutex);
  ttf.reset(new TTreeFormula(formula_arg, formula_arg, pTree.Get())); // Allocate new formula

  LockFreePointer<TTreeFormula> pFormula(ttf.get());
  if(pFormula->GetNdim() == 0)
    return false; // Invalid formula
  else {
    pFormula->SetQuickLoad(true);
    return true;
  }
} 
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Bool_t rb::Event::BranchAdd::Operate()                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t rb::Event::BranchAdd::Operate(rb::Event* const event, const char* name, const char* classname, void** address, Int_t bufsize) {
  TBranch* branch =
    LockingPointer<TTree>(event->fTree, gDataMutex)->Branch(name, classname, address, bufsize, 0);
  return branch != 0;
}
