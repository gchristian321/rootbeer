// @(#)root/gui:$Id: TGSelectDialog.cxx 40398 2011-07-29 07:37:23Z bellenot $
// Author: David Gonzalez Maline  19/07/2006

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Input Dialog Widget                                                   //
//                                                                       //
// An Input dialog box                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TGSelectDialog.h"
#include "TGButton.h"
#include "TGLabel.h"
#include "TGTextEntry.h"
#include "TGComboBox.h"

ClassImp(TGSelectDialog)


//______________________________________________________________________________
TGSelectDialog::TGSelectDialog(const TGWindow *p, const TGWindow *main, 
															 const char *prompt, const char *wintitle,
															 const std::vector<std::string>* const entries,
															 int *retint, UInt_t options) :
      TGTransientFrame(p, main, 10, 10, options)
{
   /** Create simple input dialog.  

   It is important to know that the case where the constructor in
   which all the variables are initialized to their default values is
   only used for the TBrowser to inspect on the classes. For normal
   use the only variable that should be free is options.

   Variables prompt, wintitle are the content of the input dialog while
   retstr has to be initialized to a char[256]. In case these are not
   initialized, they will show default values while retstr will be
   automatically allocated by the dialog. However this will make
   impossible to retrieve the value entered by the dialog.

   To see TGSelectDialog in use see:
   $ROOTSYS/tutorials/testInputDialog.cxx
   */

   if (!p && !main) {
      MakeZombie();
      return;
   }
   SetCleanup(kDeepCleanup);
   // create prompt label and textentry widget
   fLabel = new TGLabel(this, prompt?prompt:"Introduce value:");

   TGTextBuffer *tbuf = new TGTextBuffer(256);  //will be deleted by TGtextEntry
   tbuf->AddText(0, wintitle?wintitle:"");

   fCB = new TGComboBox(this,-1,kHorizontalFrame | kSunkenFrame | kDoubleBorder | kOwnBackground);
	 if(entries) {
		 for(size_t i=0; i< entries->size(); ++i) {
			 AddEntry(entries->operator[](i).c_str());
		 }
	 }
   fCB->Resize(260, 22);

   AddFrame(fLabel, new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 0));
   AddFrame(fCB, new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 5));

   // create frame and layout hints for Ok and Cancel buttons
   TGHorizontalFrame *hf = new TGHorizontalFrame(this, 60, 20, kFixedWidth);
   hf->SetCleanup(kDeepCleanup);

   // create OK and Cancel buttons in their own frame (hf)
   UInt_t  width = 0, height = 0;

   fOk = new TGTextButton(hf, "&Ok", 1);
   fOk->Associate(this);
   hf->AddFrame(fOk, new TGLayoutHints(kLHintsCenterY | kLHintsExpandX, 5, 5, 0, 0));
   height = fOk->GetDefaultHeight();
   width  = TMath::Max(width, fOk->GetDefaultWidth());

   fCancel = new TGTextButton(hf, "&Cancel", 2);
   fCancel->Associate(this);
   hf->AddFrame(fCancel, new TGLayoutHints(kLHintsCenterY | kLHintsExpandX, 5, 5, 0, 0));
   height = fCancel->GetDefaultHeight();
   width  = TMath::Max(width, fCancel->GetDefaultWidth());

   // place button frame (hf) at the bottom
   AddFrame(hf, new TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5));

   // keep buttons centered and with the same width
   hf->Resize((width + 20) * 2, height);

   // set dialog title
   SetWindowName(wintitle);

   // map all widgets and calculate size of dialog
   MapSubwindows();

   width  = GetDefaultWidth();
   height = GetDefaultHeight();

   Resize(width, height);

   // position relative to the parent's window
   CenterOnParent();

   // make the message box non-resizable
   SetWMSize(width, height);
   SetWMSizeHints(width, height, width, height, 0, 0);

   SetMWMHints(kMWMDecorAll | kMWMDecorResizeH | kMWMDecorMaximize |
               kMWMDecorMinimize | kMWMDecorMenu, kMWMFuncAll | 
               kMWMFuncResize | kMWMFuncMaximize | kMWMFuncMinimize,
               kMWMInputModeless);

	 fCB->Select(0);
   // popup dialog and wait till user replies
   MapWindow();
//   fCB->SetFocus();

   // if (retint == 0)
   //    retint = new int(0);

   fRetInt = retint;

   gClient->WaitFor(this);
}

//______________________________________________________________________________
TGSelectDialog::~TGSelectDialog()
{
   // Cleanup dialog.

   Cleanup();
}

//______________________________________________________________________________
Bool_t TGSelectDialog::ProcessMessage(Long_t msg, Long_t parm1, Long_t)
{
   // Handle button and text enter events

   switch (GET_MSG(msg)) {
      case kC_COMMAND:
         switch (GET_SUBMSG(msg)) {
            case kCM_BUTTON:
               switch (parm1) {
                  case 1:
                     // here copy the string from text buffer to return variable
                     // coverity[secure_coding]
										if(fRetInt)
											 *fRetInt = fCB->GetSelected();
										// if(fCB->GetTextEntry()->GetText())
										// 	 strcpy(fRetStr, fCB->GetTextEntry()->GetText());
										// else
										// 	 strcpy(fRetStr, "");
                     // if user selected an empty string, set the second 
                     // char to 1,in order to distinguish between empty string 
                     // selected with OK and Cancel button pressed
                     // if (!strcmp(fRetStr, ""))
                     //    fRetStr[1] = 1;
                     delete this;
                     break;
                  case 2:
                     // hack to detect the case where the user pressed the 
                     // Cancel button
                     // fRetInt[0] = 0;
                     // fRetStr[1] = 0;
                     delete this;
                     break;
               }
               default:
                  break;
         }
         break;

      // case kC_TEXTENTRY:
      //    switch (GET_SUBMSG(msg)) {
      //       case kTE_ENTER:
      //          // here copy the string from text buffer to return variable
      //          // coverity[secure_coding]
  		// 				 strcpy(fRetStr, fCB->GetTextEntry()->GetText());
      //          // if user selected an empty string, set the second 
      //          // char to 1,in order to distinguish between empty string 
      //          // selected with OK and Cancel button pressed
      //          if (!strcmp(fRetStr, ""))
      //             fRetStr[1] = 1;
      //          delete this;
      //          break;
      //       default:
      //          break;
      //    }
      //    break;

      default:
         break;
   }
   return kTRUE;
}

void TGSelectDialog::RemoveAll() {
	fCB->RemoveAll();
}

void TGSelectDialog::AddEntry(const char* s) {
	fCB->AddEntry(s, fCB->GetNumberOfEntries());
}
