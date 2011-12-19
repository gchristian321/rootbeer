/*! \file Unpack.cxx
 *  \brief Implements data attaching/unpacking rotines.
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

namespace rb
{
  namespace unpack
  {
    /// The data buffer
    Short_t fBuffer[BUFFER_SIZE];

    /// 
    void* AttachOnline_(void* arg);
    void* AttachFile_(void* arg);
    void* AttachList_(void* arg);

    Bool_t kAttachedOnline = kFALSE;
    Bool_t kAttachedFile   = kFALSE;
    Bool_t kAttachedList   = kFALSE;

    TThread attachOnlineThread("attachOnline", AttachOnline_);
    TThread attachOfflineThread("attachFile", AttachFile_);
    TThread attachListThread("attachList", AttachList_);

    void UnpackBuffer();

    void* AttachOnline_(void* arg) {
      kAttachedOnline = kTRUE;

      /// For now just generate fake data buffers.
      while(kAttachedOnline) {
	FakeBuffer(&(fBuffer[0]));
	UnpackBuffer();
      }
      return arg;
    }

    void* AttachFile_(void* arg) {
      /// \note 'arg' Was casted from a \c new string before passing it here.
      /// We neeed to free the memory allocated to it with <tt>delete</tt>.

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

      while(kAttachedFile) {
	ifs.read((Char_t*)(&fBuffer[0]), BUFFER_SIZE * (sizeof(Short_t)/sizeof(Char_t)));
	if(!ifs.good()) { // At end of file
	  if(stopAtEnd) // We're done.
	    break;
	  else { // Wait 10 seconds for more data to come in.
	    ifs.clear(); 
	    gSystem->Sleep(10e3);
	    continue;
	  }
	}
	UnpackBuffer();
      }
      ifs.close();

      if(kAttachedFile) {
	if(!kAttachedList) {
	  Info("AttachFile", "Done reading %s", fileName.c_str());
	  Unattach();
	}
      }
      else Info("AttachFile", "Connection aborted by user.");

      return arg;
    }

    void* AttachList_(void * arg) {
      using namespace unpack;
      Unattach();
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
	AttachFile_(filearg);
	//	gSystem->Sleep(2e3);
      }
      ifs.close();
      kAttachedFile = kFALSE;
      Unattach();
      return arg;
    }
  } // namespace unpack
} // namespace rb


void rb::AttachOnline() {
  using namespace unpack;
  Unattach();
  attachOnlineThread.Run();
}


void rb::AttachFile(const char* filename, Bool_t stop_at_end) {
  using namespace unpack;
  Unattach();
  int stop = int(stop_at_end);
  stringstream arg;
  arg << stop << filename;
  string* sarg = new string(arg.str()); // delted in AttachFile_
  void * varg = (void*)(sarg);
  attachOfflineThread.Run(varg);
}

void rb::AttachList(const char* filename) {
  unpack::attachListThread.Run((void*)filename);
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
  if(kAttachedList) {
    kAttachedList = kFALSE;
    attachListThread.Join();
  }
}


///// User Defined Unpack() Routine //////
#include "Skeleton.hh"


