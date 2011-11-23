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

    TTree* fTree = 0;
    sData* myData = new sData();

    void Initialize() {
      fTree = new TTree("t", "fTree");
      fTree->SetDirectory(0);
#define   INIT_BRANCHES
#include "Skeleton.hh"
    }

    void Cleanup() {
      Unattach();
      gSystem->Sleep(0.5e3);
      if(fTree) {
	fTree->ResetBranchAddresses();
	delete fTree;
      }
#define DELETION
#include "Skeleton.hh"
    }



    void FillHistograms() {
      for(UInt_t indx = 0; indx < Hist::GetNumber(); ++indx) {
	rb::Hist* pHist = Hist::Get(indx);
	if(pHist) pHist->Fill();
      }
    }

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


void rb::unpack::AttachOnline() {
  Unattach();
  attachOnlineThread.Run();
}


void rb::unpack::AttachFile(const char* filename) {
  Unattach();
  attachOfflineThread.Run((void*)filename);
}


void rb::unpack::Unattach() {
  if(kAttachedOnline) {
    kAttachedOnline = kFALSE;
    attachOnlineThread.Join();
  }
  if(kAttachedFile) {
    kAttachedFile = kFALSE;
    attachOfflineThread.Join();
  }
}



    // void UnpackBuffer()
#define   UNPACK_ROUTINES
#include "Skeleton.hh"


