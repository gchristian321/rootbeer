//! \file Rootbeer.cxx 
//! \brief Implements the user interface functions.
#include <memory>
#include <iostream>
#include <TCutG.h>
#include <TString.h>
#include <TObjArray.h>
#include <TVirtualPad.h>
#include "Rootbeer.hxx"
#include "Rint.hxx"
#include "Buffer.hxx"
#include "Data.hxx"
#include "Signals.hxx"
#include "hist/Hist.hxx"
#include "utils/Error.hxx"


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Public interface (Rootbeer.hxx) implementations       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::AttachOnline                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::AttachOnline(const char* host, const char* other, char** others, int nothers) {
	/// \bug Crashes when trying to attach more than once.
#ifdef MIDAS_BUFFERS
#ifdef MIDAS_ONLINE // Attach to online midas files
  if(others || nothers)
		 Warning("AttachOnline", "Arguments \'others\' and \'n_others\' are unused");

  rb::Unattach();
  rb::attach::Online::CreateAndRun(host, other, others, nothers);

#else
  Info("AttachOnline",
       "MIDAS was not found on your system; cannot attach to online MIDAS data.\n"
       "Instructions for installing MIDAS can be found online at:\n"
       "      http://daq-plone.triumf.ca/SM/docs/local/installmidas.html\n\n"
       "Please note that you will need to have the MIDASSYS environment variable\n"
       "defined in order to attach to online data with rootbeer.\n");
#endif

#elif defined NSCL_BUFFERS
  Info("AttachOnline",
       "Online attachment to NSCL data is not yet implemented.");
#else
  Info("AttachOnline",
       "Attaching to online buffers other than MIDAS and NSCL is not yet part of\n"
       "stock ROOTBEER. You'll have to define it yourself. Once you've done this,\n"
       "pleae contact the developers (Greg Christian, gchristian@triumf.ca) about\n"
       "adding it to the source code.\n");
#endif
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::AttachFile                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::AttachFile(const char* filename, Bool_t stop_at_end) {
  rb::Unattach();
  rb::attach::File::CreateAndRun(filename, stop_at_end);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::AttachList                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::AttachList(const char* filename) {
  rb::Unattach();
  rb::attach::List::CreateAndRun(filename);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Unattach()                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Unattach() {
  rb::attach::StopAll();
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// TVirtualPad* rb::CdPad                                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
TVirtualPad* rb::CdPad(TVirtualPad* owner, Int_t* subpad_numbers, Int_t depth) {
	TVirtualPad* current = owner;
	for(Int_t i=0; i< depth; ++i) {
		current = current->cd( subpad_numbers[i] );
		if(!current) {
			err::Error("CdPad") << "Invalid subpad_numbers";
			return 0;
		}
	}
	return current;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// TDirectory* rb::Cd                                    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
TDirectory* rb::Cd(const char* path_, Bool_t silent) {
	/*!
	 * \bug This doesn't seem to work at all! It says the path is invalid if specified
	 * a full "terminal" path ("home/sub") or throws an exception if specified a non-terminal
	 * path ("home"). Maybe the best fix is to re-visit why we need this function at all....
	 */
	try {
		gROOT->cd();
		std::string path(path_);
		path = path.substr(std::string(gROOT->GetName()).size()+2);
		if(path.size() == 0) {
			gROOT->cd(); return gROOT;
		}
		TDirectory* dir = 0;
		Bool_t quit = false;
		while(1) {
			unsigned long pos = path.find("/");
			std::string this_path = "";
			if(pos > path.size()) {
				quit = true;
				this_path = path;
			} else {
				this_path = path.substr(0, pos);
				path = path.substr(this_path.size()+1);
			}	
			dir = dynamic_cast<TDirectory*>(gROOT->FindObject(this_path.c_str()));
			if(!dir) {
				if(!silent)
					err::Error("rb::Cd") << "A portion of the path (" << this_path << ") is invalid.\n"
															 << "Full path = " << path_;
				break;
			}
			dir->cd();
			if(quit) break;
		}
		return dir;
	} catch (std::exception& e) {
		err::Error("rb::Cd") << "Caught an exception: " << e.what();
		return 0;
	}
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// TDirectory* rb::Mkdir                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
TDirectory* rb::Mkdir(const char* name, const char* title) {
	if(!gDirectory) {
		Error("rb::Mkdir", "gDirectory == 0");
		return 0;
	}

	TDirectory* current = gDirectory;
	gROOT->cd();
	TString sdp(name);
	std::auto_ptr<TObjArray> dirs (sdp.Tokenize("/"));
	for (int i=0; i< dirs->GetEntries(); ++i) {
		TString dirname = static_cast<TObjString*>(dirs->At(i))->GetString();
		TDirectory* dir = dynamic_cast<TDirectory*>(gDirectory->FindObject(dirname.Data()));
		if(dir) dir->cd();
		else {
			TDirectory* d = gDirectory->mkdir(dirname.Data());
			d->cd();
		}
	}
	TDirectory* newDir = gDirectory;
	if(newDir) newDir->cd();
	else current->cd();
	if(gApp()->GetHistSignals()) gApp()->GetHistSignals()->SyncHistTree();
	return newDir;

}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::SetTCutGOverwrite                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace { Bool_t gTCutGOverwrite = false; }
Bool_t rb::SetTCutGOverwrite(Bool_t on) {
	Bool_t ret = (on != gTCutGOverwrite);
	gTCutGOverwrite = on;
	return ret;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::CreateTCutG                                  //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
TCutG* rb::CreateTCutG(const char* name, Int_t n, const Double_t* x, const Double_t* y,
											 const char* varx, const char* vary, Width_t lineWidth, Color_t lineColor) {
	std::string Name = name;
	if(dynamic_cast<TCutG*>(gROOT->GetListOfSpecials()->FindObject(name))) {
		if(gTCutGOverwrite) {
			delete dynamic_cast<TCutG*>(gROOT->GetListOfSpecials()->FindObject(name));
			Name = name;
		}
		else {
			std::stringstream ssname;
			ssname << name;
			int i=1;
			while(dynamic_cast<TCutG*>(gROOT->GetListOfSpecials()->FindObject(ssname.str().c_str()))) {
				ssname.str("");
				ssname << name << "_" << i++;
			}
			Name = ssname.str();
		}
	}
	TCutG* cutg = new TCutG(Name.c_str(), n, x, y);
	if(cutg) {
		cutg->SetVarX(varx);
		cutg->SetVarY(vary);
		cutg->SetLineWidth(lineWidth);
		cutg->SetLineColor(lineColor);
	}
	return cutg;
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Double_t rb::data::GetValue                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Double_t rb::data::GetValue(const char* name) {
  data::MBasic* basicData = 0;
  basicData = data::MBasic::Find(name);
  if(!basicData) {
    Error("GetValue", "%s not found.", name);
    return -1.;
  }
  return (Double_t)basicData->GetValue();
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::SetValue                               //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::data::SetValue(const char* name, Double_t newvalue) {
  data::MBasic* basicData = 0;
  basicData = data::MBasic::Find(name);
  if(!basicData) {
    Error("SetData", "Data object: %s not found.", name);
    return;
  }
  basicData->SetValue(newvalue);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::PrintAll                               //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::data::PrintAll() {
  data::MBasic::Printer p;
  p.PrintAll();
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Histogram Creation Helper Function                    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace { rb::hist::Manager* const find_manager(Int_t code) {
  rb::Event* event = rb::gApp()->GetEvent(code);
  if(event == 0) err::Throw() << "Invalid event code: " << code;
  return event->GetHistManager();
}}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::hist::New (One-dimensional)                       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::New(const char* name, const char* title,
															Int_t bx, Double_t xl, Double_t xh,
															const char* param, const char* gate, Int_t event_code) {
	Bool_t from_gui =
		 gApp()->GetHistSignals() ? gApp()->GetHistSignals()->IsHistFromGui() : 0;
  rb::hist::Base* hist = 0;
  try {
    hist = find_manager(event_code)->Create<D1>(name, title, param, gate, event_code, bx, xl, xh);
  }
  catch (std::exception& e) {
		if(!from_gui) err::Error("rb::hist::New") << e.what();
		else throw;
  }
  return hist;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  rb::hist::New (Two-dimensional)                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::New(const char* name, const char* title,
															Int_t bx, Double_t xl, Double_t xh,
															Int_t by, Double_t yl, Double_t yh,
															const char* param, const char* gate, Int_t event_code) {
  Bool_t from_gui =
		 gApp()->GetHistSignals() ? gApp()->GetHistSignals()->IsHistFromGui() : 0;
  rb::hist::Base* hist = 0;
  try {
    hist = find_manager(event_code)->Create<D2>(name, title, param, gate, event_code, bx, xl, xh, by, yl, yh);
  }
  catch (std::exception& e) {
		if(!from_gui) err::Error("rb::hist::New") << e.what();
		else throw;
  }
  return hist;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  rb::hist::New (Three-dimensional)                    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::New(const char* name, const char* title,
															Int_t bx, Double_t xl, Double_t xh,
															Int_t by, Double_t yl, Double_t yh,
															Int_t bz, Double_t zl, Double_t zh,
															const char* param, const char* gate, Int_t event_code) {
	Bool_t from_gui =
		 gApp()->GetHistSignals() ? gApp()->GetHistSignals()->IsHistFromGui() : 0;

  rb::hist::Base* hist = 0;
  try {
		hist = find_manager(event_code)->Create<D3>(name, title, param, gate, event_code, bx, xl, xh, by, yl, yh, bz, zl, zh);
  }
  catch (std::exception& e) {
		if(!from_gui) err::Error("rb::hist::New") << e.what();
		else throw;
  }
  return hist;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::hist::NewSummary()                                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::NewSummary(const char* name, const char* title,
																		 Int_t nbins, Double_t low, Double_t high,
																		 const char* paramList,  const char* gate, Int_t event_code,
																		 const char* orient) {
	Bool_t from_gui =
		 gApp()->GetHistSignals() ? gApp()->GetHistSignals()->IsHistFromGui() : 0;

  rb::hist::Base* hist = 0;
  try {
    hist = find_manager(event_code)->Create<Summary>(name, title, paramList, gate, event_code,
																										 nbins, low, high, orient);
  }
  catch (std::exception& e) {
		if(!from_gui) err::Error("rb::hist::New") << e.what();
		else throw;
  }
  return hist;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  rb::hist::NewGamma (One-dimensional)                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::NewGamma(const char* name, const char* title,
																	 Int_t nbinsx, Double_t xlow, Double_t xhigh,
																	 const char* param, const char* gate, Int_t event_code) {
	Bool_t from_gui =
		 gApp()->GetHistSignals() ? gApp()->GetHistSignals()->IsHistFromGui() : 0;

  rb::hist::Base* hist = 0;
  try {
    hist = find_manager(event_code)->Create<Gamma>(name, title, param, gate, event_code, nbinsx, xlow, xhigh);
  }
  catch (std::exception& e) {
		if(!from_gui) err::Error("rb::hist::New") << e.what();
		else throw;
  }
  return hist;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  rb::hist::NewGamma (Two-dimensional)                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::NewGamma(const char* name, const char* title,
																	 Int_t nbinsx, Double_t xlow, Double_t xhigh,
																	 Int_t nbinsy, Double_t ylow, Double_t yhigh,
																	 const char* param, const char* gate, Int_t event_code) {
	Bool_t from_gui =
		 gApp()->GetHistSignals() ? gApp()->GetHistSignals()->IsHistFromGui() : 0;

  rb::hist::Base* hist = 0;
  try {
    hist = find_manager(event_code)->Create<Gamma>(name, title, param, gate, event_code,
																									 nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
  }
  catch (std::exception& e) {
		if(!from_gui) err::Error("rb::hist::New") << e.what();
		else throw;
  }
  return hist;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  rb::hist::NewGamma (Three-dimensional)               //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::NewGamma(const char* name, const char* title,
																	 Int_t nbinsx, Double_t xlow, Double_t xhigh,
																	 Int_t nbinsy, Double_t ylow, Double_t yhigh,
																	 Int_t nbinsz, Double_t zlow, Double_t zhigh,
																	 const char* params,  const char* gate, Int_t event_code) {
	Bool_t from_gui =
		 gApp()->GetHistSignals() ? gApp()->GetHistSignals()->IsHistFromGui() : 0;

  rb::hist::Base* hist = 0;
  try {
    hist = find_manager(event_code)->Create<Gamma>(name, title, params, gate, event_code,
																									 nbinsx, xlow, xhigh, nbinsy, ylow, yhigh, nbinsz, zlow, zhigh);
  }
  catch (std::exception& e) {
		if(!from_gui) err::Error("rb::hist::New") << e.what();
		else throw;
  }
  return hist;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  rb::hist::NewScaler                                  //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::NewScaler(const char* name, const char* title,
																		Int_t nbins, Double_t low, Double_t high,
																		const char* params,  const char* gate, Int_t event_code) {
	Bool_t from_gui =
		gApp()->GetHistSignals() ? gApp()->GetHistSignals()->IsHistFromGui() : 0;

  rb::hist::Base* hist = 0;
  try {
    hist = find_manager(event_code)->Create<Scaler>(name, title, params, gate, event_code, nbins, low, high);
  }
  catch (std::exception& e) {
		if(!from_gui) err::Error("rb::hist::New") << e.what();
		else throw;
  }
  return hist;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//  rb::hist::NewBit                                     //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::hist::NewBit(const char* name, const char* title, Int_t nbits, const char* param, const char* gate,
																 Int_t event_code) {
	Bool_t from_gui =
		 gApp()->GetHistSignals() ? gApp()->GetHistSignals()->IsHistFromGui() : 0;
  rb::hist::Base* hist = 0;
  try {
    hist = find_manager(event_code)->Create<Bit>(name, title, param, gate, event_code, nbits, 0., 1.);
  }
  catch (std::exception& e) {
		if(!from_gui) err::Error("rb::hist::New") << e.what();
		else throw;
  }
  return hist;
}
