//! \file Buffer.hxx
//! \brief Defines classes relevent to obtaining and unpacking data buffers.
#ifndef BUFFER_HXX
#define BUFFER_HXX
#include "Rint.hxx"
#include "utils/boost_scoped_ptr.h"

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
protected:
	//! Initializes refeerences to rb::data::Wrapper objects in rb::Rint (gApplication).
	BufferSource();
public:
	//! \brief Empty
	//! \warning Derived classes should \e not call virtual methods from the destructor.
	//! (note that disconnect online or offline are called when exiting from the event loop)
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

	//! \brief Defines the default file extensions.
	//! \returns Array of const char*, consisting of a pair of { description, *.extension }
	//! strings for every desired file type, and terminated by { 0, 0 }.
	//! \note This _must_ be implemented by the user. Here is an example:
	//! \code
	//! const char** rb::BufferSource::GetDefaultExtensions() {
	//! 	static const char* ext[] = {
	//! 		"MIDAS files", "*.mid",
	//! 		"All files", "*.*",
	//! 		0, 0
	//! 	};
	//! 	return ext;
	//! }
	//! \endcode
	static const char** GetDefaultExtensions();

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

#ifndef __MAKECINT__
inline BufferSource::BufferSource() {}
inline BufferSource::~BufferSource() {}
#endif

} // namespace rb

#endif
