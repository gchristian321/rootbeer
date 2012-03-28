//! \file Buffer.cxx
//! \brief Implements classes defined in Buffer.hxx
#include <fstream>
#include <memory>
#include <TError.h>
#include <TString.h>
#include <TSystem.h>
#include <TDatime.h>
#include "Rint.hxx"
#include "Buffer.hxx"

extern void attach_sync();

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
	if(!ListAttached()) gApp()->GetSignals()->Attaching(); // signal to gui
	std::string fname(kFileName);
	if(fname.find_last_of("/") < fname.size())
		 fname = fname.substr(fname.find_last_of("/")+1);
	gApp()->GetSignals()->AttachedFile(fname.c_str());

	if(gApp()->GetSaveData()) {
#ifdef RB_DEFAULT_SAVE_DIR
		std::string save_fname = RB_DEFAULT_SAVE_DIR ;
		save_fname += fname;
#else
		std::string save_fname = fname;
#endif
		save_fname = save_fname.substr(0, save_fname.find_last_of("."));
		save_fname += ".root";
		EventVector_t events = gApp()->GetEventVector();
		for(EventVector_t::iterator it = events.begin(); it != events.end(); ++it) {
			std::stringstream tname; tname << "t" << it->first;
			std::stringstream ttitle; ttitle << it->second << " data";
			gApp()->GetEvent(it->first)->
				 StartSave(save_fname.c_str(), tname.str().c_str(), ttitle.str().c_str(), gApp()->GetSaveHists());
		}
	}
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destructor                                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::attach::File::~File() {
	if(!ListAttached()) gApp()->GetSignals()->Unattaching(); // signal to gui
	EventVector_t events = gApp()->GetEventVector();
	for(EventVector_t::iterator it = events.begin(); it != events.end(); ++it) {
		gApp()->GetEvent(it->first)->StopSave();
	}
	delete fBuffer;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::attach::File::DoInThread()                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::attach::File::DoInThread() {

  Bool_t open = fBuffer->OpenFile(kFileName);
  if(!open) {
    Error("AttachFile", "File %s not readable.", kFileName);
    return;
  }

	Int_t nbuffers = 0;
  while(FileAttached() || ListAttached()) { // loop over buffers in the file
    bool read_success = fBuffer->ReadBufferOffline();
    if (read_success) {
			fBuffer->UnpackBuffer();
			gApp()->GetSignals()->UpdateBufferCounter(nbuffers++);
		}
    else if (kStopAtEnd) break; // we're done
    else gSystem->Sleep(10e3); // wait 10 sec. for more data
  }

  if(FileAttached()) { // read the complete file
    Info("AttachFile", "Done reading %s", kFileName);
	}
  else {
    if(!ListAttached()) { // told to stop externally
      Info("AttachFile", "Connection to %s aborted.", kFileName);
		}
  }
	gApp()->GetSignals()->UpdateBufferCounter(nbuffers, true);
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
	gApp()->GetSignals()->Attaching();
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destructor                                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::attach::List::~List() {
	gApp()->GetSignals()->Unattaching();
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
  fSourceArg(source),
  fOtherArg(other),
  fOtherArgs(others),
  fNumOthers(nothers) {

  fBuffer = BufferSource::New();  
	gApp()->GetSignals()->Attaching();

	if(gApp()->GetSaveData()) {
#ifdef RB_DEFAULT_SAVE_DIR
		std::string save_fname = RB_DEFAULT_SAVE_DIR ;
#else
		std::string save_fname = "";
#endif
		save_fname += "Online__";
		save_fname += get_ts_string().Data();
		save_fname += ".root";
		EventVector_t events = gApp()->GetEventVector();
		for(EventVector_t::iterator it = events.begin(); it != events.end(); ++it) {
			std::stringstream tname; tname << "t" << it->first;
			std::stringstream ttitle; ttitle << it->second << " data";
			gApp()->GetEvent(it->first)->
				 StartSave(save_fname.c_str(), tname.str().c_str(), ttitle.str().c_str(), gApp()->GetSaveHists());
		}
	}
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Destructor                                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::attach::Online::~Online() {
	delete fBuffer;
	gApp()->GetSignals()->Unattaching();
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::attach::Online::DoInThread()                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::attach::Online::DoInThread() {
  Bool_t connected = fBuffer->ConnectOnline(fSourceArg, fOtherArg, fOtherArgs, fNumOthers);
  if (!connected) return;
	gApp()->GetSignals()->AttachedOnline(fSourceArg);
	Int_t nbuffers = 0;
  while (OnlineAttached()) {
    Bool_t readSuccess = fBuffer->ReadBufferOnline();
    if(!readSuccess) break;
    fBuffer->UnpackBuffer();
		gApp()->GetSignals()->UpdateBufferCounter(nbuffers++);
  }
  fBuffer->DisconnectOnline();
	gApp()->GetSignals()->UpdateBufferCounter(nbuffers, true);
}
