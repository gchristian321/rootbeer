//! \file Rootbeer.cxx 
//!  \brief Implements the user interface functions.
#include <iostream>
#include "Rootbeer.hxx"

#define RB_INIT
#include "ImportDataMacros.h"
rb::BufferSource* gBuffers = 0;



void rb::Logo() {
  std::cout << "                                                                 \n"
	    << "                            ___                                  \n"
	    << "                          .'   '.                                \n"
	    << "                         /       \\           oOoOo              \n"
	    << "                        |         |       ,==|||||               \n"
	    << "                         \\       /       _|| |||||              \n"
	    << "                          '.___.'    _.-'^|| |||||               \n"
	    << "                        __/_______.-'     '==HHHHH               \n"
	    << "                   _.-'` /                   \"\"\"\"\"          \n"
	    << "                .-'     /   oOoOo                                \n"
	    << "                `-._   / ,==|||||                                \n"
	    << "                    '-/._|| |||||                                \n"
	    << "                     /  ^|| |||||                                \n"
	    << "                    /    '==HHHHH                                \n"
	    << "                   /________\"\"\"\"\"                           \n"
	    << "                   `\\       `\\                                 \n"
	    << "                     \\        `\\   /                           \n"
	    << "                      \\         `\\/                            \n"
	    << "                      /                                          \n"
	    << "                     /                                           \n"
	    << "                    /_____                                       \n"
	    << "                                                                 \n"
	    << "      Welcome to ROOT BEER, the ROOT Basic Event ExtractoR       \n"
	    << "                                                                 \n"
    ;
}



// class rb::Rint //

rb::Rint::Rint(const char* appClassName, int* argc, char** argv,
	       void* options, int numOptions, Bool_t noLogo) :
  TRint(appClassName, argc, argv, options, numOptions, noLogo) {
  SetPrompt("rootbeer [%d] ");

#define RB_ALLOCATE
#include "ImportDataMacros.h"

}


// Terminate function
void rb::Rint::Terminate(Int_t status) {
  rb::Canvas::StopUpdate();
  rb::Unattach();
  gSystem->Sleep(0.5e3);

#define RB_DEALLOCATE
#include "ImportDataMacros.h"

  rb::Hist::DeleteAll();
  TRint::Terminate(status);
}



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Internal helper functions                             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace
{
  //! Stop a running thread, used in rb::Unattach().
  inline void StopThread(const char* name) {
    if(!TThread::GetThread(name)) return; // Thread not running, nothing to do
    rb::Thread::Stop(name); // Send signal to stop the thread.
    while (TThread::GetThread(name)) ; // Wait until thread finishes executing
  }
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Public interface (Rootbeer.hxx) implementations       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::AttachOnline                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::AttachOnline(const char* host, const char* other, char** others, int nothers) {
#ifdef MIDAS_BUFFERS
#ifdef MIDAS_ONLINE // Attach to online midas files
  if(others || nothers)
    Warning("AttachOnline", "Arguments \'others\' and \'n_others\' are unused");

  rb::attach::Online::CreateAndRun(host, other, others, nothers);

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
  rb::attach::File::CreateAndRun(filename, stop_at_end);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::AttachList                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::AttachList(const char* filename) {
  Error("AttachList", "Implement!!");
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Unattach()                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Unattach() {
  rb::Thread::Stop("AttachFile");  
  rb::Thread::Stop("AttachOnline");
  // StopThread("AttachFile");
  // StopThread("AttachOnline");
}

