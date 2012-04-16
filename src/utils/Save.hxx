//! \file Save.hxx
//! \brief Defines a small class wrapping the objects needed to save a tree to disk.
#ifndef SAVE_HXX
#define SAVE_HXX
/*
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
	 std::vector<TTree*> fTrees;
	 std::vector<std::vector<void**> > fBranchAddr;
	 std::vector<rb::hist::Manager*> fHistManagers;
public:
	 Save(const char* filename): fFile(0) {
		 TDirectory* current = gDirectory;
		 fFile.reset(new TFile(filename, "recreate"));
		 if(fFile->IsZombie()) { fFile.reset(0); }
		 if(current) current->cd();
		 else gROOT->cd();
	 }

	 ~Save() {
		 Stop();
	 }

	 void AddEvent(TTree* tree, const char* name = "", const char* title = "", hist::Manager* manager = 0) {
		 if(!fFile.get()) return;
		 fTrees.push_back(new TTree(tree->GetName(), tree->GetTitle()));
		 std::vector<void**> addresses;
		 std::string br_name = "", br_clname = "";
		 for(int i=0; i< tree->GetListOfBranches()->GetEntries(); ++i) {
			 TBranch* branch = static_cast<TBranch*>(tree->GetListOfBranches()->At(i));
			 br_name = branch->GetName();
			 br_clname = branch->GetClassName();
			 addresses.push_back (reinterpret_cast<void**>(branch->GetAddress()));
		 }
		 fBranchAddr.push_back(addresses);
		 for(int i=0; i< tree->GetListOfBranches()->GetEntries(); ++i) {
			 if( *(addresses.end()-1)[i]) {
				 fTrees[i]->Branch(br_name.c_str(), br_clname.c_str(), *(addresses.end()-1)[i]);
			 }
			 if(strcmp(name, "")) fTrees[i]->SetName(name);
			 if(strcmp(title, "")) fTrees[i]->SetTitle(title);
		 }

		 fHistManagers.push_back(manager);
	 }

	 void Stop() {
		 if(fFile.get()) {
			 TDirectory* current = gDirectory;

			 for(size_t i=0; i< fTrees.size(); ++i) {
				 fTrees[i]->GetCurrentFile();
				 fTrees[i]->Write(fTrees[i]->GetName());
				 if(fHistManagers[i]) {
					 fHistManagers[i]->WriteAll(fFile.get());
				 }
				 fTrees[i]->ResetBranchAddresses();
			 }
			 --fgNstarts;
			 if(!fgNstarts) {
				 fFile.reset(0);
				 fTrees.clear();
				 for(size_t i=0; i< fBranchAddr.size(); ++i) {
					 fBranchAddr[i].clear();
				 }
				 fBranchAddr.clear();
			 }
			 if(current) current->cd();
			 else gROOT->cd();
		 }
	 }

	 void Fill() {return;
		 for(std::vector<TTree*>::iterator it = fTrees.begin(); it!=fTrees.end(); ++it) {
			 if(*it) (*it)->Fill();
		 }
	 }

};

}

*/

#endif
