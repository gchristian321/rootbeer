/*! \file Hist.cxx
 *  \brief Implements the histogram class member functions.
 */
#include "Hist.hxx"
using namespace std;


// Some utility functions for delegation.

/// Function to divide strings by tokens
inline vector<string> tokenize(const char* str, const char* token) {
  vector<string> out;
  string s(str), t(token);
  while(1) {
    int pos = s.rfind(token);
    if(pos< s.size()) {
      out.push_back(s.substr(pos+t.size()));
      s.erase(pos);
    }
    else {
      out.push_back(s);
      break;
    }
  }
  return out;
}

/// Gate checking function
inline std::string check_gate(const char* gate) {
  TString tsGate(gate);
  tsGate.ReplaceAll(" ", "");
  std::string sGate(tsGate.Data());
  if(0);
  else if(sGate == "") return "1";   // Null field means no gate, i.e. always true.
  else if(sGate == "0") return "!1"; // Somehow "0" evaluates to true, should be false.
  else return sGate;
}

/// Name checking function
inline std::string check_name(const char* name) {
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




// Class rb::Hist //

// Static data members.
TObjArray rb::Hist::fgArray;

TMutex rb::Hist::fgMutex;

TTree rb::Hist::fgTree;

Bool_t rb::Hist::kConstructorSuccess;


// Constructor
rb::Hist::Hist(const char* param, const char* gate, UInt_t npar) : 
  fGate("fGate", check_gate(gate).c_str(), &fgTree) {
  kConstructorSuccess = kTRUE;
  vector<string> vPar = tokenize(param, ":");

  if(vPar.size() == npar) {
    for(UInt_t i=0; i< npar; ++i) {
      stringstream name; name << "param" << i;
      fParams.push_back(new TTreeFormula(name.str().c_str(), vPar[i].c_str(), &fgTree));

      if(!fParams[i]->GetTree()) kConstructorSuccess = kFALSE; // Tell derived classes that the constrctor failed.
    }
  }

  else {
    Error("Hist", "Parameter specification %s is invalid for a %d-dimensional histogram.",
	  param, npar);
    kConstructorSuccess = kFALSE; // Tell derived classes that the constrctor failed.
  }
}


// Regate function
Int_t rb::Hist::Regate(const char* newgate) {
  TTreeFormula tempFormula("temp", check_gate(newgate).c_str(), &fgTree);
  if(!tempFormula.GetTree()) return -1;

  Hist::Lock();
  string oldGate = GetGate();

  // set new gate
  fGate.Compile(check_gate(newgate).c_str());

  // change title if appropriate
  TH1* hst = dynamic_cast<TH1*>(this);
  if(hst) { // yes we inherit from TH1 and can use SetTitle(), etc.
    // Reconstruct initial param argument
    stringstream sPar, sAll;
    for(Int_t i = GetNdimensions()-1; i > 0; --i)
      sPar << GetParam(i) << ":";
    sPar << GetParam(0);
    sAll << sPar.str() << " {" << oldGate << "}";

    if(sAll.str() == hst->GetTitle()) { // no custom title
      sAll.str("");
      sAll << sPar.str() << " {" << GetGate() << "}";
      hst->SetTitle(sAll.str().c_str());
    }
  }

  Hist::Unlock();
  return 0;
}

// Return gate string
string rb::Hist::GetGate() {
  return fGate.GetExpFormula().Data();
}

// Return parameter string
string rb::Hist::GetParam(Int_t axis) {
  if(axis< fParams.size())
    return fParams[axis]->GetExpFormula().Data();
  Error("GetParam", "Invalid axis: %d, maximum is %d (X=0, Y=1, Z=2).",
	axis, Int_t(fParams.size()));
  return "";
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

// Static branch creation function
TBranch* rb::Hist::CreateBranch(const char* name, const char* classname, void** obj,
				Int_t bufsize, Int_t splitlevel) {
  return fgTree.Branch(name, classname, obj, bufsize, splitlevel);
}

// Static alias setting function
Bool_t rb::Hist::SetAlias(const char* aliasName, const char* aliasFormula) {
  return fgTree.SetAlias(aliasName, aliasFormula);
}
  
// Static mutex locking function
void rb::Hist::Lock() {
  fgMutex.Lock();
}

// Static mutex un-locking function
void rb::Hist::Unlock() {
  fgMutex.UnLock();
}

// Static fill all function
void rb::Hist::FillAll() {
  for(UInt_t indx = 0; indx < Hist::GetNumber(); ++indx) {
    Hist* pHist = Hist::Get(indx);
    if(pHist) pHist->Fill();
  }
}

// Class rb::H1D //

// Constructor
rb::H1D::H1D (const char* name, const char* title,
	      Int_t nbinsx, Double_t xlow, Double_t xhigh,
	      const char* param, const char* gate) :
  TH1D (check_name(name).c_str(), title, nbinsx, xlow, xhigh),
  Hist(param, gate, 1)
{
  if(kConstructorSuccess) {
    if(string(title) == "") {
      stringstream sstr;
      sstr << param << " {" << GetGate() << "}";
      SetTitle(sstr.str().c_str());
    }
    GetXaxis()->SetTitle(fParams.at(0)->GetExpFormula());
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
    TH1D::Fill(fParams[0]->EvalInstance())
    : 0;
  Hist::Unlock();
  return ret;
}

// Clear Function
void rb::H1D::Clear(const Option_t* option) {
  Hist::Lock();
  for(Int_t i=0; i< fN; ++i) fArray[i] = 0;
  Hist::Unlock();
  if(string(option) != "")
    Warning("Clear", "Option %s ignored", option);
}


// Class rb::H2D //

// Constructor
rb::H2D::H2D (const char* name, const char* title,
	      Int_t nbinsx, Double_t xlow, Double_t xhigh,
	      Int_t nbinsy, Double_t ylow, Double_t yhigh,
	      const char* param, const char* gate) : 
  TH2D (check_name(name).c_str(), title,
	nbinsx, xlow, xhigh,
	nbinsy, ylow, yhigh),
  Hist(param, gate, 2)
{
  if(kConstructorSuccess) {
    if(string(title) == "") {
      stringstream sstr;
      sstr << param << " {" << GetGate() << "}";
      SetTitle(sstr.str().c_str());
    }
    GetXaxis()->SetTitle(fParams.at(0)->GetExpFormula());
    GetYaxis()->SetTitle(fParams.at(1)->GetExpFormula());
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
    TH2D::Fill(fParams[0]->EvalInstance(),
	       fParams[1]->EvalInstance())
    : 0;
  Hist::Unlock();
  return ret;
}

// Clear Function
void rb::H2D::Clear(const Option_t* option) {
  Hist::Lock();
  for(Int_t i=0; i< fN; ++i) fArray[i] = 0;
  Hist::Unlock();
  if(string(option) != "")
    Warning("Clear", "Option %s ignored", option);
}


// Class rb::H3D //

// Constructor
rb::H3D::H3D (const char* name, const char* title,
	      Int_t nbinsx, Double_t xlow, Double_t xhigh,
	      Int_t nbinsy, Double_t ylow, Double_t yhigh,
	      Int_t nbinsz, Double_t zlow, Double_t zhigh,
	      const char* param, const char* gate) :
  TH3D (check_name(name).c_str(), title,
	nbinsx, xlow, xhigh,
	nbinsy, ylow, yhigh,
	nbinsz, zlow, zhigh),
  Hist(param, gate, 3) {

  if(kConstructorSuccess) {
    if(string(title) == "") {
      stringstream sstr;
      sstr << param << " {" << GetGate() << "}";
      SetTitle(sstr.str().c_str());
    }
    GetXaxis()->SetTitle(fParams.at(0)->GetExpFormula());
    GetYaxis()->SetTitle(fParams.at(1)->GetExpFormula());
    GetZaxis()->SetTitle(fParams.at(2)->GetExpFormula());
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
    TH3D::Fill(fParams[0]->EvalInstance(),
	       fParams[1]->EvalInstance(),
	       fParams[2]->EvalInstance())
    : 0;
  Hist::Unlock();
  return ret;
}

// Clear Function
void rb::H3D::Clear(const Option_t* option) {
  Hist::Lock();
  for(Int_t i=0; i< fN; ++i) fArray[i] = 0;
  Hist::Unlock();
  if(string(option) != "")
    Warning("Clear", "Option %s ignored", option);
}

