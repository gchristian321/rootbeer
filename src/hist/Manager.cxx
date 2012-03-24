//! \file Manager.cxx
//! \brief Implements manager.hxx
#include "Hist.hxx"
#include "hist/Manager.hxx"



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Helper Functions and Classes                          //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace {
  struct HistFill { Int_t operator() (rb::hist::Base* const& hist) {
    return hist->Fill();
  } } fill_hist;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                                 //
// rb::hist::Manager                                     //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::hist::Manager::FillAll()                     //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::hist::Manager::FillAll() {
  LockingPointer<hist::Container_t> pSet(fSet, fSetMutex);
  std::for_each(pSet->begin(), pSet->end(), fill_hist);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::hist::Manager::DeleteAll()                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::hist::Manager::DeleteAll() {
  LockingPointer<hist::Container_t> pSet(fSet, fSetMutex);
  Int_t nhists = pSet->size();
  std::vector<rb::hist::Base*> addresses(pSet->begin(), pSet->end());
  for(Int_t i=0; i< nhists; ++i) delete addresses[i]; // removes from fSet
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::hist::Manager::Add()                         //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::hist::Manager::Add(rb::hist::Base* hist) {
  LockingPointer<hist::Container_t> pSet(fSet, fSetMutex);
  pSet->insert(hist);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::hist::Manager::Remove()                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::hist::Manager::Remove(rb::hist::Base* hist) {
  LockingPointer<hist::Container_t> pSet(fSet, fSetMutex);
	if(pSet->count(hist)) {
		pSet->erase(hist);
		TDirectory* directory = hist->fDirectory;
		if(directory) directory->Remove(hist);
	}
}
