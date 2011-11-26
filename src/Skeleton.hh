/*! \file Skeleton.hh
 *  \brief Lays out a code skeleton that users can fill in with their analysis codes.
 */

UserDataABC::Map_t UserDataABC::fgMap;

/// Macro to add a class instance to ROOTBEER
/*! NAME should be the variable name that you want to refer to the instance by.
 *  CLASS_NAME is the name of the class.
 *  CREATE_POINTER is a boolean telling whether or not you want CINT to automatically create
 *  a pointer to your data object that you can then usein an interactive session to
 *  access it.
 *  \note If you do create a pointer, calling the rb::WriteConfig() function will write the
 *  name and value of your data object and everything nested within it to a file. If you read in
 *  the config file, those values will override whatever is currently stored internally. This is the
 *  sort of thing you probably want for variables, but likely not for experimental parameters which
 *  change event-by-event.  If you do desire access to parameters from CINT, it is better to
 *  create a pointer in the CINT session, i.e.
 *  \code
 *  UserData<myDataType>* myData = UserData<myDataType>::Get("myDataTypeInstanceName");
 *  myData->LetMeSeeWhatSomeValueIs();
 *  \endcode
 *  If you do it this way instead of automatically here then whatever is in \c myDataTypeInstanceName
 *  won't get written to configuration files.
 */
#define ADD_CLASS_INSTANCE(NAME, CLASS_NAME, CREATE_POINTER)		\
  CLASS_NAME NAME;							\
  UserData<CLASS_NAME>  NAME##_UserData(#NAME, #CLASS_NAME, NAME, CREATE_POINTER);

/// Macro to add a class instance to ROOTBEER
/*! In case you need to use a non-default constructor for your class. */
#define ADD_CLASS_INSTANCE_ARGS(NAME, CLASS_NAME, ARGS, CREATE_POINTER)	\
  CLASS_NAME NAME ARGS;							\
  UserData<CLASS_NAME>  NAME##_UserData(#NAME, #CLASS_NAME, NAME, CREATE_POINTER);


//// Add the includes for your headers here. ////
#include "sData.hxx"



//// Define instances of your class here. ////

// Here we add an instance of sData, called myData and do not create a pointer for CINT.
ADD_CLASS_INSTANCE(myData, sData, kFALSE)

// Here we add an instance of sVaraibles, calles myVars and do create a pointer for CINT.
ADD_CLASS_INSTANCE(myVars, sVariables, kTRUE)


/// \brief Sample unpacking function, note that we can use the class instances
/// defined above.
void rb::unpack::UnpackBuffer() {
  Short_t* p = &fBuffer[0]; // point to the beginning of the buffer
  Int_t nEvts = *p++; // figure out the # of events

  for(Int_t i=0; i< nEvts; ++i) { // loop over all events
    p += myData.process_event(p); // process each event using external user code
    myData.calibrate(myVars); // calibrate everything for this event


    // NOTE: You need to include this call to rb::Hist::FillAll(); at the end
    // of your event loop if you want to be able to se histogrammed data.
    ///////////////////////////
      rb::Hist::FillAll();  ///
    ///////////////////////////
  }
}




#undef ADD_CLASS_INSTANCE
