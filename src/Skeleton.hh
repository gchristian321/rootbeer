/*! \file Skeleton.hh
 *  \brief Lays out a code skeleton that users can fill in with their analysis codes.
 */


/// Macro to add a class instance to ROOTBEER

#include <assert.h>
#define ADD_CLASS_INSTANCE(NAME, CLASS_NAME, CREATE_POINTER)		\
  rb::Data* NAME##_Data = rb::Data::New<CLASS_NAME>(#NAME, #CLASS_NAME, CREATE_POINTER);

/// Macro to add a class instance to ROOTBEER
/*! In case you need to use a non-default constructor for your class. */
#define ADD_CLASS_INSTANCE_ARGS(NAME, CLASS_NAME, ARGS, CREATE_POINTER)	\
  rb::Data* NAME##_Data = rb::Data::New<CLASS_NAME>(#NAME, #CLASS_NAME, CREATE_POINTER, ARGS);


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


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//\\ Define instances of your class here    \\//
//\\ (using the ADD_CLASS_INSTANCE macros). \\//
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

// Here we add an instance of ExampleData, called myData and do not allow viewing in CINT.
ADD_CLASS_INSTANCE(myData, ExampleData, kFALSE)

// Here we add an instance of sVariables, calles myVars and do allow viewing in CINT.
//ADD_CLASS_INSTANCE(myVars, ExampleVariables, kTRUE)
ADD_CLASS_INSTANCE_ARGS(myVars, ExampleVariables, "32", kTRUE)

ADD_CLASS_INSTANCE(mcal, cal::mona, kFALSE);
ADD_CLASS_INSTANCE(mraw, raw::mona, kFALSE);
ADD_CLASS_INSTANCE(mvar, var::mona, kTRUE);


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
//\\ Here you should define how to process your data buffers //
//\\ by implementing the UnpackBuffer() function.            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

namespace rb {
  namespace unpack {
    const Int_t BUF_SIZE = 4096;
    typedef Short_t DataType;
    vector<DataType> gBuffer(BUF_SIZE);
  }
}


#define EVENT_BUFFER 1
#define BUF_HEADER_LENGTH 16


void rb::unpack::ReadBuffer(istream& ifs) {
  if(gBuffer.size() != BUF_SIZE) gBuffer.resize(BUF_SIZE);

  Char_t* firstWord = reinterpret_cast<Char_t*>(&gBuffer[0]);
  Int_t bufferSize = BUF_SIZE * (sizeof(DataType) / sizeof(Char_t));

  ifs.read(firstWord, bufferSize);
}

void rb::unpack::UnpackBuffer() {

  // #if 0
  //   static unsigned short* pEvt, nEvts, evtLength;
  //   static int nBuffers = 0;
  //   static raw::unpacker unpack;

  //   unpack.read_online_buffer((UShort_t*)&gBuffer[0]);
  //   if(unpack.get_buffer_type() == EVENT_BUFFER)
  //     {
  //       ++nBuffers;
  //       if(1)
  // 	{
  // 	  if(nBuffers/100 > (nBuffers-1)/100)
  // 	    {
  // 	      for(int i=nBuffers; i>0; i/=10)
  // 		cout<<"\b";
  // 	      cout  <<nBuffers; flush(cout);
  // 	    }
  // 	}

  //       int runnum = unpack.get_run_number();
  //       nEvts = unpack.get_n_evts();
  //       pEvt  = unpack.get_buffer();
  //       pEvt += BUF_HEADER_LENGTH;

  //       vector<unsigned short> monalisa_packets;
  //       monalisa_packets.push_back(MONA_PACKET);
  //       monalisa_packets.push_back(LISA_PACKET);

  //       GET_LOCKING_POINTER(p_mraw, mraw, raw::mona);
  //       for(unsigned short i=0; i< nEvts; ++i) //\loop events
  // 	{
  // 	  evtLength = *pEvt;
  // 	  unpack.unpack_detector(p_mraw.Get(), monalisa_packets, pEvt);

  // 	  rb::Hist::FillAll();//
  // 	  // Move on to the next event
  // 	  pEvt += evtLength;

  // 	}

	
  // #else



  Short_t* p = &gBuffer[0]; // Point to the beginning of the buffer
  Int_t nEvts = *p++;   // Figure out the number of events
  GET_LOCKING_POINTER(pData, myData, ExampleData);
  GET_LOCKING_POINTER(pVars, myVars, ExampleVariables);


  for (Int_t event = 0; event < nEvts; ++event) { // Loop over all events


    p += pData->process_event(p);   // Process each event using external user code
    pData->calibrate(*pVars);   // Calibrate everything for this event





    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
    // NOTE: You need to include this call to rb::Hist::FillAll() at the end  //
    // of your event loop if you want to be able to see histogrammed data.    //
    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
    //\\\\\\\\\\\\\\\\\\\\\//
    rb::Hist::FillAll();//
    //\\\\\\\\\\\\\\\\\\\\\//

  } // End event loop

} // End function


#undef ADD_CLASS_INSTANCE
#undef ADD_CLASS_INSTANCE_ARGS
#undef GET_LOCKING_POINTER
