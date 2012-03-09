//! \file Error.hxx
//! \brief Defines C++ style error printing classes.
#ifndef __MAKECINT__
#ifndef ERROR_HXX
#define ERROR_HXX
#include <iostream>
#include <string>
#include "Logger.hxx"

#define OP__(CLASS, ARGTYPE) CLASS& operator<< (ARGTYPE arg) {	\
    std::cerr << arg; return *this;				\
  }
#define OPS__(CL)				\
  OP__(CL, const char*)				\
  OP__(CL, long long)				\
  OP__(CL, long)				\
  OP__(CL, int)					\
  OP__(CL, short)				\
  OP__(CL, char)				\
  OP__(CL, unsigned long long)			\
  OP__(CL, unsigned long)			\
  OP__(CL, unsigned int)			\
  OP__(CL, unsigned short)			\
  OP__(CL, unsigned char)			\
  OP__(CL, bool)				\
  OP__(CL, double)				\
  OP__(CL, float)				\
  OP__(CL, const std::string&)

namespace err
{
  struct Strm
  {
  protected:
    typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
    typedef CoutType& (*StandardEndLine)(CoutType&);
  public:
    Strm(const char* what, const char* where) {
      std::cerr << what << " in <" << where << ">: ";
    }
    OPS__(Strm)
    Strm& operator<<(StandardEndLine manip) {
        manip(std::cerr);
        return *this;
    }
    virtual ~Strm() { std::endl(std::cerr); }
  };
  struct Info: public Strm { Info(const char* where) : Strm("Info", where) {} };
  struct Error: public Strm { Error(const char* where) : Strm("Error", where) {} };
  struct Warning: public Strm { Warning(const char* where) : Strm("Warning", where) {} };
}



#undef OP__
#undef OPS__
#endif
#endif
