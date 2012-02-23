//! \file Buffer.hxx
//! \brief Defines classes relevent to obtaining and unpacking data buffers.
#ifndef BUFFER_HXX
#define BUFFER_HXX
#include "Rint.hxx"
#include "utils/Thread.hxx"

namespace rb
{
  //! \brief ABC for defining how to obtain and unpack data buffers.
  //! \details By creating a class derived from this one, users can define
  //! how to connect (disconnect) to (from) an offline or online data source, how to recieve incoming
  //! data buffers, and how to unpack those buffers into user-defined classes.  All of the
  //! non-static member functions are pure virtual and must be implemented in derived classes.  See the
  //! documentation of individual functions for an explanation of what each should do.
  class BufferSource
  {
  private:
    const Int_t kDummy;

  protected:
    //! References to the rb::data::Wrapper objects in rb::Rint::Globals.
#define RB_REFERENCE_DECLARE
#include "rb_import_data.h"

    //! Initializes refeerences to rb::data::Wrapper objects in rb::Rint (gApplication).
    BufferSource();
  public:
    //! \details Nothing to do.
    virtual ~BufferSource();

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

    //! Terminate connection to an offline data source.
    virtual void CloseFile() = 0;

    //! Terminate connection to an online data source.
    virtual void DisconnectOnline() = 0;

    //! Unpack an abstract buffer into rb::data::Wrapper-derived classes.
    //! \returns true on successful unpack, false otherwise.
    virtual Bool_t UnpackBuffer() = 0;

    //! \brief Creation function.
    //! \details This is used as a generic way to get a pointer to an instance of a class derived from BufferSource,
    //! when the specific derived class is unknown <i>a priori</i>.  This function should be implemented by users
    //! such that it returns a pointer to the specific class they want to use for reading and unpacking data. Example:
    //! \code
    //! BufferSource* New() {
    //!     return new MyBufferSource();
    //! }
    //! \endcode
    //! \returns A pointer to a \c new instance of a class derived from BufferSource.
    static BufferSource* New();
  };

  inline BufferSource::~BufferSource() {}


  //! Contains classes used to attach to various data sources.
  namespace attach
  {
    //! Defines how we attach to offline data (a file) in a threaded environment.
    class File : public rb::Thread
    {
    private:
      //! Name (path) of the offline file.
      const char* kFileName;

      //! Tells whether to stop reading at EOF (true) or stay connected and wait for more data to come in (false).
      const Bool_t kStopAtEnd;

      //! Pointer to a BufferSource derived class used for getting and unpacking buffers.
      BufferSource* fBuffer;

    protected:
      //! \details Set kFileName and kStopAtEnd, initialize fBuffer to the result
      //! of BufferSource::New()
      //! \note Protected so that we can't accidentally create a stack instance,
      //! use static New() or CreateAndRun() to make heap allocated instances.
      File(const char* filename, Bool_t stopAtEnd);

    public:
      //! \details Deallocate fBuffer.
      virtual ~File();

      //! \brief Open the file, loop contents and use fBuffer to extract and unpack data.
      void DoInThread();

      //! \brief Returns A \c new instance of rb::attach::File
      static File* New(const char* filename, Bool_t stopAtEnd);

      //! \brief Conststructs a \c new instance of rb::attach::File and calls rb::Thread::Run().
      static void CreateAndRun(const char* filename, Bool_t stopAtEnd);
    };

    inline File::~File() {
      delete fBuffer;
    }

    inline File* File::New(const char* filename, Bool_t stopAtEnd) {
      return new File(filename, stopAtEnd);
    }

    inline void File::CreateAndRun(const char* filename, Bool_t stopAtEnd) {
      File * f = new File(filename, stopAtEnd);
      f->Run();
    }

    //! Defines how we attach to a list of offline files.
    class List : public rb::Thread
    {
    private:
      //! Name (path) of the list file.
      const char* kListFileName;

      //! Pointer to a BufferSource derived class used for getting and unpacking buffers.
      BufferSource* fBuffer;

    protected:
      //! \details Set kListFileName, initialize fBuffer to the result
      //! of BufferSource::New()
      //! \note Protected so that we can't accidentally create a stack instance,
      //! use static New() or CreateAndRun() to make heap allocated instances.
      List(const char* filename);

    public:
      //! \details Deallocate fBuffer.
      virtual ~List();

      //! \brief Open the list file, attach to each file in the list in sequence.
      void DoInThread();

      //! \brief Returns A \c new instance of rb::attach::List
      static List* New(const char* filename);

      //! \brief Conststructs a \c new instance of rb::attach::List and calls rb::Thread::Run().
      static void CreateAndRun(const char* filename);
    };

    inline List::~List() {
      delete fBuffer;
    }

    inline List* List::New(const char* filename) {
      return new List(filename);
    }

    inline void List::CreateAndRun(const char* filename) {
      List * L = new List(filename);
      L->Run();
    }


    //! Defines how we attach to online data.
    //! \note The data members and constructor have been designed such that they
    //! can accomodata a wide variety of connection formats. However, depending
    //! on the particular impementation of rootbeer, it may be difficult or ackward to
    //! use the default. In this case, it is sugested that the user alter this class
    //! to better suit his/her experiment.
    class Online : public rb::Thread
    {
    private:
      //! \brief The source of the online data.
      //! \details Usually this is the host name or ip address of where the data come in from.
      const char* fSourceArg;

      //! \brief Secondary argument that might be needed to obtain online data.
      //! \details Examples: MIDAS experiment name, port number, etc.
      const char* fOtherArg;

      //! \brief Any other arguments that are not covered by the first two.
      //! \details Like the arguments to main (<tt>char** argc</tt>), this can basically be used
      //! to represent anything.
      char** fOtherArgs;

      //! The length of the \c char* array fOtherArgs.
      const int fNumOthers;

      //! Pointer to a BufferSource derived class used for getting and unpacking buffers.
      BufferSource* fBuffer;

    protected:
      //! \details Sets data fields, allocates fBuffer using BufferSource::New()
      //! \note Protected so that we can't accidentally create a stack instance,
      //! use static New() or CreateAndRun() to make heap allocated instances.
      Online(const char* source, const char* other, char** others, int nothers);

    public:
      //! \details Deallocates fBuffer
      virtual ~Online();

      //! \brief Connects to an online data source, listens for data and unpacks it when present (using fBuffer).
      void DoInThread();

      //! \brief Returns A \c new instance of rb::attach::Online
      static Online* New(const char* source, const char* other, char** others, int nothers);

      //! \brief Conststructs a \c new instance of rb::attach::Online and calls rb::Thread::Run().
      static void CreateAndRun(const char* source, const char* other, char** others, int nothers);
    };

    inline Online::~Online() {
      delete fBuffer;
    }

    inline Online* Online::New(const char* source, const char* other, char** others, int nothers) {
      return new Online(source, other, others, nothers);
    }

    inline void Online::CreateAndRun(const char* source, const char* other, char** others, int nothers) {
      Online * o = new Online(source, other, others, nothers);
      o->Run();
    }

    // Some useful functions & constants //
    namespace
    {
      const char* FILE_THREAD_NAME   = "AttachFile";
      const char* LIST_THREAD_NAME   = "AttachList";
      const char* ONLINE_THREAD_NAME = "AttachOnline";
      inline Bool_t FileAttached()   { return rb::Thread::IsRunning(FILE_THREAD_NAME);   }
      inline Bool_t ListAttached()   { return rb::Thread::IsRunning(LIST_THREAD_NAME);   }
      inline Bool_t OnlineAttached() { return rb::Thread::IsRunning(ONLINE_THREAD_NAME); }
      inline void StopAll() {
	rb::Thread::Stop(FILE_THREAD_NAME);
	rb::Thread::Stop(LIST_THREAD_NAME);
	rb::Thread::Stop(ONLINE_THREAD_NAME);
      }
    } // namespace
  } // namespace attach
} // namespace rb

#endif
