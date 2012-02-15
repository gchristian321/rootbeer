//! \file Buffer.cxx
//! \brief Implements classes deefined in Buffer.hxx
#include <iostream>
#include <fstream>
#include <string>
#include <TSystem.h>
#include <TThread.h>
#include <TError.h>
#include "Buffer.hxx"





// void* rb::BufferSource::AttachList_(void * arg) {
//   BufferSource::SetAttached(LIST_);

//   char* filename = (char*)arg;
//   ifstream ifs(filename);
//   if(!ifs.good()) {
//     Error("AttachList", "List file: %s not found.", filename);
//     return 0;
//   }
//   std::string line;
//   while(1) {
//     getline(ifs, line);
//     if(!ifs.good()) break;

//     line = line.substr(0, line.find("#"));
//     std::string str("");
//     for(UInt_t i=0; i< line.size(); ++i) {
//       if(line[i] != ' ') str.push_back(line[i]);
//     }
//     if(str.size() == 0) continue; // empty line, skip

//     /// opening current file ///
//     BufferSource* f = BufferSource::Instance(); //new USER_BUFFER_SOURCE(); // delete in ::AttachFile
//     f->FileArgs().stopEnd = kTRUE;
//     f->FileArgs().fileName = str;
//     Bool_t open = f->OpenFile(gSystem->ExpandPathName(filename));
//     if(!open) {
//       Info("AttachList", "The file %s wasn't found. Moving on to the next one.", str.c_str());
//       ///	delete f;
//       continue;
//     }
//     AttachFile_(reinterpret_cast<void*>(f));
//   }
//   BufferSource::SetNotAttached(FILE_);
//   Unattach();
//   return arg;
// }


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class rb::attach::File                                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::attach::File::File(const char* filename, Bool_t stopAtEnd) :
  rb::Thread("AttachFile"),
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
    Error("AtachFile", "File %s not readable.", kFileName);
    return;
  }

  while(rb::Thread::IsRunning("AttachFile")) { // loop over buffers in the file
    bool read_success = fBuffer->ReadBufferOffline();
    if (read_success) fBuffer->UnpackBuffer(); // got an event
    else if (kStopAtEnd) break; // we're done
    else gSystem->Sleep(10e3); // wait 10 sec. for more data
  }

  if(rb::Thread::IsRunning("AttachFile")) // read the complete file
    Info("AttachFile", "Done reading %s", kFileName);
  else                                    // told to stop externally
    Info("AttachFile", "Connection to %s aborted.", kFileName);

  fBuffer->CloseFile();
}




//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class rb::attach::Online                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::attach::Online::Online(const char* source, const char* other, char** others, int nothers) :
  rb::Thread("AttachOnline"),
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
  while (rb::Thread::IsRunning("AttachOnline")) {
    Bool_t readSuccess = fBuffer->ReadBufferOnline();
    if(!readSuccess) break;
    fBuffer->UnpackBuffer();
  }
  fBuffer->DisconnectOnline();
}
