//! \file Save.hxx
//! \brief Defines a small class wrapping the objects needed to save a tree to disk.
#ifndef SAVE_HXX
#define SAVE_HXX
#include <vector>
#include <TTree.h>
#include <TFile.h>
#include <TNamed.h>
#include <TROOT.h>
#include "hist/Manager.hxx"
#include "boost_scoped_ptr.h"

namespace rb
{

class Save
{
private:
	 boost::scoped_ptr<TFile> fFile;
	 TTree* fTree;
	 std::vector<void**> fBranchAddr;
	 rb::hist::Manager* fHistManager;
public:
	 Save(): fFile(0), fTree(0), fHistManager(0) { }
	 ~Save() {
		 Stop();
	 }
	 void Start(const char* filename, TTree* tree, const char* name = "", const char* title = "", hist::Manager* manager = 0) {
		 Stop();
		 TDirectory* current = gDirectory;
		 fFile.reset(new TFile(filename, "recreate"));
		 if(fFile->IsZombie()) { fFile.reset(0); return; }
		 // fTree = tree->CloneTree(0);
		 // fTree->SetCircular(-1);

		 fTree = new TTree(tree->GetName(), tree->GetTitle());
		 for(int i=0; i< tree->GetListOfBranches()->GetEntries(); ++i) {
			 TBranch* branch = static_cast<TBranch*>(tree->GetListOfBranches()->At(i));
			 std::string br_name = branch->GetName(), br_clname = branch->GetClassName();
			 fBranchAddr.push_back (reinterpret_cast<void**>(branch->GetAddress()));
			 if(fBranchAddr[i]) {
				 fTree->Branch(br_name.c_str(), br_clname.c_str(), fBranchAddr[i], 3);
			 }
		 }

		 if(strcmp(name, "")) fTree->SetName(name);
		 if(strcmp(title, "")) fTree->SetTitle(title);
		 fHistManager = manager;
		 if(current) current->cd();
		 else gROOT->cd();
	 }
	 void Stop() {
		 if(fFile.get() && fTree) {
			 TDirectory* current = gDirectory;
			 fFile->cd();
		 	 fTree->GetCurrentFile();
		 	 fTree->Write(fTree->GetName());
			 if(fHistManager)
					fHistManager->WriteAll(fFile.get());
			 if(current) current->cd();
			 else gROOT->cd();
			 fTree->ResetBranchAddresses();
			 fFile.reset(0);
			 fTree = 0;
			 fBranchAddr.clear();
		 }
	 }
	 void Fill() {
		 if(fTree) fTree->Fill();
	 }
};

}



#endif
