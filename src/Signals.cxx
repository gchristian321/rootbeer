//! \file Signals.cxx
//! \brief Implements rb::Signals
#include <TROOT.h>
#include <TString.h>
#include <TGMsgBox.h>
#include <TGFileDialog.h>
#include "TGSelectDialog.h"
#include "TGDivideSelect.h"
#include "Signals.hxx"
#include "Rootbeer.hxx"
#include "Event.hxx"
#include "Data.hxx"
#include "Gui.hxx"
#include "HistGui.hxx"
#include "utils/Error.hxx"

namespace { void error_box(const char* message, const char* title = "Error") {
	new TGMsgBox(gClient->GetRoot(), 0, title, message);
} }

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
void rb::Signals::WriteConfig() {
	rb::WriteConfig("todo");
}
void rb::Signals::ReadConfig() {
	rb::ReadConfig("todo");
}
void rb::Signals::WriteHistConfig() {
	rb::WriteHistograms("todo");
}
void rb::Signals::WriteVariableConfig() {
	rb::WriteVariables("todo");
}
void rb::Signals::ReadCanvasConfig() {
	rb::ReadCanvases("todo");
}
void rb::Signals::WriteCanvasConfig() {
	rb::WriteCanvases("todo");
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
void rb::Signals::ZeroAll() {
	rb::canvas::ClearAll();
}
void rb::Signals::ZeroCurrent() {
	rb::canvas::ClearCurrent();
}
void rb::Signals::ClearCurrent() {
	if(gPad) gPad->GetCanvas()->Clear();
	gPad->Modified();
	gPad->Update();
}
void rb::Signals::CreateNew() {
	std::string name = fEntryName->GetText();
	if(name == "") name = "c1";
	std::stringstream ssname(name);
	int nn = 1;
	while(gROOT->GetListOfCanvases()->FindObject(ssname.str().c_str())) {
		ssname.str(""); ssname << name << "_n" << nn++;
	}
	const UInt_t ww = 696;
	const UInt_t hh = 472;
	static UInt_t xpos = 1130;
	static UInt_t ypos = 20;
	if(xpos+ww > gClient->GetDisplayWidth())
		 xpos = gClient->GetDisplayWidth() - ww;
	if(ypos+hh > gClient->GetDisplayHeight())
		 ypos = gClient->GetDisplayHeight() - hh;
	new TCanvas(ssname.str().c_str(), ssname.str().c_str(), xpos, ypos, ww, hh);
	xpos += 20;
	ypos += 30;
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



// =========== HIST ============ //
namespace { void hist_field_enable(bool onoff, int which) {
	void* fields[3][4] =
		 { { (void*)fParamX, (void*)fBinsX, (void*)fLowX, (void*)fHighX }, 
			 { (void*)fParamY, (void*)fBinsY, (void*)fLowY, (void*)fHighY },
			 { (void*)fParamZ, (void*)fBinsZ, (void*)fLowZ, (void*)fHighZ } };
	TGComboBox* param = ((TGComboBox*)fields[which][0]);
	if(param->IsEnabled() != onoff) {
		if(!onoff) { // disabling
			param->EnableTextInput(false);
			if(param->GetTextEntry()) param->GetTextEntry()->SetText("");
			param->SetEnabled(false);
		}
		else { // enabling
			param->SetEnabled(true);
			param->EnableTextInput(true);
			if(param->GetTextEntry()) param->GetTextEntry()->SetText("");
		}
	}
	for(int i=1; i< 4; ++i)
		 ((TGTextEntry*)fields[which][i])->SetEnabled(onoff);
}
Int_t get_dim(Int_t code) {
	if (code == 0 || code == 3 || code == 4 || code == 5 || code == 8)
		 return 1;
	if (code == 1 || code == 6)
		 return 2;
	return 3;
}
std::string dimcode(int i) {
	static bool init(false);
	static std::map<int, std::string> m;
	if(!init) {
		init = true;
		m[0] = "x"; m[1] = "y"; m[2] = "z";
	}
	return m[i];
}
std::string to_upper(const std::string& str) {
	TString tstr(str.c_str());
	tstr.ToUpper();
	return std::string(tstr.Data());
}
void hist_error(const char* message) {
	error_box(message);
	fHistCreateButton->SetDown(false);
}
void populate_combo(TGComboBox* combo, const std::vector<std::string>& entries, UInt_t max_height) {
	combo->RemoveAll();
	int n(0);
	for(std::vector<std::string>::const_iterator it = entries.begin(); it != entries.end(); ++it)
		 combo->AddEntry(it->c_str(), n++);

	TGListBox* box = combo->GetListBox();
	TGDimension size = box->GetDefaultSize();
	size.fHeight = box->GetItemVsize() * box->GetNumberOfEntries();
	if(size.fHeight > max_height) size.fHeight = max_height;
	box->Resize(size);
}
} //namespace

void rb::Signals::PopulateParameters(Int_t event_code) {
	rb::Event* event = gApp()->GetEvent(event_code);
	if(!event) {
		err::Error("rb::Signals::PopulateParameters")
			 << "Invalid event code (" << event_code << ") recieved from fEventEntry.";
		fParamX->RemoveAll();
		fParamY->RemoveAll();
		fParamZ->RemoveAll();
		return;
	}
	static std::map<Int_t, std::vector<std::string> > branches;
	if(!branches.count(event_code)) {
		std::vector<std::pair<std::string, std::string> > top_branches = event->GetBranchList();
		std::vector<std::string> event_branches;
		std::vector<std::pair<std::string, std::string> >::iterator it;
		for(it = top_branches.begin(); it != top_branches.end(); ++it) {
			data::Mapper mapper(it->first.c_str(), it->second.c_str(), 0, false);
			mapper.ReadBranches(event_branches);
		}
		branches.insert(std::make_pair(event_code, event_branches));
	}
	if(fParamX->IsEnabled()) populate_combo(fParamX, branches[event_code], 400);
	if(fParamY->IsEnabled()) populate_combo(fParamY, branches[event_code], 400);
	if(fParamZ->IsEnabled()) populate_combo(fParamZ, branches[event_code], 400);
}

void rb::Signals::PopulateEvents() {
	fEventEntry->RemoveAll();
	rb::EventVector_t events = rb::gApp()->GetEventVector();
	if(!events.size()) return;
	std::stringstream entry;
	for(unsigned i=0; i< events.size(); ++i) {
		entry << events[i].second << " [ code: "
					<< events[i].first  << " ]";
		fEventEntry->AddEntry(entry.str().c_str(), events[i].first);
	}
	fEventEntry->Select(events[0].first);
	fEventEntry->Selected(events[0].first);
}
Bool_t rb::Signals::IsHistFromGui() {
	Bool_t ret = fHistFromGui;
	fHistFromGui = false;
	return ret;
}
void rb::Signals::EnableHistFields(Int_t code) {
//	1d, 2d, 3d, summary [h], summary [v], gamma1, gamma2, gamma3, bit
//   0,  1,  2,     3,          4,          5,      6,      7,     8
	if (get_dim(code) == 1) {
		hist_field_enable(false, 1);
		hist_field_enable(false, 2);
	}
	else if (get_dim(code) == 2) {
		hist_field_enable(true, 1);
		hist_field_enable(false, 2);
	}
	else {
		hist_field_enable(true, 1);
		hist_field_enable(true, 2);
	}
}

void rb::Signals::CreateHistogram() {
	Int_t type = fTypeEntry->GetSelected();
	Int_t bins[3] = { fBinsX->GetNumber(), fBinsY->GetNumber(), fBinsZ->GetNumber() };
	Int_t evt = fEventEntry->GetSelected();
	Double_t low[3]  = { fLowX->GetNumber(), fLowY->GetNumber(), fLowZ->GetNumber() };
	Double_t high[3] = { fHighX->GetNumber(), fHighY->GetNumber(), fHighZ->GetNumber() };
	std::string param[3] = { "", "", "" };
	if(fParamX->GetTextEntry()) param[0] = fParamX->GetTextEntry()->GetText();
	if(fParamY->GetTextEntry()) param[1] = fParamY->GetTextEntry()->GetText();
	if(fParamZ->GetTextEntry()) param[2] = fParamZ->GetTextEntry()->GetText();

	std::string gate = fGateEntry->GetText(),
		 name = fNameEntry->GetText(),
		 title = fTitleEntry->GetText();
	
	if(name.empty()) {
		hist_error("Please specify a histogram name.");
		return;
	}
	for(int i=0; i< get_dim(type); ++i) {
		if(param[i].empty()) {
			std::stringstream msg;
			msg << "Please specify a(n) " << dimcode(i) << " axis parameter.";
			hist_error(msg.str().c_str());
			return;
		}
		if(high[i] < low[i] && type != 8) {
			std::stringstream msg;
			msg << to_upper(dimcode(i))
					<< " axis upper limit is less than the corresponding lower limit.";
			hist_error(msg.str().c_str());
			return;
		}
	}
	std::string paramarg = "";
	for(int i= get_dim(type)-1; i >= 0; --i) {
		paramarg += param[i]; paramarg += ":";
	}
	paramarg = paramarg.substr(0, paramarg.size()-1);
	std::stringstream cmd;
	try {
		SetHistFromGui();
		switch(type) {
		case 0: // 1D
			rb::hist::New(name.c_str(), title.c_str(), bins[0], low[0], high[0], paramarg.c_str(), gate.c_str(), evt);
			break;
		case 1: // 2D
			rb::hist::New(name.c_str(), title.c_str(), bins[0], low[0], high[0], bins[1], low[1], high[1], paramarg.c_str(), gate.c_str(), evt);
			break;
		case 2: // 3D
			rb::hist::New(name.c_str(), title.c_str(), bins[0], low[0], high[0], bins[1], low[1], high[1], bins[2], low[2], high[2], paramarg.c_str(), gate.c_str(), evt);
			break;
		case 3: // summary [h]
			rb::hist::NewSummary(name.c_str(), title.c_str(), bins[0], low[0], high[0], paramarg.c_str(), gate.c_str(), evt, "h");
			break;
		case 4: // summary [v]
			rb::hist::NewSummary(name.c_str(), title.c_str(), bins[0], low[0], high[0], paramarg.c_str(), gate.c_str(), evt, "v");
			break;
		case 5: // 1D gamma
			rb::hist::NewGamma(name.c_str(), title.c_str(), bins[0], low[0], high[0], paramarg.c_str(), gate.c_str(), evt);
			break;
		case 6: // 2D gamma
			rb::hist::NewGamma(name.c_str(), title.c_str(), bins[0], low[0], high[0], bins[1], low[1], high[1], paramarg.c_str(), gate.c_str(), evt);
			break;
		case 7: // 3D gamma
			rb::hist::NewGamma(name.c_str(), title.c_str(), bins[0], low[0], high[0], bins[1], low[1], high[1], bins[2], low[2], high[2], paramarg.c_str(), gate.c_str(), evt);
			break;
		case 8: // Bit
			rb::hist::NewBit(name.c_str(), title.c_str(), bins[0], paramarg.c_str(), gate.c_str(), evt);
			break;
		default:
			break;
		}
	}
	catch (std::exception& e) {
		hist_error(e.what());
	}
}
