//! \file Rootbeer.cxx 
//! \brief Implements the user interface functions.
#include <iostream>
#include "Rootbeer.hxx"
#include "Buffer.hxx"
#include "Data.hxx"
#include "Hist.hxx"
#include "User.hxx"


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Public interface (Rootbeer.hxx) implementations       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::AttachOnline                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::AttachOnline(const char* host, const char* other, char** others, int nothers) {
#ifdef MIDAS_BUFFERS
#ifdef MIDAS_ONLINE // Attach to online midas files
  if(others || nothers)
    Warning("AttachOnline", "Arguments \'others\' and \'n_others\' are unused");

  rb::Unattach();
  rb::attach::Online::CreateAndRun(host, other, others, nothers);

#else
  Info("AttachOnline",
       "MIDAS was not found on your system; cannot attach to online MIDAS data.\n"
       "Instructions for installing MIDAS can be found online at:\n"
       "      http://daq-plone.triumf.ca/SM/docs/local/installmidas.html\n\n"
       "Please note that you will need to have the MIDASSYS environment variable\n"
       "defined in order to attach to online data with rootbeer.\n");
#endif

#elif defined NSCL_BUFFERS
  Info("AttachOnline",
       "Online attachment to NSCL data is not yet implemented.");
#else
  Info("AttachOnline",
       "Attaching to online buffers other than MIDAS and NSCL is not yet part of\n"
       "stock ROOTBEER. You'll have to define it yourself. Once you've done this,\n"
       "pleae contact the developers (Greg Christian, gchristian@triumf.ca) about\n"
       "adding it to the source code.\n");
#endif
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::AttachFile                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::AttachFile(const char* filename, Bool_t stop_at_end) {
  rb::Unattach();
  rb::attach::File::CreateAndRun(filename, stop_at_end);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::AttachList                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::AttachList(const char* filename) {
  rb::Unattach();
  rb::attach::List::CreateAndRun(filename);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Unattach()                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Unattach() {
  rb::attach::StopAll();
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Double_t rb::data::GetValue                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Double_t rb::data::GetValue(const char* name) {
  data::MBasic* basicData = 0;
  basicData = data::MBasic::Find(name);
  if(!basicData) {
    Error("GetValue", "%s not found.", name);
    return -1.;
  }
  return (Double_t)basicData->GetValue();
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::SetValue                               //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::data::SetValue(const char* name, Double_t newvalue) {
  data::MBasic* basicData = 0;
  basicData = data::MBasic::Find(name);
  if(!basicData) {
    Error("SetData", "Data object: %s not found.", name);
    return;
  }
  basicData->SetValue(newvalue);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::PrintAll                               //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::data::PrintAll() {
  data::MBasic::Printer p;
  p.PrintAll();
}





//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::hist::New (One-dimensional)                       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::Hist* rb::hist::New(const char* name, const char* title,
			Int_t nbinsx, Double_t xlow, Double_t xhigh,
			const char* param, const char* gate, Int_t event_code) {
  try {
    rb::Event* event = gApp()->GetEvent(event_code);
    if(event == 0) {
      std::stringstream error;
      error << "Invalid event code: " << event_code;
      std::invalid_argument exception(error.str().c_str());
      throw exception;
    }
    rb::Hist* hist = event->GetHistManager()->Create<rb::Hist>(name, title, param, gate, nbinsx, xlow, xhigh);
    return hist;
  } catch (std::exception& e) {
    err::Error("rb::Hist::New") << e.what();
    //    static_cast<DragonEvent*>(rb::Event::Instance<DragonEvent>())->reset_branch();
  }
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  rb::hist::New (Two-dimensional)                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::hist::New(const char* name, const char* title,
		   Int_t nbinsx, Double_t xlow, Double_t xhigh,
		   Int_t nbinsy, Double_t ylow, Double_t yhigh,
		   const char* param, const char* gate) {
  /*
  Hist::Set_t set ;
  try {
    rb::Hist * hist = new rb::Hist(name, title, param, gate, 2, tree, &set, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
  } catch (std::exception& e) {
    Error("rb::hist::New", "%s", e.what());
  }
  */
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  rb::hist::New (Three-dimensional)                    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::hist::New(const char* name, const char* title,
		   Int_t nbinsx, Double_t xlow, Double_t xhigh,
		   Int_t nbinsy, Double_t ylow, Double_t yhigh,
		   Int_t nbinsz, Double_t zlow, Double_t zhigh,
		   const char* param, const char* gate) {
  /*
  Hist::Set_t set ;
  try {
    rb::Hist * hist = new rb::Hist(name, title, param, gate, 3, tree, &set, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh, nbinsz, zlow, zhigh);
  } catch (std::exception& e) {
    Error("rb::hist::New", "%s", e.what());
  }
  */
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::hist::NewSummary()                                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::hist::NewSummary(const char* name, const char* title,
			  Int_t nbins, Double_t low, Double_t high,
			  const char* paramList,  const char* gate,
			  const char* orientation) {
  return;
  // // Create rb::Hist instance
  // rb::SummaryHist* _this = new rb::SummaryHist(name, title, paramList, gate, orientation);
  // if(!_this->kConstructorSuccess) return;

  // // Set internal histogram
  // //! \note The histogram isn't accessable to any other threads until we add it to
  // //! fgList, so it's safe to access the critical elements via a non-locking pointer.
  // LockFreePointer<CriticalElements> unlocked_critical(_this->fCritical);

  // Bool_t successfulHistCreation = kTRUE;
  // TH1::AddDirectory(kFALSE);

  // Int_t npar = unlocked_critical->fParams.size();

  // if(!_this->kOrient) { // vertical
  //   unlocked_critical->fHistogram =
  //     new TH2D(_this->fName, _this->fTitle, npar, 0, npar, nbins, low, high);
  //   unlocked_critical->fHistogram->GetXaxis()->SetTitle(paramList);
  //   unlocked_critical->fHistogram->GetYaxis()->SetTitle("");
  // }
  // else { // horizontal
  //   unlocked_critical->fHistogram =
  //     new TH2D(_this->fName, _this->fTitle, nbins, low, high, npar, 0, npar);
  //   unlocked_critical->fHistogram->GetXaxis()->SetTitle("");
  //   unlocked_critical->fHistogram->GetYaxis()->SetTitle(paramList);
  // }
  // TH1::AddDirectory(kTRUE);

  // // Add to collections
  // LockingPointer<List_t> hlist(fgList(), fgMutex());
  // if(successfulHistCreation) {
  //   hlist->push_back(_this);

  //   if(gDirectory) {
  //     _this->fDirectory = gDirectory;
  //     _this->fDirectory->Append(_this, kTRUE);
  //   }
  // }
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  rb::hist::NewGamma (One-dimensional)                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::hist::NewGamma(const char* name, const char* title,
			Int_t nbinsx, Double_t xlow, Double_t xhigh,
			const char* param, const char* gate) {
  // TTree* tree = rb::gApp()->fDataGlobals.GetLockedTree().Get();
  // Hist::Set_t* set = LockFreePointer<Hist::Set_t>(rb::gApp()->fDataGlobals.fHistograms).Get();
  // rb::GammaHist::GInitialize(name, title, param, gate, 1, tree, set, nbinsx, xlow, xhigh);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  rb::hist::NewGamma (Two-dimensional)                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::hist::NewGamma(const char* name, const char* title,
			Int_t nbinsx, Double_t xlow, Double_t xhigh,
			Int_t nbinsy, Double_t ylow, Double_t yhigh,
			const char* param, const char* gate) {
  // TTree* tree = rb::gApp()->fDataGlobals.GetLockedTree().Get();
  // Hist::Set_t* set = LockFreePointer<Hist::Set_t>(rb::gApp()->fDataGlobals.fHistograms).Get();
  // rb::GammaHist::GInitialize(name, title, param, gate, 2, tree, set, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
}
