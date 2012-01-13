//! \file Skeleton.hxx
//! \brief Defines some macros and functions used in Skeleton.cxx
#include "Data.hxx"
#include "midas/rbMidasEvent.h"

//! \brief Adds user-defined classes to ROOTBEER.
//! \details This macro creates a \c new instance of rb::Data, wrapping a user-defined class.
//! \param NAME The variable name that you want to assign to your class instance.
//! \param CLASS_NAME The name of the class.
//! \param CINT_ACCESS Boolean specifying whether or not end users should have access to the class
//!  data members from CINT.
//!
//!  Example:
//! \code
//! class MyClass {
//!   public:
//!   MyClass();
//!   // -- ETC -- //
//!   };
//!
//!   ADD_CLASS_INSTANCE(my_class_instance, MyClass, false);
//! \endcode
//!  See the documentation on rb::Data for more information.
#define ADD_CLASS_INSTANCE(NAME, CLASS_NAME, CINT_ACCESS)            \
  rb::Data* NAME##_Data = rb::Data::New<CLASS_NAME>(#NAME, #CLASS_NAME, CINT_ACCESS);

//! \brief Same as ADD_CLASS_INSTANCE, but for non-default constructors
//! \param ARGS Arguments to the user class's constructor, specified as a string.
//!
//!  Example:
//! \code
//! class MyClass {
//!   public:
//!   MyClass(int a, double b, string name);
//!   // -- ETC -- //
//!   };
//!
//!   ADD_CLASS_INSTANCE_ARGS(my_class_instance, MyClass, true, "12, 31.4, \"my_class_name\"");
//! \endcode
#define ADD_CLASS_INSTANCE_ARGS(NAME, CLASS_NAME, CINT_ACCESS, ARGS) \
  rb::Data* NAME##_Data = rb::Data::New<CLASS_NAME>(#NAME, #CLASS_NAME, CINT_ACCESS, ARGS);


//! \brief Creates an instance of LockingPointer with which you can safely access your data.
//! \param SYMBOL The pointer symbol.
//! \param NAME The variable name of the class instance you need to access.
//! \param CLASS The class you're getting a pointer to.
//! 
//! Example:
//! \code
//! ADD_CLASS_INSTANCE(my_class_instance, MyClass, false);
//! // -- ETC -- //
//! GET_LOCKING_POINTER(pMine, my_class_instance, MyClass);
//! pMine->MemberFunction();
//! \endcode
#define GET_LOCKING_POINTER(SYMBOL, NAME, CLASS)                        \
  LockingPointer<CLASS> SYMBOL (NAME##_Data->GetDataPointer<CLASS>(), NAME##_Data->fMutex);


namespace
{
  //! \brief Stock code to perform MIDAS buffer extraction.
  //! \details Midas buffers are not a fixed size, so we must first check the header to
  //! get the appropriate length, then copy the appropriate amount of data to the buffer.
  inline void ExtractMidasBuffer(std::istream& ifs, rb::Buffer& buffer)
  {
    rb::MidasEvent dummyMidasEvent; // dummy midas event to get header length
    bool readHeaderResult = dummyMidasEvent.ReadHeader(&ifs);
    if(!readHeaderResult) return; // EOF
      
    // Resize buffer appropriately.
    const Int_t headerSize = 2*sizeof(Short_t) + 3*sizeof(Int_t);
    const Int_t dataSize = dummyMidasEvent.GetDataSize();
    const Int_t totalWords = (headerSize + dataSize) / sizeof(DATA_TYPE);
    buffer.resize(totalWords);

    // Read header into buffer
    dummyMidasEvent.CopyHeader(reinterpret_cast<Char_t*>(&buffer[0]));

    // Read data into buffer
    const Int_t firstDataWord = headerSize / sizeof(DATA_TYPE);
    ifs.read(reinterpret_cast<Char_t*>(&buffer[firstDataWord]), dataSize);
  }

  //! \brief Stock code to perform NSCL buffer extraction.
  //! \details NSCL buffers are just a fixed size of 4096 16-bit words. All we need to do
  //! is copy exactly this ammount into the buffer.
  inline void ExtractNsclBuffer(std::istream& ifs, rb::Buffer& buffer)
  {
    const Int_t bufferSize = 4096;
    if(buffer.size() != bufferSize) buffer.resize(bufferSize);
    ifs.read(reinterpret_cast<Char_t*>(&buffer[0]), bufferSize * sizeof(DATA_TYPE));
  }
}
