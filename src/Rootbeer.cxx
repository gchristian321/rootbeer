/*! \file Rootbeer.cxx 
 *  \brief Implements the user interface functions and
 *  also contains the \c main program.

   \todo Get rid of gUnpacker, probably the best thing is to change
   unpacker to a namespace with "private" stuff only in the C++ file.
 */
#include <typeinfo>
#include <iostream>
#include <fstream>
#include "Rootbeer.hxx"
#include "Hist.hxx"
using namespace std;



/// User functions

void rb::hist::Add(const char* name, const char* title,
		   Int_t nbinsx, Double_t xlow, Double_t xhigh,
		   const char* param, const char* gate) {

  rb::H1D * hst = new rb::H1D(name, title,
			      nbinsx, xlow, xhigh,
			      param, gate);
}

void rb::hist::Add(const char* name, const char* title,
		   Int_t nbinsx, Double_t xlow, Double_t xhigh,
		   Int_t nbinsy, Double_t ylow, Double_t yhigh,
		   const char* param, const char* gate) {

  rb::H2D * hst = new rb::H2D(name, title,
			      nbinsx, xlow, xhigh,
			      nbinsy, ylow, yhigh,
			      param, gate);
}

void rb::hist::Add(const char* name, const char* title,
		   Int_t nbinsx, Double_t xlow, Double_t xhigh,
		   Int_t nbinsy, Double_t ylow, Double_t yhigh,
		   Int_t nbinsz, Double_t zlow, Double_t zhigh,
		   const char* param, const char* gate) {

  rb::H3D * hst = new rb::H3D(name, title,
  			      nbinsx, xlow, xhigh,
  			      nbinsy, ylow, yhigh,
  			      nbinsz, zlow, zhigh,
  			      param, gate);
}


void rb::Logo() {
    cout << "                                                                 \n"
	 << "                            ___                                  \n"
	 << "                          .'   '.                                \n"
	 << "                         /       \\           oOoOo              \n"
	 << "                        |         |       ,==|||||               \n"
	 << "                         \\       /       _|| |||||              \n"
	 << "                          '.___.'    _.-'^|| |||||               \n"
	 << "                        __/_______.-'     '==HHHHH               \n"
	 << "                   _.-'` /                   \"\"\"\"\"          \n"
	 << "                .-'     /   oOoOo                                \n"
	 << "                `-._   / ,==|||||                                \n"
	 << "                    '-/._|| |||||                                \n"
	 << "                     /  ^|| |||||                                \n"
	 << "                    /    '==HHHHH                                \n"
	 << "                   /________\"\"\"\"\"                           \n"
	 << "                   `\\       `\\                                 \n"
	 << "                     \\        `\\   /                           \n"
	 << "                      \\         `\\/                            \n"
	 << "                      /                                          \n"
	 << "                     /                                           \n"
	 << "                    /_____                                       \n"
	 << "                                                                 \n"
	 << "      Welcome to ROOT BEER, the ROOT Basic Event ExtractoR       \n"
	 << "                                                                 \n"
      ;
  }




// void rb::Rootbeer::Save(const char* filename, Bool_t prompt) {
//   kWriteData = kFALSE; // pause writing
//   ifstream ifs(filename, ios::in);
//   if(!ifs.fail() && prompt) {
//     cout << "File " << filename << " exists. Overwrite (y/n)?\n";
//     string ans;
//     cin  >> ans;
//     if(!(ans == "y" || ans == "Y")) return;
//   }
//   if(fSave) {
//     fTree->Write();
//     delete fSave; fSave = 0;
//   }
//   fSave = new TFile(filename, "RECREATE");
//   if(fSave->IsZombie()) return;
//   for(Int_t i=0; i< fTree->GetListOfBranches()->GetEntries(); ++i) {
//     static_cast<TBranch*>(fTree->GetListOfBranches()->At(i))->SetFile(fSave);
//   }
//   kWriteData = kTRUE; // resume writing
// }




// void rb::Rootbeer::ReadConfig(const char* fname) {
//   TString s = ".x "; s += fname;
//   gROOT->ProcessLine(s.Data());
// }

// Int_t rb::Rootbeer::WriteConfig(const char* fname, Bool_t prompt) {
//   TTimeStamp ts; string ts_str = ts.AsString("l");
//   if(prompt) {
//     ifstream ifs(fname, ios::in);
//     if(ifs.good()) {
//       string answer;
//       cout << "The file " << fname << " already exists. Overwrite (y/n)?\n";
//       cin  >> answer;
//       if(answer == "y" || answer == "Y") ;
//       else return -1;
//     }
//     ifs.close();
//   }

//   ofstream ofs(fname, ios::out);

//   ofs << "/// ROOTBEER CONFIGURATION FILE ///\n"
//       << "/// " << fname << " ///\n"
//       << "/// Generated on " << ts_str << " ///\n"
//       << "\n\n"
//       << "{\n\n";

//   ofs << "/// DIRECTORIES ///\n\n";

//   ofs << "\tTDirectory* ROOTBEER = gROOT;\n\n";

//   for(Int_t i=0; i< gROOT->GetList()->GetEntries(); ++i) {
//     if(TString(gROOT->GetList()->At(i)->ClassName()) == "TDirectory") {
//       TDirectory* dir = static_cast<TDirectory*> (gROOT->GetList()->At(i));
//       ofs << "TDirectory* " << dir->GetName() << " = new TDirectory (\""
// 	  << dir->GetName() << "\", "
// 	  << dir->GetTitle() << "\", "
// 	  << dir->GetOption() << "\", "
// 	  << dir->GetMotherDir()->GetName() << ");\n\n";
//     }
//   }

//   ofs << "/// HISTOGRAMS ///\n\n";

//   for(Int_t i=0; i< fHistograms->GetEntries(); ++i) {
//     rb::Hist<double>* obj  =
//       static_cast<rb::Hist<double>*>(fHistograms->At(i));
//     if(!obj) continue;

//     ofs << "\t" << obj->ClassName() << "* " << obj->GetName()
// 	<< " = new " << obj->ClassName() << "("
// 	<< "\""<<obj->GetName()<<"\", " << "\""<<obj->GetTitle() <<"\", ";

//     for(Int_t j=0; j< obj->GetNpar(); ++j) {
//       ofs << obj->GetBins()[j]  << ", "
// 	  << obj->GetLows()[j]  << ", "
// 	  << obj->GetHighs()[j] << ", " ;
//     }

//     ofs << "\"" ;
//     for(Int_t j = obj->GetNpar()-1; j>0; --j) {
//       ofs << obj->GetParam(j)->GetTitle() << ":" ;
//     }
//     ofs << obj->GetParam(0)->GetTitle() << "\", " ;
//     ofs << "\"" << obj->GetGate()->GetTitle() << "\");\n";

//     ofs << "\t" << obj->GetName() << "->SetDirectory(" << obj->GetDirectory()->GetName() << ");\n\n";
//   }

//   ofs   << "\n\n" << "}\n";
//   ofs.close();
//   return 0;
// }



