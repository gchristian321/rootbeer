//! \file Buffer.cxx
//! \brief Implements classes defined in Buffer.hxx
#include <fstream>
#include <memory>
#include <TError.h>
#include <TString.h>
#include <TSystem.h>
#include "Buffer.hxx"


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                                 //
// rb::attach::File                                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::BufferSource::BufferSource() :
#define RB_REFERENCE_INIT
#include "rb_import_data.h"
  kDummy(0) {}


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
  kStopAtEnd(stopAtEnd) {

  fBuffer = BufferSource::New();
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

  while(FileAttached() || ListAttached()) { // loop over buffers in the file
    bool read_success = fBuffer->ReadBufferOffline();
    if (read_success) fBuffer->UnpackBuffer(); // got an event
    else if (kStopAtEnd) break; // we're done
    else gSystem->Sleep(10e3); // wait 10 sec. for more data
  }

  if(FileAttached()) // read the complete file
    Info("AttachFile", "Done reading %s", kFileName);
  else {
    if(!ListAttached())  // told to stop externally
      Info("AttachFile", "Connection to %s aborted.", kFileName);
  }

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
rb::attach::Online::Online(const char* source, const char* other, char** others, int nothers) :
  rb::Thread(ONLINE_THREAD_NAME),
  fSourceArg(source),
  fOtherArg(other),
  fOtherArgs(others),
  fNumOthers(nothers) {

  fBuffer = BufferSource::New();  
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::attach::Online::DoInThread()                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::attach::Online::DoInThread() {
  Bool_t connected = fBuffer->ConnectOnline(fSourceArg, fOtherArg, fOtherArgs, fNumOthers);
  if (!connected) return;
  while (OnlineAttached()) {
    Bool_t readSuccess = fBuffer->ReadBufferOnline();
    if(!readSuccess) break;
    fBuffer->UnpackBuffer();
  }
  fBuffer->DisconnectOnline();
}
