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

namespace { void start_save(const std::string& save_fname) {
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
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                                 //
// rb::attach::File                                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::attach::File::File(const char* filename, Bool_t stopAtEnd) :
  rb::Thread(FILE_THREAD_NAME),
	kFileName(gSystem->ExpandPathName(filename)),
  kStopAtEnd(stopAtEnd)
{
  fBuffer = BufferSource::New();
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

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destructor                                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::attach::File::~File() {
	if(!ListAttached()) {
		if(Rint::gApp()->GetSignals())
			 Rint::gApp()->GetSignals()->Unattaching(); // signal to gui
	}
	EventVector_t events = Rint::gApp()->GetEventVector();
	for(EventVector_t::iterator it = events.begin(); it != events.end(); ++it) {
		Rint::gApp()->GetEvent(it->first)->StopSave();
	}
	delete fBuffer;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::attach::File::DoInThread()                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace {
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
} }
void rb::attach::File::DoInThread() {

  Bool_t open = fBuffer->OpenFile(kFileName.c_str());
  if(!open) {
    Error("AttachFile", "File %s not readable.", kFileName.c_str());
    return;
  }

	Int_t nbuffers = 0;
  while(FileAttached() || ListAttached()) { // loop over buffers in the file
    bool read_success = fBuffer->ReadBufferOffline();
    if (read_success) {
			fBuffer->UnpackBuffer();
			if(Rint::gApp()->GetSignals()) Rint::gApp()->GetSignals()->UpdateBufferCounter(nbuffers++);
			else printCounter(nbuffers++);
		}
    else if (kStopAtEnd) break; // we're done
    else gSystem->Sleep(10e3); // wait 10 sec. for more data
  }
	if (1) {
		printCounter(nbuffers, true);
		std::cerr << "\n";
	}
  if(FileAttached()) { // read the complete file
    Info("AttachFile", "Done reading %s", kFileName.c_str());
	}
  else {
    if(!ListAttached()) { // told to stop externally
      Info("AttachFile", "Connection to %s aborted.", kFileName.c_str());
		}
  }
	if(Rint::gApp()->GetSignals())
		Rint::gApp()->GetSignals()->UpdateBufferCounter(nbuffers, true);
  fBuffer->CloseFile();
}



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                                 //
// rb::attach::List                                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::attach::List::List(const char* filename) :
  rb::Thread(LIST_THREAD_NAME),
  kListFileName(gSystem->ExpandPathName(filename)) {

  fBuffer = BufferSource::New();
	if(Rint::gApp()->GetSignals())
		 Rint::gApp()->GetSignals()->Attaching();
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destructor                                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::attach::List::~List() {
	if(Rint::gApp()->GetSignals())
		 Rint::gApp()->GetSignals()->Unattaching();
	delete fBuffer;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::attach::List::DoInThread()                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::attach::List::DoInThread() {
  ifstream ifs(kListFileName);
  if(!ifs.good()) {
    Error("AttachList", "List file: %s not found.", kListFileName);
    return;
  }
  while(ListAttached()) {
    TString line;
    line.ReadLine(ifs);
    if(!ifs.good()) break;
    line.ReplaceAll(" ", "");
    if(line.Contains("#"))
      line.Remove(line.First("#"));
    if(line.IsNull()) continue; // empty line

    // Attach to the listed file //
    std::auto_ptr<File> f (File::New(line.Data(), true));
    f->DoInThread();
  }
  if(ListAttached())
    Info("AttachList", "Done reading list: %s", kListFileName);
  else
    Info("AttachList", "Connection aborted.");
}



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                                 //
// rb::attach::Online                                    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace { TString get_ts_string() {
	TDatime ts;
	TString time = ts.AsSQLString();
	time.ReplaceAll("-","_");
	time.ReplaceAll(" ","_");
	time.ReplaceAll(":","");
	return time;
}}
rb::attach::Online::Online(const char* source, const char* other, char** others, int nothers) :
  rb::Thread(ONLINE_THREAD_NAME),
  fSourceArg(0),
  fOtherArg(0),
  fOtherArgs(0),
  fNumOthers(nothers)
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

  fBuffer = BufferSource::New();
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

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destructor                                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::attach::Online::~Online() {
	delete fBuffer;
	if(Rint::gApp()->GetSignals())
		 Rint::gApp()->GetSignals()->Unattaching();
	if(fSourceArg) delete[] fSourceArg;
	if(fOtherArg)  delete[] fOtherArg;
	if(fOtherArgs) {
		for(int i=0; i< fNumOthers; ++i) {
			delete[] fOtherArgs[i];
		}
		delete[] fOtherArgs;
	}
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::attach::Online::DoInThread()                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::attach::Online::DoInThread() {
	Bool_t connected = fBuffer->ConnectOnline(fSourceArg, fOtherArg, fOtherArgs, fNumOthers);
	if (!connected) return;
	if(Rint::gApp()->GetSignals())
		 Rint::gApp()->GetSignals()->AttachedOnline(fSourceArg);
	Int_t nbuffers = 0;
  while (OnlineAttached()) {
    Bool_t readSuccess = fBuffer->ReadBufferOnline();
    if(!readSuccess) 	break;
    fBuffer->UnpackBuffer();
		Rint::gApp()->GetSignals()->UpdateBufferCounter(nbuffers++);
  }
  fBuffer->DisconnectOnline();
	if(Rint::gApp()->GetSignals())
		 Rint::gApp()->GetSignals()->UpdateBufferCounter(nbuffers, true);
}
