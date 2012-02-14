//! \file Buffer.hxx
//! \brief Defines classes relevent to obtaining and unpacking data buffers.
#ifndef BUFFER_HXX__
#define BUFFER_HXX__
#include <string>
#include <Rtypes.h>


static const Int_t N_SOURCES = 3;
enum DataSources { ONLINE_, FILE_, LIST_ };
namespace rb
{
  class BufferSource
  {
  private:
    //! Arguments for AttachFile()
    struct FileArguments {
      std::string fileName;
      Bool_t stopEnd;
      void Set(const char* fname, Bool_t stop) {
	fileName = fname;
	stopEnd = stop;
      }
    } fFileArgs;

    //! Arguments for AttachOnline()
    struct OnlineArguments {
      std::string source;
      std::string other;
      char** others;
      int nothers;
      void Set(const char* source_, const char* other_,
	       char** others_, int nothers_) {
	source = source_;
	other = other_;
	others = others_;
	nothers = nothers_;
      }
    } fOnlineArgs;

    //! Tells whether the source is attached to a data source.
    static Bool_t* Attached_() {
      static Bool_t firstTime = kTRUE;
      static Bool_t* isAttached = new Bool_t[N_SOURCES];
      if (firstTime) {
	firstTime = kFALSE;
	for(Int_t i=0; i< N_SOURCES; ++i)
	  isAttached[i] = kFALSE;
      }
      return isAttached;
    }

    //! Attach to an online data source.
    static void* AttachOnline_(void* arg);

    //! Attach to an offline data source.
    static void* AttachFile_(void* arg);

    //! Attach to a list of offline data sources.
    static void* AttachList_(void* arg);

    //! Thread for attaching to online data.
    TThread attachOnlineThread;

    //! Thread for attaching to offline data.
    TThread attachOfflineThread;

    //! Thread for attaching to a list of offline files.
    TThread attachListThread;

  public:
    void RunOnline() {
      attachOnlineThread.Run(reinterpret_cast<void*>(this));
    }
    void RunFile() {
      attachOfflineThread.Run(reinterpret_cast<void*>(this));
    }
    void RunList(const char* filename) {
      attachListThread.Run((void*)gSystem->ExpandPathName(filename));
    }

    void Unattach() {
      if(BufferSource::IsAttached(ONLINE_)) {
	BufferSource::SetNotAttached(ONLINE_);
	attachOnlineThread.Join();
      }
      if(BufferSource::IsAttached(FILE_)) {
	BufferSource::SetNotAttached(FILE_);
	attachOfflineThread.Join();
      }
      if(BufferSource::IsAttached(LIST_)) {
	BufferSource::SetNotAttached(LIST_);
	attachListThread.Join();
      }
    }


  protected:
    //! Constructor
    BufferSource() :
      attachOnlineThread ("attachOnline", AttachOnline_),
      attachOfflineThread ("attachFile", AttachFile_),
      attachListThread  ("attachList", AttachList_)
    {
      fFileArgs.Set("", kTRUE);
      fOnlineArgs.Set("", "", 0, 0);
    }

  public:
    static BufferSource* Instance();

    //! Destructor
    virtual ~BufferSource() {};

    //! Tells whether or not it's attached to a data source.
    //! \details Use the DataSources enum to denote the types.
    static Bool_t IsAttached(Int_t source) { return Attached_()[source]; }

    //! Sets appropriate Attached_ flag to true.
    static void SetAttached(Int_t source) { Attached_()[source] = kTRUE; }

    //! Sets appropriate Attached_ flag to false.
    static void SetNotAttached(Int_t source) { Attached_()[source] = kFALSE; }

    //! Return reference to file arguments
    FileArguments& FileArgs() { return fFileArgs; }

    //! Return reference to online arguments
    OnlineArguments& OnlineArgs() { return fOnlineArgs; }

    //! Open a data file
    //! \param [in] file_name Name (path) of the file to open.
    //! \param [in] other_args Any other arguments that might be needed.
    //! \param [in] n_others Number of other (tertiary) arguments.
    //! \returns true if file successfully opened, false otherwise.
    virtual Bool_t OpenFile(const char* file_name, char** other = 0, int nother = 0) = 0;

    //! Connect to an online data source.
    //! \param [in] host Name of the host from which the data are received.
    //! \param [in] other_arg Secondary argument specifying where the data come from (e.g. experiment for MIDAS).
    //! \param [in] other_args Any other arguments that might be needed.
    //! \param [in] n_others Number of other (tertiary) arguments.
    //! \returns true if connection is successfully made, false otherwise.
    virtual Bool_t ConnectOnline(const char* host, const char* other_arg = "", char** other_args = 0, int n_others = 0) = 0;

    //! Read an abstract buffer from an offline data source.
    //! \returns true if buffer is successfully read, false otherwise.
    virtual Bool_t ReadBufferOffline() = 0;

    //! Read an abstract buffer from an online data source.
    //! \returns true if buffer is successfully read, false otherwise.
    virtual Bool_t ReadBufferOnline() = 0;

    //! Unpack an abstract buffer into rb::Data classes.
    //! \returns Error code
    virtual Bool_t UnpackBuffer() = 0;
  };
}

#endif
