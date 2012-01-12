//! \file Attach.cxx
//! \brief Implements routines relevant to attaching to data sources.
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "TError.h"
#include "TSystem.h"
#include "TTree.h"
#include "TThread.h"
#include "TRandom3.h"

#include "Data.hxx"
#include "Rootbeer.hxx"
using namespace std;


void FakeEvent(Short_t* buf, Int_t dataRate = 0) {
  static Bool_t grInit(0);
  if(!grInit) {
    gRandom = new TRandom3(0);
    grInit = 1;
  }

  Double_t sleepTime = dataRate ? (1./dataRate)*1e3 : 0;

  Int_t nWords = 0;
  Short_t *p = buf, a, b, c;

  gSystem->Sleep(sleepTime);

  Double_t rabc = gRandom->Uniform(0,1);

  a = Short_t(gRandom->Gaus(20, 10));
  b = Short_t(gRandom->Gaus(30, 5));
  c = Short_t(gRandom->Gaus(50, 20));

  if(a > 0)             { *(p+1) = 1; *(p+2) = a; nWords += 2; }
  if(rabc< .9 && b > 0) { *(p+3) = 2; *(p+4) = b; nWords += 2; }
  if(rabc< .4 && c > 0) { *(p+5) = 3; *(p+6) = c; nWords += 2; }
  *p = nWords + 1;
}


void FakeBuffer(iostream& ifs,  Int_t dataRate = 0) {

  Int_t posn = 1, nEvts = 0;
  Short_t buf[4096];

  while(posn< 4080) {
    FakeEvent(&buf[posn], dataRate); nEvts++; posn += buf[posn];
  }
  buf[0] = nEvts;
  ifs.write((Char_t*)&buf[0], posn * sizeof(Short_t) / sizeof(Char_t));
}


//! Anonomyous namespace enclosing local (file-scope) functions and variables.
namespace
{
  //! Tells whether we're attached to an online source or not.
  Bool_t kAttachedOnline = kFALSE;

  //! Tells whether we're attached to an offline file or not.
  Bool_t kAttachedFile   = kFALSE;

  //! Tells whether we're attached to a list of files or not.
  Bool_t kAttachedList   = kFALSE;


  //! Attaches to an online data source (file), in the format expected by TThread.
  void* AttachOnline(void* arg) {
    kAttachedOnline = kTRUE;

    /// For now just generate fake data buffers.
    stringstream ifs;
    rb::Buffer buf;
    while(kAttachedOnline) {
      ifs.str("");
      FakeBuffer(ifs);
      rb::unpack::ReadBuffer(ifs, buf);
      rb::unpack::UnpackBuffer(buf);
    }
    return arg;
  }

  //! Attaches to an offline data source (file), in the format expected by TThread.
  //! \note 'arg' was casted from a \c new string before passing it here.
  //! We neeed to free the memory allocated to it with <tt>delete</tt>.
 void* AttachFile(void* arg) {

    kAttachedFile = kTRUE;
    string* sarg = static_cast<std::string*>(arg);
    // Decide whether to stop reading at eof or not from the first character.
    std::stringstream ss_stop;
    ss_stop << sarg->substr(0, 1);
    Bool_t stopAtEnd;
    ss_stop >> stopAtEnd;
    // Set filename from the rest.
    std::string fileName = sarg->substr(1);
    delete sarg;

    ifstream ifs(fileName.c_str(), ios::in);
    if(!ifs.good()) {
      Error("AtachFile_", "File %s not readable.", fileName.c_str());
      ifs.close();
      return 0;
    }

    rb::Buffer buf;
    while(kAttachedFile) {
      rb::unpack::ReadBuffer(ifs, buf);
      if(!ifs.good()) { // At end of file
	if(stopAtEnd) // We're done.
	  break;
	else { // Wait 10 seconds for more data to come in.
	  ifs.clear();
	  gSystem->Sleep(10e3);
	  continue;
	}
      }
      rb::unpack::UnpackBuffer(buf);
    }
    ifs.close();

    if(kAttachedFile) {
      if(!kAttachedList) {
	Info("::AttachFile", "Done reading %s", fileName.c_str());
	rb::Unattach();
      }
    }
    else Info("::AttachFile", "Connection aborted by user.");

    return arg;
  }

  //! Attaches to a list of offline files, in the format expected by TThread.
  void* AttachList(void * arg) {
    rb::Unattach();
    kAttachedList = kTRUE;

    char* filename = (char*)arg;
    ifstream ifs(filename);
    if(!ifs.good()) {
      Error("AttachList", "%s not found.", filename);
      return 0;
    }
    string line;
    while(1) {
      getline(ifs, line);
      if(!ifs.good()) break;

      line = line.substr(0, line.find("#"));
      string* str = new string("1");
      for(UInt_t i=0; i< line.size(); ++i) {
	if(line[i] != ' ') str->push_back(line[i]);
      }
      if(str->size() == 1) continue;
      ifstream ifstest(str->substr(1).c_str());
      if(!ifstest.good()) {
	Info("AttachList", "The file %s wasn't found. Moving on to the next one.", str->c_str());
	continue;
      }
      ifstest.close();
      void * filearg = (void*)str;
      ::AttachFile(filearg);
    }
    ifs.close();
    kAttachedFile = kFALSE;
    rb::Unattach();
    return arg;
  }

  //! Thread for attaching to online data.
  TThread attachOnlineThread("attachOnline", ::AttachOnline);

  //! Thread for attaching to offline data.
  TThread attachOfflineThread("attachFile", ::AttachFile);

  //! Thread for attaching to a list of offline files.
  TThread attachListThread("attachList", ::AttachList);
}
// namespace //





//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::AttachOnline                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::AttachOnline() {
  rb::Unattach();
  ::attachOnlineThread.Run();
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::AttachFile                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::AttachFile(const char* filename, Bool_t stop_at_end) {
  rb::Unattach();
  int stop = int(stop_at_end);
  stringstream arg;
  arg << stop << filename;
  string* sarg = new string(arg.str()); // delted in ::AttachFile
  void * varg = (void*)(sarg);
  ::attachOfflineThread.Run(varg);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::AttachList                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::AttachList(const char* filename) {
  ::attachListThread.Run((void*)filename);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Unattach()                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Unattach() {
  if(::kAttachedOnline) {
    ::kAttachedOnline = kFALSE;
    ::attachOnlineThread.Join();
  }
  if(::kAttachedFile) {
    ::kAttachedFile = kFALSE;
    ::attachOfflineThread.Join();
  }
  if(::kAttachedList) {
    ::kAttachedList = kFALSE;
    ::attachListThread.Join();
  }
}

