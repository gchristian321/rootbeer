//! \file rbMidasEvent.cxx
//! \brief Implements rbMidasEvent.h
#include <string.h>
#include <iostream>
#include "rbMidasEvent.h"

rb::MidasEvent::MidasEvent() : TMidasEvent() { fByteOrdering = -1; }

int rb::MidasEvent::CheckByteOrdering()
{
  int32_t test = 1;
  return *reinterpret_cast<char*>(&test) == 1 ?
    LITTLE : BIG;
}

bool rb::MidasEvent::ReadEvent(std::vector<uint16_t>& input)
{
  this->Clear();
  // Read the event header
  // note: it's not safe to read directly into the EventHeader_t struct
  // since compilers have the option of adding padding between elements
  const uint32_t nShorts = 2, nInts = 3, nHeaderWords = (nShorts*sizeof(uint16_t) + nInts*sizeof(uint32_t)) / sizeof(uint16_t);
  uint16_t headerShorts[nShorts];
  uint32_t headerInts[nInts];


  if(input.size() < nHeaderWords) return false; // not enough data for the header
  memcpy(reinterpret_cast<void*>(headerShorts), reinterpret_cast<void*>(&input[0]), nShorts*sizeof(uint16_t));
  memcpy(reinterpret_cast<void*>(headerInts), reinterpret_cast<void*>(&input[nShorts]), nInts*sizeof(uint32_t));

  fEventHeader.fEventId      = headerShorts[0];
  fEventHeader.fTriggerMask  = headerShorts[1];
  fEventHeader.fSerialNumber = headerInts[0];
  fEventHeader.fTimeStamp    = headerInts[1];
  fEventHeader.fDataSize     = headerInts[2];

  // Byte swap if big-endian
  /// \todo What if recording maching was big-endian?
  /// TMidasEvent \emph seems to assume it's always little (I may be mis-reading).
  if(fByteOrdering ==  -1) fByteOrdering = this->CheckByteOrdering();
  if(fByteOrdering == BIG) this->SwapBytesEventHeader();

  int32_t nDataWords = input.size() - nHeaderWords;
  if(nDataWords != this->GetDataSize() / sizeof(uint16_t)) {
    std::cerr << "Error: rb::Midas::ReadEvent: input vector size "
	      << "not equal to header size plus buffer size.\n"
	      << "input.size(): "   << input.size()
	      << ", nHeaderWords: " << nHeaderWords
	      << ", nDataWords: "   << nDataWords
	      << ", GetDataSize(): "<< GetDataSize()
	      << std::endl;
    return false;
  }      

  memcpy(reinterpret_cast<void*>(GetData()), reinterpret_cast<void*>(&input[nHeaderWords]), GetDataSize());
  if(fByteOrdering == BIG) this->SwapBytes(false);

  return true;
}

bool rb::MidasEvent::ReadHeader(std::istream* ifs)
{
  this->Clear();
  // Read the event header
  // note: it's not safe to read directly into the EventHeader_t struct
  // since compilers have the option of adding padding between elements
  const uint32_t nShorts = 2, nInts = 3;
  uint16_t headerShorts[nShorts];
  uint32_t headerInts[nInts];
  ifs->read(reinterpret_cast<char*>(headerShorts), nShorts * sizeof(uint16_t));
  ifs->read(reinterpret_cast<char*>(headerInts), nInts * sizeof(uint32_t));
  if(!ifs->good()) return false;

  fEventHeader.fEventId      = headerShorts[0];
  fEventHeader.fTriggerMask  = headerShorts[1];
  fEventHeader.fSerialNumber = headerInts[0];
  fEventHeader.fTimeStamp    = headerInts[1];
  fEventHeader.fDataSize     = headerInts[2];

  // Byte swap if big-endian
  /// \todo What if recording maching was big-endian?
  /// TMidasEvent \emph seems to assume it's always little (I may be mis-reading).
  if(fByteOrdering ==  -1) fByteOrdering = CheckByteOrdering();
  if(fByteOrdering == BIG) this->SwapBytesEventHeader();
  return true;
}

bool rb::MidasEvent::ReadEvent(std::istream* ifs)
{
  if ( ! this->ReadHeader(ifs) ) return false;

  ifs->read(GetData(), GetDataSize());
  if(fByteOrdering == BIG) this->SwapBytes(false);

  return ifs->good();
}

void rb::MidasEvent::CopyHeader(char* to)
{
  uint16_t hShorts[2] = { fEventHeader.fEventId, fEventHeader.fTriggerMask };
  uint32_t hInts[3] = { fEventHeader.fSerialNumber, fEventHeader.fTimeStamp, fEventHeader.fDataSize };
  memcpy(reinterpret_cast<void*>(to), reinterpret_cast<void*>(hShorts), 2 * sizeof(uint16_t));
  to += 2 * sizeof(uint16_t);
  memcpy(reinterpret_cast<void*>(to), reinterpret_cast<void*>(hInts), 3 * sizeof(uint32_t));
}
