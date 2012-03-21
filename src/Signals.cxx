//! \file Signals.cxx
//! \brief Implements rb::Signals
#include <TROOT.h>
#include <TString.h>
#include <TGFileDialog.h>
#include "Signals.hxx"
#include "Rootbeer.hxx"
#include "Gui.hxx"


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

