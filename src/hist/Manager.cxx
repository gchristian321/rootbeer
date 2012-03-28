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
class HistWrite
{
private:
	 TFile* fFile;
public:
	 HistWrite(TFile* file): fFile(file) { }
	 void operator() (rb::hist::Base* const& hist) {
		 TDirectory* current = hist->GetDirectory();
		 if(!current) return;
		 fFile->cd();
		 hist->SetDirectory(fFile);
		 hist->Write(hist->GetName());
		 hist->SetDirectory(current);
		 current->cd();
	 }
};
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
// void rb::hist::Manager::WriteAll()                    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::hist::Manager::WriteAll(TFile* file) {
	HistWrite write_hist(file);
  LockingPointer<hist::Container_t> pSet(fSet, fSetMutex);
  std::for_each(pSet->begin(), pSet->end(), write_hist);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::hist::Manager::DeleteAll()                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::hist::Manager::DeleteAll() {
  LockingPointer<hist::Container_t> pSet(fSet, fSetMutex);
  Int_t nhists = pSet->size();
	if(!nhists) return;
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
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Base* rb::hist::Manager::FindByTH1()                  //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::Manager::FindByTH1(TH1* hist) {
	LockingPointer<hist::Container_t> pSet(fSet, fSetMutex);
	for(hist::Container_t::iterator it = pSet->begin(); it != pSet->end(); ++it)
	 	 if((*it)->CompareTH1(hist)) return *it;
	return 0;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Base* rb::hist::Manager::FindByName()                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::Manager::FindByName(const char* name, TDirectory* owner) {
	LockingPointer<hist::Container_t> pSet(fSet, fSetMutex);
	for(hist::Container_t::iterator it = pSet->begin(); it != pSet->end(); ++it)
		 if(!strcmp((*it)->GetName(), name)) {
			 if(!owner || (*it)->GetDirectory() == owner) {
				 return *it;
			 }
		 }
	return 0;
}
