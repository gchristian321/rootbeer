//! \file Signals.hxx
//! \brief Defines a class used to send and recieve signals from the rootbeer gui.
#ifndef SIGNALS_HXX
#define SIGNALS_HXX
#include <TQObject.h>

namespace rb
{

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
	 void AttachOnline();
	 void AttachFile();
	 void AttachList();
	 void Unattach();
	 void UpdateAll();
	 void UpdateCurrent();
	 void ClearAll();
	 void ClearCurrent();
	 void DivideCurrent();
	 void CreateNew();
	 void Update();
	 void SyncCanvases();
	 void CdCanvas();
	 void Quit();
	 ClassDef(rb::Signals, 0);
};

}

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

#endif
