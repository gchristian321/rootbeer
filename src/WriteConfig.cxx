/*! \file WriteConfig.cxx
 *  \brief Implements methods related to saving configuration files.
 *  Put in a separate file because they're verbose and we want to avoid cluttering
 *  Rootbeer.cxx
 */
#include <iostream>
#include <fstream>
#include "TTimeStamp.h"
#include "TCutG.h"
#include "Rootbeer.hxx"
#include "Data.hxx"
using namespace std;



/// Tells the writing functions how many tabs to indent by.
static Int_t ntabs = 2;

// Some helper functions, etc.

/// Return axis based on number code
TAxis* get_axis(TH1* hst, UInt_t n) {
  switch(n) {
  case 0: return hst->GetXaxis(); break;
  case 1: return hst->GetYaxis(); break;
  case 2: return hst->GetZaxis(); break;
  default: return 0;
  }
}

/// Write a histogram constructor format to a stream.
void write_hist(TObject* object, ostream& ofs) {
  TH1* hst = dynamic_cast<TH1*>(object);
  rb::Hist* rbhst = dynamic_cast<rb::Hist*>(object);
  if(!(hst && rbhst)) return;

  for(int i=0; i< ntabs; ++i) ofs << "    ";
  ofs << "  rb::AddHist(\"" << hst->GetName() << "\", \"" << ((TH1*)hst)->GetTitle() << "\", ";
  string param = "";
  for(UInt_t i=0; i< rbhst->GetNdimensions(); ++i) {
    param.insert(0, rbhst->GetParam(i));
    param.insert(0, ":"); // append :par in reverse order
    TAxis* axis = get_axis(hst, i);
    ofs << axis->GetNbins() << ", " << axis->GetBinLowEdge(1) << ", " << axis->GetBinLowEdge(1+axis->GetNbins()) <<", ";
  }
  param.erase(0, 1); // get rid of leading ":"
  ofs << "\"" << param << "\", \"" << rbhst->GetGate() << "\"); "
      << "// " << hst->GetDirectory()->GetName() << "\n";
}

/// Write a diretory constructor to a stream.
Bool_t write_directory(TObject* object, ostream& ofs) {
  TDirectory* dir = dynamic_cast<TDirectory*>(object);
  if(!dir) return kFALSE;
  TDirectory* mother = dir->GetMotherDir();
  if(!mother) return kFALSE;
  string motherOut = (mother == gROOT) ? "gROOT" : mother->GetName();
  ofs << "\n"; for(int i=0; i< ntabs; ++i) ofs << "    ";
  ofs << "  TDirectory* " << dir->GetName() << " = new TDirectory(\""
      << dir->GetName() << "\", \"" << dir->GetTitle() << "\", \""
      << dir->GetOption() << "\", " << motherOut << ");  ";
  ofs << dir->GetName() << "->cd();\n";
  ++ntabs;
  return kTRUE;
}

/// Recurse through all directories, write their owned histograms
/// to a stream as well as the directory constructors themselves.
void recurse_directory(TDirectory* dir, ostream& ofs) {
  for(Int_t i=0; i< dir->GetList()->GetEntries(); ++i) {
    write_hist(dir->GetList()->At(i), ofs);
  }
  //  ++ntabs;
  for(Int_t i=0; i< dir->GetList()->GetEntries(); ++i) {
    if(write_directory(dir->GetList()->At(i), ofs)) {
      recurse_directory(static_cast<TDirectory*>(dir->GetList()->At(i)), ofs);
    }
  }
  --ntabs;
}


/// Save a TCutG.
void write_cut(TObject* obj, ostream& ofs)
{
  TCutG* cut = dynamic_cast<TCutG*>(obj);
  if(!cut) return;
  const char* nme   = cut->GetName();
  const char* varx  = cut->GetVarX();
  const char* vary  = cut->GetVarY();
  const Width_t www = cut->GetLineWidth();
  const Color_t ccc = cut->GetLineColor();

  int np = cut->GetN();
  double *xx = new double[np], *yy = new double[np];
  for(int i(0); i<np; ++i) {
    cut->GetPoint(i,xx[i],yy[i]);
  }

  ofs << "     TCutG* " << nme << " = new TCutG(\"" << nme << "\", " << np << ");\n";
  ofs << "         " << nme << "->SetVarX(\"" << varx << "\");\n";
  ofs << "         " << nme << "->SetVarY(\"" << vary << "\");\n";
  ofs << "         " << nme << "->SetTitle(\"" << nme << "\");\n";
  ofs << "         " << nme << "->SetFillColor(1);\n";
  ofs << "         " << nme << "->SetLineColor(" << ccc << ");\n";
  ofs << "         " << nme << "->SetLineWidth(" << www << ");\n";
  for(int i=0; i< np; ++i) {
    ofs << "         " << nme << "->SetPoint(" << i << ", " << xx[i] << ", " << yy[i] << ");\n";
  }
  ofs << "\n";
  delete[] xx; delete[] yy;
}

/// Ask user to overwrite a file or not.
Bool_t overwrite(const char* fname) {
  Bool_t out = kTRUE;
  ifstream ifs(fname, ios::in);
  if(ifs.good()) {
    string answer;
    cout << "The file " << fname << " already exists. Overwrite (y/n)?\n";
    cin  >> answer;
    if(!(answer == "y" || answer == "Y")) out = kFALSE;
  }
  ifs.close();
  return out;
}


// Write rootbeer configuration file
Int_t rb::WriteConfig(const char* fname, Bool_t prompt) {
  if(prompt) {
    if(!overwrite(fname))
      return 1;
  }
  TTimeStamp ts; string ts_str = ts.AsString("l");

  ofstream ofs(fname, ios::out);
  ofs << "// ROOTBEER CONFIGURATION FILE \n"
      << "// " << fname << "\n"
      << "// Generated on " << ts_str << "\n"
      << "\n"
      << "{\n\n";

  ofs << "  // CONTOUR GATES //\n";
  for(Int_t i=0; i< gROOT->GetListOfSpecials()->GetEntries(); ++i) {
    write_cut(gROOT->GetListOfSpecials()->At(i), ofs);
  }
  ofs << "\n\n";

  TDirectory* dirInitial = gDirectory;
  gROOT->cd();
  ofs << "  // DIRECTORIES AND HISTOGRAMS //\n";
  ofs << "  // gROOT\n";
  recurse_directory(gROOT, ofs);
  ofs << "  gROOT->cd();\n";
  dirInitial->cd();

  ofs << "\n\n" << "  // VARIABLES //\n";
  rb::Data::SavePrimitive(ofs);

  ofs << "\n}";
  return 0;
}

// Just write the histograms.
Int_t rb::WriteHistograms(const char* fname, Bool_t prompt) {
  if(prompt) {
    if(!overwrite(fname))
      return 1;
  }
  TTimeStamp ts; string ts_str = ts.AsString("l");

  ofstream ofs(fname, ios::out);
  ofs << "// ROOTBEER HISTOGRAM CONFIGURATION FILE \n"
      << "// " << fname << "\n"
      << "// Generated on " << ts_str << "\n"
      << "\n"
      << "{\n\n";


  TDirectory* dirInitial = gDirectory;
  gROOT->cd();
  ofs << "  // DIRECTORIES AND HISTOGRAMS //\n";
  ofs << "  // gROOT\n";
  recurse_directory(gROOT, ofs);
  ofs << "  gROOT->cd();\n";
  dirInitial->cd();

  ofs << "\n}";
  return 0;
}

Int_t rb::WriteVariables(const char* fname, Bool_t prompt) {
  if(prompt) {
    if(!overwrite(fname))
      return 1;
  }
  TTimeStamp ts; string ts_str = ts.AsString("l");

  ofstream ofs(fname, ios::out);
  ofs << "// ROOTBEER VARIABLE CONFIGURATION FILE \n"
      << "// " << fname << "\n"
      << "// Generated on " << ts_str << "\n"
      << "\n"
      << "{\n\n";


  ofs << "\n\n" << "  // VARIABLES //\n";
  rb::Data::SavePrimitive(ofs);

  ofs << "\n}";
  return 0;
}


void rb::ReadConfig(const char* filename) {
  stringstream sstr;
  sstr << ".x " << filename;
  gROOT->ProcessLine(sstr.str().c_str());
}





// Just write the canvases.
Int_t rb::WriteCanvases(const char* fname, Bool_t prompt) {
  if(prompt) {
    if(!overwrite(fname))
      return 1;
  }
  TTimeStamp ts; string ts_str = ts.AsString("l");

  ofstream ofs(fname, ios::out);
  ofs << "// ROOTBEER CANVAS CONFIGURATION FILE \n"
      << "// " << fname << "\n"
      << "// Generated on " << ts_str << "\n"
      << "\n"
      << "{\n\n";
  ofs << "/// \todo Get Working" << endl;
  return 0;
}

