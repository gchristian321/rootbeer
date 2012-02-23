//! \file Hist.cxx
//! \brief Implements the histogram class member functions.
#include <algorithm>
#include <iostream>
#include <fstream>
#include "Hist.hxx"
using namespace std;


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Utility functions                                     //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// tokenize                                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
/// Divides a string into tokens
inline vector<string> tokenize(const char* str, const char* token) {
  vector<string> out;
  string s(str), t(token);
  while(1) {
    ULong_t pos = s.rfind(token);
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

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  check_gate                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
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

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  check_name                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
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
    Info("rb::Hist::New", "The name %s is already in use, creating %s_%d instead.", name, name, n-1);
  }
  return ret;
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                                 //
// rb::Hist                                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Static data members.                                  //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::Mutex rb::Hist::fgMutex (kFALSE);
// volatile rb::Hist::List_t rb::Hist::fgList;
// volatile TTree  rb::Hist::fgTree;


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::Hist::Hist(const char* name, const char* title, const char* param, const char* gate, UInt_t npar, TTree* tree, Set_t* set) :
  kConstructorSuccess(kTRUE), kDimensions(npar), fHistogramClone(0), fTree(tree), fSet(set) {
  LockFreePointer<CriticalElements>  critical(fCritical);
  critical->fHistogram = 0;

  // Initialize gate
  fCritical.fGate = new TTreeFormula("fGate", check_gate(gate).c_str(), critical->GetTree(this));
  if(!fCritical.fGate->GetNdim()) {
    kConstructorSuccess = kFALSE;
    return;
  }

  // Initialize parameters
  vector<string> vPar = tokenize(param, ":");
  if(vPar.size() != npar) {
    Error("Hist", "Parameter specification %s is invalid for a %d-dimensional histogram.",
	  param, npar);
    kConstructorSuccess = kFALSE;
    return;
  }

  for(UInt_t i=0; i< npar; ++i) {
    stringstream parname; parname << "param" << i;
    critical->fParams.push_back(new TTreeFormula(parname.str().c_str(), vPar[i].c_str(), critical->GetTree(this)));

    if(!critical->fParams[i]->GetNdim())
      kConstructorSuccess = kFALSE;
  }

  // Set name & title
  fName  = check_name(name).c_str();

  stringstream sstr;
  sstr << param << " {" << fCritical.fGate->GetExpFormula().Data() << "}";
  kDefaultTitle = sstr.str();
  if(string(title).empty())
    fTitle = kDefaultTitle.c_str();
  else
    fTitle = title;
  kInitialTitle = fTitle;
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destuctor                                             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::Hist::~Hist() {
  LockingPointer<CriticalElements> critical(fCritical, fgMutex());

  // remove from fgList and directory
  List_t* pList = critical->GetList();
  List_t::iterator it = find(pList->begin(), pList->end(), this);
  pList->erase(it);
  fDirectory->Remove(this);

  // free memory allocated to TTreeFormulae
  delete critical->fGate;
  for(UInt_t i=0; i< critical->fParams.size(); ++i)
    delete critical->fParams[i];

  if(critical->fHistogram) delete critical->fHistogram;
  if(fHistogramClone) delete fHistogramClone;
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// static rb::Hist::Initialize                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t rb::Hist::Initialize(const char* name, const char* title,
			    const char* param, const char* gate,
			    UInt_t ndim, TTree* tree, Set_t* set,
			    Int_t nbinsx, Double_t xlow, Double_t xhigh,
			    Int_t nbinsy, Double_t ylow, Double_t yhigh,
			    Int_t nbinsz, Double_t zlow, Double_t zhigh) {
  // Create rb::Hist instance
  rb::Hist* _this = new rb::Hist(name, title, param, gate, ndim, tree, set);
  if(!_this->kConstructorSuccess) return kFALSE;

  // Set internal histogram
  //! \note The histogram isn't accessable to any other threads until we add it to
  //! fgList, so it's safe to access the critical elements via a non-locking pointer.
  LockFreePointer<CriticalElements> unlocked_critical(_this->fCritical);

  Bool_t successfulHistCreation = kTRUE;
  TH1::AddDirectory(kFALSE);
  switch(ndim) {
  case 1:
    unlocked_critical->fHistogram =
      new TH1D(_this->fName, _this->fTitle, nbinsx, xlow, xhigh);
    unlocked_critical->fHistogram->GetXaxis()->SetTitle(unlocked_critical->fParams.at(0)->GetExpFormula());
    break;
  case 2:
    unlocked_critical->fHistogram =
      new TH2D(_this->fName, _this->fTitle, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
    unlocked_critical->fHistogram->GetXaxis()->SetTitle(unlocked_critical->fParams.at(0)->GetExpFormula());
    unlocked_critical->fHistogram->GetYaxis()->SetTitle(unlocked_critical->fParams.at(1)->GetExpFormula());
    break;
  case 3:
    unlocked_critical->fHistogram =
      new TH3D(_this->fName, _this->fTitle, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh, nbinsz, zlow, zhigh);
    unlocked_critical->fHistogram->GetXaxis()->SetTitle(unlocked_critical->fParams.at(0)->GetExpFormula());
    unlocked_critical->fHistogram->GetYaxis()->SetTitle(unlocked_critical->fParams.at(1)->GetExpFormula());
    unlocked_critical->fHistogram->GetZaxis()->SetTitle(unlocked_critical->fParams.at(2)->GetExpFormula());
    break;
  default:
    _this->Error("Initialize", "%d-dimenstional histograms are not supported.", ndim);
    successfulHistCreation = kFALSE;
    break;
  }
  TH1::AddDirectory(kTRUE);

  // Add to collections
  LockingPointer<List_t> hlist(fgList(), fgMutex());
  if(successfulHistCreation) {
    hlist->push_back(_this);

    if(gDirectory) {
      _this->fDirectory = gDirectory;
      _this->fDirectory->Append(_this, kTRUE);
    }
  }
  return successfulHistCreation;
}

// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // static rb::Hist::New (One-dimensional)                //
// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Hist::New(const char* name, const char* title,
// 		   Int_t nbinsx, Double_t xlow, Double_t xhigh,
// 		   const char* param, const char* gate) {
//   rb::Hist::Initialize(name, title, param, gate, 1, nbinsx, xlow, xhigh);
// }

// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // static rb::Hist::New (Two-dimensional)                //
// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Hist::New(const char* name, const char* title,
// 		   Int_t nbinsx, Double_t xlow, Double_t xhigh,
// 		   Int_t nbinsy, Double_t ylow, Double_t yhigh,
// 		   const char* param, const char* gate) {
//   rb::Hist::Initialize(name, title, param, gate, 2, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
// }

// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // static rb::Hist::New (Three-dimensional)              //
// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Hist::New(const char* name, const char* title,
// 		   Int_t nbinsx, Double_t xlow, Double_t xhigh,
// 		   Int_t nbinsy, Double_t ylow, Double_t yhigh,
// 		   Int_t nbinsz, Double_t zlow, Double_t zhigh,
// 		   const char* param, const char* gate) {
//   rb::Hist::Initialize(name, title, param, gate, 3, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh, nbinsz, zlow, zhigh);
// }

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::Hist::Regate                                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Int_t rb::Hist::Regate(const char* newgate) {
  LockingPointer<CriticalElements> critical(fCritical, fgMutex());

  // check that new gate formula is valid
  TTreeFormula tempFormula("temp", check_gate(newgate).c_str(), critical->GetTree(this));
  if(!tempFormula.GetNdim()) {
    return -1;
  }

  // It's valid, so set the new gate
  if(critical->fGate) delete critical->fGate;
  critical->fGate = new TTreeFormula("fGate", check_gate(newgate).c_str(), critical->GetTree(this));

  // Change title if appropriate
  if(kInitialTitle == kDefaultTitle) {
    stringstream newTitle;
    for(Int_t i = GetNdimensions()-1; i > 0; --i)
      newTitle << critical->fParams.at(i)->GetExpFormula().Data() << ":";
    newTitle << critical->fParams.at(0)->GetExpFormula().Data();
    newTitle << " {" << critical->fGate->GetExpFormula().Data() << "}";
    fTitle = newTitle.str().c_str();
    critical->fHistogram->SetTitle(fTitle);
  }
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::Hist::GetParam                                    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
string rb::Hist::GetParam(UInt_t axis) {
  LockingPointer<CriticalElements> critical(fCritical, fgMutex());
  if(axis < critical->fParams.size()) {
    return critical->fParams[axis]->GetExpFormula().Data();
  }
  Error("GetParam", "Invalid axis: %d, maximum is %d (X=0, Y=1, Z=2).",
	axis, Int_t(critical->fParams.size()));
  return "";
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::Hist::GetHist()                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
TH1* rb::Hist::GetHist() {
  if(fHistogramClone) delete fHistogramClone;
  TH1::AddDirectory(kFALSE);
  fHistogramClone =
    static_cast<TH1*>(LockingPointer<CriticalElements>(fCritical, fgMutex())->fHistogram->Clone());
  TH1::AddDirectory(kTRUE);
  return fHistogramClone;
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::Hist::DoFill()                                    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Int_t rb::Hist::DoFill(TH1* hst, TTreeFormula* gate, vector<TTreeFormula*>& params) {
   if(!gate->EvalInstance()) return 0;

   switch(kDimensions) {
   case 1:
     return static_cast<TH1D*>(hst)->Fill(params[0]->EvalInstance());
     break;
   case 2:
     return static_cast<TH2D*>(hst)->Fill(params[0]->EvalInstance(),
					  params[1]->EvalInstance());
     break;
   case 3:
     return static_cast<TH3D*>(hst)->Fill(params[0]->EvalInstance(),
					  params[1]->EvalInstance(),
					  params[2]->EvalInstance());
     break;
   default:
     Error("Fill", "Invalid kDimensions %d", kDimensions);
     return -1;
     break;
   }
 }


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// static rb::Hist::FillAll                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Hist::FillAll() {
  LockingPointer<List_t> hlist(fgList(), fgMutex()); // makes it safe to not lock the others
  List_t::iterator it;
  for(it = hlist->begin(); it != hlist->end(); ++it) {
    rb::Hist* pHst = *it;
    LockFreePointer<CriticalElements> critical(pHst->fCritical);
    pHst->DoFill(critical->fHistogram, critical->fGate, critical->fParams);
  }
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// static rb::Hist::DeleteAll                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Hist::DeleteAll() {
  rb::Mutex deleteMutex; // create a local mutex to avoid deadlock when calling delete
  LockingPointer<List_t> hlist(fgList(), deleteMutex);
   while(1) {
     if(hlist->size() == 0) break;
     delete *hlist->begin();
   }
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::Hist::Fill()                                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Int_t rb::Hist::Fill() {
  LockingPointer<CriticalElements> critical(fCritical, fgMutex());
  return DoFill(critical->fHistogram, critical->fGate, critical->fParams);
}



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                                 //
// rb::SummaryHist                                       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::SummaryHist::SummaryHist                          //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::SummaryHist::SummaryHist(const char* name, const char* title, const char* param, const char* gate, const char* orient, TTree* tree) {
  fTree = tree;
  kConstructorSuccess = kTRUE; kDimensions = 2; fHistogramClone = 0;
  TString sOrient(orient);
  sOrient.ToLower();
  kOrient = 0; // vertical
  if (!sOrient.CompareTo("h")) kOrient = 1;
  else if (sOrient.CompareTo("v"))
    Warning("SummaryHist", "Orientation option %s ws not recognzed. Defaulting to vertical", orient);
  else ;

  LockFreePointer<CriticalElements>  critical(fCritical);
  critical->fHistogram = 0;

  // Initialize gate
  fCritical.fGate = new TTreeFormula("fGate", check_gate(gate).c_str(), critical->GetTree(this));
  if(!fCritical.fGate->GetNdim()) {
    kConstructorSuccess = kFALSE;
    return;
  }

  // Initialize parameters
  vector<string> vPar = tokenize(param, ";");
  for(Int_t i = vPar.size()-1; i >= 0; --i) {
    stringstream parname; parname << "param" << i-vPar.size();

    string param = vPar[i];
    ULong_t brktPos = param.find("["), dashPos = param.find("-"), brktPosLast = param.find("]");
    if(brktPos > param.size() || dashPos > param.size()) {
      critical->fParams.push_back(new TTreeFormula(parname.str().c_str(), param.c_str(), critical->GetTree(this)));
      if(!critical->fParams[critical->fParams.size()-1]->GetNdim()) {
	kConstructorSuccess = kFALSE;
	return;
      }
    }
    else {
      string sFirst = param.substr(brktPos + 1, dashPos - brktPos - 1);
      string sLast  = param.substr(dashPos + 1, brktPosLast - dashPos - 1);
      string sBase  = param.substr(0, brktPos);
      UInt_t first = atoi(sFirst.c_str()), last = atoi(sLast.c_str());
      for(UInt_t indx = first; indx <= last; ++indx) {
	stringstream sstrParam;
	sstrParam << sBase << "[" << indx << "]";
	parname << "_" << indx;
	critical->fParams.push_back(new TTreeFormula(parname.str().c_str(), sstrParam.str().c_str(), critical->GetTree(this)));
	if(!critical->fParams[critical->fParams.size()-1]->GetNdim()) {
	  kConstructorSuccess = kFALSE;
	  return;
	}
      }
    }
    nPar = critical->fParams.size();
  }

  // Set name & title
  fName  = check_name(name).c_str();

  stringstream sstr;
  sstr << param << " {" << fCritical.fGate->GetExpFormula().Data() << "}";
  kDefaultTitle = sstr.str();
  if(string(title).empty())
    fTitle = kDefaultTitle.c_str();
  else
    fTitle = title;
  kInitialTitle = fTitle;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::SummaryHist::New()                                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::SummaryHist::New(const char* name, const char* title,
// 		    Int_t nbins, Double_t low, Double_t high,
// 		    const char* paramList,  const char* gate,
// 		    const char* orientation) {
//   // Create rb::Hist instance
//   rb::SummaryHist* _this = new rb::SummaryHist(name, title, paramList, gate, orientation);
//   if(!_this->kConstructorSuccess) return;

//   // Set internal histogram
//   //! \note The histogram isn't accessable to any other threads until we add it to
//   //! fgList, so it's safe to access the critical elements via a non-locking pointer.
//   LockFreePointer<CriticalElements> unlocked_critical(_this->fCritical);

//   Bool_t successfulHistCreation = kTRUE;
//   TH1::AddDirectory(kFALSE);

//   Int_t npar = unlocked_critical->fParams.size();

//   if(!_this->kOrient) { // vertical
//     unlocked_critical->fHistogram =
//       new TH2D(_this->fName, _this->fTitle, npar, 0, npar, nbins, low, high);
//     unlocked_critical->fHistogram->GetXaxis()->SetTitle(paramList);
//     unlocked_critical->fHistogram->GetYaxis()->SetTitle("");
//   }
//   else { // horizontal
//     unlocked_critical->fHistogram =
//       new TH2D(_this->fName, _this->fTitle, nbins, low, high, npar, 0, npar);
//     unlocked_critical->fHistogram->GetXaxis()->SetTitle("");
//     unlocked_critical->fHistogram->GetYaxis()->SetTitle(paramList);
//   }
//   TH1::AddDirectory(kTRUE);

//   // Add to collections
//   LockingPointer<List_t> hlist(fgList(), fgMutex());
//   if(successfulHistCreation) {
//     hlist->push_back(_this);

//     if(gDirectory) {
//       _this->fDirectory = gDirectory;
//       _this->fDirectory->Append(_this, kTRUE);
//     }
//   }
// }

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::Hist::DoFill()                                    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Int_t rb::SummaryHist::DoFill(TH1* hst, TTreeFormula* gate, vector<TTreeFormula*>& params) {
  if(!gate->EvalInstance()) return 0;
  TH2D* h2d = static_cast<TH2D*>(hst);
  for(Int_t i = 0; i< GetNPar(); ++i) {
    Double_t parVal = params[i]->EvalInstance();
    kOrient ?  h2d->Fill(parVal, i): h2d->Fill(i, parVal);
  }
  return 0;
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                                 //
// rb::GammaHist                                         //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::GammaHist::GammaHist                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::GammaHist::GammaHist(const char* name, const char* title, const char* param, const char* gate, Int_t dimensions, TTree* tree) {
  fTree = tree;
  kConstructorSuccess = kTRUE; fHistogramClone = 0; kDimensions = dimensions;

  LockFreePointer<CriticalElements>  critical(fCritical);
  critical->fHistogram = 0;

  // Initialize gate
  fCritical.fGate = new TTreeFormula("fGate", check_gate(gate).c_str(), critical->GetTree(this));
  if(!fCritical.fGate->GetNdim()) {
    kConstructorSuccess = kFALSE;
    return;
  }

  // Initialize parameters
  vector<string> vPar1 = tokenize(param, ":");
  if(kDimensions != vPar1.size()) {
    Error("GammaHist", "Invalid parameter argument for a %d-dimensional hist (%s)", dimensions, param);
    kConstructorSuccess = kFALSE;
    return;
  }
  vector<Int_t> nParams;
  for(UInt_t j=0; j< vPar1.size(); ++j) {
    vector<string> vPar = tokenize(vPar1[j].c_str(), ";");
    nParams.push_back(vPar.size());

    for(Int_t i = vPar.size()-1; i >= 0; --i) {
      stringstream parname; parname << "param" << j << "_" << i-vPar.size();
      string param = vPar[i];
      ULong_t brktPos = param.find("["), dashPos = param.find("-"), brktPosLast = param.find("]");
      if(brktPos > param.size() || dashPos > param.size()) {
	critical->fParams.push_back(new TTreeFormula(parname.str().c_str(), param.c_str(), critical->GetTree(this)));
	if(!critical->fParams[critical->fParams.size()-1]->GetNdim()) {
	  kConstructorSuccess = kFALSE;
	  return;
	}
      }
      else {
	string sFirst = param.substr(brktPos + 1, dashPos - brktPos - 1);
	string sLast  = param.substr(dashPos + 1, brktPosLast - dashPos - 1);
	string sBase  = param.substr(0, brktPos);
	UInt_t first = atoi(sFirst.c_str()), last = atoi(sLast.c_str());
	for(UInt_t indx = first; indx <= last; ++indx) {
	  stringstream sstrParam;
	  sstrParam << sBase << "[" << indx << "]";
	  parname << "_" << indx;
	  critical->fParams.push_back(new TTreeFormula(parname.str().c_str(), sstrParam.str().c_str(), critical->GetTree(this)));
	  if(!critical->fParams[critical->fParams.size()-1]->GetNdim()) {
	    kConstructorSuccess = kFALSE;
	    return;
	  }
	}
      }
      nPar = critical->fParams.size();
    }
  }

  if(nParams.size() == 0) {
    Error("GammaHist", "Invalid parameter argument %s", param);
    kConstructorSuccess = kFALSE;
    return;
  }
  else if (nParams.size() > 1) {
    Int_t nParams0 = nParams[0];
    for(UInt_t k=1; k< nParams.size(); ++k) {
      if(nParams[k] != nParams0) {
	Error("GammaHist", "Invalid parameter argument %s", param);
	kConstructorSuccess = kFALSE;
	return;
      }
    }
  }
  else ;	

  // Set name & title
  fName  = check_name(name).c_str();

  stringstream sstr;
  sstr << param << " {" << fCritical.fGate->GetExpFormula().Data() << "}";
  kDefaultTitle = sstr.str();
  if(string(title).empty())
    fTitle = kDefaultTitle.c_str();
  else
    fTitle = title;
  kInitialTitle = fTitle;
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// static rb::GammaHist::Initialize                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t rb::GammaHist::GInitialize(const char* name, const char* title,
				  const char* paramList, const char* gate,
				  UInt_t ndim, TTree* tree,
				  Int_t nbinsx, Double_t xlow, Double_t xhigh,
				  Int_t nbinsy, Double_t ylow, Double_t yhigh,
				  Int_t nbinsz, Double_t zlow, Double_t zhigh) {

  // Create rb::Hist instance
  rb::GammaHist* _this = new rb::GammaHist(name, title, paramList, gate, ndim, tree);
  if(!_this->kConstructorSuccess) return kFALSE;

  // Set internal histogram
  //! \note The histogram isn't accessable to any other threads until we add it to
  //! fgList, so it's safe to access the critical elements via a non-locking pointer.
  LockFreePointer<CriticalElements> unlocked_critical(_this->fCritical);

  Bool_t successfulHistCreation = kTRUE;
  TH1::AddDirectory(kFALSE);

  Int_t npar = unlocked_critical->fParams.size();

  switch(_this->kDimensions) {
  case 1: {
    unlocked_critical->fHistogram =
      new TH1D(_this->fName, _this->fTitle, nbinsx, xlow, xhigh);
    unlocked_critical->fHistogram->GetXaxis()->SetTitle(paramList);
    unlocked_critical->fHistogram->GetYaxis()->SetTitle("");
    break;
  }
  case 2: {
    unlocked_critical->fHistogram =
      new TH2D(_this->fName, _this->fTitle, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
    vector<string> pars = tokenize(paramList, ":");
    unlocked_critical->fHistogram->GetXaxis()->SetTitle(pars[1].c_str());
    unlocked_critical->fHistogram->GetYaxis()->SetTitle(pars[0].c_str());
    break;
  }
  default:
    fprintf(stderr, "Error in <GInitialize>: %d-dimensional Gamma Histograms are not yet supported", _this->kDimensions);
    TH1::AddDirectory(kTRUE);
    return kFALSE;
    break;
  }

  TH1::AddDirectory(kTRUE);

  // Add to collections
  LockingPointer<List_t> hlist(fgList(), fgMutex());
  if(successfulHistCreation) {
    hlist->push_back(_this);

    if(gDirectory) {
      _this->fDirectory = gDirectory;
      _this->fDirectory->Append(_this, kTRUE);
    }
  }
  return kTRUE;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::GammaHist::DoFill()                               //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Int_t rb::GammaHist::DoFill(TH1* hst, TTreeFormula* gate, vector<TTreeFormula*>& params) {
  if(!gate->EvalInstance()) return 0;

  Int_t ret = -1;
  switch(kDimensions) {
  case 1:
    for(Int_t i=0; i< params.size(); ++i)
      static_cast<TH1D*>(hst)->Fill(params[i]->EvalInstance());
    ret = 1;
    break;
  case 2:
    for(Int_t i=0; i< params.size() / 2; ++i)
      static_cast<TH2D*>(hst)->Fill(params[i]->EvalInstance(), params[i+params.size()/2]->EvalInstance());
    ret = 1;
    break;
  default:
    Error("DoFill", "Invalid kDimensions %d", kDimensions);
    ret = -1;
    break;
  }
  return ret;
}

// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // static rb::GammaHist::New (One-dimensional)           //
// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::GammaHist::New(const char* name, const char* title,
// 		    Int_t nbinsx, Double_t xlow, Double_t xhigh,
// 		    const char* param, const char* gate) {
//   rb::GammaHist::GInitialize(name, title, param, gate, 1, nbinsx, xlow, xhigh);
// }

// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // static rb::GammaHist::New (Two-dimensional)           //
// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::GammaHist::New(const char* name, const char* title,
// 		    Int_t nbinsx, Double_t xlow, Double_t xhigh,
// 		    Int_t nbinsy, Double_t ylow, Double_t yhigh,
// 		    const char* param, const char* gate) {
//   rb::GammaHist::GInitialize(name, title, param, gate, 2, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
// }
