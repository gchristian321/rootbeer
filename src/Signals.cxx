//! \file Signals.cxx
//! \brief Implements rb::Signals
#include <TROOT.h>
#include <TString.h>
#include <TGFileDialog.h>
#include "TGSelectDialog.h"
#include "TGDivideSelect.h"
#include "Signals.hxx"
#include "Rootbeer.hxx"
#include "Gui.hxx"


void rb::Signals::UpdateBufferCounter(Int_t n, Bool_t force) {
	// if(!fNBuffers) return;
	// if(n % 100 != 0 && !force) return;
	// std::stringstream sstr;
	// sstr << n;
	// fNBuffers->ChangeText(sstr.str().c_str());
}

void rb::Signals::AttachOnline() { printf("todo\n"); }

void rb::Signals::AttachFile() {
	Bool_t continuous = !fIsContinuous->IsOn();

	TGFileInfo fileInfo;
	const char* ext[] =
		 { "MIDAS Files", "*.mid",
			 "Zipped MIDAS Files", "*.mid.gz",
			 "NSCL Event Files", "*.evt",
			 "All Files", "*",
			 0, 0 };
	fileInfo.fFileTypes = reinterpret_cast<const char**>(ext);
#ifdef RB_DEFAULT_FILE_DIRECTORY
	static TString dirInit = RB_DEFAULT_FILE_DIRECTORY ;
#else
	static TString dirInit = "." ;
#endif
	fileInfo.fIniDir = StrDup(dirInit);

	new TGFileDialog(gClient->GetRoot(), 0, kFDOpen, &fileInfo);
	if(fileInfo.fFilename != 0)
		 rb::AttachFile(fileInfo.fFilename, continuous);
	fAttachFile->SetDown(false);
}

void rb::Signals::AttachList() {
	TGFileInfo fileInfo;
	const char* ext[] =
		 { "dat files", "*.dat",
			 "txt files", "*.txt",
			 "All Files", "*",
			 0, 0 };
	fileInfo.fFileTypes = reinterpret_cast<const char**>(ext);
#ifdef RB_DEFAULT_FILE_DIRECTORY
	static TString dirInit = RB_DEFAULT_FILE_DIRECTORY ;
#else
	static TString dirInit = "." ;
#endif
	fileInfo.fIniDir = StrDup(dirInit);

	new TGFileDialog(gClient->GetRoot(), 0, kFDOpen, &fileInfo);
	if(fileInfo.fFilename != 0)
		 rb::AttachList(fileInfo.fFilename);
	fAttachList->SetDown(false);
}
void rb::Signals::Quit() {
	std::cout << "\n";
	gApp()->Terminate(0);
}
void rb::Signals::Unattach() {
	rb::Unattach();
}
void rb::Signals::UpdateAll() {
	rb::canvas::UpdateAll();
}
void rb::Signals::UpdateCurrent() {
	rb::canvas::UpdateCurrent();
}
void rb::Signals::ClearAll() {
	rb::canvas::ClearAll();
}
void rb::Signals::ClearCurrent() {
	rb::canvas::ClearCurrent();
}
void rb::Signals::CreateNew() {
	std::string name = fEntryName->GetText();
	if(name == "") name = "c1";
	std::stringstream ssname(name);
	int nn = 1;
	while(gROOT->GetListOfCanvases()->FindObject(ssname.str().c_str())) {
		ssname.str(""); ssname << name << "_n" << nn++;
	}
	new TCanvas(ssname.str().c_str(), ssname.str().c_str(), 500, 10, 696, 472);
}
void rb::Signals::Update() {
	if(!rb::canvas::GetUpdateRate()) {
		int rate = fUpdateRate->GetIntNumber();
		rb::canvas::StartUpdate(rate);
	}
	else
		 rb::canvas::StopUpdate();
}
void rb::Signals::ChangeUpdateRate(Long_t rate) {
	if(rb::canvas::GetUpdateRate())
		 rb::canvas::StartUpdate(fUpdateRate->GetIntNumber());
}


namespace
{
std::map<std::string, TPad*> pads;

bool is_divided(TPad* p) {
	for(int i=0; i< p->GetListOfPrimitives()->GetEntries(); ++i)
		 if(dynamic_cast<TPad*>(p->GetListOfPrimitives()->At(i))) return true;
	return false;
}

void get_end_pads(TPad* canvas) {
	TList* primitives = canvas->GetListOfPrimitives();
	for(int i=0; i < primitives->GetEntries(); ++i) {
			TPad* pad = dynamic_cast<TPad*>(primitives->At(i));
			if(!pad) continue;
			if(!is_divided(pad))
				 pads.insert(std::make_pair<std::string, TPad*>(pad->GetName(), pad));
			else {
				pads.insert(std::make_pair<std::string, TPad*>(pad->GetName(), pad));
				get_end_pads(pad);
			}
	}
} }
void rb::Signals::SyncCanvases() {
	pads.clear();
	for(int i=0; i< gROOT->GetListOfCanvases()->GetEntries(); ++i) {
		TPad* pad = dynamic_cast<TPad*>(gROOT->GetListOfCanvases()->At(i));
		/*if(!is_divided(pad))*/ pads.insert(std::make_pair<std::string, TPad*>(pad->GetName(), pad));
		get_end_pads(pad);
	}
}

void rb::Signals::CdCanvas() {
	SyncCanvases();
	std::vector<std::string> names;
	for(std::map<std::string, TPad*>::iterator it = pads.begin(); it!=pads.end(); ++it)
		 names.push_back(it->first);
	int which = -1;
	new TGSelectDialog(gClient->GetRoot(), 0, "Select Canvas:", "Select Canvas", &names, &which);
	if(which >= 0 && which < (int)names.size()) {
		pads[names[which]]->cd();
	}
	gPad->Modified();
	gPad->Update();
	fSelectCanvas->SetDown(false);
}

void rb::Signals::DivideCurrent() {
	if(!gPad) return;
	int h = -1, v = -1;
	new TGDivideSelect(gClient->GetRoot(), 0, &h, &v);
	if(h > 0 && v > 0) {
		gPad->Clear();
		gPad->Divide(h, v);
		if(h==1 && v == 1) gPad->cd();
		else gPad->cd(1);
	}
	gPad->Modified();
	gPad->Update();
	fDivideCurrent->SetDown(false);
}
