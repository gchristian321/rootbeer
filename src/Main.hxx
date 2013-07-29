//! \file Main.hxx
//! \brief Defines a base class to implement the main() function
#ifndef RB_MAIN_HEADER
#define RB_MAIN_HEADER

namespace rb {

class Main
{
public:
	//! \brief Empty constructor
	Main() {}
	//! \brief Empty destructor
	virtual ~Main() {}
	//! \brief The main() function
	virtual int Run(int argc, char** argv);
};

//! \brief Abstract function to return a new rb::Main instance.
//! \details Must be implemented by the user. If deault behavior is s desired,
//! simply have this function be:
//! \code
//!  rb::Main* rb::GetMain() { return new rb::Main(); }
//! \endcode
rb::Main* GetMain();

}



#endif
