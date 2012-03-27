//! \file WriteConfig.cxx
//! \brief Implements methods related to saving configuration files.
//!  \details Put in a separate file because they're verbose and we want to avoid cluttering
//!  Rootbeer.cxx
#include <iostream>
#include <fstream>
#include <TTimeStamp.h>
#include <TCutG.h>
#include "Rootbeer.hxx"
#include "Rint.hxx"
#include "Data.hxx"
#include "Signals.hxx"
#include "hist/Hist.hxx"
using namespace std;


namespace {

/// Tells the writing functions how many tabs to indent by.
Int_t ntabs = 2;

// Some helper functions, etc.

/// Return axis based on number code
TAxis* get_axis(rb::hist::Base* hst, UInt_t n) {
  switch(n) {
  case 0: return hst->GetXaxis(); break;
  case 1: return hst->GetYaxis(); break;
  case 2: return hst->GetZaxis(); break;
  default: return 0;
  }
}

void write_std_hist(rb::hist::Base* rbhist, std::ostream& ofs) {
	for(int i=0; i< ntabs; ++i) ofs << "    ";
	ofs << "  rb::hist::New(\"" << rbhist->GetName() << "\", \"" << rbhist->GetTitle() << "\", " ;
	for(UInt_t dim = 0; dim < rbhist->GetNdimensions(); ++dim) {
		TAxis* axis = get_axis(rbhist, dim);
    ofs << axis->GetNbins() << ", " << axis->GetBinLowEdge(1) << ", " << axis->GetBinLowEdge(1+axis->GetNbins()) <<", ";
	}
	std::string param = rbhist->GetInitialParams();
	ofs << "\"" << param << "\", \"" << rbhist->GetGate() << "\");\n";
}

/// Write a histogram constructor format to a stream.
void write_hist(TObject* object, std::ostream& ofs) {
  rb::hist::Base* rbhist = dynamic_cast<rb::hist::Base*>(object);
  if(!rbhist) return;
  TH1* hst = rbhist->GetHist();
  if(!hst) return;
	std::string class_name = rbhist->ClassName();
	class_name = class_name.substr(std::string("rb::hist::").size());
	if(class_name == "D1" || class_name == "D2" || class_name == "D3")
		 write_std_hist(rbhist, ofs);
	else ;
}

/// Write a diretory constructor to a stream.
Bool_t write_directory(TObject* object, std::ostream& ofs) {
  TDirectory* dir = dynamic_cast<TDirectory*>(object);
  if(!dir) return kFALSE;
  TDirectory* mother = dir->GetMotherDir();
  if(!mother) return kFALSE;
  std::string motherOut = (mother == gROOT) ? "gROOT" : mother->GetName();
  ofs << "\n"; for(int i=0; i< ntabs; ++i) ofs << "    ";
  ofs << "  TDirectory* " << dir->GetName() << " = new TDirectory(\""
      << dir->GetName() << "\", \"" << dir->GetTitle() << "\", \""
      << dir->GetOption() << "\", " << motherOut << ");  ";
  ofs << dir->GetName() << "->cd();\n";
  ++ntabs;
  return kTRUE;
}

void recursive_delete_directory(TDirectory* dir) {
  for(Int_t i=0; i< dir->GetList()->GetEntries(); ++i) {
		if(dynamic_cast<rb::hist::Base*>(dir->GetList()->At(i))) {
			delete dynamic_cast<rb::hist::Base*>(dir->GetList()->At(i));
		}
  }
  for(Int_t i=0; i< dir->GetList()->GetEntries(); ++i) {
		if(dynamic_cast<TDirectory*>(dir->GetList()->At(i))) {
			recursive_delete_directory(dynamic_cast<TDirectory*>(dir->GetList()->At(i)));
		}
  }
}

/// Recurse through all directories, write their owned histograms
/// to a stream as well as the directory constructors themselves.
void recurse_directory(TDirectory* dir, std::ostream& ofs) {
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

inline void write_hists_and_directories(std::ostream& ofs) {
  TDirectory* dirInitial = gDirectory;
  std::string initialName = (dirInitial == gROOT) ? "gROOT" : dirInitial->GetName();

  gROOT->cd();
  ofs << "  // DIRECTORIES AND HISTOGRAMS //\n";
  ofs << "  // gROOT\n";
  recurse_directory(gROOT, ofs);
  ofs << "  "<< initialName << "->cd();\n";
  dirInitial->cd();
}


/// Save a TCutG.
void write_cut(TObject* obj, std::ostream& ofs)
{
  TCutG* cut = dynamic_cast<TCutG*>(obj);
  if(!cut) return;
  const char* nme   = cut->GetName();
  const char* varx  = cut->GetVarX();
  const char* vary  = cut->GetVarY();
  const Width_t www = cut->GetLineWidth();
  const Color_t ccc = cut->GetLineColor();
  Int_t np = cut->GetN();

  Double_t xx, yy;
  std::stringstream sX, sY;
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
    std::string answer;
    cout << "The file " << fname << " already exists. Overwrite (y/n)?\n";
    cin  >> answer;
    if(!(answer == "y" || answer == "Y")) out = kFALSE;
  }
  ifs.close();
  return out;
}
} // namespace

// Write rootbeer configuration file
Int_t rb::WriteConfig(const char* fname, Bool_t prompt) {
  if(prompt) {
    if(!overwrite(fname))
			 return 1;
  }
  TTimeStamp ts; std::string ts_str = ts.AsString("l");

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
  TTimeStamp ts; std::string ts_str = ts.AsString("l");

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
  TTimeStamp ts; std::string ts_str = ts.AsString("l");

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
    std::stringstream sstr;
    sstr << ".x " << filename;
    gROOT->ProcessLine(sstr.str().c_str());
  }
  else if(!opt.CompareTo("o")) {
    TDirectory* dirInitial = gDirectory;
    ifstream ifs(filename);
    std::string line;
    while(1) {
      getline(ifs, line);
      if(!ifs.good()) break;
      UInt_t pos = line.find("TCutG*");
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

      pos = line.find("rb::hist::");
      if(pos < line.size()) {
				line = line.substr(pos);
				line = line.substr(1+line.find("("));
				line = line.substr(1+line.find("\""));
				string name = line.substr(0, line.find("\""));
				rb::hist::Base* old = dynamic_cast<rb::hist::Base*> (gDirectory->FindObject(name.c_str()));
				if(old) old->Delete(); //delete old;
				continue;
      }
    }
    ReadConfig(filename, "c");
    dirInitial->cd();
  }
  else if(!opt.CompareTo("r")) {
		gROOT->cd();
		recursive_delete_directory(gROOT);
		// EventVector_t events = rb::gApp()->GetEventVector();
		// for(EventVector_t::iterator it = events.begin(); it != events.end(); ++it) {
		// 	rb::gApp()->GetEvent(it->first)->GetHistManager()->DeleteAll();
		// }

    TSeqCollection* primitives = gROOT->GetListOfSpecials();
    for(Int_t i=0; i< primitives->GetSize(); ++i) {
      TCutG* cut = dynamic_cast<TCutG*>(primitives->At(i));
      if(cut) delete cut;
    }
//    ReadConfig(filename, "c");
  }
  else {
    Error("ReadConfig", "Valid options are: \"r\" (reset), \"o\" (overwrite), and \"c\" (cumulate).");
  }
	gApp()->GetSignals()->SyncHistTree();
}


// Just write the canvases.
Int_t rb::WriteCanvases(const char* fname, Bool_t prompt) {
  if(prompt) {
    if(!overwrite(fname))
			 return 1;
  }
  TTimeStamp ts; std::string ts_str = ts.AsString("l");

  ofstream ofs(fname, ios::out);
  ofs << "// ROOTBEER CANVAS CONFIGURATION FILE \n"
      << "// " << fname << "\n"
      << "// Generated on " << ts_str << "\n"
      << "\n"
      << "{\n\n";
  ofs << "/// \todo Get Working" << endl;
  return 0;
}

// Just write the canvases.
Int_t rb::ReadCanvases(const char* fname) {
	std::cout << "Todo\n";
  return 0;
}

