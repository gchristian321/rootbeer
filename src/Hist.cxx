/*! \file Hist.cxx
 *  \brief Implements the histogram class member functions.
 */
#include "Hist.hxx"
#include "Mutex.hxx"

// Class rb::H1D //

// Constructor
rb::H1D::H1D (const char* name, const char* title,
	      Int_t nbinsx, Double_t xlow, Double_t xhigh,
	      const char* param, const char* gate) :
  TH1D (Hist::CheckName(name).c_str(), title,
	nbinsx, xlow, xhigh),
  fTree (gUnpacker.fTree),
  fGate ("fGate", Hist::CheckGate(gate).c_str(), fTree),
  fParam("fParam", param, fTree)
{
  if(fGate.GetTree() && fParam.GetTree()) {
    HistMutex::Lock();
    gHistograms.Add(this);
    HistMutex::Unlock();
  }
  else {
    this->Delete();
  }
}

// Fill function
Int_t rb::H1D::Fill() {
  HistMutex::Lock();
  Int_t ret = fGate.EvalInstance() ?
    TH1D::Fill(fParam.EvalInstance())
    : 0;
  HistMutex::Unlock();
  return ret;
}

// Clear function
void rb::H1D::Clear(Option_t* option) {
  if(!TString(option).EqualTo("")) Warning("Clear", "Option %s ignored", option);

  HistMutex::Lock();
  for(Int_t i=1; i<= GetNbinsX(); ++i)
    SetBinContent(i, 0);
  HistMutex::Unlock();
}


// Class rb::H2D //

// Constructor
rb::H2D::H2D (const char* name, const char* title,
	      Int_t nbinsx, Double_t xlow, Double_t xhigh,
	      Int_t nbinsy, Double_t ylow, Double_t yhigh,
	      const char* param, const char* gate) :
  TH2D (Hist::CheckName(name).c_str(), title,
	nbinsx, xlow, xhigh,
	nbinsy, ylow, yhigh) ,
  fTree (gUnpacker.fTree),
  fGate ("fGate", Hist::CheckGate(gate).c_str(), fTree),
  fParamX("fParamX", Hist::ParseParam2d(param,0).c_str(), fTree),
  fParamY("fParamY", Hist::ParseParam2d(param,1).c_str(), fTree)
{
  if(fGate.GetTree() &&
     fParamX.GetTree() &&
     fParamY.GetTree()) {
    HistMutex::Lock();
    gHistograms.Add(this);
    HistMutex::Unlock();
  }
  else {
    this->Delete();
  }
}

// Fill function
Int_t rb::H2D::Fill() {
  HistMutex::Lock();
  Int_t ret = fGate.EvalInstance() ?
    TH2D::Fill(fParamX.EvalInstance(),
	       fParamY.EvalInstance())
    : 0;
  HistMutex::Unlock();
  return ret;
}

// Clear function
void rb::H2D::Clear(Option_t* option) {
  if(!TString(option).EqualTo("")) Warning("Clear", "Option %s ignored", option);

  HistMutex::Lock();
  for(Int_t i=1; i<= GetNbinsX(); ++i) {
    for(Int_t j=1; j<= GetNbinsY(); ++j) {
      SetBinContent(i, j, 0);
    }
  }
  HistMutex::Unlock();
}


// Class rb::H3D //

// Constructor
rb::H3D::H3D (const char* name, const char* title,
	      Int_t nbinsx, Double_t xlow, Double_t xhigh,
	      Int_t nbinsy, Double_t ylow, Double_t yhigh,
	      Int_t nbinsz, Double_t zlow, Double_t zhigh,
	      const char* param, const char* gate) :
  TH3D (Hist::CheckName(name).c_str(), title,
	nbinsx, xlow, xhigh,
	nbinsy, ylow, yhigh,
	nbinsz, zlow, zhigh) ,
  fTree (gUnpacker.fTree),
  fGate ("fGate", Hist::CheckGate(gate).c_str(), fTree),
  fParamX("fParamX", Hist::ParseParam3d(param,0).c_str(), fTree),
  fParamY("fParamY", Hist::ParseParam3d(param,1).c_str(), fTree),
  fParamZ("fParamZ", Hist::ParseParam3d(param,2).c_str(), fTree)
{
  if(fGate.GetTree() &&
     fParamX.GetTree() &&
     fParamY.GetTree()) {
    HistMutex::Lock();
    gHistograms.Add(this);
    HistMutex::Unlock();
  }
  else {
    this->Delete();
  }
}

// Fill function
Int_t rb::H3D::Fill() {
  HistMutex::Lock();
  Int_t ret = fGate.EvalInstance() ?
    TH3D::Fill(fParamX.EvalInstance(),
	       fParamY.EvalInstance(),
	       fParamZ.EvalInstance())
    : 0;
  HistMutex::Unlock();
  return ret;
}

// Clear function
void rb::H3D::Clear(Option_t* option) {
  if(option != "") Warning("Clear", "Option %s ignored", option);

  HistMutex::Lock();
  for(Int_t i=1; i<= GetNbinsX(); ++i) {
    for(Int_t j=1; j<= GetNbinsY(); ++j) {
      for(Int_t k=1; k<= GetNbinsZ(); ++k) {
	SetBinContent(i, j, k, 0);
      }
    }
  }
  HistMutex::Unlock();
}


// For the class members that are basically identical between
// H1D, H2D, etc. we define the functions in a single file with
// the HCLASS and HBASE macros defining which histograms we are
// dealing with.

// One-dimentional histograms
#define HCLASS H1D // define members for rb::H1D
#define HBASE TH1D // which inherits from TH1D
#include "HistMembers.hxx"
#undef HCLASS
#undef HBASE

// Two-dimentional histograms
#define HCLASS H2D // define members for rb::H2D
#define HBASE TH2D // which inherits from TH2D
#include "HistMembers.hxx"
#undef HCLASS
#undef HBASE

// Three-dimentional histograms
#define HCLASS H3D // define members for rb::H3D
#define HBASE TH3D // which inherits from TH3D
#include "HistMembers.hxx"
#undef HCLASS
#undef HBASE
