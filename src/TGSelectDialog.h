// @(#)root/gui:$Id: TGSelectDialog.h 23115 2008-04-10 13:35:37Z rdm $
// Author: David Gonzalez Maline  19/07/2006

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

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
