//! \file Event.cxx
//! \brief Implements Event.hxx
#include "Event.hxx"
#include "hist/Hist.hxx"
#include "utils/Logger.hxx"

namespace rb { rb::Mutex gDataMutex("gDataMutex"); }

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                                 //
// rb::Event                                             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::Event::Event(): fTree(new TTree("tree", "Rootbeer event tree")),
		    fHistManager() {
  LockingPointer<TTree> pTree(fTree, gDataMutex);
//  pTree->SetDirectory(0);
  pTree->SetCircular(1); // Allows storage of only one event
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Event::Process()                             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Event::Process(void* event_address, Int_t nchar) {
  Bool_t success = false;
  {
    rb::ScopedLock<TVirtualMutex> cint_lock (gCINTMutex);
    LockingPointer<TTree> pTree(fTree, gDataMutex);
    success = DoProcess(event_address, nchar);
    if(success) {
      pTree->Fill();
      pTree->LoadTree(0);
    }
  } // Locks go out of scope & unlock
  if(success) fHistManager.FillAll();
  else HandleBadEvent();
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::Event::GetBranchList()                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
std::vector< std::pair<std::string, std::string> > rb::Event::GetBranchList() {
	LockingPointer<TTree> pTree(fTree, gDataMutex);
	std::vector< std::pair<std::string, std::string> > out;
	for(Int_t i=0; i< pTree->GetListOfBranches()->GetEntries(); ++i) {
		TBranch* branch = static_cast<TBranch*>(pTree->GetListOfBranches()->At(i));
		out.push_back(std::make_pair<std::string, std::string> (branch->GetName(), branch->GetClassName()));
	}
	return out;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::Event::FindHistogram()                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::Event::FindHistogram(TH1* th1) {
	return fHistManager.FindByTH1(th1);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::Event::FindHistogram()                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::Event::FindHistogram(const char* name, TDirectory* owner) {
	return fHistManager.FindByName(name, owner);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Bool_t rb::Event::InitFormula::Operate()              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
TTreeFormula* rb::Event::InitFormula::Operate(rb::Event* const event, const char* formula_arg) {
  //  LockingPointer<TTree> pTree(event->fTree, gDataMutex);
  LockFreePointer<TTree> pTree(event->fTree);
  return new TTreeFormula(formula_arg, formula_arg, pTree.Get());
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Bool_t rb::Event::BranchAdd::Operate()                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t rb::Event::BranchAdd::Operate(rb::Event* const event, const char* name, const char* classname, void** address, Int_t bufsize) {
  TBranch* branch =
    LockingPointer<TTree>(event->fTree, gDataMutex)->Branch(name, classname, address, bufsize, 0);
  return branch != 0;
}
