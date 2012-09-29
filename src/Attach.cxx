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
#ifdef HAVE_MIDAS
#include "midas/TMidasOnline.h"
#endif
using namespace std;


#error ("I am no longer in use!")

//! Anonomyous namespace enclosing local (file-scope) functions and variables.
namespace
{

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Local constants                                       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

  //! Tells whether we're attached to an online source or not.
  Bool_t kAttachedOnline = kFALSE;

  //! Tells whether we're attached to an offline file or not.
  Bool_t kAttachedFile   = kFALSE;

  //! Tells whether we're attached to a list of files or not.
  Bool_t kAttachedList   = kFALSE;


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Online MIDAS Routines                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
#ifdef HAVE_MIDAS
  //! Encloses internal functions used to attach to online MIDAS data.
  namespace midas
  {
    //! Wrapper for online arguments, because we need to cast from void*
    struct OnlineArgs {
      string host;
      string expt;
    };

    //! Called when a new run starts
    void run_start(int transition, int run_number, int trans_time) {
      cout << "Beginning MIDAS run number " << run_number << ".\n";
    }

    //! Called when a run stops
    void run_stop(int transition, int run_number, int trans_time) {
      cout << "Ending MIDAS run number " << run_number << ".\n";
    }

    //! Called when we pause a run
    void run_pause(int transition, int run_number, int trans_time) {
      cout << "Pausing run number " << run_number << ".\n";
    }

    //! Called when we resume a run
    void run_resume(int transition, int run_number, int trans_time) {
      cout << "Resuming run number " << run_number << ".\n";
    }

    //! Listens for online buffers and calls unpacking routines.
    //! \bug Assertion on line 1494 of odb.c failed while attached online.
    //! Assertion has to do with checking the number of thread locks.
    int attach_online(const string& hostname, const string& exptname) {
      TMidasOnline* onlineMidas = TMidasOnline::instance();
      int err = onlineMidas->connect(hostname.c_str(), exptname.c_str(), "rootbeer");
      if (err) {
	Error("AttachOnline", "Cannot connect to MIDAS, error %d", err);
	return -1;
      }
      onlineMidas->setTransitionHandlers(midas::run_start, midas::run_stop, midas::run_pause, midas::run_resume);
      onlineMidas->registerTransitions();
      int req = onlineMidas->eventRequest("SYSTEM",-1,-1,(1<<1));

      stringstream ifs;
      BUFFER_TYPE buf;
      while(kAttachedOnline) {
	char pevent[100*1024];
	int size = onlineMidas->receiveEvent(req, pevent, sizeof(pevent), true);
	if (size > 0) { // Got a valid event, unpack it.
	  ifs.str("");
	  buf.Clear();
	  ifs.read((char*)pevent, size);
	  rb::unpack::ReadBuffer(ifs, buf);
	  rb::unpack::UnpackBuffer(buf);
	}
	else if (size == 0) { // Waiting for an event
	  if (!onlineMidas->poll(1000)) break; // Polling failed, exit (error thrown in TMidasOnline)
	  else continue;
	}
	else { // (size < 0)
	  Error("AttachOnline", "onlineMidas->receiveEvent return val < 0.\n");
	  break;
	}
      }

      onlineMidas->disconnect();
      return 0;
    }
  }
#endif // #ifdef HAVE_MIDAS


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// TThread functions for attaching to data               //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

  //! \brief Attaches to an online data source, in the format expected by TThread.
  /// \details \bug (Midas online) assertion on line 1494 of odb.c failed while attached online.
  /// Assertion has to do with checking the number of thread locks.
   void* AttachOnline(void* arg) {
#ifdef HAVE_MIDAS
    midas::OnlineArgs* midasArgs = reinterpret_cast<midas::OnlineArgs*>(arg);
    string hostname = midasArgs->host, exptname = midasArgs->expt;
    delete midasArgs;
    kAttachedOnline = kTRUE;
    int mAttach = midas::attach_online(hostname, exptname);
    return (!mAttach) ? arg : 0; 
#else
    kAttachedOnline = kTRUE;
    return arg;
#endif
  }

  //! \brief Attaches to an offline data source (file), in the format expected by TThread.
  /// \details \note 'arg' was casted from a \c new string before passing it here.
  /// We neeed to free the memory allocated to it with <tt>delete</tt>.
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
void rb::AttachOnline(const char* host, const char* expt, const vector<string>* others) {
#ifdef _MIDAS_

#ifdef HAVE_MIDAS // Attach to online midas files
  if(others) Warning("AttachOnline", "Argument: const vector<string>* others unused.");
  midas::OnlineArgs* arg = new midas::OnlineArgs(); // deleted in ::AttachOnline
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
  ::attachOfflineThread.Run(reinterpret_cast<void*>(sarg));
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

