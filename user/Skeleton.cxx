//! \file Skeleton.cxx
//! \brief This is where users plug their analysis codes into ROOTBEER.
#include "Skeleton.hxx"
#include "Rootbeer.hxx"




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
#include "Dragon.hxx"




//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//\\ Define instances of your class here    \\//
//\\ (using the ADD_CLASS_INSTANCE macros). \\//
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

// Here we add an instance of ExampleData, called myData and do not allow viewing in CINT.
ADD_CLASS_INSTANCE(myData, ExampleData, kFALSE)

// Here we add an instance of sVariables, called myVars and do allow viewing in CINT.
// Also, we use a non-default constructor, with the argument 32.
ADD_CLASS_INSTANCE_ARGS(myVars, ExampleVariables,  kTRUE, "32")

ADD_CLASS_INSTANCE(mcal, cal::mona, kFALSE);
ADD_CLASS_INSTANCE(mraw, raw::mona, kFALSE);
ADD_CLASS_INSTANCE(mvar, var::mona, kTRUE);

ADD_CLASS_INSTANCE(ts, TimeStamp, kFALSE);
ADD_CLASS_INSTANCE(dragon, data::Dragon, kFALSE);
ADD_CLASS_INSTANCE(vdragon, variables::Dragon, kTRUE);






//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//\\ Here you should define how to process your data buffers        \\//
//\\ by implementing the ReadBuffer() and UnpackBuffer() functions. \\//
//\\ For more information, see the doxygen page on rb::unpack       \\//
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::unpack::ReadBuffer(istream& ifs, rb::Buffer& buffer) {

#define _MIDAS_ // Uncomment to use pre-defined MIDAS buffer extraction routines.
  // #define _NSCL_  // Uncomment to use pre-defined NSCL buffer extraction routines.

#ifdef _MIDAS_
  ExtractMidasBuffer(ifs, buffer); // defined in Skeleton.hxx
#elif defined _NSCL_
  ExtractNsclBuffer(ifs, buffer); // defined in Skeleton.hxx
#else
  // Define your own buffer extraction method
#endif
}


void rb::unpack::UnpackBuffer(rb::Buffer& buffer) {

#ifdef _MIDAS_
  rb::MidasEvent midasEvent;
  if ( ! midasEvent.ReadEvent(buffer) ) return;

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

  unpack.read_online_buffer((UShort_t*)&buffer[0]);
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




      // Short_t* p = &buffer[0]; // Point to the beginning of the buffer
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
