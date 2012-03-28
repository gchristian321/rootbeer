//! \file Signals.hxx
//! \brief Defines a class used to send and recieve signals from the rootbeer gui.
#ifndef SIGNALS_HXX
#define SIGNALS_HXX
#include <TQObject.h>

class TGListTreeItem;
class TGTextButton;
namespace rb
{
namespace hist { class Base; }
class Signals: public TQObject
{
public:
	 void Unattaching(); //*SIGNAL*
	 void Attaching(); //*SIGNAL*
	 void StartingUpdate(); //*SIGNAL*
	 void StartingUpdate(Int_t); //*SIGNAL*
	 void StoppingUpdate(); //*SIGNAL*
	 void ChangeUpdateRate(Long_t); //*SIGNAL*
	 void AttachedOnline(const char*); //*SIGNAL*
	 void AttachedFile(const char*); //*SIGNAL*
	 void ChangedCanvases(); //*SIGNAL*
	 void UpdateBufferCounter(Int_t n, Bool_t force = false);
	 void SaveData();
	 void SaveHists();
	 void EnableSaveHists();
	 void AttachOnline();
	 void AttachFile();
	 void AttachList();
	 void Unattach();
	 void UpdateAll();
	 void UpdateCurrent();
	 void ZeroAll();
	 void ZeroCurrent();
	 void ClearCurrent();
	 void DivideCurrent();
	 void CreateNew();
	 void Update();
	 void SyncCanvases();
	 void CdCanvas();
	 void Quit();

// === Hist === //
private:
	 Bool_t fHistFromGui;
public:
	 void NewOrDeleteHist(); //*SIGNAL*
	 void EnableHistFields(Int_t);
	 void PopulateEvents();
	 void PopulateParameters(Int_t);
	 void SetHistFromGui();
	 Bool_t IsHistFromGui();
	 void CreateHistogram();
	 void SyncHistTree();
	 rb::hist::Base* GetSelectedHist();
	 void HistTreeItemClicked(TGListTreeItem*, Int_t);
	 void HistTreeItemSelect(TGListTreeItem*, Int_t);
	 void HistTreeItemSelect(TGListTreeItem*);
	 void DrawHist(TGListTreeItem*, Int_t);
	 void DrawHist(TGListTreeItem* item);
	 void DrawHist();
	 void Mkdir();
	 void Cd(TGListTreeItem*, Int_t);
	 void Cd(TGListTreeItem* item);
	 void Cd();
	 void DeleteHist();
	 void HistMemberFn();

// === Variables/Config === //
	 enum { WRITE_ALL, WRITE_CANVAS, WRITE_VAR, WRITE_HIST };
	 void ClickedLoadButton(Int_t);
	 void WriteConfig(Int_t which);
	 void ReadConfig(Bool_t);
	 void ReadCanvasConfig();
	 void SyncVariables();
	 void ReadVariable(TGListTreeItem*, Int_t);
	 void ReadVariable();
	 void SetVariable();
	 

	 Signals();
	 ClassDef(rb::Signals, 0);
};

}

inline rb::Signals::Signals():
	fHistFromGui (false) {}
inline void rb::Signals::Unattaching() {
	Emit("Unattaching()");
}
inline void rb::Signals::Attaching() {
	Emit("Attaching()");
}
inline void rb::Signals::StartingUpdate() {
	Emit("StartingUpdate()");
}
inline void rb::Signals::StartingUpdate(Int_t rate) {
	StartingUpdate();
	Emit("StartingUpdate(Int_t)", rate);
}
inline void rb::Signals::StoppingUpdate() {
	Emit("StoppingUpdate()");
}
inline void rb::Signals::AttachedFile(const char* name) {
	Emit("AttachedFile(const char*)", name);
}
inline void rb::Signals::AttachedOnline(const char* host) {
	Emit("AttachedOnline(const char*)", host);
}
inline void rb::Signals::ChangedCanvases() {
	Emit("ChangedCanvases()");
}
inline void rb::Signals::SetHistFromGui() {
	fHistFromGui = true;
}
inline void rb::Signals::NewOrDeleteHist() {
	Emit("NewOrDeleteHist()");
}
inline void rb::Signals::HistTreeItemSelect(TGListTreeItem* item) {
	HistTreeItemSelect(item, 0);
}
inline void rb::Signals::DrawHist(TGListTreeItem* item) {
	DrawHist(item, 0);
}
inline void rb::Signals::Cd(TGListTreeItem* item) {
	Cd(item, 0);
}


#endif
