//! \file Attach.cxx
//! \brief Implements Attach.hxx
//! \file Buffer.cxx
//! \brief Implements classes defined in Buffer.hxx
#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include <algorithm>
#include <TFile.h>
#include <TError.h>
#include <TString.h>
#include <TSystem.h>
#include <TDatime.h>
#include "utils/Assorted.hxx"
#include "Rint.hxx"
#include "Buffer.hxx"
#include "Rootbeer.hxx"
#include "Attach.hxx"



namespace { 

const Long_t ATTACH_TIMEOUT = 10; // check for data every 10 ms
const Long_t READ_TIME = 100; // read data for 100 ms before returning 

inline void printCounter(Int_t n, bool force = false) {
	if (TString(rb::Rint::gApp()->ApplicationName()) != "Rbunpack") return;
	if(n == 0) {
		std::cerr << "Buffers unpacked: ";
		std::flush(std::cerr);
	}
	if(n % 1000 != 0 && !force) return;

	static int nOut = 0;
	std::stringstream out; out << n;
	if(nOut) {
		for(int i=0; i< nOut; ++i)
			std::cerr << "\b";
	}
	nOut = out.str().size();
	std::cerr << n;
	std::flush(std::cerr);
}

inline Int_t find_timer(TClass* timerclass, TTimer*& output) {
	output = 0;
	Int_t retval = 0;
	TSeqCollection* timers = gSystem->GetListOfTimers();
	for(Int_t i=0; i< timers->GetEntries(); ++i) {
		TObject* t = timers->At(i);
		if(t->InheritsFrom(timerclass)) {
			++retval;
			output = static_cast<TTimer*>(t);
		}
	}
	return retval;
}

void start_save(const std::string& save_fname) {
	TDirectory* current = gDirectory;
	if(!current) current = gROOT;
	boost::shared_ptr<TFile> file (new TFile(save_fname.c_str(), "recreate"));
	current->cd();
	rb::EventVector_t events = rb::Rint::gApp()->GetEventVector();
	for(rb::EventVector_t::iterator it = events.begin(); it != events.end(); ++it) {
		std::stringstream tname; tname << "t" << it->first;
		std::stringstream ttitle; ttitle << it->second << " data";
		rb::Rint::gApp()->GetEvent(it->first)->
			 StartSave(file, tname.str().c_str(), ttitle.str().c_str(), rb::Rint::gApp()->GetSaveHists());
	}
}

inline void call_begin_run() {
	// call BeginRun() on all rb::Events
	rb::EventVector_t events = rb::Rint::gApp()->GetEventVector();
	rb::Event::RunBegin begin_run_functor;
	std::for_each(events.begin(), events.end(), begin_run_functor);
}

template <class T>
Bool_t check_attached() {
	TTimer* t;
	return find_timer(rb::AttachTimer<T>::Class(), t);
}
} // namespace

Bool_t rb::FileAttached() {
	return check_attached<rb::FileAttach>();
}

Bool_t rb::OnlineAttached() {
	return check_attached<rb::OnlineAttach>();
}

Bool_t rb::ListAttached() {
	return check_attached<rb::ListAttach>();
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//\\\\\\\\\\\\ Class rb::FileAttached \\\\\\\\\\\\//
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

rb::FileAttach::FileAttach(const char* filename, Bool_t stopAtEnd):
	fTimeout(ATTACH_TIMEOUT),
	fTimer(0),
	fBuffer(0),
	kFileName(filename),
	kStopAtEnd(stopAtEnd),
	fNbuffers(0) {

	TString file1 = kFileName;
	gSystem->ExpandPathName(file1);
	kFileName = file1;

	if(!ListAttached()) rb::Unattach();
	call_begin_run(); 	// call begin run on all events

	if(!ListAttached()) {
		if(Rint::gApp()->GetSignals()) Rint::gApp()->GetSignals()->Attaching(); // signal to gui
	}
	std::string fname(kFileName);
	if(fname.find_last_of("/") < fname.size())
		 fname = fname.substr(fname.find_last_of("/")+1);
	if(Rint::gApp()->GetSignals())
		 Rint::gApp()->GetSignals()->AttachedFile(fname.c_str());
	if(Rint::gApp()->GetSaveData()) {
		std::string save_fname = expand_path_std(kSaveStaticDefault, "$RB_SAVEDIR");
		save_fname += "/";
		save_fname += fname;
		save_fname = save_fname.substr(0, save_fname.find_last_of("."));
		save_fname += ".root";
		start_save(save_fname);
	}
}

rb::FileAttach::~FileAttach() {
	if(!ListAttached()) {
		if(Rint::gApp()->GetSignals())
			 Rint::gApp()->GetSignals()->Unattaching(); // signal to gui
	}
	EventVector_t events = Rint::gApp()->GetEventVector();
	for(EventVector_t::iterator it = events.begin(); it != events.end(); ++it) {
		Rint::gApp()->GetEvent(it->first)->StopSave();
	}
}

void rb::FileAttach::Stop() {
	TTimer* t;
	while(find_timer(rb::AttachTimer<rb::FileAttach>::Class(), t)) {
		t->TurnOff();
	}	
}

void rb::FileAttach::TimerAction() {
	if(!fBuffer.get()) {
		fNbuffers = 0;
		fBuffer.reset(rb::BufferSource::New());
		Bool_t open = fBuffer->OpenFile(kFileName.c_str());
		if(!open) {
			Error("FileAttach", "File %s not readable.", kFileName.c_str());
			fTimer->TurnOff();
			return;
		}
	}

	rb::Timeout timeout(READ_TIME);
  while (1) {
    bool read_success = fBuffer->ReadBufferOffline();
    if (read_success) {
			fBuffer->UnpackBuffer();
			if(Rint::gApp()->GetSignals())
				Rint::gApp()->GetSignals()->UpdateBufferCounter(fNbuffers++);
			else printCounter(fNbuffers++);
		}
    else if (kStopAtEnd)
			break; // we're done
		else
			return; // yield

		if(timeout.Check()) // yield
			return;
  }
	if (1) {
		printCounter(fNbuffers, true);
		std::cerr << "\n";
	}
  if(FileAttached()) { // read the complete file
    Info("FileAttach", "Done reading %s", kFileName.c_str());
	}
  else {
    if(!ListAttached()) { // told to stop externally
      Info("FileAttach", "Connection to %s aborted.", kFileName.c_str());
		}
  }
	if(Rint::gApp()->GetSignals())
		Rint::gApp()->GetSignals()->UpdateBufferCounter(fNbuffers, true);
  fBuffer->CloseFile();
	fTimer->TurnOff();
};



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//\\\\\\\\\\\\ Class rb::ListAttached \\\\\\\\\\\\//
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

rb::ListAttach::ListAttach(const char* filename):
	fTimeout(ATTACH_TIMEOUT),
	fTimer(0),
	fBuffer(0),
	kListName(filename),
	fNbuffers(0),
	fFileIndex(0) {
	rb::Unattach();

	// parse input file
	std::ifstream ifs(kListName.c_str());
	std::string line;
	while(1) {
		std::getline(ifs, line);
		if(!ifs.good()) break;

		line = line.substr(0, line.find("#"));
		fFileNames.push_back(line);
	}

}

rb::ListAttach::~ListAttach() {
}

void rb::ListAttach::Stop() {
	TTimer* t;
	while(find_timer(rb::AttachTimer<rb::ListAttach>::Class(), t)) {
		t->TurnOff();
	}	
}

void rb::ListAttach::TimerAction() {
	if(rb::FileAttached() == false) {
		size_t index = fFileIndex++;
		if(index < fFileNames.size())
			rb::AttachFile(fFileNames.at(index).c_str(), kTRUE);
		else
			fTimer->TurnOff();
	}
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//\\\\\\\\\\\\ Class rb::OnlineAttached \\\\\\\\\\\\//
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace { TString get_ts_string() {
	TDatime ts;
	TString time = ts.AsSQLString();
	time.ReplaceAll("-","_");
	time.ReplaceAll(" ","_");
	time.ReplaceAll(":","");
	return time;
}}

rb::OnlineAttach::OnlineAttach(const char* source, const char* other, char** others, int nothers):
	fTimeout(ATTACH_TIMEOUT),
	fTimer(0),
  fSourceArg(0),
  fOtherArg(0),
  fOtherArgs(0),
  fNumOthers(nothers),
	fBuffer(0),
	fNbuffers(0)
{

	fSourceArg = new char [strlen(source)+1];
	strcpy(fSourceArg, source);

	fOtherArg = new char[strlen(other)+1];
	strcpy(fOtherArg, other);

	fOtherArgs = new char*[nothers];
	for(int i=0; i< nothers; ++i) {
		fOtherArgs[i] = new char[strlen(others[i])+1];
		strcpy(fOtherArgs[i], others[i]);
	}

	call_begin_run();

	if(Rint::gApp()->GetSignals())
		 Rint::gApp()->GetSignals()->Attaching();

	if(Rint::gApp()->GetSaveData()) {
		std::string save_fname = expand_path_std(kSaveStaticDefault, "$RB_SAVEDIR");
		save_fname += "Online_";
		save_fname += get_ts_string().Data();
		save_fname += ".root";
		start_save(save_fname);
	}
}

rb::OnlineAttach::~OnlineAttach() {
	if(Rint::gApp()->GetSignals()) {
		Rint::gApp()->GetSignals()->UpdateBufferCounter(fNbuffers, true);
		Rint::gApp()->GetSignals()->Unattaching();
	}
	if(fSourceArg) delete[] fSourceArg;
	if(fOtherArg)  delete[] fOtherArg;
	if(fOtherArgs) {
		for(int i=0; i< fNumOthers; ++i) {
			delete[] fOtherArgs[i];
		}
		delete[] fOtherArgs;
	}

  fBuffer->DisconnectOnline();
}

void rb::OnlineAttach::Stop() {
	TTimer* t;
	while(find_timer(rb::AttachTimer<rb::OnlineAttach>::Class(), t)) {
		t->TurnOff();
	}	
}

void rb::OnlineAttach::TimerAction() {
	if(!fBuffer.get()) {
		fNbuffers = 0;
		fBuffer.reset(BufferSource::New());
		Bool_t connected =
			fBuffer->ConnectOnline(fSourceArg, fOtherArg, fOtherArgs, fNumOthers);
		if (!connected) {
			fTimer->TurnOff();
			return;
		}
	}

	if(Rint::gApp()->GetSignals())
		 Rint::gApp()->GetSignals()->AttachedOnline(fSourceArg);


	rb::Timeout timeout(READ_TIME);
  while (1) {
    Bool_t haveEvent = fBuffer->ReadBufferOnline();

		if (haveEvent) {
			fBuffer->UnpackBuffer();
			Rint::gApp()->GetSignals()->UpdateBufferCounter(fNbuffers++);
		}

		if(timeout.Check())
			return; // yield
  }

	fTimer->TurnOff();
};
