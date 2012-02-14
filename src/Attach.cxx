//! \file Attach.cxx
//! \brief Implements routines relevant to attaching to data sources.
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <TError.h>
#include <TSystem.h>
#include <TTree.h>
#include <TThread.h>
#include <TRandom3.h>
#include "Data.hxx"
#include "Rootbeer.hxx"
using namespace std;


//! Anonomyous namespace enclosing local (file-scope) functions and variables.
namespace
{
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// TThread functions for attaching to data               //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

  //! Attaches to an online data source, in the format expected by TThread.
  void* AttachOnline(void* arg) {
    BufferSource* pOnline = reinterpret_cast<BufferSource*> (arg);
    BufferSource::SetAttached(ONLINE_);

    Bool_t connected =
      pOnline->ConnectOnline(pOnline->OnlineArgs().source.c_str(),
			     pOnline->OnlineArgs().other.c_str(),
			     pOnline->OnlineArgs().others,
			     pOnline->OnlineArgs().nothers);
    if (!connected) return 0;

    while (BufferSource::IsAttached(ONLINE_)) {
      Bool_t readSuccess = pOnline->ReadBufferOnline();
      if(!readSuccess) break;
      pOnline->UnpackBuffer();
    }
    return 0;
  }

  //! Attaches to an offline data source (file), in the format expected by TThread.
  void* AttachFile(void* arg) {
    BufferSource* pFile = reinterpret_cast<BufferSource*> (arg);
    BufferSource::SetAttached(FILE_);

    while(BufferSource::IsAttached(FILE_)) {
      bool read_success = pFile->ReadBufferOffline();
      if (read_success) pFile->UnpackBuffer(); // got an event
      else if (pFile->FileArgs().stopEnd) break; // we're done
      else gSystem->Sleep(10e3); // Wait 10 seconds for more data to come in 
    }

    if(BufferSource::IsAttached(FILE_)) {
      if(!BufferSource::IsAttached(LIST_)) {
	Info("AttachFile", "Done reading %s", pFile->FileArgs().fileName.c_str());
	rb::Unattach();
      }
    }
    else Info("AttachFile", "Connection aborted by user.");
    return arg;
  }

  //! Attaches to a list of offline files, in the format expected by TThread.
  void* AttachList(void * arg) {
    BufferSource::SetAttached(LIST_);

    char* filename = (char*)arg;
    ifstream ifs(filename);
    if(!ifs.good()) {
      Error("AttachList", "List file: %s not found.", filename);
      return 0;
    }
    string line;
    while(1) {
      getline(ifs, line);
      if(!ifs.good()) break;

      line = line.substr(0, line.find("#"));
      string str("");
      for(UInt_t i=0; i< line.size(); ++i) {
	if(line[i] != ' ') str.push_back(line[i]);
      }
      if(str.size() == 0) continue; // empty line, skip

      /// opening current file ///
      BufferSource* f = BufferSource::Instance(); //new USER_BUFFER_SOURCE(); // delete in ::AttachFile
      f->FileArgs().stopEnd = kTRUE;
      f->FileArgs().fileName = str;
      Bool_t open = f->OpenFile(gSystem->ExpandPathName(filename));
      if(!open) {
	Info("AttachList", "The file %s wasn't found. Moving on to the next one.", str.c_str());
	///	delete f;
	continue;
      }
      ::AttachFile(reinterpret_cast<void*>(f));
    }
    BufferSource::SetNotAttached(FILE_);
    rb::Unattach();
    return arg;
  }

  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  // Local TThread definitions                             //
  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

  //! Thread for attaching to online data.
  TThread attachOnlineThread("attachOnline", ::AttachOnline);

  //! Thread for attaching to offline data.
  TThread attachOfflineThread("attachFile", ::AttachFile);

  //! Thread for attaching to a list of offline files.
  TThread attachListThread("attachList", ::AttachList);
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Public interface (Rootbeer.hxx) implementations       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::AttachOnline                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::AttachOnline(const char* host, const char* other, char** others, int nothers) {
  rb::Unattach();

#ifdef MIDAS_BUFFERS
#ifdef MIDAS_ONLINE // Attach to online midas files
  if(others || nothers) Warning("AttachOnline", "Arguments \'others\' and \'n_others\' are unused");
  BufferSource* online = BufferSource::Instance(); // new USER_BUFFER_SOURCE(); // delete in ::AttachOnline
  online->OnlineArgs().Set(host, other, others, nothers);
  attachOnlineThread.Run(reinterpret_cast<void*>(online));
#else
  Info("AttachOnline",
       "MIDAS was not found on your system; cannot attach to online MIDAS data.\n"
       "Instructions for installing MIDAS can be found online at:\n"
       "      http://daq-plone.triumf.ca/SM/docs/local/installmidas.html\n\n"
       "Please note that you will need to have the MIDASSYS environment variable\n"
       "defined in order to attach to online data with rootbeer.\n");
#endif

#elif defined _NSCL_
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
  BufferSource* file = BufferSource::Instance(); //new USER_BUFFER_SOURCE(); // delete in ::AttachFile
  Bool_t open = file->OpenFile(gSystem->ExpandPathName(filename));
  if(!open) {
    Error("AtachFile", "File %s not readable.", filename);
    //// delete file;
    return;
  }
  file->FileArgs().Set(filename, stop_at_end);
  attachOfflineThread.Run(reinterpret_cast<void*>(file));
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::AttachList                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::AttachList(const char* filename) {
  rb::Unattach();
  attachListThread.Run((void*)gSystem->ExpandPathName(filename));
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Unattach()                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Unattach() {
  if(BufferSource::IsAttached(ONLINE_)) {
    BufferSource::SetNotAttached(ONLINE_);
    attachOnlineThread.Join();
  }
  if(BufferSource::IsAttached(FILE_)) {
    BufferSource::SetNotAttached(FILE_);
    attachOfflineThread.Join();
  }
  if(BufferSource::IsAttached(LIST_)) {
    BufferSource::SetNotAttached(LIST_);
    attachListThread.Join();
  }
}
