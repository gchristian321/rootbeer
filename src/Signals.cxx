//! \file Signals.cxx
//! \brief Implements rb::Signals
#include <algorithm>
#include <TROOT.h>
#include <TString.h>
#include <TGMsgBox.h>
#include <TGFileDialog.h>
#include <TGInputDialog.h>
#include "TGSelectDialog.h"
#include "TGDivideSelect.h"
#include "Signals.hxx"
#include "Rootbeer.hxx"
#include "Event.hxx"
#include "Data.hxx"
#include "Rint.hxx"
#include "Gui.hxx"
#include "HistGui.hxx"
#include "hist/Hist.hxx"
#include "utils/Error.hxx"
#include "utils/Assorted.hxx"
#include <iostream>

rb::Signals::Signals() { }

rb::Signals::~Signals() { }

namespace {

ANSort ansort;

void error_box(const char* message, const char* title = "Error") {
	new TGMsgBox(gClient->GetRoot(), 0, title, message);
}

}

void rb::Signals::UpdateBufferCounter(Int_t n, Bool_t force) {
	if(!rb::Rint::gApp()->fRbeerFrame->fNbuffers) return;
	if(n % 1000 != 0 && !force) return;
	std::stringstream sstr;
	sstr << n;
	rb::Rint::gApp()->fRbeerFrame->fNbuffers->ChangeText(sstr.str().c_str());
}

void rb::Signals::SaveData() {
	EnableSaveHists();
	if(rb::Rint::gApp()->fRbeerFrame->fSaveData->IsOn()) {
	        rb::Rint::gApp()->StartSave(rb::Rint::gApp()->fRbeerFrame->fSaveHist->IsOn());
	}
	else {
		rb::Rint::gApp()->StopSave();
	}
}

void rb::Signals::SaveHists() {
	if(rb::Rint::gApp()->fRbeerFrame->fSaveData->IsOn()) {
	        rb::Rint::gApp()->StartSave(rb::Rint::gApp()->fRbeerFrame->fSaveHist->IsOn());
	}
}

void rb::Signals::SetFilterCondition(Int_t key, std::string filter) {
        rb::Rint::gApp()->SetFilterCondition(key, filter);
}

void rb::Signals::EnableSaveHists() {
	rb::Rint::gApp()->fRbeerFrame->fSaveHist->SetEnabled(rb::Rint::gApp()->fRbeerFrame->fSaveData->IsOn());
}

void rb::Signals::AttachOnline() {
	rb::AttachOnline(rb::Rint::gApp()->fRbeerFrame->fEntryHost->GetText(), rb::Rint::gApp()->fRbeerFrame->fEntryPort->GetText());
}

void rb::Signals::AttachFile() {
	Bool_t continuous = !rb::Rint::gApp()->fRbeerFrame->fIsContinuous->IsOn();

	TGFileInfo fileInfo;
	const char* ext[] =
		 { "MIDAS Files", "*.mid",
			 "Zipped MIDAS Files", "*.mid.gz",
			 "NSCL Event Files", "*.evt",
			 "All Files", "*",
			 0, 0 };
	fileInfo.fFileTypes = reinterpret_cast<const char**>(ext);

	TString dirInit = expand_path(kFileStaticDefault, "$RB_FILEDIR");
	fileInfo.fIniDir = StrDup(dirInit);

	new TGFileDialog(gClient->GetRoot(), 0, kFDOpen, &fileInfo);
	if(fileInfo.fFilename != 0)
		 rb::AttachFile(fileInfo.fFilename, continuous);
	rb::Rint::gApp()->fRbeerFrame->fAttachFile->SetDown(false);

}

void rb::Signals::AttachList() {
	TGFileInfo fileInfo;
	const char* ext[] =
		 { "dat files", "*.dat",
			 "txt files", "*.txt",
			 "All Files", "*",
			 0, 0 };
	fileInfo.fFileTypes = reinterpret_cast<const char**>(ext);
	TString dirInit = expand_path(kFileStaticDefault, "$RB_FILEDIR");
	fileInfo.fIniDir = StrDup(dirInit);

	new TGFileDialog(gClient->GetRoot(), 0, kFDOpen, &fileInfo);
	if(fileInfo.fFilename != 0)
		 rb::AttachList(fileInfo.fFilename);
	rb::Rint::gApp()->fRbeerFrame->fAttachList->SetDown(false);
}
void rb::Signals::Unattach() {
	rb::Unattach();
}
void rb::Signals::UpdateAll() {
	rb::canvas::UpdateAll();
	gPad->Modified();
	gPad->Update();
}
void rb::Signals::UpdateCurrent() {
	rb::canvas::UpdateCurrent();
	gPad->Modified();
	gPad->Update();
}
void rb::Signals::ZeroAll() {
	rb::canvas::ClearAll();
	if(gPad) {
		gPad->Modified();
		gPad->Update();
	}
}
void rb::Signals::ZeroCurrent() {
	rb::canvas::ClearCurrent();
	if(gPad) {
		gPad->Modified();
		gPad->Update();
	}
}
void rb::Signals::ClearCurrent() {
	if(gPad) {
		gPad->GetCanvas()->Clear();
		gPad->Modified();
		gPad->Update();
	}	
}
void rb::Signals::CreateNew() {
	std::string name = rb::Rint::gApp()->fRbeerFrame->fEntryName->GetText();
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
		int rate = rb::Rint::gApp()->fRbeerFrame->fUpdateRate->GetIntNumber();
		rb::canvas::StartUpdate(rate);
	}
	else
		 rb::canvas::StopUpdate();
}
void rb::Signals::ChangeUpdateRate(Long_t rate) {
	if(rb::canvas::GetUpdateRate())
		 rb::canvas::StartUpdate(rb::Rint::gApp()->fRbeerFrame->fUpdateRate->GetIntNumber());
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

namespace { 
bool gpad_log_x() { return gPad->GetLogx(); }
bool gpad_log_y() { return gPad->GetLogy(); }
bool gpad_log_z() { return gPad->GetLogz(); }
void gpad_set_log_x(bool on) { gPad->SetLogx(on); }
void gpad_set_log_y(bool on) { gPad->SetLogy(on); }
void gpad_set_log_z(bool on) { gPad->SetLogz(on); }
bool(*isLog[3])()      = { gpad_log_x, gpad_log_y, gpad_log_z };
void(*setLog[3])(bool) = { gpad_set_log_x, gpad_set_log_y, gpad_set_log_z };
}

void rb::Signals::ToggleLog(Int_t axis) {
	if(!gPad) return;
	if(axis < 0 || axis > 2) {
		std::cerr << "Invalid axis: " << axis << "! ::" << __FILE__ << ", " << __LINE__ << "\n";
		return;
	}
	
	TGCheckButton* logButtons[3] =
		{ 0, rb::Rint::gApp()->fRbeerFrame->fLogy, rb::Rint::gApp()->fRbeerFrame->fLogz };
	bool isAxisLog = isLog[axis]();
	setLog[axis](!isAxisLog);
	gPad->Modified();
	gPad->Update();
	logButtons[axis]->SetOn(isAxisLog);
}

void rb::Signals::DoubleClickCanvas(Int_t event, Int_t, Int_t, TObject* selected) {
	if (event == kButton1Double && selected->InheritsFrom(TVirtualPad::Class())) {
		static_cast<TVirtualPad*>(selected)->cd();
		if(gPad){ gPad->Modified(); gPad->Update(); }
	}
}

void rb::Signals::SyncWithGpad() {
	if(!gPad) return;
	rb::Rint::gApp()->fRbeerFrame->fLogy->SetOn(gPad->GetLogy());
	rb::Rint::gApp()->fRbeerFrame->fLogz->SetOn(gPad->GetLogz());
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
	rb::Rint::gApp()->fRbeerFrame->fDivideCurrent->SetDown(false);
}

void rb::Signals::PopulateEvents() {
	rb::Rint::gApp()->fRbeerFrame->fFilterType->RemoveAll();
	rb::EventVector_t events = rb::Rint::gApp()->GetEventVector();
	if(!events.size()) return;
	std::stringstream entry;
	for(unsigned i=0; i< events.size(); ++i) {
		entry.str("");
		entry << events[i].second << " [ code: "
					<< events[i].first  << " ]";
		rb::Rint::gApp()->fRbeerFrame->fFilterType->AddEntry(entry.str().c_str(), events[i].first);
	}
	rb::Rint::gApp()->fRbeerFrame->fFilterType->Select(events[0].first);
	rb::Rint::gApp()->fRbeerFrame->fFilterType->Selected(events[0].first);
}

void rb::Signals::SetFilter() {
  std::string filter = rb::Rint::gApp()->fRbeerFrame->fFilterEntry->GetText();
  Int_t filter_type = rb::Rint::gApp()->fRbeerFrame->fFilterType->GetSelected();
  
  rb::Rint::gApp()->SetFilterCondition(filter_type, filter);
  
  TGTextLBEntry *filter_text_entry = (TGTextLBEntry *)rb::Rint::gApp()->fRbeerFrame->fFilterType->GetSelectedEntry();
  std::string filter_text = filter_text_entry->GetTitle();
  cout << "Filter type: " << filter_text << ".\tValue: " << filter << ".\n";
  
}


// =========== HIST ============ //
rb::HistSignals::HistSignals():
	fHistFromGui (false) { }

rb::HistSignals::~HistSignals() { }

void rb::HistSignals::Quit() {
	std::cout << "\n";
	Rint::gApp()->Terminate(0);
}
namespace {
Int_t get_dim(Int_t code) {
/*
	 fTypeEntry->AddEntry("1D ", ne++); //0
   fTypeEntry->AddEntry("2D ", ne++); //1
   fTypeEntry->AddEntry("3D ", ne++); //2
	 fTypeEntry->AddEntry("Scaler", ne++); //3
   fTypeEntry->AddEntry("Summary [h]", ne++); //4
   fTypeEntry->AddEntry("Summary [v]", ne++); //5
   fTypeEntry->AddEntry("1D gamma ", ne++); //6
   fTypeEntry->AddEntry("2D gamma ", ne++); //7
   fTypeEntry->AddEntry("3D gamma ", ne++); //8
   fTypeEntry->AddEntry("Bit ", ne++); //9
*/
	if (code == 0 || code == 3 || code == 4 || code == 5 || code == 6 || code == 9)
		 return 1;
	if (code == 1 || code == 7)
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
void populate_combo(TGComboBox* combo, const std::vector<std::string>& entries, UInt_t max_height) {
	bool enabled = combo->IsEnabled();
	if(!enabled) combo->SetEnabled();
	combo->RemoveAll();
	int n(0);
	for(std::vector<std::string>::const_iterator it = entries.begin(); it != entries.end(); ++it)
		 combo->AddEntry(it->c_str(), n++);

	TGListBox* box = combo->GetListBox();
	TGDimension size = box->GetDefaultSize();
	size.fHeight = box->GetItemVsize() * box->GetNumberOfEntries();
	if(size.fHeight > max_height) size.fHeight = max_height;
	box->Resize(size);
	if(!enabled) combo->SetEnabled(false);
}
} //namespace

void rb::HistSignals::hist_field_enable(bool onoff, int which) {
	void* fields[3][4] =
		 { { (void*)rb::Rint::gApp()->fHistFrame->fParamX, (void*)rb::Rint::gApp()->fHistFrame->fBinsX, (void*)rb::Rint::gApp()->fHistFrame->fLowX, (void*)rb::Rint::gApp()->fHistFrame->fHighX },
			 { (void*)rb::Rint::gApp()->fHistFrame->fParamY, (void*)rb::Rint::gApp()->fHistFrame->fBinsY, (void*)rb::Rint::gApp()->fHistFrame->fLowY, (void*)rb::Rint::gApp()->fHistFrame->fHighY },
			 { (void*)rb::Rint::gApp()->fHistFrame->fParamZ, (void*)rb::Rint::gApp()->fHistFrame->fBinsZ, (void*)rb::Rint::gApp()->fHistFrame->fLowZ, (void*)rb::Rint::gApp()->fHistFrame->fHighZ } };
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

void rb::HistSignals::hist_error(const char* message) {
	error_box(message);
	rb::Rint::gApp()->fHistFrame->fHistCreateButton->SetDown(false);
}


void rb::HistSignals::PopulateParameters(Int_t event_code) {
	rb::Event* event = Rint::gApp()->GetEvent(event_code);
	if(!event) {
		rb::err::Error("rb::HistSignals::PopulateParameters")
			 << "Invalid event code (" << event_code << ") recieved from rb::Rint::gApp()->fHistFrame->fEventEntry.";
		rb::Rint::gApp()->fHistFrame->fParamX->RemoveAll();
		rb::Rint::gApp()->fHistFrame->fParamY->RemoveAll();
		rb::Rint::gApp()->fHistFrame->fParamZ->RemoveAll();
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
		std::sort(event_branches.begin(), event_branches.end(), ansort);
		branches.insert(std::make_pair(event_code, event_branches));
	}
	populate_combo(rb::Rint::gApp()->fHistFrame->fParamX, branches[event_code], 400);
	populate_combo(rb::Rint::gApp()->fHistFrame->fParamY, branches[event_code], 400);
	populate_combo(rb::Rint::gApp()->fHistFrame->fParamZ, branches[event_code], 400);
}

void rb::HistSignals::PopulateEvents() {
	rb::Rint::gApp()->fHistFrame->fEventEntry->RemoveAll();
	rb::EventVector_t events = rb::Rint::gApp()->GetEventVector();
	if(!events.size()) return;
	std::stringstream entry;
	for(unsigned i=0; i< events.size(); ++i) {
		entry.str("");
		entry << events[i].second << " [ code: "
					<< events[i].first  << " ]";
		rb::Rint::gApp()->fHistFrame->fEventEntry->AddEntry(entry.str().c_str(), events[i].first);
	}
	rb::Rint::gApp()->fHistFrame->fEventEntry->Select(events[0].first);
	rb::Rint::gApp()->fHistFrame->fEventEntry->Selected(events[0].first);
}

Bool_t rb::HistSignals::IsHistFromGui() {
	Bool_t ret = fHistFromGui;
	fHistFromGui = false;
	return ret;
}

void rb::HistSignals::EnableHistFields(Int_t code) {
//	1d, 2d, 3d, scaler, summary [h], summary [v], gamma1, gamma2, gamma3, bit
//   0,  1,  2,    3,       4,          5,          6,      7,      8      9
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


void rb::HistSignals::CreateHistogram() {
	if(GetSelectedHist()) {
		delete GetSelectedHist();
	};

	Int_t type = rb::Rint::gApp()->fHistFrame->fTypeEntry->GetSelected();
	Int_t bins[3] = { rb::Rint::gApp()->fHistFrame->fBinsX->GetNumber(), rb::Rint::gApp()->fHistFrame->fBinsY->GetNumber(), rb::Rint::gApp()->fHistFrame->fBinsZ->GetNumber() };
	Int_t evt = rb::Rint::gApp()->fHistFrame->fEventEntry->GetSelected();
	Double_t low[3]  = { rb::Rint::gApp()->fHistFrame->fLowX->GetNumber(), rb::Rint::gApp()->fHistFrame->fLowY->GetNumber(), rb::Rint::gApp()->fHistFrame->fLowZ->GetNumber() };
	Double_t high[3] = { rb::Rint::gApp()->fHistFrame->fHighX->GetNumber(), rb::Rint::gApp()->fHistFrame->fHighY->GetNumber(), rb::Rint::gApp()->fHistFrame->fHighZ->GetNumber() };
	std::string param[3] = { "", "", "" };
	if(rb::Rint::gApp()->fHistFrame->fParamX->GetTextEntry()) param[0] = rb::Rint::gApp()->fHistFrame->fParamX->GetTextEntry()->GetText();
	if(rb::Rint::gApp()->fHistFrame->fParamY->GetTextEntry()) param[1] = rb::Rint::gApp()->fHistFrame->fParamY->GetTextEntry()->GetText();
	if(rb::Rint::gApp()->fHistFrame->fParamZ->GetTextEntry()) param[2] = rb::Rint::gApp()->fHistFrame->fParamZ->GetTextEntry()->GetText();

	std::string gate = rb::Rint::gApp()->fHistFrame->fGateEntry->GetText(),
		 name = rb::Rint::gApp()->fHistFrame->fNameEntry->GetText(),
		 title = rb::Rint::gApp()->fHistFrame->fTitleEntry->GetText();

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
		case 3: // scaler
			rb::hist::NewScaler(name.c_str(), title.c_str(), bins[0], low[0], high[0], paramarg.c_str(), gate.c_str(), evt);
			break;
		case 4: // summary [h]
			rb::hist::NewSummary(name.c_str(), title.c_str(), bins[0], low[0], high[0], paramarg.c_str(), gate.c_str(), evt, "h");
			break;
		case 5: // summary [v]
			rb::hist::NewSummary(name.c_str(), title.c_str(), bins[0], low[0], high[0], paramarg.c_str(), gate.c_str(), evt, "v");
			break;
		case 6: // 1D gamma
			rb::hist::NewGamma(name.c_str(), title.c_str(), bins[0], low[0], high[0], paramarg.c_str(), gate.c_str(), evt);
			break;
		case 7: // 2D gamma
			rb::hist::NewGamma(name.c_str(), title.c_str(), bins[0], low[0], high[0], bins[1], low[1], high[1], paramarg.c_str(), gate.c_str(), evt);
			break;
		case 8: // 3D gamma
			rb::hist::NewGamma(name.c_str(), title.c_str(), bins[0], low[0], high[0], bins[1], low[1], high[1], bins[2], low[2], high[2], paramarg.c_str(), gate.c_str(), evt);
			break;
		case 9: // Bit
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


void rb::HistSignals::ClickedLoadButton(Int_t which) {
	TGRadioButton* buttons[3] = {	rb::Rint::gApp()->fHistFrame->fConfigLoadMethodReset,
																rb::Rint::gApp()->fHistFrame->fConfigLoadMethodOverwrite,
																rb::Rint::gApp()->fHistFrame->fConfigLoadMethodCumulate };
	for(int i=0; i< 3; ++i) {
		if(i != which) buttons[i]->SetOn(false);
		else buttons[i]->SetOn(true);
	}
}

namespace {
std::map<TGListTreeItem*, rb::hist::Base*> hist_map;
std::map<TGListTreeItem*, TDirectory*> directory_map;
}

void rb::HistSignals::recurse_directory(TDirectory* dir, TGListTreeItem* item) {
	const TGPicture *p_ofolder = gClient->GetPicture("ofolder_t.xpm");
	const TGPicture *p_folder = gClient->GetPicture("folder_t.xpm");
	const TGPicture *p_hist = gClient->GetPicture("h1_t.xpm");
  for(Int_t i=0; i< dir->GetList()->GetEntries(); ++i) {
		rb::hist::Base* hist = dynamic_cast<rb::hist::Base*>(dir->GetList()->At(i));
    if(hist) {
			TGListTreeItem* hist_item = rb::Rint::gApp()->fHistFrame->fHistTree->AddItem(item, hist->GetName(), p_hist, p_hist);
			hist_map.insert(std::make_pair(hist_item, hist));
		}
	}
  for(Int_t i=0; i< dir->GetList()->GetEntries(); ++i) {
		TDirectory* directory = dynamic_cast<TDirectory*>(dir->GetList()->At(i));
		if(directory) {
			if(!rb::Rint::gApp()->fHistFrame->fHistTree->FindChildByName(item, dir->GetName()))
				;
			TGListTreeItem* this_ = rb::Rint::gApp()->fHistFrame->fHistTree->AddItem(item, directory->GetName(), p_ofolder, p_folder);
			directory_map.insert(std::make_pair(this_, directory));
			this_->SetOpen(true);
      recurse_directory(directory, this_);
    }
  }
}

void rb::HistSignals::SyncHistTree() {
	TGListTreeItem* fRootbeer = rb::Rint::gApp()->fHistFrame->fHistTree->FindItemByPathname(gROOT->GetName());
	rb::Rint::gApp()->fHistFrame->fHistTree->DeleteChildren(fRootbeer);
	hist_map.clear();
	directory_map.clear();
	directory_map.insert(std::make_pair(fRootbeer, gROOT));
	recurse_directory(gROOT, fRootbeer);
	rb::Rint::gApp()->fHistFrame->fHistTree->ClearViewPort();
}

void rb::HistSignals::Cd() {
	TGListTreeItem* item = rb::Rint::gApp()->fHistFrame->fHistTree->GetSelected();
	return Cd(item, 1);
}

void rb::HistSignals::Cd(TGListTreeItem* item, Int_t btn) {
	if(0 && btn);
	else if(directory_map.count(item)) {
		directory_map.find(item)->second->cd();
	}
	else if(!strcmp(item->GetText(), gROOT->GetName()))
		 gROOT->cd();
}

void rb::HistSignals::Mkdir() {
	char name[500];
	new TGInputDialog(gClient->GetRoot(), 0, "Enter Directory Name: ", 0, name);
	rb::Rint::gApp()->fHistFrame->fMkdirButton->SetDown(false);
	if(name[0] == 0 && name[1] == 0) // "Cancel" pressed
		 return;
	TGListTreeItem* current = rb::Rint::gApp()->fHistFrame->fHistTree->GetSelected();
	mkdir:
	if(directory_map.count(current)) {
		directory_map.find(current)->second->mkdir(name);
		directory_map.find(current)->second->cd(name);
	}
	else if(hist_map.count(current)) {
		current = current->GetParent();
		goto mkdir;
	}
	else {
		current = rb::Rint::gApp()->fHistFrame->fHistTree->FindItemByPathname(gROOT->GetName());
		goto mkdir;
	}
	SyncHistTree();
}

void rb::HistSignals::DrawHist() {
	TGListTreeItem* item = rb::Rint::gApp()->fHistFrame->fHistTree->GetSelected();
	return DrawHist(item, 1);
}

void rb::HistSignals::DrawHist(TGListTreeItem* item, Int_t btn) {
	if(btn)
		;
	if(!gPad) {
		if(Rint::gApp()->GetSignals()) Rint::gApp()->GetSignals()->CreateNew();
	}
	if(hist_map.count(item)) {
		hist_map.find(item)->second->Draw(rb::Rint::gApp()->fHistFrame->fDrawOptionEntry->GetText());
		gPad->Modified();
		gPad->Update();
	}
}

void rb::HistSignals::RegateHist() {
	if(!GetSelectedHist()) return;
	GetSelectedHist()->Regate(rb::Rint::gApp()->fHistFrame->fGateEntry->GetText());
}

void rb::HistSignals::SyncHistMenu(rb::hist::Base* hist) {
	if(!hist) return;
	//	1d, 2d, 3d, summary [h], summary [v], gamma1, gamma2, gamma3, bit
  //   0,  1,  2,     3,          4,          5,      6,      7,     8
	int code = -1;
	if(0);
	else if(dynamic_cast<rb::hist::D1*>(hist)) {
		rb::Rint::gApp()->fHistFrame->fTypeEntry->Select(0);
		code = 0;
	}
	else if(dynamic_cast<rb::hist::D2*>(hist)) {
		rb::Rint::gApp()->fHistFrame->fTypeEntry->Select(1);
		code = 1;
	}
	else if(dynamic_cast<rb::hist::D3*>(hist)) {
		rb::Rint::gApp()->fHistFrame->fTypeEntry->Select(2);
		code = 2;
	}
	else if(dynamic_cast<rb::hist::Summary*>(hist)) {
		if(!dynamic_cast<rb::hist::Summary*>(hist)->GetOrientation()) {
			rb::Rint::gApp()->fHistFrame->fTypeEntry->Select(3);
			code = 3;
		}
		else {
			rb::Rint::gApp()->fHistFrame->fTypeEntry->Select(4);
			code = 4;
		}
	}
	else if(dynamic_cast<rb::hist::Gamma*>(hist)) {
		rb::Rint::gApp()->fHistFrame->fTypeEntry->Select(4 + hist->GetNdimensions());
		code = 4 + hist->GetNdimensions();
	}
	else { // bit
		rb::Rint::gApp()->fHistFrame->fTypeEntry->Select(8);
		code = 8;
	}	

	rb::Rint::gApp()->fHistFrame->fNameEntry->SetText(hist->GetName());
	if(!hist->UseDefaultTitle()) {
		rb::Rint::gApp()->fHistFrame->fTitleEntry->SetText(hist->GetTitle());
	} else {
		rb::Rint::gApp()->fHistFrame->fTitleEntry->SetText("");
	}
	if(hist->GetGate() != "1") {
		rb::Rint::gApp()->fHistFrame->fGateEntry->SetText(hist->GetGate().c_str());
	}
	else {
		rb::Rint::gApp()->fHistFrame->fGateEntry->SetText("");
	}
	rb::Rint::gApp()->fHistFrame->fEventEntry->Select(hist->GetEventCode());

/*
	TGComboBox* par_boxes[] = { rb::Rint::gApp()->fHistFrame->fParamX, rb::Rint::gApp()->fHistFrame->fParamY, rb::Rint::gApp()->fHistFrame->fParamZ };
	for(int i=0; i< get_dim(code); ++i) {
		TGLBEntry* entry = par_boxes[i]->FindEntry(hist->GetParam(i).c_str());
		if(entry) {
			par_boxes[i]->Select(entry->EntryId());
			par_boxes[i]->Selected(entry->EntryId());
		}
	}
*/
}

void rb::HistSignals::HistTreeKeyPressed(TGListTreeItem* item, UInt_t keycode, UInt_t mask) {
	// Apparently this doesn't work....
	// printf("KEY PRESSED: 0x%x 0x%x\n", keycode, mask);
}

void rb::HistSignals::HistTreeItemClicked(TGListTreeItem* item, Int_t btn) {
	if(0 && btn);
	else if (hist_map.count(item)) {
		SyncHistMenu(GetSelectedHist());
		HistTreeItemClicked(item->GetParent(), btn);
	}
	else if (directory_map.count(item)) Cd(item, btn);
	else;
}
void rb::HistSignals::HistTreeItemSelect(TGListTreeItem* item, Int_t btn) {
	if(0 && btn);
	else if (hist_map.count(item)) DrawHist(item, btn);
	else if (directory_map.count(item)) Cd(item, btn);
	else;
}

void rb::HistSignals::DeleteHist() {
	if(GetSelectedHist()) {
		delete GetSelectedHist();
	}
	else if(directory_map.count(rb::Rint::gApp()->fHistFrame->fHistTree->GetSelected())) {
		TDirectory* directory = directory_map.find(rb::Rint::gApp()->fHistFrame->fHistTree->GetSelected())->second;
		if(directory != gROOT) delete directory;
		SyncHistTree();
	}
	rb::canvas::UpdateAll();
}

rb::hist::Base* rb::HistSignals::GetSelectedHist() {
	TGListTreeItem* item = rb::Rint::gApp()->fHistFrame->fHistTree->GetSelected();
	return hist_map.count(item) ? hist_map.find(item)->second : 0;
}

void rb::HistSignals::HistMemberFn() {
	if(!GetSelectedHist()) return;
	std::string method = rb::Rint::gApp()->fHistFrame->fCommandEntry->GetText();
	TClass* cl_hist = TClass::GetClass("rb::hist::Base");
	if(cl_hist) { if (!cl_hist->GetMethodAny(method.substr(0, method.find("(")).c_str())) {
			std::stringstream msg;
			msg << rb::Rint::gApp()->fHistFrame->fCommandEntry->GetText() << " is an invalid command. Commands must be a valid member function of "
					<< "rb::hist::Base*. For a complete list see:\n"
					<< "http://trshare.triumf.ca/~gchristian/rootbeer/doc/html/classrb_1_1hist_1_1_base.html";
			error_box(msg.str().c_str(), "Invalid Command");
			rb::Rint::gApp()->fHistFrame->fCommandOk->SetDown(false);
			return;
		}
	}
	std::stringstream cmd;
	cmd << "rb::Rint::gApp()->GetHistSignals()->GetSelectedHist()->" << rb::Rint::gApp()->fHistFrame->fCommandEntry->GetText();
	gROOT->ProcessLineFast(cmd.str().c_str());
	rb::canvas::UpdateAll();
}


// =========== VARIABLES / CONFIG ============ //
std::string rb::HistSignals::get_variable(TGListTreeItem* item) {
	static std::vector<std::string> variables = rb::data::MBasic::GetAll();
	char path[1000];
	rb::Rint::gApp()->fHistFrame->fVariablesTree->GetPathnameFromItem(item, path);
	TString sPath(&path[1]);
	sPath.ReplaceAll("/", ".");
	if(std::find(variables.begin(), variables.end(), sPath.Data()) == variables.end())
		 return "";
	return sPath.Data();
}
void rb::HistSignals::ReadVariable() {
	TGListTreeItem* item = rb::Rint::gApp()->fHistFrame->fVariablesTree->GetSelected();
	ReadVariable(item, 1);
}
void rb::HistSignals::ReadVariable(TGListTreeItem* item, Int_t nbd) {
	if(nbd)
		;
	std::string varname = get_variable(item);
	if(!varname.empty()) {
		Double_t val = rb::data::GetValue(varname.c_str());
		rb::Rint::gApp()->fHistFrame->fVarEntry->SetNumber(val);
	}
}
void rb::HistSignals::SetVariable() {
	std::string varname = get_variable(rb::Rint::gApp()->fHistFrame->fVariablesTree->GetSelected());
	if(!varname.empty()) {
		rb::data::SetValue(varname.c_str(), rb::Rint::gApp()->fHistFrame->fVarEntry->GetNumber());
	}
}

void rb::HistSignals::SyncVariables() {
	const TGPicture* pbr_o = gClient->GetPicture("branch_t.xpm");
	const TGPicture* pbr_c = gClient->GetPicture("branch-cl_t.xpm");
	const TGPicture* pvar = gClient->GetPicture("leaf_t.xpm");
	static std::vector<std::string> variables = rb::data::MBasic::GetAll();
	std::sort(variables.begin(), variables.end(), ansort);
	for(UInt_t i=0; i< variables.size(); ++i) {
		std::string name = variables[i];
		TGListTreeItem* item = 0;
		while(name.find(".") < name.size()) {
			std::string name0 = name.substr(0, name.find("."));
			if(name == variables[i]) {
				if(!rb::Rint::gApp()->fHistFrame->fVariablesTree->FindItemByPathname(name0.c_str())) {
					rb::Rint::gApp()->fHistFrame->fVariablesTree->AddRoot(name0.c_str());
					rb::Rint::gApp()->fHistFrame->fVariablesTree->FindItemByPathname(name0.c_str())->SetPictures(pbr_o, pbr_c);
				}
				item = rb::Rint::gApp()->fHistFrame->fVariablesTree->FindItemByPathname(name0.c_str());
				item->SetOpen(true);
			}
			else {
				item = rb::Rint::gApp()->fHistFrame->fVariablesTree->FindChildByName(item, name0.c_str()) ?
					 rb::Rint::gApp()->fHistFrame->fVariablesTree->FindChildByName(item, name0.c_str()) :
					 rb::Rint::gApp()->fHistFrame->fVariablesTree->AddItem(item, name0.c_str(), pbr_o, pbr_c);
				item->SetOpen(true);
			}
			name = name.substr(name.find(".") + 1);
		}
		rb::Rint::gApp()->fHistFrame->fVariablesTree->AddItem(item, name.c_str(), pvar, pvar);
	}
}

void rb::HistSignals::WriteConfig(Int_t which) {
	TGFileInfo fileInfo;
	const char* ext[] =
		 { "ROOT macro files", "*.[Cc]",
			 "ROOT macro files", "*.cc",
			 "ROOT macro files", "*.cxx",
			 "All Files", "*",
			 0, 0 };
	fileInfo.fFileTypes = reinterpret_cast<const char**>(ext);
	TString dirInit = expand_path(kConfigStaticDefault, "$RB_CONFIGDIR");
	fileInfo.fIniDir = StrDup(dirInit);

	new TGFileDialog(gClient->GetRoot(), 0, kFDSave, &fileInfo);
	if(fileInfo.fFilename != 0) {
		switch(which) {
		case WRITE_HIST:
			rb::WriteHistograms(fileInfo.fFilename, false); break;
		case WRITE_VAR:
			rb::WriteVariables(fileInfo.fFilename, false); break;
		case WRITE_CANVAS:
			rb::WriteCanvases(fileInfo.fFilename, false); break;
		case WRITE_ALL:
			rb::WriteConfig(fileInfo.fFilename, false); break;
		default: break;
		}
 	}
}

void rb::HistSignals::ReadConfig(Bool_t type_prompt) {
	TGRadioButton* buttons[3] = {	rb::Rint::gApp()->fHistFrame->fConfigLoadMethodReset,
																rb::Rint::gApp()->fHistFrame->fConfigLoadMethodOverwrite,
																rb::Rint::gApp()->fHistFrame->fConfigLoadMethodCumulate };
	std::string opt[3] = {"r", "o", "c"};
	int which = 0;
	if(!type_prompt) {
		for(which=0; which< 3; ++which) {
			if(buttons[which]->IsOn()) break;
		}
	}
	else {
		which = -1;
		std::string arr_names[3] = { "Reset", "Overwrite", "Cumulate" };
		std::vector<std::string> names(arr_names, arr_names+3);	
		new TGSelectDialog(gClient->GetRoot(), 0, "Select Method:", "Select Method", &names, &which);
		if(which < 0) return;
	}
	TGFileInfo fileInfo;
	const char* ext[] =
		 { "ROOT macro files", "*.[Cc]",
			 "ROOT macro files", "*.cc",
			 "ROOT macro files", "*.cxx",
			 "All Files", "*",
			 0, 0 };
	fileInfo.fFileTypes = reinterpret_cast<const char**>(ext);

	TString dirInit = expand_path(kConfigStaticDefault, "$RB_CONFIGDIR");
	fileInfo.fIniDir = StrDup(dirInit);

	new TGFileDialog(gClient->GetRoot(), 0, kFDOpen, &fileInfo);
	if(fileInfo.fFilename != 0) {
		rb::ReadConfig(fileInfo.fFilename, opt[which].c_str());
	}
}

void rb::HistSignals::ReadCanvasConfig() {
	TGFileInfo fileInfo;
	const char* ext[] =
		 { "ROOT macro files", "*.[Cc]",
			 "ROOT macro files", "*.cc",
			 "ROOT macro files", "*.cxx",
			 "All Files", "*",
			 0, 0 };
	fileInfo.fFileTypes = reinterpret_cast<const char**>(ext);

	TString dirInit = expand_path(kConfigStaticDefault, "$RB_CONFIGDIR");
	fileInfo.fIniDir = StrDup(dirInit);

	new TGFileDialog(gClient->GetRoot(), 0, kFDOpen, &fileInfo);
	if(fileInfo.fFilename != 0) {
		rb::ReadCanvases(fileInfo.fFilename);
	}
}

