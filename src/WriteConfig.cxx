//! \file WriteConfig.cxx
//! \brief Implements methods related to saving configuration files.
//!  \details Put in a separate file because they're verbose and we want to avoid cluttering
//!  Rootbeer.cxx
#include <iostream>
#include <fstream>
#include <TTimeStamp.h>
#include <TCutG.h>
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
  rb::Hist* rbhst = dynamic_cast<rb::Hist*>(object);
  if(!rbhst) return;
  TH1* hst = rbhst->GetHist();
  if(!hst) return;

  for(int i=0; i< ntabs; ++i) ofs << "    ";
  ofs << "  rb::Hist::New(\"" << rbhst->GetName() << "\", \"" << rbhst->GetTitle() << "\", ";
  string param = "";
  for(UInt_t i=0; i< rbhst->GetNdimensions(); ++i) {
    param.insert(0, rbhst->GetParam(i));
    param.insert(0, ":"); // append :par in reverse order
    TAxis* axis = get_axis(hst, i);
    ofs << axis->GetNbins() << ", " << axis->GetBinLowEdge(1) << ", " << axis->GetBinLowEdge(1+axis->GetNbins()) <<", ";
  }
  param.erase(0, 1); // get rid of leading ":"
  ofs << "\"" << param << "\", \"" << rbhst->GetGate() << "\");\n";
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

inline void write_hists_and_directories(ostream& ofs) {
  TDirectory* dirInitial = gDirectory;
  string initialName = (dirInitial == gROOT) ? "gROOT" : dirInitial->GetName();

  gROOT->cd();
  ofs << "  // DIRECTORIES AND HISTOGRAMS //\n";
  ofs << "  // gROOT\n";
  recurse_directory(gROOT, ofs);
  ofs << "  "<< initialName << "->cd();\n";
  dirInitial->cd();
}


/// Save a TCutG.
void write_cut(TObject* obj, ostream& ofs)
{
  TCutG* cut = dynamic_cast<TCutG*>(obj);
  if(!cut) return;
  const char* nme   = cut->GetName();
  const char* ttl   = cut->GetTitle();
  const char* varx  = cut->GetVarX();
  const char* vary  = cut->GetVarY();
  const Width_t www = cut->GetLineWidth();
  const Color_t ccc = cut->GetLineColor();
  Int_t np = cut->GetN();

  Double_t xx, yy;
  stringstream sX, sY;
  sX << "     Double_t px[] = { " ;
  sY << "     Double_t py[] = { " ;
  for(Int_t i=0; i< np; ++i) {
    cut->GetPoint(i, xx, yy);
    if(i < np-1) {
      sX << xx << ", ";
      sY << yy << ", ";
    } else {
      sX << xx << " };\n";
      sY << yy << " };\n";
    }
  }
  ofs << sX.str() << sY.str();
  ofs << "     TCutG* " << nme << " = new TCutG(\"" << nme << "\", " << np << ", px, py);\n";
  ofs << nme << "->SetVarX(\"" << varx << "\");\n";
  ofs << nme << "->SetVarY(\"" << vary << "\");\n";
  ofs << nme << "->SetLineWidth(" << www <<");\n";
  ofs << nme << "->SetLineColor(" << ccc <<");\n";
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

  write_hists_and_directories(ofs);

  ofs << "\n\n" << "  // VARIABLES //\n";
  data::MBasic::Printer p;
  p.SavePrimitive(ofs);

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

  write_hists_and_directories(ofs);


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
  data::MBasic::Printer p;
  p.SavePrimitive(ofs);

  ofs << "\n}";
  return 0;
}


void rb::ReadConfig(const char* filename, Option_t* option) {
  ifstream ifs(filename);
  if(ifs.fail()) {
    Error("ReadConfig", "The file %s wasn't found or isn't readable.", filename);
    return;
  }
  ifs.close();

  TString opt(option);
  opt.ToLower();
  if(0);
  else if(!opt.CompareTo("c")) {
    stringstream sstr;
    sstr << ".x " << filename;
    gROOT->ProcessLine(sstr.str().c_str());
  }
  else if(!opt.CompareTo("o")) {
    TDirectory* dirInitial = gDirectory;
    ifstream ifs(filename);
    string line;
    while(1) {
      getline(ifs, line);
      if(!ifs.good()) break;
      int pos = line.find("TCutG*");
      if(pos < line.size()) {
	line = line.substr(pos);
	line = line.substr(1+line.find("("));
	line = line.substr(1+line.find("\""));
	string name = line.substr(0, line.find("\""));
	TCutG* old = dynamic_cast<TCutG*> (gROOT->GetListOfSpecials()->FindObject(name.c_str()));
	if(old) delete old;
	continue;
      }

      pos = line.find("cd()");
      if(pos < line.size()) gROOT->ProcessLine(line.c_str());

      pos = line.find("rb::Hist::New");
      if(pos < line.size()) {
	line = line.substr(pos);
	line = line.substr(1+line.find("("));
	line = line.substr(1+line.find("\""));
	string name = line.substr(0, line.find("\""));
	rb::Hist* old = dynamic_cast<rb::Hist*> (gDirectory->FindObject(name.c_str()));
	if(old) delete old;
	continue;
      }
    }
    ReadConfig(filename, "c");
  }
  else if(!opt.CompareTo("r")) {
    rb::Hist::DeleteAll();
    TSeqCollection* primitives = gROOT->GetListOfSpecials();
    for(Int_t i=0; i< primitives->GetSize(); ++i) {
      TCutG* cut = dynamic_cast<TCutG*>(primitives->At(i));
      if(cut) delete cut;
    }
    ReadConfig(filename, "c");
  }
  else {
    Error("ReadConfig", "Valid options are: \"r\" (reset), \"o\" (overwrite), and \"c\" (cumulate).");
  }

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

