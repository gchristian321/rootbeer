// Mainframe macro generated from application: root.exe
// By ROOT version 5.32/01 on 2012-03-22 21:01:12

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
#ifndef ROOT_TGColorSelect
#include "TGColorSelect.h"
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
#ifndef ROOT_TGSplitter
#include "TGSplitter.h"
#endif
#ifndef ROOT_TGStatusBar
#include "TGStatusBar.h"
#endif
#ifndef ROOT_TGListTree
#include "TGListTree.h"
#endif
#ifndef ROOT_TGuiBldGeometryFrame
#include "TGuiBldGeometryFrame.h"
#endif
#ifndef ROOT_TGToolTip
#include "TGToolTip.h"
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

#include "Riostream.h"

void GuiLayout()
{

   TGFont *ufont;         // will reflect user font changes
   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // main frame
   TGMainFrame *fRbeerFrame = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
   fRbeerFrame->SetName("fRbeerFrame");
   fRbeerFrame->SetLayoutBroken(kTRUE);
	 fRbeerFrame->ChangeBackground((Pixel_t)0xd4cf87);

	 
   // composite frame
   TGCompositeFrame *fMainFrame1596 = new TGCompositeFrame(fRbeerFrame,349,426,kVerticalFrame);
   fMainFrame1596->SetName("fMainFrame1596");
   fMainFrame1596->SetLayoutBroken(kTRUE);
	 fMainFrame1596->ChangeBackground((Pixel_t)0xd4cf87);

   ULong_t ucolor;        // will reflect user color changes

   // composite frame
   TGCompositeFrame *fMainFrame6310 = new TGCompositeFrame(fMainFrame1596,352,428,kVerticalFrame,ucolor);
   fMainFrame6310->SetName("fMainFrame6310");
   fMainFrame6310->SetLayoutBroken(kTRUE);
	 fMainFrame6310->ChangeBackground((Pixel_t)0xd4cf87);

   // "Canvas" group frame
   TGGroupFrame *fGroupCanvas = new TGGroupFrame(fMainFrame6310,"Canvas",kVerticalFrame,TGGroupFrame::GetDefaultGC()(),TGGroupFrame::GetDefaultFontStruct(),ucolor);
   fGroupCanvas->SetTitlePos(TGGroupFrame::kCenter);
   fGroupCanvas->SetLayoutBroken(kTRUE);
	 fGroupCanvas->ChangeBackground((Pixel_t)0xd4cf87);
   TGTextButton *fZeroAll = new TGTextButton(fGroupCanvas,"Zero All");
   fZeroAll->SetTextJustify(36);
   fZeroAll->SetMargins(0,0,0,0);
   fZeroAll->SetWrapLength(-1);
   fZeroAll->Resize(91,24);
   fGroupCanvas->AddFrame(fZeroAll, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fZeroAll->MoveResize(110,100,95,24);
   TGTextButton *fDivideCurrent = new TGTextButton(fGroupCanvas,"Divide Current");
   fDivideCurrent->SetTextJustify(36);
   fDivideCurrent->SetMargins(0,0,0,0);
   fDivideCurrent->SetWrapLength(-1);
   fDivideCurrent->Resize(91,24);
   fGroupCanvas->AddFrame(fDivideCurrent, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fDivideCurrent->MoveResize(8,132,95,24);

   TGGC   *uGC;           // will reflect user GC changes
   // graphics context changes
   GCValues_t valEntry1852;
   valEntry1852.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valEntry1852.fForeground);
   gClient->GetColorByName("#e0e0e0",valEntry1852.fBackground);
   valEntry1852.fFillStyle = kFillSolid;
   valEntry1852.fFont = ufont->GetFontHandle();
   valEntry1852.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valEntry1852, kTRUE);

   TGTextEntry *fEntryName = new TGTextEntry(fGroupCanvas, new TGTextBuffer(14),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fEntryName->SetMaxLength(4096);
   fEntryName->SetAlignment(kTextLeft);
   fEntryName->SetText("");
   fEntryName->Resize(80,fEntryName->GetDefaultHeight());
   fGroupCanvas->AddFrame(fEntryName, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fEntryName->MoveResize(208,167,80,22);

   gClient->GetColorByName("#00ff00",ucolor);
   TGTextButton *fCreateNew = new TGTextButton(fGroupCanvas,"Create New");
   fCreateNew->SetTextJustify(36);
   fCreateNew->SetMargins(0,0,0,0);
   fCreateNew->SetWrapLength(-1);
   fCreateNew->Resize(91,24);

   fCreateNew->ChangeBackground(ucolor);
   fGroupCanvas->AddFrame(fCreateNew, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fCreateNew->MoveResize(110,165,95,24);

   gClient->GetColorByName("#d4cf87",ucolor);
   TGLabel *fLabelName = new TGLabel(fGroupCanvas,"Name:",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);
   fLabelName->SetTextJustify(33);
   fLabelName->SetMargins(0,0,0,0);
   fLabelName->SetWrapLength(-1);
   fGroupCanvas->AddFrame(fLabelName, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelName->MoveResize(210,145,35,22);

   TGTextButton *fSelectCanvas = new TGTextButton(fGroupCanvas,"Cd");
   fSelectCanvas->SetTextJustify(36);
   fSelectCanvas->SetMargins(0,0,0,0);
   fSelectCanvas->SetWrapLength(-1);
   fSelectCanvas->Resize(91,24);
   fGroupCanvas->AddFrame(fSelectCanvas, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fSelectCanvas->MoveResize(110,132,95,24);
   TGNumberEntry *fUpdateRate = new TGNumberEntry(fGroupCanvas, (Double_t) 0,6,-1,(TGNumberFormat::EStyle) 5);
   fGroupCanvas->AddFrame(fUpdateRate, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fUpdateRate->MoveResize(110,28,59,22);

   TGLabel *fLabelRate = new TGLabel(fGroupCanvas,"Rate [sec.]",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);
   fLabelRate->SetTextJustify(36);
   fLabelRate->SetMargins(0,0,0,0);
   fLabelRate->SetWrapLength(-1);
   fGroupCanvas->AddFrame(fLabelRate, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelRate->MoveResize(174,28,64,18);

   gClient->GetColorByName("#00ff00",ucolor);
   TGTextButton *fStartRefresh = new TGTextButton(fGroupCanvas,"Start Refresh");
   fStartRefresh->SetTextJustify(36);
   fStartRefresh->SetMargins(0,0,0,0);
   fStartRefresh->SetWrapLength(-1);
   fStartRefresh->Resize(91,24);

   fStartRefresh->ChangeBackground(ucolor);
   fGroupCanvas->AddFrame(fStartRefresh, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fStartRefresh->MoveResize(8,28,95,24);
   TGTextButton *fRefreshCurrent = new TGTextButton(fGroupCanvas,"Refresh Current");
   fRefreshCurrent->SetTextJustify(36);
   fRefreshCurrent->SetMargins(0,0,0,0);
   fRefreshCurrent->SetWrapLength(-1);
   fRefreshCurrent->Resize(91,24);
   fGroupCanvas->AddFrame(fRefreshCurrent, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fRefreshCurrent->MoveResize(8,68,95,24);
   TGTextButton *fRefreshAll = new TGTextButton(fGroupCanvas,"Refresh All");
   fRefreshAll->SetTextJustify(36);
   fRefreshAll->SetMargins(0,0,0,0);
   fRefreshAll->SetWrapLength(-1);
   fRefreshAll->Resize(91,24);
   fGroupCanvas->AddFrame(fRefreshAll, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fRefreshAll->MoveResize(110,68,95,24);
   TGTextButton *fZeroCurrent = new TGTextButton(fGroupCanvas,"Zero Current");
   fZeroCurrent->SetTextJustify(36);
   fZeroCurrent->SetMargins(0,0,0,0);
   fZeroCurrent->SetWrapLength(-1);
   fZeroCurrent->Resize(91,24);
   fGroupCanvas->AddFrame(fZeroCurrent, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fZeroCurrent->MoveResize(8,100,95,24);
   TGTextButton *fClearCurrent = new TGTextButton(fGroupCanvas,"Clear Canvas");
   fClearCurrent->SetTextJustify(36);
   fClearCurrent->SetMargins(0,0,0,0);
   fClearCurrent->SetWrapLength(-1);
   fClearCurrent->Resize(91,24);
   fGroupCanvas->AddFrame(fClearCurrent, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fClearCurrent->MoveResize(8,165,95,24);

   gClient->GetColorByName("#d4cf87",ucolor);

   // "Configuration" group frame
   TGGroupFrame *fGroupConfig = new TGGroupFrame(fGroupCanvas,"Configuration",kVerticalFrame,TGGroupFrame::GetDefaultGC()(),TGGroupFrame::GetDefaultFontStruct(),ucolor);
   fGroupConfig->SetTitlePos(TGGroupFrame::kCenter);
   fGroupConfig->SetLayoutBroken(kTRUE);

   gClient->GetColorByName("#ffcc00",ucolor);
   TGTextButton *fConfigLoad = new TGTextButton(fGroupConfig,"Load");
   fConfigLoad->SetTextJustify(36);
   fConfigLoad->SetMargins(0,0,0,0);
   fConfigLoad->SetWrapLength(-1);
   fConfigLoad->Resize(64,24);

   fConfigLoad->ChangeBackground(ucolor);
   fGroupConfig->AddFrame(fConfigLoad, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fConfigLoad->MoveResize(12,56,64,24);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valButton1868;
   valButton1868.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valButton1868.fForeground);
   gClient->GetColorByName("#e0e0e0",valButton1868.fBackground);
   valButton1868.fFillStyle = kFillSolid;
   valButton1868.fFont = ufont->GetFontHandle();
   valButton1868.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valButton1868, kTRUE);

   gClient->GetColorByName("#0000ff",ucolor);
   TGTextButton *fConfigSave = new TGTextButton(fGroupConfig,"Save",-1,uGC->GetGC());
   fConfigSave->SetTextJustify(36);
   fConfigSave->SetMargins(0,0,0,0);
   fConfigSave->SetWrapLength(-1);
   fConfigSave->Resize(64,24);

   fConfigSave->ChangeBackground(ucolor);
   fGroupConfig->AddFrame(fConfigSave, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fConfigSave->MoveResize(12,24,64,24);

   fGroupConfig->SetLayoutManager(new TGVerticalLayout(fGroupConfig));
   fGroupConfig->Resize(88,96);
   fGroupCanvas->AddFrame(fGroupConfig, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fGroupConfig->MoveResize(228,55,88,96);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valEntry1869;
   valEntry1869.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valEntry1869.fForeground);
   gClient->GetColorByName("#e0e0e0",valEntry1869.fBackground);
   valEntry1869.fFillStyle = kFillSolid;
   valEntry1869.fFont = ufont->GetFontHandle();
   valEntry1869.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valEntry1869, kTRUE);

   fGroupCanvas->SetLayoutManager(new TGVerticalLayout(fGroupCanvas));
   fGroupCanvas->Resize(336,208);
   fMainFrame6310->AddFrame(fGroupCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fGroupCanvas->MoveResize(8,217,336,208);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valpFrame1870;
   valpFrame1870.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valpFrame1870.fForeground);
   gClient->GetColorByName("#e0e0e0",valpFrame1870.fBackground);
   valpFrame1870.fFillStyle = kFillSolid;
   valpFrame1870.fFont = ufont->GetFontHandle();
   valpFrame1870.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valpFrame1870, kTRUE);

   gClient->GetColorByName("#d4cf87",ucolor);

   // "Data" group frame
   TGGroupFrame *fGroupData = new TGGroupFrame(fMainFrame6310,"Data",kVerticalFrame | kRaisedFrame,uGC->GetGC(),ufont->GetFontStruct(),ucolor);
   fGroupData->SetTitlePos(TGGroupFrame::kCenter);
   fGroupData->SetLayoutBroken(kTRUE);

   gClient->GetColorByName("#ffcc00",ucolor);
   TGTextButton *fAttachOnline = new TGTextButton(fGroupData,"Attach Online");
   fAttachOnline->SetTextJustify(36);
   fAttachOnline->SetMargins(0,0,0,0);
   fAttachOnline->SetWrapLength(-1);
   fAttachOnline->Resize(91,24);

   fAttachOnline->ChangeBackground(ucolor);
   fGroupData->AddFrame(fAttachOnline, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fAttachOnline->MoveResize(8,32,95,24);

   TGTextButton *fAttachFile = new TGTextButton(fGroupData,"Attach File");
   fAttachFile->SetTextJustify(36);
   fAttachFile->SetMargins(0,0,0,0);
   fAttachFile->SetWrapLength(-1);
   fAttachFile->Resize(91,24);

   fAttachFile->ChangeBackground(ucolor);
   fGroupData->AddFrame(fAttachFile, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fAttachFile->MoveResize(8,72,95,24);

   TGTextButton *fAttachList = new TGTextButton(fGroupData,"Attach List");
   fAttachList->SetTextJustify(36);
   fAttachList->SetMargins(0,0,0,0);
   fAttachList->SetWrapLength(-1);
   fAttachList->Resize(91,24);

   fAttachList->ChangeBackground(ucolor);
   fGroupData->AddFrame(fAttachList, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fAttachList->MoveResize(8,104,95,24);
   TGTextButton *fUnattach = new TGTextButton(fGroupData,"Unattach");
   fUnattach->SetTextJustify(36);
   fUnattach->SetMargins(0,0,0,0);
   fUnattach->SetWrapLength(-1);
   fUnattach->Resize(91,24);
   fGroupData->AddFrame(fUnattach, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fUnattach->MoveResize(8,136,95,24);

   // graphics context changes
   GCValues_t valButton1875;
   valButton1875.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valButton1875.fForeground);
   gClient->GetColorByName("#e0e0e0",valButton1875.fBackground);
   valButton1875.fFillStyle = kFillSolid;
   valButton1875.fFont = ufont->GetFontHandle();
   valButton1875.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valButton1875, kTRUE);

   gClient->GetColorByName("#ff0000",ucolor);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valEntry1876;
   valEntry1876.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valEntry1876.fForeground);
   gClient->GetColorByName("#e0e0e0",valEntry1876.fBackground);
   valEntry1876.fFillStyle = kFillSolid;
   valEntry1876.fFont = ufont->GetFontHandle();
   valEntry1876.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valEntry1876, kTRUE);
   TGTextEntry *fEntryHost = new TGTextEntry(fGroupData, new TGTextBuffer(14),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fEntryHost->SetMaxLength(4096);
   fEntryHost->SetAlignment(kTextLeft);
   fEntryHost->SetText("");
   fEntryHost->Resize(176,fEntryHost->GetDefaultHeight());
   fGroupData->AddFrame(fEntryHost, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fEntryHost->MoveResize(144,24,176,22);

   gClient->GetColorByName("#d4cf87",ucolor);
   TGLabel *fLabelHost = new TGLabel(fGroupData,"Host:",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);
   fLabelHost->SetTextJustify(36);
   fLabelHost->SetMargins(0,0,0,0);
   fLabelHost->SetWrapLength(-1);
   fGroupData->AddFrame(fLabelHost, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelHost->MoveResize(104,24,32,18);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valEntry1878;
   valEntry1878.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valEntry1878.fForeground);
   gClient->GetColorByName("#e0e0e0",valEntry1878.fBackground);
   valEntry1878.fFillStyle = kFillSolid;
   valEntry1878.fFont = ufont->GetFontHandle();
   valEntry1878.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valEntry1878, kTRUE);
   TGTextEntry *fEntryPort = new TGTextEntry(fGroupData, new TGTextBuffer(14),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fEntryPort->SetMaxLength(4096);
   fEntryPort->SetAlignment(kTextLeft);
   fEntryPort->SetText("");
   fEntryPort->Resize(88,fEntryPort->GetDefaultHeight());
   fGroupData->AddFrame(fEntryPort, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fEntryPort->MoveResize(144,48,88,22);

   TGLabel *fLabelPort = new TGLabel(fGroupData,"Expt:",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);
   fLabelPort->SetTextJustify(36);
   fLabelPort->SetMargins(0,0,0,0);
   fLabelPort->SetWrapLength(-1);
   fGroupData->AddFrame(fLabelPort, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelPort->MoveResize(104,48,32,18);

   TGCheckButton *fIsContinuous = new TGCheckButton(fGroupData,"Continuous");
   fIsContinuous->ChangeBackground(ucolor);
   fIsContinuous->SetTextJustify(36);
   fIsContinuous->SetMargins(0,0,0,0);
   fIsContinuous->SetWrapLength(-1);
   fGroupData->AddFrame(fIsContinuous, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fIsContinuous->MoveResize(104,80,88,19);

   TGCheckButton *fSaveData = new TGCheckButton(fGroupData,"Save Data");
   fSaveData->ChangeBackground(ucolor);
   fSaveData->SetTextJustify(36);
   fSaveData->SetMargins(0,0,0,0);
   fSaveData->SetWrapLength(-1);
   fGroupData->AddFrame(fSaveData, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fSaveData->MoveResize(140,125,88,19);

   TGCheckButton *fSaveHist = new TGCheckButton(fGroupData,"Save Histograms");
   fSaveHist->ChangeBackground(ucolor);
   fSaveHist->SetTextJustify(36);
   fSaveHist->SetMargins(0,0,0,0);
   fSaveHist->SetWrapLength(-1);
   fGroupData->AddFrame(fSaveHist, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fSaveHist->MoveResize(140,143,118,19);


   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t vall1881;
   vall1881.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#0000ff",vall1881.fForeground);
   gClient->GetColorByName("#e0e0e0",vall1881.fBackground);
   vall1881.fFillStyle = kFillSolid;
   vall1881.fFont = ufont->GetFontHandle();
   vall1881.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall1881, kTRUE);

   TGLabel *fLabelSource = new TGLabel(fGroupData,"[none]",uGC->GetGC(),ufont->GetFontStruct(),kChildFrame,ucolor);
   fLabelSource->SetTextJustify(17);
   fLabelSource->SetMargins(0,0,0,0);
   fLabelSource->SetWrapLength(-1);
   fGroupData->AddFrame(fLabelSource, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelSource->MoveResize(24,184,300,18);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t vall1882;
   vall1882.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#0000ff",vall1882.fForeground);
   gClient->GetColorByName("#e0e0e0",vall1882.fBackground);
   vall1882.fFillStyle = kFillSolid;
   vall1882.fFont = ufont->GetFontHandle();
   vall1882.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall1882, kTRUE);

   TGLabel *fLabelDataSource = new TGLabel(fGroupData,"Data source:",uGC->GetGC(),ufont->GetFontStruct(),kChildFrame,ucolor);
   fLabelDataSource->SetTextJustify(17);
   fLabelDataSource->SetMargins(0,0,0,0);
   fLabelDataSource->SetWrapLength(-1);
   fGroupData->AddFrame(fLabelDataSource, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelDataSource->MoveResize(8,166,80,18);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t vall1883;
   vall1883.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#0000ff",vall1883.fForeground);
   gClient->GetColorByName("#e0e0e0",vall1883.fBackground);
   vall1883.fFillStyle = kFillSolid;
   vall1883.fFont = ufont->GetFontHandle();
   vall1883.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall1883, kTRUE);

   TGLabel *fNbuffersLabelDivider = new TGLabel(fGroupData," | ",uGC->GetGC(),ufont->GetFontStruct(),kChildFrame,ucolor);
   fNbuffersLabelDivider->SetTextJustify(17);
   fNbuffersLabelDivider->SetMargins(0,0,0,0);
   fNbuffersLabelDivider->SetWrapLength(-1);
   fGroupData->AddFrame(fNbuffersLabelDivider, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fNbuffersLabelDivider->MoveResize(90,166,8,18);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t vall1884;
   vall1884.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#0000ff",vall1884.fForeground);
   gClient->GetColorByName("#e0e0e0",vall1884.fBackground);
   vall1884.fFillStyle = kFillSolid;
   vall1884.fFont = ufont->GetFontHandle();
   vall1884.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall1884, kTRUE);

   TGLabel *fNbuffersLabel = new TGLabel(fGroupData,"Buffers Analyzed:",uGC->GetGC(),ufont->GetFontStruct(),kChildFrame,ucolor);
   fNbuffersLabel->SetTextJustify(17);
   fNbuffersLabel->SetMargins(0,0,0,0);
   fNbuffersLabel->SetWrapLength(-1);
   fGroupData->AddFrame(fNbuffersLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fNbuffersLabel->MoveResize(110,166,120,18);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t vall1885;
   vall1885.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#0000ff",vall1885.fForeground);
   gClient->GetColorByName("#e0e0e0",vall1885.fBackground);
   vall1885.fFillStyle = kFillSolid;
   vall1885.fFont = ufont->GetFontHandle();
   vall1885.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall1885, kTRUE);

   TGLabel *fNbuffers = new TGLabel(fGroupData,"0",uGC->GetGC(),ufont->GetFontStruct(),kChildFrame,ucolor);
   fNbuffers->SetTextJustify(17);
   fNbuffers->SetMargins(0,0,0,0);
   fNbuffers->SetWrapLength(-1);
   fGroupData->AddFrame(fNbuffers, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fNbuffers->MoveResize(210,166,100,18);

   fGroupData->SetLayoutManager(new TGVerticalLayout(fGroupData));
   fGroupData->Resize(336,216);
   fMainFrame6310->AddFrame(fGroupData, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fGroupData->MoveResize(8,2,336,216);

   fMainFrame1596->AddFrame(fMainFrame6310, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame6310->MoveResize(0,1,352,428);

   fRbeerFrame->AddFrame(fMainFrame1596, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame1596->MoveResize(0,0,349,426);

   fRbeerFrame->SetMWMHints(kMWMDecorAll,
                        kMWMFuncAll,
                        kMWMInputModeless);
   fRbeerFrame->MapSubwindows();

   fRbeerFrame->Resize(fRbeerFrame->GetDefaultSize());
   fRbeerFrame->MapWindow();
   fRbeerFrame->Resize(349,424);
}  
