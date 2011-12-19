/*! \file Skeleton.hh
 *  \brief Lays out a code skeleton that users can fill in with their analysis codes.
 */


/// Macro to add a class instance to ROOTBEER
/*! NAME should be the variable name that you want to refer to the instance by.
 *  CLASS_NAME is the name of the class.
 *  CREATE_POINTER is a boolean telling whether or not you want CINT to automatically create
 *  a pointer to your data object that you can then use in an interactive session to
 *  access it.
 *  \note If you do create a pointer, calling the rb::WriteConfig() function will write the
 *  name and value of your data object and everything nested within it to a file. If you read in
 *  the config file, those values will override whatever is currently stored internally. This is the
 *  sort of thing you probably want for variables, but likely not for experimental parameters which
 *  change event-by-event.  If you do desire access to parameters from CINT, it is better to
 *  create a pointer explicitly in the CINT session, i.e.
 *  \code
 *  rb::Data<myDataType>* myData = rb::Data<myDataType>::Get("myDataTypeInstanceName");
 *  myData->DoSomething();
 *  \endcode
 *  If you do it this way instead of automatically here, then whatever is in \c myDataTypeInstanceName
 *  won't get written to configuration files.
 */
#include <assert.h>
#define ADD_CLASS_INSTANCE(NAME, CLASS_NAME, CREATE_POINTER)		\
  rb::Data* NAME##_Data = rb::Data::New<CLASS_NAME>(#NAME, #CLASS_NAME, CREATE_POINTER);

/// Macro to add a class instance to ROOTBEER
/*! In case you need to use a non-default constructor for your class. */
#define ADD_CLASS_INSTANCE_ARGS(NAME, CLASS_NAME, ARGS, CREATE_POINTER)	\
  rb::Data* NAME##_Data = rb::Data::New<CLASS_NAME>(#NAME, #CLASS_NAME, CREATE_POINTER, ARGS);


#define GET_LOCKING_POINTER(SYMBOL, NAME, CLASS)		\
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

#define EVENT_BUFFER 1
#define BUF_HEADER_LENGTH 16

void rb::unpack::UnpackBuffer() {

  static unsigned short* pEvt, nEvts, evtLength;
  static int nBuffers = 0;
  static raw::unpacker unpack;

  unpack.read_online_buffer((UShort_t*)&fBuffer[0]);
      if(unpack.get_buffer_type() == EVENT_BUFFER)
	{
	  ++nBuffers;
	  if(1)
	    {
	      if(nBuffers/100 > (nBuffers-1)/100)
		{
		  for(int i=nBuffers; i>0; i/=10)
		    cout<<"\b";
		  cout  <<nBuffers; flush(cout);
		}
	    }

	  int runnum = unpack.get_run_number();
	  nEvts = unpack.get_n_evts();
	  pEvt  = unpack.get_buffer();
	  pEvt += BUF_HEADER_LENGTH;

	  vector<unsigned short> monalisa_packets;
	  monalisa_packets.push_back(MONA_PACKET);
	  monalisa_packets.push_back(LISA_PACKET);

	  GET_LOCKING_POINTER(p_mraw, mraw, raw::mona);
	  for(unsigned short i=0; i< nEvts; ++i) //\loop events
	    {
	      evtLength = *pEvt;
	      unpack.unpack_detector(p_mraw.Get(), monalisa_packets, pEvt);

       rb::Hist::FillAll();//
	      // Move on to the next event
	      pEvt += evtLength;

	    }

	




  // Short_t* p = &fBuffer[0]; // Point to the beginning of the buffer
  // Int_t nEvts = *p++;   // Figure out the number of events
  //   GET_LOCKING_POINTER(pData, myData, ExampleData);
  //   GET_LOCKING_POINTER(pVars, myVars, ExampleVariables);


  // for (Int_t event = 0; event < nEvts; ++event) { // Loop over all events


  //   p += pData->process_event(p);   // Process each event using external user code
  //   pData->calibrate(*pVars);   // Calibrate everything for this event





    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
    // NOTE: You need to include this call to rb::Hist::FillAll() at the end  //
    // of your event loop if you want to be able to see histogrammed data.    //
    //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
    //\\\\\\\\\\\\\\\\\\\\\//
      //       rb::Hist::FillAll();//
    //\\\\\\\\\\\\\\\\\\\\\//

  } // End event loop

} // End function




#undef ADD_CLASS_INSTANCE
#undef ADD_CLASS_INSTANCE_ARGS
#undef GET_LOCKING_POINTER
