#include "TGDivideSelect.h"
#include "TGButton.h"
#include "TGLabel.h"
#include "TGNumberEntry.h"
#include "TGComboBox.h"


//______________________________________________________________________________
TGDivideSelect::TGDivideSelect(const TGWindow *p, const TGWindow *main, 
															 int* ret_h, int* ret_v, UInt_t options) :
      TGTransientFrame(p, main, 10, 10, options)
{
   /** Create simple input dialog.  

   It is important to know that the case where the constructor in
   which all the variables are initialized to their default values is
   only used for the TBrowser to inspect on the classes. For normal
   use the only variable that should be free is options.

   Variables prompt, defval are the content of the input dialog while
   retstr has to be initialized to a char[256]. In case these are not
   initialized, they will show default values while retstr will be
   automatically allocated by the dialog. However this will make
   impossible to retrieve the value entered by the dialog.

   To see TGDivideSelect in use see:
   $ROOTSYS/tutorials/testInputDialog.cxx
   */

   if (!p && !main) {
      MakeZombie();
      return;
   }
   SetCleanup(kDeepCleanup);


	 TGHorizontalFrame *hfL = new TGHorizontalFrame(this, 60, 10, kFixedWidth);
   hfL->SetCleanup(kDeepCleanup);
	 fLabelH = new TGLabel(hfL, "Horizontal");
	 fLabelV = new TGLabel(hfL, "Vertical");
   hfL->AddFrame(fLabelH, new TGLayoutHints(kLHintsBottom | kLHintsLeft, 5, 5, 5, 5));
   hfL->AddFrame(fLabelV, new TGLayoutHints(kLHintsBottom | kLHintsRight, 5, 5, 5, 5));


	 TGHorizontalFrame *hf0 = new TGHorizontalFrame(this, 60, 20, kFixedWidth);
   hf0->SetCleanup(kDeepCleanup);


   fNEH= new TGNumberEntryField(hf0);
	 fNEH->Associate(this);
   fNEH->Resize(29, 22);
	 fNEH->SetLimits(TGNumberFormat::kNELLimitMinMax, 1, 99);
   hf0->AddFrame(fNEH, new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 5));

	 fX =  new TGLabel(hf0,"x");
	 fX->SetTextFont("Arial 10 bold");
	 hf0->AddFrame(fX, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 5, 5, 5, 5));

   fNEV= new TGNumberEntryField(hf0);
	 fNEV->Associate(this);
   fNEV->Resize(29, 22);
	 fNEV->SetLimits(TGNumberFormat::kNELLimitMinMax, 1, 99);
   hf0->AddFrame(fNEV, new TGLayoutHints(kLHintsTop | kLHintsRight, 5, 5, 5, 5));


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


   AddFrame(hfL, new TGLayoutHints(kLHintsTop | kLHintsCenterX, 0, 0, 5, 5));
   hfL->Resize((width + 10) * 2, height);

   AddFrame(hf0, new TGLayoutHints(kLHintsTop | kLHintsCenterX, 0, 0, 5, 5));
   hf0->Resize((width + 10) * 2, height);

   // place button frame (hf) at the bottom
   AddFrame(hf, new TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5));

   // keep buttons centered and with the same width
   hf->Resize((width + 20) * 2, height);

   // set dialog title
//   SetWindowName("Select Divisions");

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

   // popup dialog and wait till user replies
   MapWindow();
   fNEH->SetFocus();

	 fRetH = ret_h;
	 fRetV = ret_v;

   gClient->WaitFor(this);
}

//______________________________________________________________________________
TGDivideSelect::~TGDivideSelect()
{
   // Cleanup dialog.

   Cleanup();
}

//______________________________________________________________________________
Bool_t TGDivideSelect::ProcessMessage(Long_t msg, Long_t parm1, Long_t)
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
 										 if(fRetH) *fRetH = fNEH->GetIntNumber();
										 if(fRetV) *fRetV = fNEV->GetIntNumber();

                     delete this;
                     break;
                  case 2:
                     // hack to detect the case where the user pressed the 
                     // Cancel button
                     delete this;
                     break;
               }
               default:
                  break;
         }
         break;

      case kC_TEXTENTRY:
         switch (GET_SUBMSG(msg)) {
            case kTE_ENTER:
               // here copy the string from text buffer to return variable
               // coverity[secure_coding]
 							 if(fRetH) *fRetH = fNEH->GetIntNumber();
							 if(fRetV) *fRetV = fNEV->GetIntNumber();
               delete this;
               break;
            default:
               break;
         }
         break;

      default:
         break;
   }
   return kTRUE;
}
