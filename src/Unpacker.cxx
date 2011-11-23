/*! \file Unpacker.cxx
 *  \brief Implements data attaching/unpacking rotines.
 *  \note Much of the unpacking is user-defined, so this
 *  is really more of a skeleton or framework.
 */
#include <iostream>
#include <fstream>
#include <vector>
#include "TError.h"
#include "TSystem.h"

#include "Hist.hxx"
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


//// Class Unpacker ////

rb::Unpacker::Unpacker() :
  attachOnlineThread("attachOnline", rb::Unpacker::AttachOnline_),
  attachOfflineThread("attachOffline", rb::Unpacker::AttachFile_) {

  fTree = new TTree("t", "fTree");
  fTree->SetDirectory(0);

#define   INIT_BRANCHES
#include "Skeleton.hh"


  kAttachedOnline  = kFALSE;
  kAttachedOffline = kFALSE;
  offlineFileName  = "";
}

rb::Unpacker::~Unpacker() {
  Unattach();
  gSystem->Sleep(0.5e3);
  if(fTree) {
    fTree->ResetBranchAddresses();
    delete fTree;
  }
#define DELETION
#include "Skeleton.hh"
}

void* rb::Unpacker::AttachOnline_(void* arg) {
  rb::Unpacker* _this = (rb::Unpacker*)arg;
  _this->kAttachedOnline = kTRUE;

  /// For now just generate fake data buffers.
  while(_this->kAttachedOnline) {
    FakeBuffer(&(_this->fBuffer[0]));
    _this->UnpackBuffer();
  }
  return (void*)0;
}

void* rb::Unpacker::AttachFile_(void* arg) {
  rb::Unpacker* _this = (rb::Unpacker*)arg;
  _this->kAttachedOffline = kTRUE;

  ifstream ifs(_this->offlineFileName.c_str(), ios::in);
  if(!ifs.good()) {
    Error("AtachFile_", "File %s not readable.", _this->offlineFileName.c_str());
    ifs.close();
    _this->offlineFileName = "";
    return (void*)-1;
  }

  while(_this->kAttachedOffline) {
    ifs.read((Char_t*)(&_this->fBuffer[0]), BUFFER_SIZE * (sizeof(Short_t)/sizeof(Char_t)));
    if(!ifs.good()) break; // Done reading file.

    _this->UnpackBuffer();
  }
  ifs.close();

  if(_this->kAttachedOffline) {
    Info("AttachFile", "Done reading %s", _this->offlineFileName.c_str());
    _this->Unattach();
  }
  else Info("AttachFile", "Connection aborted by user");

  return (void*)0;
}

void rb::Unpacker::AttachOnline() {
  attachOnlineThread.Run((void*)this);
}

void rb::Unpacker::AttachFile(const char* filename) {
  offlineFileName = filename;
  attachOfflineThread.Run((void*)this);
}

void rb::Unpacker::Unattach() {
  if(kAttachedOnline) {
    kAttachedOnline = kFALSE;
    attachOnlineThread.Join();
  }
  if(kAttachedOffline) {
    kAttachedOffline = kFALSE;
    offlineFileName = "";
    attachOfflineThread.Join();
  }
}

// void rb::Unpacker::UnpackBuffer()
#define   UNPACK_ROUTINES
#include "Skeleton.hh"


void rb::Unpacker::FillHistograms() {
  for(UInt_t indx = 0; indx < Hist::GetNumber(); ++indx) {
    rb::Hist* pHist = Hist::Get(indx);
    if(pHist) pHist->Fill();
  }
}
