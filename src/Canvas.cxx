//! \file Canvas.cxx
//! \brief Implements canvas updating functions.
//! \details Also defines a number of internal functions to be called by the
//! user ones.
#include <TCanvas.h>
#include "Rint.hxx"
#include "Rootbeer.hxx"
#include "hist/Hist.hxx"
#include "utils/Timer.hxx"
#include "utils/Thread.hxx"
#include "utils/LockingPointer.hxx"
#include "utils/Error.hxx"

#define CANVAS_LOCKGUARD rb::ScopedLock<rb::Mutex> LOCK__ (TTHREAD_GLOBAL_MUTEX);


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Internal constants, data, functions, and classes.     //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace
{
/// Maximum update rate, currently set to one second.
const Int_t MAX_RATE = 1;

/// Name of the thread
const char* CanvasThreadName = "CanvasUpdate";

/// The rate at which canvases are updated (in seconds).
Int_t updateRate = 0;

/// Run the canvas updating in a separate thread.
class CanvasUpdate : public rb::Thread
{
private:
	 Int_t fRate; // update rate (seconds)
	 CanvasUpdate(const char* name, Int_t rate) :
		 rb::Thread(name), fRate(rate) {}
public:
	 ~CanvasUpdate() {}
	 static void CreateAndRun(const char* name, Int_t rate) {
		 CanvasUpdate * c = new CanvasUpdate(name, rate);
		 c->Run();
	 }
	 void DoInThread() {
		 rb::Timer t(fRate);
		 while(rb::Thread::IsRunning(fName)) {
			 if(t.Check()) rb::canvas::UpdateAll();
		 }
	 }
};

inline void SendUpdate(TVirtualPad* pad) {
	pad->Update();
}

/// Update whatever histograms are on the current canvas/pad,
/// including any sub-pads owned by this one.
void UpdatePad(TVirtualPad* pad) {

	CANVAS_LOCKGUARD;

	std::string type = pad->ClassName();
	pad = dynamic_cast<TPad*>(pad);
	if(!pad) { Error("UpdatePad", "Passed an invalid type, %s.", type.c_str()); return; }

	pad->cd();
	TList* primitives = pad->GetListOfPrimitives();
	for(Int_t i=0; i< primitives->GetEntries(); ++i) {
		TVirtualPad* subpad = dynamic_cast<TVirtualPad*>(primitives->At(i));
		if(subpad) {
			UpdatePad(subpad);
		}
	}
	pad->Modified();
	SendUpdate(pad);
}

/// Clear whatever histograms are on the current canvas/pad,
/// including any sub-pads owned by this one.
void ClearPad(TVirtualPad* pad) {
	std::string type = pad->ClassName();
	pad = dynamic_cast<TPad*>(pad);
	if(!pad) { Error("ClearPad", "Passed an invalid type, %s.", type.c_str()); return; }

	CANVAS_LOCKGUARD;
	pad->cd();
	TList* primitives = pad->GetListOfPrimitives();

	for(Int_t i=0; i< primitives->GetEntries(); ++i) {
		TVirtualPad* subpad = dynamic_cast<TVirtualPad*>(primitives->At(i));
		if(subpad) ClearPad(subpad);
	}
	rb::canvas::ClearCurrent();
}
} // namespace




//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Implementation of                                     //
// namespace rb::canvas                                  //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

void rb::canvas::UpdateCurrent() {
  CANVAS_LOCKGUARD;
  if(gPad) {
    gPad->Modified();
    SendUpdate(gPad);
  }
}

void rb::canvas::UpdateAll() {
  TPad* pInitial = dynamic_cast<TPad*>(gPad);
  TPad* pad;
  for(Int_t i=0; i< gROOT->GetListOfCanvases()->GetEntries(); ++i) {
    pad = dynamic_cast<TPad*>(gROOT->GetListOfCanvases()->At(i));
    if(pad) UpdatePad(pad);
  }
  CANVAS_LOCKGUARD;
  if(pInitial) pInitial->cd();
}

Int_t rb::canvas::StopUpdate() {
  rb::Thread::Stop(CanvasThreadName);
  updateRate = 0;
//	gApp()->SyncAll();
	if(gApp()->GetSignals())
		 gApp()->GetSignals()->StoppingUpdate();
  return updateRate;
}

Int_t rb::canvas::StartUpdate(Int_t rate) {
  if(rate < MAX_RATE) {
    Error("StartUpdate",
					"Passed an invalid update rate: %d. The update rate must be >= %d second(s).",
					rate, MAX_RATE);
    return -1;
  }
  else {
    StopUpdate();
    updateRate = rate;
    CanvasUpdate::CreateAndRun(CanvasThreadName, rate);
//		gApp()->SyncAll();
		if(gApp()->GetSignals())
			 gApp()->GetSignals()->StartingUpdate(rate);
    return 0;
  }  
}

Int_t rb::canvas::GetUpdateRate() {
  return updateRate;
}

void rb::canvas::ClearCurrent() {
  CANVAS_LOCKGUARD;
  if(gPad) {
    for(Int_t i = 0; i < gPad->GetListOfPrimitives()->GetEntries(); ++i) {
      TH1* hst = dynamic_cast<TH1*> (gPad->GetListOfPrimitives()->At(i));
      if(hst) {
				LockFreePointer<TH1D> hstd (static_cast<TH1D*>(hst));
				for(Int_t p = 0; p < hstd->fN; ++p) hstd->fArray[p] = 0.;
      }
      gPad->Modified();
      SendUpdate(gPad);
    }
  }
}

void rb::canvas::ClearAll() {
  TPad* pInitial = dynamic_cast<TPad*>(gPad);
  TPad* pad;
  for(Int_t i=0; i< gROOT->GetListOfCanvases()->GetEntries(); ++i) {
    pad = dynamic_cast<TPad*>(gROOT->GetListOfCanvases()->At(i));
    if(pad) ClearPad(pad);
  }
  CANVAS_LOCKGUARD;
  if(pInitial) pInitial->cd();
}

