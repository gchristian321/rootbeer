//! \file Logger.hxx
//! \brief Defines a very simple class for error message logging.
#ifndef LOGGER_HXX
#define LOGGER_HXX
#include <iostream>
#ifndef RB_LOGGING
#define RB_LOG if(0) std::cerr
#else
#include <fstream>
#include <string>

#ifndef __MAKECINT__

#define LOG_OP__(CLASS, ARGTYPE) CLASS& operator<< (ARGTYPE arg) {	\
    fStrm << arg; return *this;					\
  }
#define LOG_OPS__(CL)				\
  LOG_OP__(CL, const char*)			\
  LOG_OP__(CL, long long)			\
  LOG_OP__(CL, long)				\
  LOG_OP__(CL, int)				\
  LOG_OP__(CL, short)				\
  LOG_OP__(CL, char)				\
  LOG_OP__(CL, unsigned long long)		\
  LOG_OP__(CL, unsigned long)			\
  LOG_OP__(CL, unsigned int)			\
  LOG_OP__(CL, unsigned short)			\
  LOG_OP__(CL, unsigned char)			\
  LOG_OP__(CL, bool)				\
  LOG_OP__(CL, double)				\
  LOG_OP__(CL, float)				\
  LOG_OP__(CL, const std::string&)


class Logger
{
private:
  typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
  typedef CoutType& (*StandardEndLine)(CoutType&);
  const char* const kFileName;
  std::ofstream fStrm;
public:
  static Logger& Instance();
  LOG_OPS__(Logger)
  Logger& operator<<(StandardEndLine manip);
  ~Logger();
 
private:
  Logger(const char* fname);
};  

inline Logger::Logger(const char* fname): kFileName(fname),
					  fStrm(kFileName) { }

inline Logger::~Logger() {
  fStrm.close();
}

inline Logger& Logger::Instance() {
  static Logger logger("rbeer.log");
  return logger;
}

inline Logger& Logger::operator<<(StandardEndLine manip) {
  manip(fStrm);
  fStrm.flush();
  return *this;
}

#define RB_LOG  Logger::Instance() << "Logging in <" << __func__ << "> (" << __FILE__ << ", line " << __LINE__ << "): "

#endif // #ifdef RB_LOGGING

#endif // #ifndef __MAKECINT__
#endif // #ifndeg LOGGER_HXX
