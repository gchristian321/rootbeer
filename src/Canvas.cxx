//! \file Canvas.cxx
//! \brief Implements canvas updating functions.
//! \details Also defines a number of internal functions to be called by the
//! user ones.
#include <TCanvas.h>
#include "Rootbeer.hxx"
#include "hist/Hist.hxx"
#include "utils/Timer.hxx"
#include "utils/Thread.hxx"
#include "utils/LockingPointer.hxx"
#include "utils/Error.hxx"

#define RB_SCOPED_LOCK // rb::ScopedLock<rb::Mutex> lock (TTHREAD_GLOBAL_MUTEX); ///err::Info("Canvas") << "Locking: line " << __LINE__;


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Internal constants, data, functions, and classes.     //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace
{
  /// Maximum update rate, currently set to one second.
  const Int_t MAX_RATE = 1;

  /// Name of the thread
  const char* THREAD_NAME = "CanvasUpdate";

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
	// TCanvas * c1 = (TCanvas*)gROOT->GetListOfCanvases()->At(0);
	// if(c1) {c1->Modified(); c1->Update(); }
      }
    }
  };

  void send_update(TVirtualPad* pad) {
    //    TThread::UnLock();
    pad->Update();
  }

  /// Update whatever histograms are on the current canvas/pad,
  /// including any sub-pads owned by this one.
  void UpdatePad(TVirtualPad* pad) {
    //    TVirtualPad* pad = pad_;

    RB_SCOPED_LOCK;

    std::string type = pad->ClassName();
    pad = dynamic_cast<TPad*>(pad);
    if(!pad) { Error("UpdatePad", "Passed an invalid type, %s.", type.c_str()); return; }

    pad->cd();
    TList* primitives = pad->GetListOfPrimitives();
    for(Int_t i=0; i< primitives->GetEntries(); ++i) {
      TVirtualPad* subpad = dynamic_cast<TVirtualPad*>(primitives->At(i));
      if(subpad) {
	// pad = subpad;
	// goto start;
	err::Info("UpdatePad") <<  " recursing";
	UpdatePad(subpad);
      }
    }
    pad->Modified();
    send_update(pad);
  }

  /// Clear whatever histograms are on the current canvas/pad,
  /// including any sub-pads owned by this one.
  void ClearPad(TVirtualPad* pad) {
    std::string type = pad->ClassName();
    pad = dynamic_cast<TPad*>(pad);
    if(!pad) { Error("ClearPad", "Passed an invalid type, %s.", type.c_str()); return; }

    RB_SCOPED_LOCK
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
  RB_SCOPED_LOCK
  if(gPad) {
    gPad->Modified();
    send_update(gPad);
  }
}

void rb::canvas::UpdateAll() {
  TPad* pInitial = dynamic_cast<TPad*>(gPad);
  TPad* pad;
  for(Int_t i=0; i< gROOT->GetListOfCanvases()->GetEntries(); ++i) {
    pad = dynamic_cast<TPad*>(gROOT->GetListOfCanvases()->At(i));
    if(pad) UpdatePad(pad);
  }
  RB_SCOPED_LOCK
  if(pInitial) pInitial->cd();
}

Int_t rb::canvas::StopUpdate() {
  rb::Thread::Stop(THREAD_NAME);
  updateRate = 0;
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
    CanvasUpdate::CreateAndRun(THREAD_NAME, rate);
    return 0;
  }  
}

Int_t rb::canvas::GetUpdateRate() {
  return updateRate;
}

void rb::canvas::ClearCurrent() {
  RB_SCOPED_LOCK
  if(gPad) {
    for(Int_t i = 0; i < gPad->GetListOfPrimitives()->GetEntries(); ++i) {
      TH1* hst = dynamic_cast<TH1*> (gPad->GetListOfPrimitives()->At(i));
      if(hst) {
	LockFreePointer<TH1D> hstd (static_cast<TH1D*>(hst));
	for(UInt_t p = 0; p < hstd->fN; ++p) hstd->fArray[p] = 0.;
      }
      gPad->Modified();
      send_update(gPad);
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
  RB_SCOPED_LOCK
  if(pInitial) pInitial->cd();
}

#undef RB_SCOPED_LOCK
