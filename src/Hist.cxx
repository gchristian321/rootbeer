/*! \file Hist.cxx
 *  \brief Implements the histogram class member functions.
 */
#include "Hist.hxx"


// Class rb::Hist //
// Static data members.
TObjArray rb::Hist::fgArray;
TMutex rb::Hist::fgMutex;

// Constructor
rb::Hist::Hist(const char* gate, TTree* tree) :
  fTree(tree),
  fGate("fGate", CheckGate(gate).c_str(), fTree) {
  if(!fTree) Error("Hist", "Passed an invalid Tree");
}

// Regate function
Int_t rb::Hist::Regate(const char* newgate) {
  TTreeFormula tempFormula("temp", CheckGate(newgate).c_str(), fTree);
  if(!tempFormula.GetTree()) return -1;
  Hist::Lock();
  fGate.Compile(Hist::CheckGate(newgate).c_str());
  Hist::Unlock();
  return 0;
}

// Get number of histograms
UInt_t rb::Hist::GetNumber() {
  return fgArray.GetEntries();
}

// Static "getter" function
rb::Hist* rb::Hist::Get(UInt_t indx) {
  rb::Hist* hist;

  if(indx > fgArray.GetEntries()) {
    Error("Get", "Invalid index: %d, maximum is %d.\n",
	  indx, fgArray.GetEntries()-1);
    hist = 0;
  }

  else {
    hist = dynamic_cast<rb::Hist*>(fgArray.At(indx));

    if(!hist) {
      TObject* object = fgArray.At(indx);
      Error("Get", "Bad dyamic cast to rb::Hist*, original class was: %s.",
	    object->ClassName());
    }
  }

  return hist;
}

// Static mutex locking function
void rb::Hist::Lock() {
  fgMutex.Lock();
}

// Static mutex un-locking function
void rb::Hist::Unlock() {
  fgMutex.UnLock();
}


// Gate checking function
std::string rb::Hist::CheckGate(const char* gate) {
  TString tsGate(gate);
  tsGate.ReplaceAll(" ", "");
  std::string sGate(tsGate.Data());
  if(0);
  else if(sGate == "") return "1";   // Null field means no gate, i.e. always true.
  else if(sGate == "0") return "!1"; // Somehow "0" evaluates to true, should be false.
  else return sGate;
}

// Name checking function
std::string rb::Hist::CheckName(const char* name) {
  std::string ret = name;
  if(gROOT->FindObject(name)) {
    Int_t n = 1;
    while(1) {
      std::stringstream sstr;
      sstr << name << "_" << n++;
      ret = sstr.str();
      if(!gROOT->FindObject(ret.c_str())) break;
    }
    Info("AddHist", "The name %s is already in use, creating %s_%d instead.", name, name, n-1);
  }
  return ret;
}

// 2d string parsing into parameters
std::string rb::Hist::ParseParam2d(const char* par, Int_t axis) {
  std::string spar (par);
  Int_t p1 = spar.find(":");
  std::string ret = "";
  switch(axis) {
  case 1: ret = spar.substr(0, p1); break;
  case 0: ret = spar.substr(p1+1); break;
  default:
    Error("ParseParam2d",
	  "Invalid axis specification %d (should be x:0, y:1)",
	  axis);
    break;
  }
  return ret;
}

// 3d string parsing into parameters
std::string rb::Hist::ParseParam3d(const char* par, Int_t axis)  {
  std::string spar (par);
  Int_t p1 = spar.find(":");
  Int_t p2 = spar.find_last_of(":");
  std::string ret = "";
  switch(axis) {
  case 2: ret = spar.substr(0, p1);         break;
  case 1: ret = spar.substr(p1+1, p2-p1-1); break;
  case 0: ret = spar.substr(p2+1);          break;
  default:
    Error("ParseParam3d",
	  "Invalid axis specification %d (x = 0, y = 1, z = 2)",
	  axis);
    break;
  }
  return ret;
}



// Class rb::H1D //

// Constructor
rb::H1D::H1D (const char* name, const char* title,
	      Int_t nbinsx, Double_t xlow, Double_t xhigh,
	      const char* param, const char* gate, TTree* tree) :
  TH1D (CheckName(name).c_str(), title,
	nbinsx, xlow, xhigh),
  Hist(gate, tree),
  fParam("fParam", param, tree)
{
  if(fTree && fGate.GetTree() && fParam.GetTree()) {
    Hist::Lock();
    fgArray.Add(this);
    Hist::Unlock();
  }
  else {
    this->Delete();
  }
}

// Destructor
rb::H1D::~H1D() {
  Hist::Lock();
  fgArray.Remove(this);
  fgArray.Compress();  
  Hist::Unlock();
}

// Draw function
void rb::H1D::Draw(Option_t* option) {
  Hist::Lock();
  TH1D::Draw(option);
  Hist::Unlock();
}

// Fill function
Int_t rb::H1D::Fill() {
  Hist::Lock();
  Int_t ret = fGate.EvalInstance() ?
    TH1D::Fill(fParam.EvalInstance())
    : 0;
  Hist::Unlock();
  return ret;
}

// Clear function
void rb::H1D::Clear(Option_t* option) {
  if(!TString(option).EqualTo("")) Warning("Clear", "Option %s ignored", option);

  Hist::Lock();
  for(Int_t i=1; i<= GetNbinsX(); ++i)
    SetBinContent(i, 0);
  Hist::Unlock();
}


// Class rb::H2D //

// Constructor
rb::H2D::H2D (const char* name, const char* title,
	      Int_t nbinsx, Double_t xlow, Double_t xhigh,
	      Int_t nbinsy, Double_t ylow, Double_t yhigh,
	      const char* param, const char* gate, TTree* tree) :
  TH2D (CheckName(name).c_str(), title,
	nbinsx, xlow, xhigh,
	nbinsy, ylow, yhigh),
  Hist(gate, tree),
  fParamX("fParamX", ParseParam2d(param,0).c_str(), tree),
  fParamY("fParamY", ParseParam2d(param,1).c_str(), tree)
{
  if(fTree && fGate.GetTree() &&
     fParamX.GetTree() &&
     fParamY.GetTree()) {
    Hist::Lock();
    fgArray.Add(this);
    Hist::Unlock();
  }
  else {
    this->Delete();
  }
}

// Destructor
rb::H2D::~H2D() {
  Hist::Lock();
  fgArray.Remove(this);
  fgArray.Compress();
  Hist::Unlock();
}

// Draw function
void rb::H2D::Draw(Option_t* option) {
  Hist::Lock();
  TH2D::Draw(option);
  Hist::Unlock();
}

// Fill function
Int_t rb::H2D::Fill() {
  Hist::Lock();
  Int_t ret = fGate.EvalInstance() ?
    TH2D::Fill(fParamX.EvalInstance(),
	       fParamY.EvalInstance())
    : 0;
  Hist::Unlock();
  return ret;
}

// Clear function
void rb::H2D::Clear(Option_t* option) {
  if(!TString(option).EqualTo("")) Warning("Clear", "Option %s ignored", option);

  Hist::Lock();
  for(Int_t i=1; i<= GetNbinsX(); ++i) {
    for(Int_t j=1; j<= GetNbinsY(); ++j) {
      SetBinContent(i, j, 0);
    }
  }
  Hist::Unlock();
}


// Class rb::H3D //

// Constructor
rb::H3D::H3D (const char* name, const char* title,
	      Int_t nbinsx, Double_t xlow, Double_t xhigh,
	      Int_t nbinsy, Double_t ylow, Double_t yhigh,
	      Int_t nbinsz, Double_t zlow, Double_t zhigh,
	      const char* param, const char* gate, TTree* tree) :
  TH3D (CheckName(name).c_str(), title,
	nbinsx, xlow, xhigh,
	nbinsy, ylow, yhigh,
	nbinsz, zlow, zhigh),
  Hist(gate, tree),
  fParamX("fParamX", ParseParam3d(param,0).c_str(), tree),
  fParamY("fParamY", ParseParam3d(param,1).c_str(), tree),
  fParamZ("fParamZ", ParseParam3d(param,2).c_str(), tree)
{
  if(fTree && fGate.GetTree() &&
     fParamX.GetTree() &&
     fParamY.GetTree()) {
    Hist::Lock();
    fgArray.Add(this);
    Hist::Unlock();
  }
  else {
    this->Delete();
  }
}

// Destructor
rb::H3D::~H3D() {
  Hist::Lock();
  fgArray.Remove(this);
  fgArray.Compress();
  Hist::Unlock();
}

// Draw function
void rb::H3D::Draw(Option_t* option) {
  Hist::Lock();
  TH3D::Draw(option);
  Hist::Unlock();
}

// Fill function
Int_t rb::H3D::Fill() {
  Hist::Lock();
  Int_t ret = fGate.EvalInstance() ?
    TH3D::Fill(fParamX.EvalInstance(),
	       fParamY.EvalInstance(),
	       fParamZ.EvalInstance())
    : 0;
  Hist::Unlock();
  return ret;
}

// Clear function
void rb::H3D::Clear(Option_t* option) {
  if(option != "") Warning("Clear", "Option %s ignored", option);

  Hist::Lock();
  for(Int_t i=1; i<= GetNbinsX(); ++i) {
    for(Int_t j=1; j<= GetNbinsY(); ++j) {
      for(Int_t k=1; k<= GetNbinsZ(); ++k) {
	SetBinContent(i, j, k, 0);
      }
    }
  }
  Hist::Unlock();
}
