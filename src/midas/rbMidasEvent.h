//! \file rbMidasEvent.h
//! \brief Defines rb::MidasEvent
#ifndef _RB_MIDAS_EVENT_
#define _RB_MIDAS_EVENT_
#include <istream>
#include <vector>
#include "TMidasEvent.h"


namespace rb
{
  /// Enumerate byte ordering.
  enum { LITTLE, BIG };

  /// Class derived from TMidasEvent.
  /*! Adds a method to read events from a std::istream, to conform
   *  to the way things are done in ROOTBEER. */
  class MidasEvent : public TMidasEvent
  {
  protected:
    int fByteOrdering; ///< byte ordering on the present machine
  public:
    MidasEvent(); ///< constructor
    bool ReadEvent(std::vector<uint16_t>& input); ///< read full event data from a vector<int16_t>
    bool ReadEvent(std::istream* ifs); ///< read full event data from a stream
    bool ReadHeader(std::istream* ifs); ///< read event header from a stream
    void CopyHeader(char* to); ///< copy event header
    int  CheckByteOrdering(); ///< check byte ordering on the present system
  };
}


#endif
