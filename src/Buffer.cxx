//! \file Buffer.cxx
//! \brief Implements rb::BufferSource
#include <iostream>
#include <fstream>
#include <string>
#include <TSystem.h>
#include <TThread.h>
#include <TError.h>
#include "Buffer.hxx"
#include "Rootbeer.hxx"




// rb::Connector::Connector(/*BufferSource* buf*/) : fRun(kTRUE) {
//   fBuffer = BufferSource::GetFromUser();
//   fThread = &fBuffer->fThread;
// }

// rb::Connector::~Connector() {
//   fRun = kFALSE;
//   fThread->Join();
//   delete fBuffer;
// }

// rb::File::File(const char* filename, Bool_t stopAtEnd/*, BufferSource* buf*/) :
//   Connector(/*buf*/), kFileName(filename), kStopAtEnd(stopAtEnd) {};

// void rb::File::Attach() { Info("Attach","");
//   while(fRun) {
//     bool read_success = fBuffer->ReadBufferOffline();
//     if (read_success) fBuffer->UnpackBuffer(); // got an event
//     else if (kStopAtEnd) break; // we're done
//     else gSystem->Sleep(10e3); // Wait 10 seconds for more data to come in 
//   }

//   // if(BufferSource::IsAttached(FILE_)) {
//   //   if(!BufferSource::IsAttached(LIST_)) {
//   if(fRun) {
//       Info("AttachFile", "Done reading %s", kFileName.c_str());
//       // Unattach();
//     // }
//   }
//   else Info("AttachFile", "Connection aborted by user.");
//   fBuffer->CloseFile();
//   delete this;
// }

// rb::Online::Online(const char* source, const char* other, char** others, int nothers, rb::BufferSource* buf) :		   
//   Connector(/*buf*/), fSourceArg (source), fOtherArg(other), fOtherArgs(others), fNumOthers(nothers) {}

// void rb::Online::Attach() {
//   Bool_t connected = fBuffer->ConnectOnline(fSourceArg, fOtherArg, fOtherArgs, fNumOthers);
//   if (!connected) return;
//   while (fRun) {
//     Bool_t readSuccess = fBuffer->ReadBufferOnline();
//     if(!readSuccess) break;
//     fBuffer->UnpackBuffer();
//   }
//   fBuffer->DisconnectOnline();
// }


// #if 0
// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// // TThread functions for attaching to data               //
// //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

// void * rb::BufferSource::AttachFunction (void * arg) {
//   BufferSource* fBuf = reinterpret_cast<BufferSource*>(arg);
//   int type = fBuf->fType;
//   switch(type) {
//   case FILE_:
//     {
//       BufferSource* pFile = fBuf;
//       BufferSource::SetAttached(FILE_);

//       while(BufferSource::IsAttached(FILE_)) {
// 	bool read_success = pFile->ReadBufferOffline();
// 	if (read_success) pFile->UnpackBuffer(); // got an event
// 	else if (pFile->FileArgs().stopEnd) break; // we're done
// 	else gSystem->Sleep(10e3); // Wait 10 seconds for more data to come in 
//       }

//       if(BufferSource::IsAttached(FILE_)) {
// 	if(!BufferSource::IsAttached(LIST_)) {
// 	  Info("AttachFile", "Done reading %s", pFile->FileArgs().fileName.c_str());
// 	  Unattach();
// 	}
//       }
//       else Info("AttachFile", "Connection aborted by user.");
	
//       break;
//     }
//   case ONLINE_:
//     {
//       BufferSource* pOnline = fBuf;
//       BufferSource::SetAttached(ONLINE_);

//       Bool_t connected =
// 	pOnline->ConnectOnline(pOnline->OnlineArgs().source.c_str(),
// 			       pOnline->OnlineArgs().other.c_str(),
// 			       pOnline->OnlineArgs().others,
// 			       pOnline->OnlineArgs().nothers);
//       if (!connected) return 0;

//       while (BufferSource::IsAttached(ONLINE_)) {
// 	Bool_t readSuccess = pOnline->ReadBufferOnline();
// 	if(!readSuccess) break;
// 	pOnline->UnpackBuffer();
//       }
//       return 0;

//       break;
//     }
//   default:
//     break;
//   }
// }


// void* rb::BufferSource::AttachOnline_(void* arg) {
//   BufferSource* pOnline = reinterpret_cast<BufferSource*> (arg);
//   BufferSource::SetAttached(ONLINE_);

//   Bool_t connected =
//     pOnline->ConnectOnline(pOnline->OnlineArgs().source.c_str(),
// 			   pOnline->OnlineArgs().other.c_str(),
// 			   pOnline->OnlineArgs().others,
// 			   pOnline->OnlineArgs().nothers);
//   if (!connected) return 0;

//   while (BufferSource::IsAttached(ONLINE_)) {
//     Bool_t readSuccess = pOnline->ReadBufferOnline();
//     if(!readSuccess) break;
//     pOnline->UnpackBuffer();
//   }
//   return 0;
// }

// void* rb::BufferSource::AttachFile_(void* arg) {
//   BufferSource* pFile = reinterpret_cast<BufferSource*> (arg);
//   BufferSource::SetAttached(FILE_);

//   while(BufferSource::IsAttached(FILE_)) {
//     bool read_success = pFile->ReadBufferOffline();
//     if (read_success) pFile->UnpackBuffer(); // got an event
//     else if (pFile->FileArgs().stopEnd) break; // we're done
//     else gSystem->Sleep(10e3); // Wait 10 seconds for more data to come in 
//   }

//   if(BufferSource::IsAttached(FILE_)) {
//     if(!BufferSource::IsAttached(LIST_)) {
//       Info("AttachFile", "Done reading %s", pFile->FileArgs().fileName.c_str());
//       Unattach();
//     }
//   }
//   else Info("AttachFile", "Connection aborted by user.");
//   return arg;
// }

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
// #endif  /////////////////



rb::BufferSource::BufferSource() {}
//   fThread("AttachThread", Connector::RunThread) {
// }

// void rb::BufferSource::StartConnection(rb::Connector* new_) {
//   BufferSource::Unattach();
//   fConnector.Start(new_);
//   fConnector()->Run();
// }  

void rb::BufferSource::Unattach() {
  //  fConnector.Stop();
}

// void rb::BufferSource::RunOnline(const char* host, const char*  other, char** others, Int_t nothers) {

// #ifdef MIDAS_BUFFERS
// #ifdef MIDAS_ONLINE // Attach to online midas files
//   if(others || nothers) Warning("AttachOnline", "Arguments \'others\' and \'n_others\' are unused");
//   StartConnection(new Online(host, other, others, nothers, this));
// #else
//   Info("AttachOnline",
//        "MIDAS was not found on your system; cannot attach to online MIDAS data.\n"
//        "Instructions for installing MIDAS can be found online at:\n"
//        "      http://daq-plone.triumf.ca/SM/docs/local/installmidas.html\n\n"
//        "Please note that you will need to have the MIDASSYS environment variable\n"
//        "defined in order to attach to online data with rootbeer.\n");
// #endif

// #elif defined _NSCL_
//   Info("AttachOnline",
//        "Online attachment to NSCL data is not yet implemented.");
// #else
//   Info("AttachOnline",
//        "Attaching to online buffers other than MIDAS and NSCL is not yet part of\n"
//        "stock ROOTBEER. You'll have to define it yourself. Once you've done this,\n"
//        "pleae contact the developers (Greg Christian, gchristian@triumf.ca) about\n"
//        "adding it to the source code.\n");
// #endif
// }

// void rb::BufferSource::RunFile(const char* filename, Bool_t stopAtEnd) {
//   Unattach();
//   Bool_t open = this->OpenFile(gSystem->ExpandPathName(filename));
//   if(!open) {
//     Error("AtachFile", "File %s not readable.", filename);
//     return;
//   }
//   //  StartConnection(new File(filename, stopAtEnd, this));
//   Unattach();
//   File * c = new File(filename, stopAtEnd);
//   c->Run();
  
// }

// void rb::BufferSource::RunList(const char* filename) {
//   Error("RunList", "Needs to be implemented");
// }








/////// GOES IN ROOTBEER.CXX //////////

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Public interface (Rootbeer.hxx) implementations       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::AttachOnline                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::AttachOnline(const char* host, const char* other, char** others, int nothers) {
#ifdef MIDAS_BUFFERS
#ifdef MIDAS_ONLINE // Attach to online midas files
  if(others || nothers) Warning("AttachOnline", "Arguments \'others\' and \'n_others\' are unused");
  ThreadOnline * o = new ThreadOnline(host, other, others, nothers);
  o->Run();
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
  ThreadFile * f = new ThreadFile(filename, stop_at_end);
  f->Run(); 
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::AttachList                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::AttachList(const char* filename) {
  Error("AttachList", "Implement!!");
  //  fConnector = new List ...


  // rb::Unattach();
  // gBuffers->RunList(filename); //attachListThread.Run((void*)gSystem->ExpandPathName(filename);)
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Unattach()                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Unattach() {
  //  gBuffers->Unattach();
}


rb::ThreadFile::ThreadFile(const char* filename, Bool_t stopAtEnd) :
  ThreadExecutor("AttachFile"),
  kFileName(gSystem->ExpandPathName(filename)),
  kStopAtEnd(stopAtEnd) {

  fBuffer = BufferSource::GetFromUser(); //new Midas();
}

rb::ThreadFile::~ThreadFile()  { delete fBuffer; }

void rb::ThreadFile::DoInThread() {
  Bool_t open = fBuffer->OpenFile(kFileName);
  if(!open) {
    Error("AtachFile", "File %s not readable.", kFileName);
    return;
  }
  //  StartConnection(new File(filename, stopAtEnd, this));

  while(1) {
    bool read_success = fBuffer->ReadBufferOffline();
    if (read_success) fBuffer->UnpackBuffer(); // got an event
    else if (kStopAtEnd) break; // we're done
    else gSystem->Sleep(10e3); // Wait 10 seconds for more data to come in 
  }

  // if(BufferSource::IsAttached(FILE_)) {
  //   if(!BufferSource::IsAttached(LIST_)) {
  if(1) {
    Info("AttachFile", "Done reading %s", kFileName);
    // Unattach();
    // }
  }
  else Info("AttachFile", "Connection aborted by user.");
  fBuffer->CloseFile();
}


rb::ThreadOnline::ThreadOnline(const char* source, const char* other, char** others, int nothers) :
  ThreadExecutor("AttachOnline"),
  fSourceArg(source),
  fOtherArg(other),
  fOtherArgs(others),
  fNumOthers(nothers) {

  fBuffer = BufferSource::GetFromUser();  
}


rb::ThreadOnline::~ThreadOnline() {
  delete fBuffer;
}
    
void rb::ThreadOnline::DoInThread() {
  Bool_t connected = fBuffer->ConnectOnline(fSourceArg, fOtherArg, fOtherArgs, fNumOthers);
  if (!connected) return;
  while (1) {
    Bool_t readSuccess = fBuffer->ReadBufferOnline();
    if(!readSuccess) break;
    fBuffer->UnpackBuffer();
  }
  fBuffer->DisconnectOnline();
}
