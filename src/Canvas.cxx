/*! \file Canvas.cxx
 * \brief Implements canvas updating functions.
 * 
 * Also defines a number of internal functions to be called by the
 * user ones.
 * \todo Implement Clear() functions based on canvases: ClearCurrent() and ClearAll().
 */
#include "TCanvas.h"
#include "TThread.h"
#include "TMutex.h"
#include "Timer.hxx"
#include "Hist.hxx"
#include "Rootbeer.hxx"
using namespace std;

/// Here we define some internal functions that we don't want \c CINT
/// to know about
namespace rb
{
  namespace canvas
  {
    /*! \brief Update whatever histograms are on the current canvas/pad,
     * including any sub-pads owned by this one. */
    void UpdatePad(TVirtualPad* pad);

    /// Set cavases to update every on a regular basis.
    void* TimedUpdate(void* rate);

    /// Maximum update rate, currently set to two seconds.
    static const Int_t MAX_RATE = 1;

    /// The rate at which canvases are updated (in seconds).
    Int_t updateRate = 0;

    /// Thread in which the canvas updating functions are run.
    TThread thCanvas("thCanvas", rb::canvas::TimedUpdate);

    /// Mutex for locking threded canvas operations.
    TMutex Mutex;

    /// Mutex locking function
    void Lock() { rb::canvas::Mutex.Lock(); }

    /// Mutex un-locking function
    void Unlock() { rb::canvas::Mutex.UnLock(); }
  }
}


void rb::canvas::UpdateCurrent() {
  rb::canvas::Lock();
  if(gPad) {
    gPad->Modified();
    gPad->Update();
  }
  rb::canvas::Unlock();
}

void rb::canvas::UpdatePad(TVirtualPad* pad) {
  string type = pad->ClassName();
  pad = dynamic_cast<TPad*>(pad);
  if(!pad) Error("rb::canvas::UpdatePad", "Passed an invalid type, %s.", type.c_str());

  rb::canvas::Lock();
  pad->cd();
  TList* primitives = pad->GetListOfPrimitives();
  rb::canvas::Unlock();

  for(Int_t i=0; i< primitives->GetEntries(); ++i) {
    TVirtualPad* subpad = dynamic_cast<TVirtualPad*>(primitives->At(i));
    if(subpad) rb::canvas::UpdatePad(subpad);
  }

  rb::canvas::Lock();
  pad->Modified();
  pad->Update();
  rb::canvas::Unlock();
}

void rb::canvas::UpdateAll() {
  TPad* pInitial = dynamic_cast<TPad*>(gPad);
  TPad* pad;
  for(Int_t i=0; i< gROOT->GetListOfCanvases()->GetEntries(); ++i) {
    pad = dynamic_cast<TPad*>(gROOT->GetListOfCanvases()->At(i));
    if(pad) rb::canvas::UpdatePad(pad);
  }
  rb::canvas::Lock();
  if(pInitial) pInitial->cd();
  rb::canvas::Unlock();
}

void* rb::canvas::TimedUpdate(void * rate) {
  rb::Timer t(updateRate);
  while(updateRate > 0) {
    if(t.Check()) UpdateAll();
  }
  return rate;
}

Int_t rb::canvas::StopUpdate() {
  if(updateRate > 0) {
    updateRate = 0;
    thCanvas.Join();
  }
  else {
    updateRate = 0;
  }
  return updateRate;
}

Int_t rb::canvas::StartUpdate(Int_t rate) {
  Int_t r = (Int_t)rate;
  if(r < MAX_RATE) {
    Error("StartUpdate",
	  "Passed an invalid update rate: %d. The update rate must be >= %d second(s).",
	  rate, MAX_RATE);
    return -1;
  }
  else {
    StopUpdate();
    updateRate = rate;
    thCanvas.Run();
    return 0;
  }
}

Int_t rb::canvas::GetUpdateRate() {
  return updateRate;
}


void rb::canvas::ClearCurrent() {
  rb::canvas::Lock();
  if(gPad) {
    for(Int_t i = 0; i < gPad->GetListOfPrimitives()->GetEntries(); ++i) {
      TObject* primitive = gPad->GetListOfPrimitives()->At(i);
      //      if(primitive->InheritsFrom("rb::Hist")) {
	if(0);
	else if(primitive->ClassName() == "rb::H1D")
	  static_cast<rb::H1D*>(primitive)->Clear();
	else if(primitive->ClassName() == "rb::H2D")
	  static_cast<rb::H2D*>(primitive)->Clear();
	else if(primitive->ClassName() == "rb::H3D")
	  static_cast<rb::H3D*>(primitive)->Clear();
	else;
	//      }
    }
  }
  rb::canvas::Unlock();
}


void rb::canvas::ClearAll() {
  // TPad* pInitial = dynamic_cast<TPad*>(gPad);
  // TPad* pad;
  // for(Int_t i=0; i< gROOT->GetListOfCanvases()->GetEntries(); ++i) {
  //   pad = dynamic_cast<TPad*>(gROOT->GetListOfCanvases()->At(i));
  //   if(pad) rb::canvas::ClearPad(pad);
  // }
  // rb::canvas::Lock();
  // if(pInitial) pInitial->cd();
  // rb::canvas::Unlock();
}