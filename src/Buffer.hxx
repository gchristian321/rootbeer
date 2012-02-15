//! \file Buffer.hxx
//! \brief Defines classes relevent to obtaining and unpacking data buffers.
#ifndef BUFFER_HXX__
#define BUFFER_HXX__
#include <string>
#include <Rtypes.h>
#include "utils/ThreadExecutor.hxx"


static const Int_t N_SOURCES = 3;
enum DataSources { ONLINE_, FILE_, LIST_ };
namespace rb
{
  class BufferSource;

  class ThreadFile : public ThreadExecutor
  {
  private:
    const char* kFileName;
    const Bool_t kStopAtEnd;
    BufferSource* fBuffer;
  public:
    ThreadFile(const char* filename, Bool_t stopAtEnd);
    virtual ~ThreadFile();

    void DoInThread();
  };

  class ThreadOnline : public ThreadExecutor
  {
  private:
    const char* fSourceArg;
    const char* fOtherArg;
    char** fOtherArgs;
    const int fNumOthers;
    BufferSource* fBuffer;
  public:
    ThreadOnline(const char* source, const char* other, char** others, int nothers);
    virtual ~ThreadOnline();
    
    void DoInThread();
  };



  // class Connector
  // {
  // private:
  //   TThread* fThread;
  // protected:
  //   BufferSource* fBuffer;
  //   Bool_t fRun;
  // public:
  //   Connector(); //BufferSource* buf);
  //   virtual ~Connector();
  //   virtual void Attach() = 0;
  //   void Run() { fThread->Run((void*)this); }
  // private:
  //   static void* RunThread(void* args)  {
  //     reinterpret_cast<Connector*>(args)->Attach();
  //   }
  //   friend class BufferSource;
  // };

  // class File : public Connector
  // {
  // private:
  //   const Bool_t kStopAtEnd;
  //   const std::string kFileName;
  // public:
  //   File(const char* filename, Bool_t stopAtEnd); //, BufferSource* buf);
  //   virtual ~File() {};
  //   void Attach();
  // };

  // class Online : public Connector
  // {
  // private:
  //   const char* fSourceArg;
  //   const char* fOtherArg;
  //   char** fOtherArgs;
  //   const int fNumOthers;
  // public:
  //   Online(const char* source, const char* other, char** others, int nothers, BufferSource* buf);
  //   virtual ~Online() {};
  //   void Attach();
  // };


  class BufferSource
  {
  private:
    //! Thread for attaching to buffer sources.
    //    TThread fThread;

    // class PrivateConnector {
    // private:
    //   Connector* fConnector_;
    // public:
    //   PrivateConnector() : fConnector_(0) {}
    //   void Start(Connector* new_) { fConnector_ = new_; }
    //   void Stop() { if (fConnector_) { delete fConnector_; fConnector_ = 0; } }
    //   Connector* operator() () { return fConnector_; }
    //   ~PrivateConnector() { Stop(); }
    // }
    //   fConnector;

    //    void StartConnection(Connector* new_);
  public:
    Bool_t IsConnected() { return 1; } //fConnector()->fRun; }
    // void RunOnline(const char* host, const char*  other, char** others, Int_t nothers);
    // void RunFile(const char* filename, Bool_t stopAtEnd);
    // void RunList(const char* filename);
    void Unattach();
    static BufferSource* GetFromUser();

  protected:
    //! Constructor
    BufferSource();

  public:
    //! Destructor
    virtual ~BufferSource() {};

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

    virtual void CloseFile() = 0;

    virtual void DisconnectOnline() = 0;

    //! Unpack an abstract buffer into rb::Data classes.
    //! \returns Error code
    virtual Bool_t UnpackBuffer() = 0;

    //    friend class Connector;
  };
}

#endif
