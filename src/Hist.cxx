//! \file Hist.cxx
//! \brief Implements the histogram class member functions.
#include <algorithm>
#include <iostream>
#include <fstream>
#include "Hist.hxx"

typedef std::vector<std::string> StringVector_t;

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Utility functions & classes                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace
{
  /// Divides a string into tokens
  inline StringVector_t tokenize(const char* str, char token) {
    std::istringstream iss(str);
    std::string entry;
    StringVector_t out;
    while(std::getline(iss, entry, token))
      out.push_back(entry);
    return out;
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
      err::Info("rb::Hist::Hist") << "The name " << name <<
	" is already in use, creating " << name << "_" << n-1 << " instead.";
    }
    return ret;
  }
  /// Reconstruct original parameter argument 
  inline std::string initial_param_arg(Int_t ndimensions, rb::formula::HistWrapper& gate_par) {
    std::stringstream out;
    for(Int_t i = ndimensions-1; i > 0; --i)
      out << gate_par.Get.OperateUnlocked(i) << ":"; // << NOTE: No Mutex Locking //
    out << gate_par.Get.OperateUnlocked(0);
    return out.str();
  }
  /// Set default title
  inline std::string default_title(const char* gate, const char* params) {
    std::stringstream out;
    out << params << " { " << gate << " }";
    return out.str();
  }
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                                 //
// rb::Hist                                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destuctor                                             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::Hist::~Hist() {
  rb::ScopedLock<rb::Mutex> lock(fMutex);
  fDirectory->Remove(this);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Hist::Init()                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Hist::Init(const char* name, const char* title,  const char* param, const char* gate) {
  // Set name & title
  fName = check_name(name).c_str();
  kDefaultTitle = default_title(fGateParams.Get(GATE).c_str(), param);
  kUseDefaultTitle = std::string(title).empty();
  fTitle = kUseDefaultTitle ? kDefaultTitle.c_str() : title;

  // Add to collections
  ScopedLock<rb::Mutex> lock(fMutex);
  if(gDirectory) {
    this->fDirectory = gDirectory;
    this->fDirectory->Append(this, kTRUE);
  }
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::Hist::Regate                                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Int_t rb::Hist::Regate(const char* newgate) {
  Bool_t success = fGateParams.Change(GATE, newgate);
  if(!success) return -1;

  // Change title if appropriate
  if(kUseDefaultTitle) {
    fTitle = default_title(fGateParams.Get(GATE).c_str(), // locks data mutex
			   initial_param_arg(kDimensions, fGateParams).c_str()).c_str();
    visit::hist::DoMember(&fMutex, fHistVariant, &TH1::SetTitle, fTitle.Data());
  }
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::Hist::GetHist()                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
TH1* rb::Hist::GetHist() {
  hist::StopAddDirectory stop_add;
  visit::hist::Clone::Do(&fMutex, fHistVariant, fHistogramClone);
  return fHistogramClone.get();
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::Hist::DoFill()                                    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Int_t rb::Hist::DoFill(HistVariant& hst, boost::scoped_ptr<TTreeFormula>& gate, boost::scoped_ptr<TTreeFormula>* params) {
  // if(!(bool)fGateParams.Eval(GATE)) return 0;
  //  Double_t axes[3] = {0,0,0};
  //  for(UInt_t u=0; u< kDimensions; ++u)
  //    axes[u] = 
  //  visit::hist::Fill::Do(0, hst, axes[0], axes[1], axes[2]);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// static rb::Hist::DeleteAll                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Hist::DeleteAll() {
  std::cerr << "Info in <rb::Hist::DeleteAll>: Calling...but it doesn't do anything!\n";
  // rb::Mutex deleteMutex; // create a local mutex to avoid deadlock when calling delete
  // LockingPointer<List_t> hlist(fgList(), deleteMutex);
  //  while(1) {
  //    if(hlist->size() == 0) break;
  //    delete *hlist->begin();
  //  }
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::Hist::Fill()                                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Int_t rb::Hist::Fill() {
  if(!(bool)fGateParams.Eval.OperateUnlocked(GATE)) return 0;
  Double_t axes[3] = {0,0,0};
  for(UInt_t u=0; u< kDimensions; ++u)
    axes[u] = fGateParams.Eval.OperateUnlocked(u);
  visit::hist::Fill::Do(0, fHistVariant, axes[0], axes[1], axes[2]);
}






















// #if 0
// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // Class                                                 //
// // rb::SummaryHist                                       //
// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // rb::SummaryHist::SummaryHist                          //
// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::SummaryHist::SummaryHist(const char* name, const char* title, const char* param, const char* gate, const char* orient, TTree* tree) {
//   fTree = tree;
//   kConstructorSuccess = kTRUE; kDimensions = 2; fHistogramClone = 0;
//   TString sOrient(orient);
//   sOrient.ToLower();
//   kOrient = 0; // vertical
//   if (!sOrient.CompareTo("h")) kOrient = 1;
//   else if (sOrient.CompareTo("v"))
//     Warning("SummaryHist", "Orientation option %s ws not recognzed. Defaulting to vertical", orient);
//   else ;

//   LockFreePointer<CriticalElements>  critical(fCritical);
//   critical->fHistogram = 0;

//   // Initialize gate
//   fCritical.fGate = new TTreeFormula("fGate", check_gate(gate).c_str(), critical->GetTree(this));
//   if(!fCritical.fGate->GetNdim()) {
//     kConstructorSuccess = kFALSE;
//     return;
//   }

//   // Initialize parameters
//   StringVector_t vPar = tokenize(param, ';');
//   for(Int_t i = vPar.size()-1; i >= 0; --i) {
//     stringstream parname; parname << "param" << i-vPar.size();

//     string param = vPar[i];
//     ULong_t brktPos = param.find("["), dashPos = param.find("-"), brktPosLast = param.find("]");
//     if(brktPos > param.size() || dashPos > param.size()) {
//       critical->fParams.push_back(new TTreeFormula(parname.str().c_str(), param.c_str(), critical->GetTree(this)));
//       if(!critical->fParams[critical->fParams.size()-1]->GetNdim()) {
// 	kConstructorSuccess = kFALSE;
// 	return;
//       }
//     }
//     else {
//       string sFirst = param.substr(brktPos + 1, dashPos - brktPos - 1);
//       string sLast  = param.substr(dashPos + 1, brktPosLast - dashPos - 1);
//       string sBase  = param.substr(0, brktPos);
//       UInt_t first = atoi(sFirst.c_str()), last = atoi(sLast.c_str());
//       for(UInt_t indx = first; indx <= last; ++indx) {
// 	stringstream sstrParam;
// 	sstrParam << sBase << "[" << indx << "]";
// 	parname << "_" << indx;
// 	critical->fParams.push_back(new TTreeFormula(parname.str().c_str(), sstrParam.str().c_str(), critical->GetTree(this)));
// 	if(!critical->fParams[critical->fParams.size()-1]->GetNdim()) {
// 	  kConstructorSuccess = kFALSE;
// 	  return;
// 	}
//       }
//     }
//     nPar = critical->fParams.size();
//   }

//   // Set name & title
//   fName  = check_name(name).c_str();

//   stringstream sstr;
//   sstr << param << " {" << fCritical.fGate->GetExpFormula().Data() << "}";
//   kDefaultTitle = sstr.str();
//   if(string(title).empty())
//     fTitle = kDefaultTitle.c_str();
//   else
//     fTitle = title;
//   kInitialTitle = fTitle;
// }

// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // rb::SummaryHist::New()                                //
// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // void rb::SummaryHist::New(const char* name, const char* title,
// // 		    Int_t nbins, Double_t low, Double_t high,
// // 		    const char* paramList,  const char* gate,
// // 		    const char* orientation) {
// //   // Create rb::Hist instance
// //   rb::SummaryHist* _this = new rb::SummaryHist(name, title, paramList, gate, orientation);
// //   if(!_this->kConstructorSuccess) return;

// //   // Set internal histogram
// //   //! \note The histogram isn't accessable to any other threads until we add it to
// //   //! fgList, so it's safe to access the critical elements via a non-locking pointer.
// //   LockFreePointer<CriticalElements> unlocked_critical(_this->fCritical);

// //   Bool_t successfulHistCreation = kTRUE;
// //   TH1::AddDirectory(kFALSE);

// //   Int_t npar = unlocked_critical->fParams.size();

// //   if(!_this->kOrient) { // vertical
// //     unlocked_critical->fHistogram =
// //       new TH2D(_this->fName, _this->fTitle, npar, 0, npar, nbins, low, high);
// //     unlocked_critical->fHistogram->GetXaxis()->SetTitle(paramList);
// //     unlocked_critical->fHistogram->GetYaxis()->SetTitle("");
// //   }
// //   else { // horizontal
// //     unlocked_critical->fHistogram =
// //       new TH2D(_this->fName, _this->fTitle, nbins, low, high, npar, 0, npar);
// //     unlocked_critical->fHistogram->GetXaxis()->SetTitle("");
// //     unlocked_critical->fHistogram->GetYaxis()->SetTitle(paramList);
// //   }
// //   TH1::AddDirectory(kTRUE);

// //   // Add to collections
// //   LockingPointer<List_t> hlist(fgList(), fMutex);
// //   if(successfulHistCreation) {
// //     hlist->push_back(_this);

// //     if(gDirectory) {
// //       _this->fDirectory = gDirectory;
// //       _this->fDirectory->Append(_this, kTRUE);
// //     }
// //   }
// // }

// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // rb::Hist::DoFill()                                    //
// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Int_t rb::SummaryHist::DoFill(TH1* hst, TTreeFormula* gate, vector<TTreeFormula*>& params) {
//   if(!gate->EvalInstance()) return 0;
//   TH2D* h2d = static_cast<TH2D*>(hst);
//   for(Int_t i = 0; i< GetNPar(); ++i) {
//     Double_t parVal = params[i]->EvalInstance();
//     kOrient ?  h2d->Fill(parVal, i): h2d->Fill(i, parVal);
//   }
//   return 0;
// }


// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // Class                                                 //
// // rb::GammaHist                                         //
// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // rb::GammaHist::GammaHist                              //
// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::GammaHist::GammaHist(const char* name, const char* title, const char* param, const char* gate, Int_t dimensions, TTree* tree, Set_t* set) {
//   fTree = tree; fSet = set;
//   kConstructorSuccess = kTRUE; fHistogramClone = 0; kDimensions = dimensions;

//   LockFreePointer<CriticalElements>  critical(fCritical);
//   critical->fHistogram = 0;

//   // Initialize gate
//   fCritical.fGate = new TTreeFormula("fGate", check_gate(gate).c_str(), critical->GetTree(this));
//   if(!fCritical.fGate->GetNdim()) {
//     kConstructorSuccess = kFALSE;
//     return;
//   }

//   // Initialize parameters
//   StringVector_t vPar1 = tokenize(param, ':');
//   if(kDimensions != vPar1.size()) {
//     Error("GammaHist", "Invalid parameter argument for a %d-dimensional hist (%s)", dimensions, param);
//     kConstructorSuccess = kFALSE;
//     return;
//   }
//   vector<Int_t> nParams;
//   for(UInt_t j=0; j< vPar1.size(); ++j) {
//     StringVector_t vPar = tokenize(vPar1[j].c_str(), ';');
//     nParams.push_back(vPar.size());

//     for(Int_t i = vPar.size()-1; i >= 0; --i) {
//       stringstream parname; parname << "param" << j << "_" << i-vPar.size();
//       string param = vPar[i];
//       ULong_t brktPos = param.find("["), dashPos = param.find("-"), brktPosLast = param.find("]");
//       if(brktPos > param.size() || dashPos > param.size()) {
// 	critical->fParams.push_back(new TTreeFormula(parname.str().c_str(), param.c_str(), critical->GetTree(this)));
// 	if(!critical->fParams[critical->fParams.size()-1]->GetNdim()) {
// 	  kConstructorSuccess = kFALSE;
// 	  return;
// 	}
//       }
//       else {
// 	string sFirst = param.substr(brktPos + 1, dashPos - brktPos - 1);
// 	string sLast  = param.substr(dashPos + 1, brktPosLast - dashPos - 1);
// 	string sBase  = param.substr(0, brktPos);
// 	UInt_t first = atoi(sFirst.c_str()), last = atoi(sLast.c_str());
// 	for(UInt_t indx = first; indx <= last; ++indx) {
// 	  stringstream sstrParam;
// 	  sstrParam << sBase << "[" << indx << "]";
// 	  parname << "_" << indx;
// 	  critical->fParams.push_back(new TTreeFormula(parname.str().c_str(), sstrParam.str().c_str(), critical->GetTree(this)));
// 	  if(!critical->fParams[critical->fParams.size()-1]->GetNdim()) {
// 	    kConstructorSuccess = kFALSE;
// 	    return;
// 	  }
// 	}
//       }
//       nPar = critical->fParams.size();
//     }
//   }

//   if(nParams.size() == 0) {
//     Error("GammaHist", "Invalid parameter argument %s", param);
//     kConstructorSuccess = kFALSE;
//     return;
//   }
//   else if (nParams.size() > 1) {
//     Int_t nParams0 = nParams[0];
//     for(UInt_t k=1; k< nParams.size(); ++k) {
//       if(nParams[k] != nParams0) {
// 	Error("GammaHist", "Invalid parameter argument %s", param);
// 	kConstructorSuccess = kFALSE;
// 	return;
//       }
//     }
//   }
//   else ;	

//   // Set name & title
//   fName  = check_name(name).c_str();

//   stringstream sstr;
//   sstr << param << " {" << fCritical.fGate->GetExpFormula().Data() << "}";
//   kDefaultTitle = sstr.str();
//   if(string(title).empty())
//     fTitle = kDefaultTitle.c_str();
//   else
//     fTitle = title;
//   kInitialTitle = fTitle;
// }


// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // static rb::GammaHist::Initialize                      //
// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Bool_t rb::GammaHist::GInitialize(const char* name, const char* title,
// 				  const char* paramList, const char* gate,
// 				  UInt_t ndim, TTree* tree, Set_t* set,
// 				  Int_t nbinsx, Double_t xlow, Double_t xhigh,
// 				  Int_t nbinsy, Double_t ylow, Double_t yhigh,
// 				  Int_t nbinsz, Double_t zlow, Double_t zhigh) {

//   // Create rb::Hist instance
//   rb::GammaHist* _this = new rb::GammaHist(name, title, paramList, gate, ndim, tree, set);
//   if(!_this->kConstructorSuccess) return kFALSE;

//   // Set internal histogram
//   //! \note The histogram isn't accessable to any other threads until we add it to
//   //! fgList, so it's safe to access the critical elements via a non-locking pointer.
//   LockFreePointer<CriticalElements> unlocked_critical(_this->fCritical);

//   Bool_t successfulHistCreation = kTRUE;
//   TH1::AddDirectory(kFALSE);

//   Int_t npar = unlocked_critical->fParams.size();

//   switch(_this->kDimensions) {
//   case 1: {
//     unlocked_critical->fHistogram =
//       new TH1D(_this->fName, _this->fTitle, nbinsx, xlow, xhigh);
//     unlocked_critical->fHistogram->GetXaxis()->SetTitle(paramList);
//     unlocked_critical->fHistogram->GetYaxis()->SetTitle("");
//     break;
//   }
//   case 2: {
//     unlocked_critical->fHistogram =
//       new TH2D(_this->fName, _this->fTitle, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
//     StringVector_t pars = tokenize(paramList, ':');
//     unlocked_critical->fHistogram->GetXaxis()->SetTitle(pars[1].c_str());
//     unlocked_critical->fHistogram->GetYaxis()->SetTitle(pars[0].c_str());
//     break;
//   }
//   default:
//     fprintf(stderr, "Error in <GInitialize>: %d-dimensional Gamma Histograms are not yet supported", _this->kDimensions);
//     TH1::AddDirectory(kTRUE);
//     return kFALSE;
//     break;
//   }

//   TH1::AddDirectory(kTRUE);

//   // Add to collections
//   LockingPointer<List_t> hlist(fgList(), fMutex);
//   if(successfulHistCreation) {
//     hlist->push_back(_this);

//     if(gDirectory) {
//       _this->fDirectory = gDirectory;
//       _this->fDirectory->Append(_this, kTRUE);
//     }
//   }
//   return kTRUE;
// }

// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // rb::GammaHist::DoFill()                               //
// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Int_t rb::GammaHist::DoFill(TH1* hst, TTreeFormula* gate, vector<TTreeFormula*>& params) {
//   if(!gate->EvalInstance()) return 0;

//   Int_t ret = -1;
//   switch(kDimensions) {
//   case 1:
//     for(Int_t i=0; i< params.size(); ++i)
//       static_cast<TH1D*>(hst)->Fill(params[i]->EvalInstance());
//     ret = 1;
//     break;
//   case 2:
//     for(Int_t i=0; i< params.size() / 2; ++i)
//       static_cast<TH2D*>(hst)->Fill(params[i]->EvalInstance(), params[i+params.size()/2]->EvalInstance());
//     ret = 1;
//     break;
//   default:
//     Error("DoFill", "Invalid kDimensions %d", kDimensions);
//     ret = -1;
//     break;
//   }
//   return ret;
// }

// // //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // // static rb::GammaHist::New (One-dimensional)           //
// // //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // void rb::GammaHist::New(const char* name, const char* title,
// // 		    Int_t nbinsx, Double_t xlow, Double_t xhigh,
// // 		    const char* param, const char* gate) {
// //   rb::GammaHist::GInitialize(name, title, param, gate, 1, nbinsx, xlow, xhigh);
// // }

// // //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // // static rb::GammaHist::New (Two-dimensional)           //
// // //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // void rb::GammaHist::New(const char* name, const char* title,
// // 		    Int_t nbinsx, Double_t xlow, Double_t xhigh,
// // 		    Int_t nbinsy, Double_t ylow, Double_t yhigh,
// // 		    const char* param, const char* gate) {
// //   rb::GammaHist::GInitialize(name, title, param, gate, 2, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
// // }
// #endif

