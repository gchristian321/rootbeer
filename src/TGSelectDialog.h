//! \file TGSelectDialog.h
//! \brief Defines a pop-up window class for selecting a specific
//! canvas to \c cd() to.
#ifndef ROOT_TGSelectDialog
#define ROOT_TGSelectDialog

///////////////////////////////////////////////////////////////////////////
//                                                                       //
// Input Dialog Widget                                                   //
//                                                                       //
// An Input dialog box                                                   //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#include <string>
#include <vector>

class TGLabel;
class TGComboBox;
class TGTextEntry;
class TGTextButton;

//! Class to select a canvas to \c cd() to
/* Mostly copied and pasted from TGInputDialog.h, with modifications to create a
 * canvas selection menu instead.*/
class TGSelectDialog : public TGTransientFrame {

private:
   TGLabel          *fLabel;   // text entry label
   TGComboBox       *fCB;      // combo box
   TGTextButton     *fOk;      // ok button
   TGTextButton     *fCancel;  // cancel button
   int              *fRetInt;  // address to store return entry

   TGSelectDialog(const TGSelectDialog&);  // Not implemented
   TGSelectDialog &operator= (const TGSelectDialog&); // Not implemented

public:
   TGSelectDialog(const TGWindow *p = 0, const TGWindow *main = 0,
                 const char *prompt =0, const char *wintitle = 0,
								 const std::vector<std::string>* const entries = 0,
                 int *ret = 0, UInt_t options = kVerticalFrame);
   ~TGSelectDialog();
   virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);
	 virtual void RemoveAll();
	 virtual void	AddEntry(const char* s);
	 
   ClassDef(TGSelectDialog, 0)  // Simple input dialog

};

#endif
