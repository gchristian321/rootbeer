/*! \file Mutex.hxx
 *  \brief Contains classes that wrap \c TMutex
 *  objects and provide static functions to access them.
 * \note \c CINT doesn't play nice with static member variables
 *  so that's why the various mutexes are global. They should only
 *  be accessed through their respective wrapper classes.
 */
#ifndef __MUTEX_HXX__
#define __MUTEX_HXX__
 // CINT doesn't need to know about this.
#ifndef __CINT__
#include "TThread.h"

namespace rb
{

  /// \global Canvas mutex.
  static TMutex gCanvasMutex;

  /// \global Histogram mutex.
  static TMutex gHistMutex;


  // Classes
  /// Mutex wrapper for \c TCanvas classes.
  struct CanvasMutex
  {
    static void Lock()   { gCanvasMutex.Lock();   }
    static void Unlock() { gCanvasMutex.UnLock(); }
  };

  /// Mutex wrapper for \c rb::H*F classes.
  struct HistMutex
  {
    static void Lock()   { gHistMutex.Lock();   }
    static void Unlock() { gHistMutex.UnLock(); }
  };


}



#endif // #ifndef __CINT__
#endif // #ifndef __MUTEX_HXX__
