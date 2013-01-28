//! \file Event.cxx
//! \brief Implements Event.hxx
#include <cassert>
#include "Event.hxx"
#include "Rint.hxx"
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
										fHistManager(), fSave(new rb::Event::Save(this))
{									
  LockingPointer<TTree> pTree(fTree, gDataMutex);
  pTree->SetDirectory(0);
  pTree->SetCircular(1); // Allows storage of only one event
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Event::Process()                             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Event::Process(const void* event_address, Int_t nchar) {
	RB_LOG << "Processing new event...\n";
  Bool_t success = false;
  {
    rb::ScopedLock<TVirtualMutex> cint_lock (gCINTMutex);
    LockingPointer<TTree> pTree(fTree, gDataMutex);
		LockFreePointer<rb::Event::Save> pSave(fSave);
    success = DoProcess(event_address, nchar);
    if(success) {
      pTree->Fill();
      pTree->LoadTree(0);
			pSave->Fill();
    }
  } // Locks go out of scope & unlock
 if(success) fHistManager.FillAll();
 else HandleBadEvent();
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Event::StartSave()                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Event::StartSave(boost::shared_ptr<TFile> file, const char* name, const char* title, Bool_t save_hists) {
	LockingPointer<rb::Event::Save> pSave(fSave, gDataMutex);
	pSave->Start(file, name, title, save_hists);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Event::StopSave()                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Event::StopSave() {
	LockingPointer<rb::Event::Save> pSave(fSave, gDataMutex);
	pSave->Stop();
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
rb::DataFormula* rb::Event::InitFormula::Operate(rb::Event* const event, const char* formula_arg) {
  LockFreePointer<TTree> pTree(event->fTree);
	TBranch* branch = reinterpret_cast<TBranch*>(pTree->GetListOfBranches()->At(0));
	assert(branch);
	assert(pTree->GetListOfBranches()->GetEntries() == 1);

	LockFreePointer<Long_t> pAddr(event->fClassAddr);
	rb::DirectDataFormula* direct =
		new rb::DirectDataFormula(branch->GetName(), branch->GetClassName(), reinterpret_cast<void*>(*pAddr), formula_arg);
	if(direct->IsZombie() == false) return direct;

	delete direct;
  return new rb::TTreeDataFormula(formula_arg, formula_arg, pTree.Get());
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Bool_t rb::Event::BranchAdd::Operate()                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t rb::Event::BranchAdd::Operate(rb::Event* const event, const char* name, const char* classname, void** address, Int_t bufsize) {
  TBranch* branch =
    LockingPointer<TTree>(event->fTree, gDataMutex)->Branch(name, classname, address, bufsize, 0);
	if(branch) {
		LockingPointer<Long_t> pAddr(event->fClassAddr, gDataMutex);
		*pAddr = reinterpret_cast<Long_t>(*address);
	}
  return branch != 0;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Event::Save::Start()                         //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Event::Save::Start(boost::shared_ptr<TFile> file, const char* name, const char* title, Bool_t save_hists) {
	TDirectory* current = gDirectory;
	fFile = file;
	fFile->cd();
	fSaveHistograms = save_hists;
	LockFreePointer<TTree> pEventTree(fEvent->fTree);
	fTree = new TTree(pEventTree->GetName(), pEventTree->GetTitle());
	if(strcmp(name, "")) fTree->SetName(name);
	if(strcmp(title, "")) fTree->SetTitle(title);
	std::string br_name = "", br_clname = "";
	for(int i=0; i< pEventTree->GetListOfBranches()->GetEntries(); ++i) {
		TBranch* branch = static_cast<TBranch*>(pEventTree->GetListOfBranches()->At(i));
		br_name = branch->GetName();
		br_clname = branch->GetClassName();
		fBranchAddr.push_back(reinterpret_cast<void**>(branch->GetAddress()));
		fTree->Branch(br_name.c_str(), br_clname.c_str(), fBranchAddr.at(i));
	}
	fIsActive = true;
	if(current) current->cd();
	else gROOT->cd();
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Event::Save::Stop()                          //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Event::Save::Stop() {
	if(!fTree) return;
	if(!fFile.get()) return;
	TDirectory* current = gDirectory;
	fFile->cd();
	fTree->GetCurrentFile();
	fTree->Write(fTree->GetName());
	fTree->ResetBranchAddresses();
	if(fSaveHistograms) fEvent->fHistManager.WriteAll(fFile.get());
	if(current) current->cd();
	else gROOT->cd();
	if(fFile.get()) fFile.reset();
	fIsActive = false;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Event::Save::Fill()                          //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Event::Save::Fill() {
	if(fIsActive && fTree) fTree->Fill();
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Event::RunBegin::operator()                  //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Event::RunBegin::operator() (const std::pair<Int_t, std::string>& e) {
	rb::Event* event = gApp()->GetEvent(e.first);
	if(!event) {
		err::Error("rb::Event::RunBegin::operator()") << "gApp()->GetEvent(code = " << e.first << ") returned NULL. "
																									<< "Skipping this event." << ERR_FILE_LINE;
	}
	else {
		event->BeginRun();
	}
}
