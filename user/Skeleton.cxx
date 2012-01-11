//! \file Skeleton.cxx
//! \brief This is where users plug their analysis codes into ROOTBEER.
#include <assert.h>
#include <vector>
#include <iostream>
#include "Data.hxx"
#include "Rootbeer.hxx"


/// Macro to add a class instance to ROOTBEER
#define ADD_CLASS_INSTANCE(NAME, CLASS_NAME, CREATE_POINTER)		\
  rb::Data* NAME##_Data = rb::Data::New<CLASS_NAME>(#NAME, #CLASS_NAME, CREATE_POINTER);

/// Same as ADD_CLASS_INSTANCE, but for non-default constructors
#define ADD_CLASS_INSTANCE_ARGS(NAME, CLASS_NAME, CREATE_POINTER, ARGS)	\
  rb::Data* NAME##_Data = rb::Data::New<CLASS_NAME>(#NAME, #CLASS_NAME, CREATE_POINTER, ARGS);

/// Macro to get a mutex-ocking pointer to data objects.
#define GET_LOCKING_POINTER(SYMBOL, NAME, CLASS)			\
  LockingPointer<CLASS> SYMBOL (NAME##_Data->GetDataPointer<CLASS>(), NAME##_Data->fMutex);




//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//                                   //
// ROOTBEER USER DATA CODE SKELETON  //
//                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//\\ Add the header includes for your classes here. //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

#include "ExampleData.hh"

#include "mona.hh"
#include "unpacker.hh"
#include "mona_definitions.hh"

#include <TTimeStamp.h>
#include "midas/rbMidasEvent.h"
#include "Dragon.hxx"



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//\\ Define instances of your class here    \\//
//\\ (using the ADD_CLASS_INSTANCE macros). \\//
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

// Here we add an instance of ExampleData, called myData and do not allow viewing in CINT.
ADD_CLASS_INSTANCE(myData, ExampleData, kFALSE)

// Here we add an instance of sVariables, calles myVars and do allow viewing in CINT.
// Also, we use a non-default constructor, with the argument 32.
ADD_CLASS_INSTANCE_ARGS(myVars, ExampleVariables,  kTRUE, "32")

ADD_CLASS_INSTANCE(mcal, cal::mona, kFALSE);
ADD_CLASS_INSTANCE(mraw, raw::mona, kFALSE);
ADD_CLASS_INSTANCE(mvar, var::mona, kTRUE);

ADD_CLASS_INSTANCE(ts, TimeStamp, kFALSE);
ADD_CLASS_INSTANCE(dragon, data::Dragon, kFALSE);
ADD_CLASS_INSTANCE(vdragon, variables::Dragon, kTRUE);


namespace rb {
  namespace unpack {


    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
    //\\ Define the type of data stored in your buffers here.           \\//
    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
    typedef UShort_t DataType; // Example: the buffers store 16-bit words.


    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
    //\\ Actual buffer declaration.                  \\//
    //\\ Likely you won't want to touch this.        \\//
    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
    std::vector<DataType> gBuffer; //\\\\\\\\\\\\\\\\\\//
    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//



    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
    //\\ Here you should define how to process your data buffers        \\//
    //\\ by implementing the ReadBuffer() and UnpackBuffer() functions. \\//
    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
    void ReadBuffer(istream& ifs) {

#define _MIDAS_ // Uncomment to use pre-defined MIDAS buffer extraction routines.
// #define _NSCL_  // Uncomment to use pre-defined NSCL buffer extraction routines.


#ifdef _MIDAS_
      // Stock MIDAS buffer extraction.
      // Midas buffers are not a fixed size, so we must first check the header to
      // get the appropriate length.
      // Midas headers are structured as follows:
      // fEventId      [16-bit]
      // fTriggerMask  [16-bit]
      // fSerialNumber [32-bit]
      // fTimeStamp    [32-bit]
      // fDataSize     [32-bit]
      // All we care about here is fDataSize, so we'll just read into an
      // array of four 32-bit words.
      const Int_t headerSize = 2*sizeof(Short_t) + 3*sizeof(Int_t);
      const Int_t headerLength32 = headerSize / sizeof(Int_t);
      const Int_t dataPosn = 3;
      Int_t eventHeader[headerLength32];
      ifs.read(reinterpret_cast<Char_t*>(eventHeader), headerLength32 * sizeof(Int_t));
      if(!ifs.good()) return; // EOF

      // Resize gBuffer appropriately.
      Int_t dataSize = eventHeader[dataPosn]; /// \todo Do we need to byte-swap???
      Int_t totalWords = (headerSize + dataSize) / sizeof(DataType);
      gBuffer.resize(totalWords);

      // Read header into gBuffer
      memcpy(reinterpret_cast<void*>(&gBuffer[0]), reinterpret_cast<void*>(eventHeader), headerSize);

      // Read data into gBuffer
      const Int_t firstDataWord = headerSize / sizeof(DataType);
      ifs.read(reinterpret_cast<Char_t*>(&gBuffer[firstDataWord]), dataSize);

      // Bool_t readSuccess = gMidasEvent.ReadEvent(&ifs);
      // if(!readSuccess) return;
      // Int_t nWords = gMidasEvent.GetDataSize() / sizeof(DataType);
      // gBuffer.resize(nWords);
      // memcpy(reinterpret_cast<void*>(&gBuffer[0]),
      // 	     reinterpret_cast<void*>(gMidasEvent.GetData()),
      // 	     gMidasEvent.GetDataSize());

#elif defined _NSCL_
      // Stock NSCL buffer extraction
      // Each buffer is jst a fixed size, 4096 16-bit words.
      const Int_t bufferSize = 4096;
      if(gBuffer.size() != bufferSize) gBuffer.resize(bufferSize);
      Char_t* firstWord = reinterpret_cast<Char_t*>(&gBuffer[0]);
      Int_t bufferSize = bufferSize * sizeof(DataType);
      ifs.read(firstWord, bufferSize);

#else
      std::cerr << "ERROR: rb::Unpack::ReadBuffer not defined.\n";
      exit(1);
#endif
    }

    void UnpackBuffer() {

#ifdef _MIDAS_
      rb::MidasEvent midasEvent;
      if ( ! midasEvent.ReadEvent(gBuffer) ) return;

      GET_LOCKING_POINTER(pTS, ts, TimeStamp);
      pTS->time = midasEvent.GetTimeStamp();

      GET_LOCKING_POINTER(pDragon, dragon, data::Dragon);
      GET_LOCKING_POINTER(pDragonVars, vdragon, variables::Dragon);

      int16_t eventId = midasEvent.GetEventId();
      bool unpackResult;
      switch(eventId) {
      case EV_DRAGON_G:
      case EV_DRAGON_H:
	pDragon->Reset();
	unpackResult = pDragon->Unpack(midasEvent, *pDragonVars);
	rb::Hist::FillAll();
	break;
      default:
	break;
      }

#else
      static const UInt_t EVENT_BUFFER = 1;
      static const UInt_t BUF_HEADER_LENGTH = 16;
      static unsigned short* pEvt, nEvts, evtLength;
      static int nBuffers = 0;
      static raw::unpacker unpack;

      unpack.read_online_buffer((UShort_t*)&gBuffer[0]);
      if(unpack.get_buffer_type() == EVENT_BUFFER)
	{
	  ++nBuffers;

	  int runnum = unpack.get_run_number();
	  nEvts = unpack.get_n_evts();
	  pEvt  = unpack.get_buffer();
	  pEvt += BUF_HEADER_LENGTH;

	  std::vector<unsigned short> monalisa_packets;
	  monalisa_packets.push_back(MONA_PACKET);
	  monalisa_packets.push_back(LISA_PACKET);

	  GET_LOCKING_POINTER(p_mraw, mraw, raw::mona);
	  GET_LOCKING_POINTER(p_mcal, mcal, cal::mona);
	  GET_LOCKING_POINTER(p_mvar, mvar, var::mona);

	  for(unsigned short i=0; i< nEvts; ++i) //\loop events
	    {
	      evtLength = *pEvt;
	      unpack.unpack_detector(p_mraw.Get(), monalisa_packets, pEvt);
	      p_mcal->calibrate(p_mraw.Get(), *p_mvar);

	      rb::Hist::FillAll();//
	      // Move on to the next event
	      pEvt += evtLength;

	    }




      // Short_t* p = &gBuffer[0]; // Point to the beginning of the buffer
      // Int_t nEvts = *p++;   // Figure out the number of events
      // GET_LOCKING_POINTER(pData, myData, ExampleData);
      // GET_LOCKING_POINTER(pVars, myVars, ExampleVariables);


      // for (Int_t event = 0; event < nEvts; ++event) { // Loop over all events


      // 	p += pData->process_event(p);   // Process each event using external user code
      // 	pData->calibrate(*pVars);   // Calibrate everything for this event





      // 	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
      // 	// NOTE: You need to include this call to rb::Hist::FillAll() at the end  //
      // 	// of your event loop if you want to be able to see histogrammed data.    //
      // 	//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
      // 	//\\\\\\\\\\\\\\\\\\\\\//
      // 	rb::Hist::FillAll();//
      // 	//\\\\\\\\\\\\\\\\\\\\\//

      } // End event loop

#endif
    } // End function

  } // namespace unpack
} // namsepace rb

#undef ADD_CLASS_INSTANCE
#undef ADD_CLASS_INSTANCE_ARGS
#undef GET_LOCKING_POINTER
