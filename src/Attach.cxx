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
#include "midas/TMidasOnline.h"
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


  //! Attaches to an online data source, in the format expected by TThread.
#ifdef HAVE_MIDAS
 struct MidasOnline {
  string host;
  string expt;
};
#endif

  void* AttachOnline(void* arg) {
#ifdef HAVE_MIDAS

 MidasOnline* midasArgs = reinterpret_cast<MidasOnline*>(arg);
 string hostname = midasArgs->host, exptname = midasArgs->expt;
 delete midasArgs;

 TMidasOnline *midas = TMidasOnline::instance();

 int err = midas->connect(hostname.c_str(), exptname.c_str(), "rootbeer");
 if (err != 0)
   {
     fprintf(stderr,"Cannot connect to MIDAS, error %d\n", err);
     return 0;
   }

 // gOdb = midas;

 // midas->setTransitionHandlers(startRun,endRun,NULL,NULL);
 // midas->registerTransitions();

 /* reqister event requests */

 // midas->setEventHandler(eventHandler);
 midas->eventRequest("SYSTEM",-1,-1,(1<<1));

 /* fill present run parameters */

 // gRunNumber = gOdb->odbReadInt("/runinfo/Run number");

 // if ((gOdb->odbReadInt("/runinfo/State") == 3))
 //   startRun(0,gRunNumber,0);

 // printf("Startup: run %d, is running: %d, is pedestals run: %d\n",gRunNumber,gIsRunning,gIsPedestalsRun);

 // MyPeriodic tm(100,MidasPollHandler);
 //MyPeriodic th(1000,SISperiodic);
 //MyPeriodic tn(1000,StepThroughSISBuffer);
 //MyPeriodic to(1000,Scalerperiodic);

 /*---- start main loop ----*/

 // loop_online();
 // app->Run(kTRUE);

 /* disconnect from experiment */
 //  midas->disconnect();

 return arg;

#else
    kAttachedOnline = kTRUE;

    /// For now just generate fake data buffers.
    stringstream ifs;
    BUFFER_TYPE buf;
    while(kAttachedOnline) {
      ifs.str("");
      FakeBuffer(ifs);
      if(rb::unpack::ReadBuffer(ifs, buf))
	rb::unpack::UnpackBuffer(buf);
    }
    return arg;
#endif
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

    BUFFER_TYPE buf;
    while(kAttachedFile) {
      bool read_success = rb::unpack::ReadBuffer(ifs, buf);
      if(!read_success) { // At end of file
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





//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::AttachOnline                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::AttachOnline(const char* host, const char* expt, const vector<string>* others) {
#ifdef _MIDAS_

#ifdef HAVE_MIDAS // Attach to online midas files
  if(others) Warning("AttachOnline", "Argument: const vector<string>* others unused");
  MidasOnline* arg = new MidasOnline();
  arg->host = host;
  arg->expt = expt;
  ::attachOnlineThread.Run(reinterpret_cast<void*>(arg));
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
  int stop = int(stop_at_end);
  stringstream arg;
  arg << stop << gSystem->ExpandPathName(filename);
  string* sarg = new string(arg.str()); // delted in ::AttachFile
  void * varg = (void*)(sarg);
  ::attachOfflineThread.Run(varg);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::AttachList                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::AttachList(const char* filename) {
  ::attachListThread.Run((void*)gSystem->ExpandPathName(filename));
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

