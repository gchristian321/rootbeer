//! \file Rootbeer.cxx 
//! \brief Implements the user interface functions.
#include <iostream>
#include "Rootbeer.hxx"
#include "Buffer.hxx"
#include "Data.hxx"
#include "hist/Hist.hxx"


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
// Histogram Creation Helper Function                    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace { rb::hist::Manager* const find_manager(Int_t code) {
  rb::Event* event = rb::gApp()->GetEvent(code);
  if(event == 0) err::Throw() << "Invalid event code: " << code;
  return event->GetHistManager();
} }

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::hist::New (One-dimensional)                       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::New(const char* name, const char* title,
			      Int_t bx, Double_t xl, Double_t xh,
			      const char* param, const char* gate, Int_t event_code) {
  rb::hist::Base* hist = 0;
  try {
    hist = find_manager(event_code)->Create<D1>(name, title, param, gate, event_code, bx, xl, xh);
  }
  catch (std::exception& e) {
    err::Error("rb::hist::New") << e.what();
  }
  return hist;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  rb::hist::New (Two-dimensional)                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::New(const char* name, const char* title,
			      Int_t bx, Double_t xl, Double_t xh,
			      Int_t by, Double_t yl, Double_t yh,
			      const char* param, const char* gate, Int_t event_code) {
  rb::hist::Base* hist = 0;
  try {
    hist = find_manager(event_code)->Create<D2>(name, title, param, gate, event_code, bx, xl, xh, by, yl, yh);
  }
  catch (std::exception& e) {
    err::Error("rb::hist::New") << e.what();
  }
  return hist;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  rb::hist::New (Three-dimensional)                    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::New(const char* name, const char* title,
			      Int_t bx, Double_t xl, Double_t xh,
			      Int_t by, Double_t yl, Double_t yh,
			      Int_t bz, Double_t zl, Double_t zh,
			      const char* param, const char* gate, Int_t event_code) {
  rb::hist::Base* hist = 0;
  try {
      hist = find_manager(event_code)->Create<D3>(name, title, param, gate, event_code, bx, xl, xh, by, yl, yh, bz, zl, zh);
  }
  catch (std::exception& e) {
    err::Error("rb::hist::New") << e.what();
  }
  return hist;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::hist::NewSummary()                                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::NewSummary(const char* name, const char* title,
				     Int_t nbins, Double_t low, Double_t high,
				     const char* paramList,  const char* gate, Int_t event_code,
				     const char* orient) {
  rb::hist::Base* hist = 0;
  try {
    hist = find_manager(event_code)->Create<Summary>(name, title, paramList, gate, event_code,
						     nbins, low, high, orient);
  }
  catch (std::exception& e) {
    err::Error("rb::hist::New") << e.what();
  }
  return hist;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  rb::hist::NewGamma (One-dimensional)                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::NewGamma(const char* name, const char* title,
				   Int_t nbinsx, Double_t xlow, Double_t xhigh,
				   const char* param, const char* gate, Int_t event_code) {
  rb::hist::Base* hist = 0;
  try {
    hist = find_manager(event_code)->Create<Gamma>(name, title, param, gate, event_code, nbinsx, xlow, xhigh);
  }
  catch (std::exception& e) {
    err::Error("rb::hist::New") << e.what();
  }
  return hist;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  rb::hist::NewGamma (Two-dimensional)                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::NewGamma(const char* name, const char* title,
				   Int_t nbinsx, Double_t xlow, Double_t xhigh,
				   Int_t nbinsy, Double_t ylow, Double_t yhigh,
				   const char* param, const char* gate, Int_t event_code) {
  rb::hist::Base* hist = 0;
  try {
    hist = find_manager(event_code)->Create<Gamma>(name, title, param, gate, event_code,
						   nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
  }
  catch (std::exception& e) {
    err::Error("rb::hist::New") << e.what();
  }
  return hist;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  rb::hist::NewGamma (Three-dimensional)               //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::NewGamma(const char* name, const char* title,
				   Int_t nbinsx, Double_t xlow, Double_t xhigh,
				   Int_t nbinsy, Double_t ylow, Double_t yhigh,
				   Int_t nbinsz, Double_t zlow, Double_t zhigh,
				   const char* params,  const char* gate, Int_t event_code) {
  rb::hist::Base* hist = 0;
  try {
    hist = find_manager(event_code)->Create<Gamma>(name, title, params, gate, event_code,
						   nbinsx, xlow, xhigh, nbinsy, ylow, yhigh, nbinsz, zlow, zhigh);
  }
  catch (std::exception& e) {
    err::Error("rb::hist::New") << e.what();
  }
  return hist;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  rb::hist::NewBit                                     //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::NewBit(const char* name, const char* title, Int_t nbits, const char* param, const char* gate,
				 Int_t event_code) {
  rb::hist::Base* hist = 0;
  try {
    hist = find_manager(event_code)->Create<Bit>(name, title, param, gate, event_code, nbits, 0., 1.);
  }
  catch (std::exception& e) {
    err::Error("rb::hist::New") << e.what();
  }
  return hist;
}
