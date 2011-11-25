/*! \file Unpacker.cxx
 *  \brief Implements data attaching/unpacking rotines.
 *  \note Much of the unpacking is user-defined, so this
 *  is really more of a skeleton or framework.
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "TError.h"
#include "TSystem.h"
#include "TTree.h"
#include "TThread.h"
#include "TRandom3.h"

#include "Hist.hxx"
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


void FakeBuffer(Short_t* buf, Int_t dataRate = 0) {

  Int_t posn = 1, nEvts = 0;

  while(posn< 4080) {
    FakeEvent(&buf[posn], dataRate); nEvts++; posn += buf[posn];
  }
  buf[0] = nEvts;
}

/// Buffer size
/// \todo Make this easier to set for users.
static const Int_t BUFFER_SIZE = 4096;

/// Implementation of the statuc map of UserDataABC pointers.
UserDataABC::Map_t UserDataABC::Map;


namespace rb
{
  namespace unpack
  {
    /// The data buffer
    Short_t fBuffer[BUFFER_SIZE];

    /// 
    void* AttachOnline_(void* arg);
    void* AttachFile_(void* arg);

    Bool_t kAttachedOnline = kFALSE;
    Bool_t kAttachedFile = kFALSE;

    TThread attachOnlineThread("attachOnline", AttachOnline_);
    TThread attachOfflineThread("attachFile", AttachFile_);

    void UnpackBuffer();

    void* AttachOnline_(void* arg) {
      kAttachedOnline = kTRUE;

      /// For now just generate fake data buffers.
      while(kAttachedOnline) {
	FakeBuffer(&(fBuffer[0]));
	UnpackBuffer();
      }
      return (void*)0;
    }

    void* AttachFile_(void* arg) {
      kAttachedFile = kTRUE;

      std::string* fileName = static_cast<std::string*>(arg);
      ifstream ifs(fileName->c_str(), ios::in);

      if(!ifs.good()) {
	Error("AtachFile_", "File %s not readable.", fileName->c_str());
	ifs.close();
	return (void*)-1;
      }

      while(kAttachedFile) {
	ifs.read((Char_t*)(&fBuffer[0]), BUFFER_SIZE * (sizeof(Short_t)/sizeof(Char_t)));
	if(!ifs.good()) break; // Done reading file.

	UnpackBuffer();
      }
      ifs.close();

      if(kAttachedFile) {
	Info("AttachFile", "Done reading %s", fileName->c_str());
	Unattach();
      }
      else Info("AttachFile", "Connection aborted by user");

      return (void*)0;
    }
  }
}


void rb::AttachOnline() {
  using namespace unpack;
  Unattach();
  attachOnlineThread.Run();
}


void rb::AttachFile(const char* filename) {
  using namespace unpack;
  Unattach();
  attachOfflineThread.Run((void*)filename);
}


void rb::Unattach() {
  using namespace unpack;
  if(kAttachedOnline) {
    kAttachedOnline = kFALSE;
    attachOnlineThread.Join();
  }
  if(kAttachedFile) {
    kAttachedFile = kFALSE;
    attachOfflineThread.Join();
  }
}



///// User Defined Unpack() Routine //////
#include "Skeleton.hh"


