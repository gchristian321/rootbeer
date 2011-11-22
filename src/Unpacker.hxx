/*! \file Unpacker.hxx
 *  \brief Defined the class which
 *  attaches to and upacks data buffers.
 */
#ifndef __UNPACKER__
#define __UNPACKER__
#include <string>
#include "TTree.h"
#include "TThread.h"
#include "TRandom3.h"


/// Buffer size
/// \todo Make this easier to set for users.
static const Int_t BUFFER_SIZE = 4096;

/// Example of a really simple user class that holds data.
struct sData {
  Short_t a;
  Short_t b;
  Short_t c;

  void reset() { a=-1; b=-1; c=-1; }

  Int_t process_event(Short_t* pEvt) {
    reset();
    Short_t* p0 = pEvt, * p = pEvt;
    Int_t size = *p++;
    while(p< p0 + size + 1) {
      Short_t code = *p++;
      switch(code) {
      case 1: a = *p++; break;
      case 2: b = *p++; break;
      case 3: c = *p++; break;
      default: p++; break;
      }
    }
    return p - p0 - 2;
  }

  Int_t unpack_buffer(Short_t* pBuf) {
    Short_t* p = pBuf;
    Int_t nEvts = *p++;
    for(Int_t i=0; i< nEvts; ++i) {
      p += process_event(p);
    }
    return 0;
  }

  sData() { reset(); }
  sData(sData& other) : a(other.a), b(other.b), c(other.c) { };

};


namespace rb
{
  // Forward declarations of histogram classes.
  class H1D;
  class H2D;
  class H3D;

  /// \brief Class to unpack data into user structs, accessable via an
  /// internal <tt>TTree</tt>.
  /*! Functions:
   *  -# Grab buffers from a souce (typically online or a file).
   *  -# Unpack buffers into user data classes, event-by-event (user defined code).
   *  -# Fill all \c ROOTBEER Histogram objects.
   *
   *  This class generally runs as thread, using <tt>TThread</tt>, to allow
   *  the user access to the \c CINT command line while processing online data.
   *  Generally, the locking of shared objects (e.g. Histograms) is performed by the
   *  object itself, not by the Unpacker.
   */
  class Unpacker
  {

  private:
    /// Data buffer.
    Short_t fBuffer[BUFFER_SIZE];

    /// Data classes.
    /*! Defined by the user. */
    sData* fData;

    /// Internal tree for histogram filling.
    TTree* fTree;

    /// Thread for attaching to an online buffer source.
    TThread attachOnlineThread;

    /// Thread for attaching to 
    TThread attachOfflineThread;

    /// Tells if attached to an online data source or not.
    Bool_t kAttachedOnline;

    /// Tells if attached to an offline data source or not.
    Bool_t kAttachedOffline;

    /// Name of the offline file.
    std::string offlineFileName;


  public:
    /// Constructor
    /*! Initializes \c fTree and creates branches from \c fData.
     *  Also initializes the internal \cc TThread and internal variables
     *  that tell whether we're attached or not.*/
    Unpacker();

    /// Destructor
    /*! Unattach from any data source and free resources
     *  allocated to <tt>fTree</tt>. */
    ~Unpacker();

    /// Grab buffers from an online source (threaded).
    void AttachOnline();

    /// Grab buffers from an offline source (threaded).
    void AttachFile(const char* filename);

    /// Stop receiving buffers.
    void Unattach();

    /// Unpack buffer into user structs
    /*! Should be user-defined, and call FillHistograms at completion.
     \todo Implement skeleton-type user interface. */
    void UnpackBuffer();

  private:
    /// Internal routine to fill all of the histograms
    /// in <tt>ghistograms</tt>.
    void FillHistograms();

    /// Internal routine to attach to online data.
    /*! Basically this does all of the work of attaching to an
     *  online buffer source, and then the public interface 
     *  \c AttachOnline just runs this function in a <tt>TThread</tt>. */
    static void* AttachOnline_(void* arg);

    /// Internal routine to attach to offline data stored in a file.
    /*! Basically this does all of the work of attaching to an
     *  offline buffer source, and then the public interface 
     *  \c AttachFile just runs this function in a <tt>TThread</tt>. */
    static void* AttachFile_(void* arg);


    /// \note Histogram classes need access to \c fTree.
    friend class rb::H1D;
    friend class rb::H2D;
    friend class rb::H3D;

  };

}


#endif