//! \file Signals.hxx
//! \brief Defines a class used to send and recieve signals from the rootbeer gui.
#ifndef SIGNALS_HXX
#define SIGNALS_HXX
#include <TQObject.h>

namespace rb
{

class Signals: public TQObject
{
public:
	 void Unattaching(); //*SIGNAL*
	 void Attaching(); //*SIGNAL*
	 void AttachOnline(); //*SIGNAL*
	 void AttachFile(); //*SIGNAL*
	 void AttachList(); //*SIGNAL*
	 void Unattach(); //*SIGNAL*
	 void UpdateAll(); //*SIGNAL*
	 void UpdateCurrent(); //*SIGNAL*
	 void ClearAll(); //*SIGNAL*
	 void ClearCurrent(); //*SIGNAL*
	 ClassDef(rb::Signals, 0);
};

}

inline void rb::Signals::Unattaching() {
	Emit("Unattaching()");
}
inline void rb::Signals::Attaching() {
	Emit("Attaching()");
}


#endif
