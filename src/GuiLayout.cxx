// Mainframe macro generated from application: root.exe
// By ROOT version 5.32/01 on 2012-03-19 16:19:39

#ifndef ROOT_TGDockableFrame
#include "TGDockableFrame.h"
#endif
#ifndef ROOT_TGMenu
#include "TGMenu.h"
#endif
#ifndef ROOT_TGMdiDecorFrame
#include "TGMdiDecorFrame.h"
#endif
#ifndef ROOT_TG3DLine
#include "TG3DLine.h"
#endif
#ifndef ROOT_TGMdiFrame
#include "TGMdiFrame.h"
#endif
#ifndef ROOT_TGMdiMainFrame
#include "TGMdiMainFrame.h"
#endif
#ifndef ROOT_TGMdiMenu
#include "TGMdiMenu.h"
#endif
#ifndef ROOT_TGColorDialog
#include "TGColorDialog.h"
#endif
#ifndef ROOT_TGListBox
#include "TGListBox.h"
#endif
#ifndef ROOT_TGNumberEntry
#include "TGNumberEntry.h"
#endif
#ifndef ROOT_TGScrollBar
#include "TGScrollBar.h"
#endif
#ifndef ROOT_TGComboBox
#include "TGComboBox.h"
#endif
#ifndef ROOT_TGuiBldHintsEditor
#include "TGuiBldHintsEditor.h"
#endif
#ifndef ROOT_TGuiBldNameFrame
#include "TGuiBldNameFrame.h"
#endif
#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGFileDialog
#include "TGFileDialog.h"
#endif
#ifndef ROOT_TGShutter
#include "TGShutter.h"
#endif
#ifndef ROOT_TGButtonGroup
#include "TGButtonGroup.h"
#endif
#ifndef ROOT_TGCanvas
#include "TGCanvas.h"
#endif
#ifndef ROOT_TGFSContainer
#include "TGFSContainer.h"
#endif
#ifndef ROOT_TGFontDialog
#include "TGFontDialog.h"
#endif
#ifndef ROOT_TGuiBldEditor
#include "TGuiBldEditor.h"
#endif
#ifndef ROOT_TGColorSelect
#include "TGColorSelect.h"
#endif
#ifndef ROOT_TGTextEdit
#include "TGTextEdit.h"
#endif
#ifndef ROOT_TGButton
#include "TGButton.h"
#endif
#ifndef ROOT_TGFSComboBox
#include "TGFSComboBox.h"
#endif
#ifndef ROOT_TGLabel
#include "TGLabel.h"
#endif
#ifndef ROOT_TGMsgBox
#include "TGMsgBox.h"
#endif
#ifndef ROOT_TRootGuiBuilder
#include "TRootGuiBuilder.h"
#endif
#ifndef ROOT_TGTab
#include "TGTab.h"
#endif
#ifndef ROOT_TGListView
#include "TGListView.h"
#endif
#ifndef ROOT_TGStatusBar
#include "TGStatusBar.h"
#endif
#ifndef ROOT_TGView
#include "TGView.h"
#endif
#ifndef ROOT_TGuiBldGeometryFrame
#include "TGuiBldGeometryFrame.h"
#endif
#ifndef ROOT_TGToolBar
#include "TGToolBar.h"
#endif
#ifndef ROOT_TRootEmbeddedCanvas
#include "TRootEmbeddedCanvas.h"
#endif
#ifndef ROOT_TCanvas
#include "TCanvas.h"
#endif
#ifndef ROOT_TGuiBldDragManager
#include "TGuiBldDragManager.h"
#endif
#ifndef ROOT_TGObject
#include "TGObject.h"
#endif

#include "Riostream.h"

void GuiLayout()
{

   // main frame
   TGMainFrame *fMainFrame1053 = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
   fMainFrame1053->SetName("fMainFrame1053");
   fMainFrame1053->SetLayoutBroken(kTRUE);

   // composite frame
   TGCompositeFrame *fMainFrame1027 = new TGCompositeFrame(fMainFrame1053,490,372,kVerticalFrame);
   fMainFrame1027->SetName("fMainFrame1027");
   fMainFrame1027->SetLayoutBroken(kTRUE);

   // composite frame
   TGCompositeFrame *fMainFrame959 = new TGCompositeFrame(fMainFrame1027,490,372,kVerticalFrame);
   fMainFrame959->SetName("fMainFrame959");
   fMainFrame959->SetLayoutBroken(kTRUE);

   // composite frame
   TGCompositeFrame *fMainFrame1515 = new TGCompositeFrame(fMainFrame959,490,372,kVerticalFrame);
   fMainFrame1515->SetName("fMainFrame1515");
   fMainFrame1515->SetLayoutBroken(kTRUE);

   // composite frame
   TGCompositeFrame *fMainFrame895 = new TGCompositeFrame(fMainFrame1515,490,372,kVerticalFrame);
   fMainFrame895->SetName("fMainFrame895");
   fMainFrame895->SetLayoutBroken(kTRUE);

   // composite frame
   TGCompositeFrame *fMainFrame1781 = new TGCompositeFrame(fMainFrame895,490,372,kVerticalFrame);
   fMainFrame1781->SetName("fMainFrame1781");
   fMainFrame1781->SetLayoutBroken(kTRUE);

   // composite frame
   TGCompositeFrame *fMainFrame863 = new TGCompositeFrame(fMainFrame1781,490,372,kVerticalFrame);
   fMainFrame863->SetName("fMainFrame863");
   fMainFrame863->SetLayoutBroken(kTRUE);

   // composite frame
   TGCompositeFrame *fMainFrame1525 = new TGCompositeFrame(fMainFrame863,431,372,kVerticalFrame);
   fMainFrame1525->SetName("fMainFrame1525");
   fMainFrame1525->SetLayoutBroken(kTRUE);

   ULong_t ucolor;        // will reflect user color changes
   gClient->GetColorByName("#ffffff",ucolor);

   // composite frame
   TGCompositeFrame *fRbeerFrame = new TGCompositeFrame(fMainFrame1525,421,368,kVerticalFrame,ucolor);
   fRbeerFrame->SetName("fRbeerFrame");
   fRbeerFrame->SetLayoutBroken(kTRUE);

   TGFont *ufont;         // will reflect user font changes
   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   TGGC   *uGC;           // will reflect user GC changes
   // graphics context changes
   GCValues_t valchOnline;
   valchOnline.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valchOnline.fForeground);
   gClient->GetColorByName("#e0e0e0",valchOnline.fBackground);
   valchOnline.fFillStyle = kFillSolid;
   valchOnline.fFont = ufont->GetFontHandle();
   valchOnline.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valchOnline, kTRUE);

   gClient->GetColorByName("#0000ff",ucolor);
   TGTextButton *fAttachOnline = new TGTextButton(fRbeerFrame,"Attach Online",-1,uGC->GetGC());
   fAttachOnline->SetTextJustify(36);
   fAttachOnline->SetMargins(0,0,0,0);
   fAttachOnline->SetWrapLength(-1);
   fAttachOnline->Resize(91,24);

   fAttachOnline->ChangeBackground(ucolor);
   fRbeerFrame->AddFrame(fAttachOnline, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fAttachOnline->MoveResize(16,16,91,24);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valchFile;
   valchFile.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valchFile.fForeground);
   gClient->GetColorByName("#e0e0e0",valchFile.fBackground);
   valchFile.fFillStyle = kFillSolid;
   valchFile.fFont = ufont->GetFontHandle();
   valchFile.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valchFile, kTRUE);

   TGTextButton *fAttachFile = new TGTextButton(fRbeerFrame,"Attach File",-1,uGC->GetGC());
   fAttachFile->SetTextJustify(36);
   fAttachFile->SetMargins(0,0,0,0);
   fAttachFile->SetWrapLength(-1);
   fAttachFile->Resize(91,24);

   fAttachFile->ChangeBackground(ucolor);
   fRbeerFrame->AddFrame(fAttachFile, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fAttachFile->MoveResize(16,48,91,24);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valchList;
   valchList.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valchList.fForeground);
   gClient->GetColorByName("#e0e0e0",valchList.fBackground);
   valchList.fFillStyle = kFillSolid;
   valchList.fFont = ufont->GetFontHandle();
   valchList.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valchList, kTRUE);

   TGTextButton *fAttachList = new TGTextButton(fRbeerFrame,"Attach List",-1,uGC->GetGC());
   fAttachList->SetTextJustify(36);
   fAttachList->SetMargins(0,0,0,0);
   fAttachList->SetWrapLength(-1);
   fAttachList->Resize(91,24);

   fAttachList->ChangeBackground(ucolor);
   fRbeerFrame->AddFrame(fAttachList, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fAttachList->MoveResize(16,80,91,24);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valctHost;
   valctHost.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valctHost.fForeground);
   gClient->GetColorByName("#e0e0e0",valctHost.fBackground);
   valctHost.fFillStyle = kFillSolid;
   valctHost.fFont = ufont->GetFontHandle();
   valctHost.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valctHost, kTRUE);
   TGTextEntry *fSelectHost = new TGTextEntry(fRbeerFrame, new TGTextBuffer(14),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fSelectHost->SetMaxLength(4096);
   fSelectHost->SetAlignment(kTextLeft);
   fSelectHost->SetText("localhost");
   fSelectHost->Resize(84,fSelectHost->GetDefaultHeight());
   fRbeerFrame->AddFrame(fSelectHost, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fSelectHost->MoveResize(112,16,84,22);
   TGCheckButton *fIsContinuous = new TGCheckButton(fRbeerFrame,"Continuous");

   gClient->GetColorByName("#ffffff",ucolor);
   fIsContinuous->ChangeBackground(ucolor);
   fIsContinuous->SetTextJustify(36);
   fIsContinuous->SetMargins(0,0,0,0);
   fIsContinuous->SetWrapLength(-1);
   fRbeerFrame->AddFrame(fIsContinuous, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fIsContinuous->MoveResize(112,56,88,19);
   TGTextButton *fUnattach = new TGTextButton(fRbeerFrame,"Unattach");
   fUnattach->SetTextJustify(36);
   fUnattach->SetMargins(0,0,0,0);
   fUnattach->SetWrapLength(-1);
   fUnattach->Resize(91,24);
   fRbeerFrame->AddFrame(fUnattach, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fUnattach->MoveResize(16,112,91,24);

   fMainFrame1525->AddFrame(fRbeerFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fRbeerFrame->MoveResize(8,0,421,368);

   fMainFrame863->AddFrame(fMainFrame1525, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame1525->MoveResize(0,0,431,372);

   fMainFrame1781->AddFrame(fMainFrame863, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame863->MoveResize(0,0,490,372);

   fMainFrame895->AddFrame(fMainFrame1781, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame1781->MoveResize(0,0,490,372);

   fMainFrame1515->AddFrame(fMainFrame895, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame895->MoveResize(0,0,490,372);

   fMainFrame959->AddFrame(fMainFrame1515, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame1515->MoveResize(0,0,490,372);

   fMainFrame1027->AddFrame(fMainFrame959, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame959->MoveResize(0,0,490,372);

   fMainFrame1053->AddFrame(fMainFrame1027, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame1027->MoveResize(0,0,490,372);

   fMainFrame1053->SetMWMHints(kMWMDecorAll,
                        kMWMFuncAll,
                        kMWMInputModeless);
   fMainFrame1053->MapSubwindows();

   fMainFrame1053->Resize(fMainFrame1053->GetDefaultSize());
   fMainFrame1053->MapWindow();
   fMainFrame1053->Resize(490,372);
}  
