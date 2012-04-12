//! \file WriteConfig.cxx
//! \brief Implements methods related to saving configuration files.
//!  \details Put in a separate file because they're verbose and we want to avoid cluttering
//!  Rootbeer.cxx
#include <iostream>
#include <fstream>
#include <TCanvas.h>
#include <TFrame.h>
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
	std::string title = rbhist->UseDefaultTitle() ? "" : rbhist->GetTitle();
	for(int i=0; i< ntabs; ++i) ofs << "    ";
	ofs << "  rb::hist::New(\"" << rbhist->GetName() << "\", \"" << title << "\", " ;
	for(UInt_t dim = 0; dim < rbhist->GetNdimensions(); ++dim) {
		TAxis* axis = get_axis(rbhist, dim);
    ofs << axis->GetNbins() << ", " << axis->GetBinLowEdge(1) << ", " << axis->GetBinLowEdge(1+axis->GetNbins()) <<", ";
	}
	std::string param = rbhist->GetInitialParams();
	ofs << "\"" << param << "\", \"" << rbhist->GetGate() << "\", " << rbhist->GetEventCode() << ");\n";
}

void write_summary_hist(rb::hist::Summary* rbhist, std::ostream& ofs) {
	std::string title = rbhist->UseDefaultTitle() ? "" : rbhist->GetTitle();
	for(int i=0; i< ntabs; ++i) ofs << "    ";
	ofs << "  rb::hist::NewSummary(\"" << rbhist->GetName() << "\", \"" << title << "\", " ;
	Int_t orientation = rbhist->GetOrientation();
	Bool_t vertical = orientation == rb::hist::Summary::VERTICAL;
	TAxis* axis = vertical? rbhist->GetXaxis() : rbhist->GetYaxis();
	std::string orient_arg =  vertical? "v" : "h";
	ofs << axis->GetNbins() << ", " << axis->GetBinLowEdge(1) << ", " << axis->GetBinLowEdge(1+axis->GetNbins()) <<", ";
	std::string param = rbhist->GetInitialParams();
	ofs << "\"" << param << "\", \"" << rbhist->GetGate() << "\", \"" << orient_arg << "\", " << rbhist->GetEventCode() << ");\n";
}

void write_bit_hist(rb::hist::Bit* rbhist, std::ostream& ofs) {
	std::string title = rbhist->UseDefaultTitle() ? "" : rbhist->GetTitle();
	for(int i=0; i< ntabs; ++i) ofs << "    ";
	ofs << "  rb::hist::NewBit(\"" << rbhist->GetName() << "\", \"" << title << "\", " ;
	TAxis* axis = rbhist->GetXaxis();
	ofs << axis->GetNbins() << ", ";
	std::string param = rbhist->GetInitialParams();
	ofs << "\"" << param << "\", \"" << rbhist->GetGate() << "\", " << rbhist->GetEventCode() << ");\n";
}

/// Write a histogram constructor format to a stream.
void write_hist(TObject* object, std::ostream& ofs) {
  rb::hist::Base* rbhist = dynamic_cast<rb::hist::Base*>(object);
  if(!rbhist) return;
  TH1* hst = rbhist->GetHist();
  if(!hst) return;
	std::string class_name = rbhist->ClassName();
	class_name = class_name.substr(std::string("rb::hist::").size());
	if(class_name == "D1" || class_name == "D2" || class_name == "D3" || class_name == "Gamma")
		 write_std_hist(rbhist, ofs);
	else if(class_name == "Summary")
		 write_summary_hist(static_cast<rb::hist::Summary*>(rbhist), ofs);
	else if(class_name == "Bit")
		 write_bit_hist(static_cast<rb::hist::Bit*>(rbhist), ofs);
	else;
}

/// Write a diretory constructor to a stream.
Bool_t write_directory(TObject* object, std::ostream& ofs) {
  TDirectory* dir = dynamic_cast<TDirectory*>(object);
  if(!dir) return kFALSE;
  TDirectory* mother = dir->GetMotherDir();
  if(!mother) return kFALSE;
	mother->cd();
	std::string title = "";
	if(gDirectory != gROOT) { title = gDirectory->GetTitle(); title += "/"; }
	title += dir->GetName();
  ofs << "\n"; for(int i=0; i< ntabs; ++i) ofs << "    ";
	ofs << "  rb::Mkdir(\"" << dir->GetName() << "\", \"" << title << "\");\n";
  ++ntabs;
  return kTRUE;
}

/// Recurse through all directories, write their owned histograms
/// to a stream as well as the directory constructors themselves.
void recurse_directory(TDirectory* dir, std::ostream& ofs, Bool_t top = true) {
  for(Int_t i=0; i< dir->GetList()->GetEntries(); ++i) {
    write_hist(dir->GetList()->At(i), ofs);
  }
  //  ++ntabs;
  for(Int_t i=0; i< dir->GetList()->GetEntries(); ++i) {
    if(write_directory(dir->GetList()->At(i), ofs)) {
      recurse_directory(static_cast<TDirectory*>(dir->GetList()->At(i)), ofs, false);
    }
  }
	if(!top) {
		for(int i=0; i< ntabs; ++i) ofs << "    ";
		ofs << "  " << "gDirectory->GetMotherDir()->cd();\n";
	}
  --ntabs;
}

inline void write_hists_and_directories(std::ostream& ofs) {
  TDirectory* dirInitial = gDirectory;
  std::string initialName = (dirInitial == gROOT) ? "gROOT" : dirInitial->GetName();

  gROOT->cd();
  ofs << "  // DIRECTORIES AND HISTOGRAMS //\n";
  ofs << "  // gROOT\n";
	ofs << "  gROOT->cd();\n";
  recurse_directory(gROOT, ofs);
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
  ofs << "     TCutG* " << nme << " = rb::CreateTCutG(\"" << nme << "\", " << np << ", px, py, ";
	ofs << "\"" << varx << "\", \"" << vary << "\", " << www << ", " << ccc << ");\n";
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


namespace {
std::vector<TObject*>
get_object_vector(TSeqCollection* collection) {
	std::vector<TObject*> out;
	for(int i=0; i< collection->GetSize(); ++i)
		 out.push_back(collection->At(i));
	return out;
}

template <class T>
Bool_t try_delete(TObject* object) {
	Bool_t ret = true;
	T* t = dynamic_cast<T*>(object);
	if(t) delete t;
	else ret = false;
	return ret;
} }

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
		Bool_t changed1 = rb::hist::Base::SetOverwrite(true);
		Bool_t changed2 = rb::SetTCutGOverwrite(true);
		ReadConfig(filename, "c");
		if(changed1) rb::hist::Base::SetOverwrite(false);
		if(changed2) rb::SetTCutGOverwrite(false);
  }
  else if(!opt.CompareTo("r")) {
		std::vector<TObject*> objects = get_object_vector(gROOT->GetList());
		for(std::vector<TObject*>::iterator it = objects.begin(); it != objects.end(); ++it) {
			if (try_delete<TDirectory>(*it));
			else if (try_delete<rb::hist::Base>(*it));
		}
		std::vector<TObject*> specials = get_object_vector(gROOT->GetList());
		for(std::vector<TObject*>::iterator it = specials.begin(); it != specials.end(); ++it) {
			try_delete<TCutG>(*it);
		}
    ReadConfig(filename, "c");
  }
  else {
    Error("ReadConfig", "Valid options are: \"r\" (reset), \"o\" (overwrite), and \"c\" (cumulate).");
  }
	if(gApp()->GetHistSignals()) gApp()->GetHistSignals()->SyncHistTree();
}

namespace {
Int_t get_n_subpads(TPad* pad) {
	Int_t out = 0;
	for(Int_t i=0; i< pad->GetListOfPrimitives()->GetEntries(); ++i) {
		if(dynamic_cast<TPad*>(pad->GetListOfPrimitives()->At(i))) ++out;
	}
	return out;
}
void write_canvas_hist(TH1* th1, std::ostream& ofs) {
	rb::hist::Base* hist = 0;
	if(th1) {
		rb::EventVector_t events = rb::gApp()->GetEventVector();
		for(UInt_t k = 0; k< events.size(); ++k) {
			hist = rb::gApp()->GetEvent(events[k].first)->FindHistogram(th1);
			if(hist) {
				TDirectory* dir = hist->GetDirectory();
				const std::string dir_path = dir->GetPath();
						
				ofs << "  if(rb::Cd(\"" << dir_path << "\", true) &&  rb::gApp()->FindHistogram(\"" << hist->GetName() << "\"))\n"
						<< "      rb::gApp()->FindHistogram(\"" << hist->GetName() << "\")->Draw();\n";
				break;
			}
		}
	}
}
void recurse_pad(TPad* pad, TCanvas* owner, std::vector<Int_t>& path, std::ostream& ofs) {
	for(Int_t i=0; i< pad->GetListOfPrimitives()->GetEntries(); ++i) {
		*(path.end()-1)++;
		TH1* th1 = dynamic_cast<TH1*>(pad->GetListOfPrimitives()->At(i));
		TPad* subpad = dynamic_cast<TPad*>(pad->GetListOfPrimitives()->At(i));
		if(th1) {
			ofs << "Int_t path[" << path.size() << "] = { ";
			for(UInt_t j=0; j< path.size(); ++j) {
				ofs << path[j];
				if(j<path.size()-1) ofs << ", ";
			}
			ofs << " };\n";
			ofs << "rb::CdPad(" << owner->GetName() << ", path, " << path.size() << ");\n";
			write_canvas_hist(th1, ofs);
		}
		else if(subpad) {
			path.push_back(0);
			recurse_pad(subpad, owner, path, ofs);
		}
	}
}

}

Int_t rb::WriteCanvases(const char* fname, Bool_t prompt) {
	TVirtualPad* current = 0;
	if(gPad) current = gPad;
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
	std::stringstream tmpfile;
	tmpfile << "TEMP_" << ts.GetTime();
	for(Int_t i=0; i< gROOT->GetListOfCanvases()->GetEntries(); ++i) {
		TCanvas* canvas = dynamic_cast<TCanvas*>(gROOT->GetListOfCanvases()->At(i));
	 	if(!canvas) continue;
		canvas->SaveSource(tmpfile.str().c_str());
		std::ifstream ifs(tmpfile.str().c_str());
		std::string line, pad_name = "";
		for(int i=0; i< 3; ++i) std::getline(ifs, line);
		while(1) {
			std::getline(ifs, line);
			if(!ifs.good()) break;
			if(line.find("------------>Primitives in pad") < line.size()) {
				pad_name = line.substr(std::string("// ------------>Primitives in pad: ").size());
				ofs << line << "\n";
			}
			else if(line.find("   TCanvas *") < line.size()) {
				pad_name = line.substr(line.find("(")+2, line.find(",") - (line.find("(")+2)-1);
				ofs << line << "\n";
			}
			else if(line.find("   TH1D *") < line.size()) {
				std::stringstream cmd;
				cmd << pad_name << "->cd();";
				gROOT->ProcessLine(cmd.str().c_str());
				if(gPad) {
					for(Int_t i=0; i< gPad->GetListOfPrimitives()->GetEntries(); ++i) {
						if(dynamic_cast<TH1*>(gPad->GetListOfPrimitives()->At(i)))
							 write_canvas_hist(dynamic_cast<TH1*>(gPad->GetListOfPrimitives()->At(i)), ofs);
					}
				}
				while(line.find("->Draw()") > line.size()) {
					std::getline(ifs, line);
				}
			}
			else if (line == "}");
			else {
				ofs << line << "\n";
			}
		}
		std::stringstream rm_cmd;
		rm_cmd << "rm -f " << tmpfile.str();
		gSystem->Exec(rm_cmd.str().c_str());
	}
	ofs << "\n}\n";
	current->cd();
	return 0;
}

  // ofs << "// ROOTBEER CANVAS CONFIGURATION FILE \n"
  //     << "// " << fname << "\n"
  //     << "// Generated on " << ts_str << "\n"
  //     << "\n"
  //     << "{\n\n";

	// for(Int_t i=0; i< gROOT->GetListOfCanvases()->GetEntries(); ++i) {
	// 	TCanvas* canvas = dynamic_cast<TCanvas*>(gROOT->GetListOfCanvases()->At(i));
	// 	if(!canvas) continue;		
	// 	ofs << "  TCanvas* " << canvas->GetName() << " = new TCanvas(\""
	// 			<< canvas->GetName() << "\", \"" << canvas->GetTitle() << "\", "
	// 			<< canvas->GetWindowTopX() << ", " << canvas->GetWindowTopY() << ", "
	// 			<< canvas->GetWw() << ", " << canvas->GetWh() << ");\n";
	// 	if(get_n_subpads(canvas))
	// 		 ofs << "  " << canvas->GetName() << "->Divide"
			
	// 	for(Int_t j=0; j< canvas->GetListOfPrimitives()->GetEntries(); ++j) {
	// 		TH1* th1 = dynamic_cast<TH1*>(canvas->GetListOfPrimitives()->At(j));
	// 		TPad* pad = dynamic_cast<TPad*>(canvas->GetListOfPrimitives()->At(j));
	// 		std::vector<Int_t> path;
	// 		if(th1) write_canvas_hist(th1, ofs);
	// 		else if(pad) recurse_pad(pad, canvas, path, ofs);
	// 	}
	// }
	// ofs <<"\n}\n";
  // return 0;
// }

Int_t rb::ReadCanvases(const char* fname) {
	std::vector<TObject*> canvases = get_object_vector(gROOT->GetListOfCanvases());
	for(std::vector<TObject*>::iterator it = canvases.begin(); it != canvases.end(); ++it)
		 try_delete<TCanvas>(*it);
	std::string cmd = ".x "; cmd += fname;
	gROOT->ProcessLine(cmd.c_str());
  return 0;
}
