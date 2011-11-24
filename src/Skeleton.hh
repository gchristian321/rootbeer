/*! \file DataSkeleton.hxx
 *
 */
#define ADD_CLASS_INSTANCE(NAME, CLASS_NAME)				\
  CLASS_NAME NAME;							\
  UserData<CLASS_NAME>  NAME##_UserData(#NAME, #CLASS_NAME, NAME);

#define ADD_CLASS_INSTANCE_ARGS(NAME, CLASS_NAME, ARGS)			\
  CLASS_NAME NAME ARGS;							\
  UserData<CLASS_NAME>  NAME##_UserData(#NAME, #CLASS_NAME, NAME);


//// Add the includes for your headers here. ////
#include "sData.hxx"



//// Define instances of your class here. ////
ADD_CLASS_INSTANCE(myData, sData)
ADD_CLASS_INSTANCE_ARGS(myDataCopy, sData, (myData))



void rb::unpack::UnpackBuffer() {
  Short_t* p = &fBuffer[0];
  Int_t nEvts = *p++;

  for(Int_t i=0; i< nEvts; ++i) {
    p += myData.process_event(p);
    myData.calibrate();



   /////////////////////
    FillHistograms(); //
   /////////////////////
  }
}




#undef ADD_CLASS_INSTANCE
