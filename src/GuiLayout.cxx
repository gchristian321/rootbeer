// Mainframe macro generated from application: root.exe
// By ROOT version 5.32/01 on 2012-03-20 19:01:38

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

   // main frame
   TGMainFrame *fRbeerFrame = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
   fRbeerFrame->SetName("fRbeerFrame");
	 fRbeerFrame->SetWindowName("Rootbeer");
   fRbeerFrame->SetLayoutBroken(kTRUE);

   // composite frame
   TGCompositeFrame *fMainFrame2710 = new TGCompositeFrame(fRbeerFrame,366,503,kVerticalFrame);
   fMainFrame2710->SetName("fMainFrame2710");
   fMainFrame2710->SetLayoutBroken(kTRUE);

   // composite frame
   TGCompositeFrame *fMainFrame1877 = new TGCompositeFrame(fMainFrame2710,369,506,kVerticalFrame);
   fMainFrame1877->SetName("fMainFrame1877");
   fMainFrame1877->SetLayoutBroken(kTRUE);

   ULong_t ucolor;        // will reflect user color changes
   gClient->GetColorByName("#d4cf87",ucolor);

   // composite frame
   TGCompositeFrame *fMainFrame6310 = new TGCompositeFrame(fMainFrame1877,370,496,kVerticalFrame,ucolor);
   fMainFrame6310->SetName("fMainFrame6310");
   fMainFrame6310->SetLayoutBroken(kTRUE);

   TGFont *ufont;         // will reflect user font changes
   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   TGGC   *uGC;           // will reflect user GC changes
   // graphics context changes
   GCValues_t valpFrame2889;
   valpFrame2889.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valpFrame2889.fForeground);
   gClient->GetColorByName("#e0e0e0",valpFrame2889.fBackground);
   valpFrame2889.fFillStyle = kFillSolid;
   valpFrame2889.fFont = ufont->GetFontHandle();
   valpFrame2889.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valpFrame2889, kTRUE);


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
   fAttachOnline->MoveResize(8,32,91,24);

   TGTextButton *fAttachFile = new TGTextButton(fGroupData,"Attach File");
   fAttachFile->SetTextJustify(36);
   fAttachFile->SetMargins(0,0,0,0);
   fAttachFile->SetWrapLength(-1);
   fAttachFile->Resize(91,24);

   fAttachFile->ChangeBackground(ucolor);
   fGroupData->AddFrame(fAttachFile, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fAttachFile->MoveResize(8,72,91,24);

   TGTextButton *fAttachList = new TGTextButton(fGroupData,"Attach List");
   fAttachList->SetTextJustify(36);
   fAttachList->SetMargins(0,0,0,0);
   fAttachList->SetWrapLength(-1);
   fAttachList->Resize(91,24);

   fAttachList->ChangeBackground(ucolor);
   fGroupData->AddFrame(fAttachList, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fAttachList->MoveResize(8,104,91,24);
   TGTextButton *fUnattach = new TGTextButton(fGroupData,"Unattach");
   fUnattach->SetTextJustify(36);
   fUnattach->SetMargins(0,0,0,0);
   fUnattach->SetWrapLength(-1);
   fUnattach->Resize(91,24);
   fGroupData->AddFrame(fUnattach, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fUnattach->MoveResize(8,136,91,24);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valEntry2894;
   valEntry2894.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valEntry2894.fForeground);
   gClient->GetColorByName("#e0e0e0",valEntry2894.fBackground);
   valEntry2894.fFillStyle = kFillSolid;
   valEntry2894.fFont = ufont->GetFontHandle();
   valEntry2894.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valEntry2894, kTRUE);
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
   GCValues_t valEntry2896;
   valEntry2896.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valEntry2896.fForeground);
   gClient->GetColorByName("#e0e0e0",valEntry2896.fBackground);
   valEntry2896.fFillStyle = kFillSolid;
   valEntry2896.fFont = ufont->GetFontHandle();
   valEntry2896.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valEntry2896, kTRUE);
   TGTextEntry *fEntryPort = new TGTextEntry(fGroupData, new TGTextBuffer(14),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fEntryPort->SetMaxLength(4096);
   fEntryPort->SetAlignment(kTextLeft);
   fEntryPort->SetText("");
   fEntryPort->Resize(88,fEntryPort->GetDefaultHeight());
   fGroupData->AddFrame(fEntryPort, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fEntryPort->MoveResize(144,48,88,22);

   TGLabel *fLabelPort = new TGLabel(fGroupData,"Port:",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);
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

   TGLabel *fLabelSource = new TGLabel(fGroupData,"[none]",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);
   fLabelSource->SetTextJustify(kTextBottom | kTextLeft);
   fLabelSource->SetMargins(0,0,0,0);
   fLabelSource->SetWrapLength(-1);
   fGroupData->AddFrame(fLabelSource, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelSource->MoveResize(24,198,300,18);
	 fLabelSource->SetTextColor(0x0000ff);

   TGLabel *fLabelDataSource = new TGLabel(fGroupData,"Data source:",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);
   fLabelDataSource->SetTextJustify(kTextBottom | kTextLeft);
   fLabelDataSource->SetMargins(0,0,0,0);
   fLabelDataSource->SetWrapLength(-1);
   fGroupData->AddFrame(fLabelDataSource, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	 fLabelDataSource->SetTextColor(0x0000ff);
   fLabelDataSource->MoveResize(8,180,80,18);


   fGroupData->SetLayoutManager(new TGVerticalLayout(fGroupData));
   fGroupData->Resize(336,240);
   fMainFrame6310->AddFrame(fGroupData, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fGroupData->MoveResize(16,16,336,240);

	 // TGIcon *fIcon793 =
	 // new TGIcon(fRbeerFrame,"etc/rbeer.eps");
	 // fIcon793->SetName("fIcon793");
	 // fRbeerFrame->AddFrame(fIcon793, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	 // fIcon793->MoveResize(220,120,84,94);

   // "Canvas" group frame
   TGGroupFrame *fGroupCanvas = new TGGroupFrame(fMainFrame6310,"Canvas",kVerticalFrame,TGGroupFrame::GetDefaultGC()(),TGGroupFrame::GetDefaultFontStruct(),ucolor);
   fGroupCanvas->SetTitlePos(TGGroupFrame::kCenter);
   fGroupCanvas->SetLayoutBroken(kTRUE);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valButton2902;
   valButton2902.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valButton2902.fForeground);
   gClient->GetColorByName("#e0e0e0",valButton2902.fBackground);
   valButton2902.fFillStyle = kFillSolid;
   valButton2902.fFont = ufont->GetFontHandle();
   valButton2902.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valButton2902, kTRUE);

   gClient->GetColorByName("#0000ff",ucolor);
   TGTextButton *fZeroAll = new TGTextButton(fGroupCanvas,"Zero All");
   fZeroAll->SetTextJustify(36);
   fZeroAll->SetMargins(0,0,0,0);
   fZeroAll->SetWrapLength(-1);
   fZeroAll->Resize(91,24);

//   fZeroAll->ChangeBackground(ucolor);
   fGroupCanvas->AddFrame(fZeroAll, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fZeroAll->MoveResize(176,112,91,24);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valButton2903;
   valButton2903.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valButton2903.fForeground);
   gClient->GetColorByName("#e0e0e0",valButton2903.fBackground);
   valButton2903.fFillStyle = kFillSolid;
   valButton2903.fFont = ufont->GetFontHandle();
   valButton2903.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valButton2903, kTRUE);

   TGTextButton *fDivideCurrent = new TGTextButton(fGroupCanvas,"Divide Current");
   fDivideCurrent->SetTextJustify(36);
   fDivideCurrent->SetMargins(0,0,0,0);
   fDivideCurrent->SetWrapLength(-1);
   fDivideCurrent->Resize(91,24);

//   fDivideCurrent->ChangeBackground(ucolor);
   fGroupCanvas->AddFrame(fDivideCurrent, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fDivideCurrent->MoveResize(64,144,91,24);

   gClient->GetColorByName("#00ff00",ucolor);
   TGTextButton *fCreateNew = new TGTextButton(fGroupCanvas,"Create New");
   fCreateNew->SetTextJustify(36);
   fCreateNew->SetMargins(0,0,0,0);
   fCreateNew->SetWrapLength(-1);
   fCreateNew->Resize(91,24);

	 fCreateNew->ChangeBackground(ucolor);
   fGroupCanvas->AddFrame(fCreateNew, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fCreateNew->MoveResize(176,144,91,24);

   gClient->GetColorByName("#000000",ucolor);
   TGTextEntry *fEntryName = new TGTextEntry(fGroupCanvas, new TGTextBuffer(14),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fEntryName->SetMaxLength(4096);
   fEntryName->SetAlignment(kTextLeft);
   fEntryName->SetText("");
   fEntryName->SetTextColor(ucolor);
   fEntryName->Resize(176,fEntryName->GetDefaultHeight());
   fGroupData->AddFrame(fEntryName, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fEntryName->MoveResize(222,177,45,22);

   gClient->GetColorByName("#d4cf87",ucolor);
   TGLabel *fLabelName = new TGLabel(fGroupCanvas,"Name:",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);
   fLabelName->SetTextJustify(kTextTop | kTextLeft);
   fLabelName->SetMargins(0,0,0,0);
   fLabelName->SetWrapLength(-1);
   fGroupCanvas->AddFrame(fLabelName, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelName->MoveResize(176,177,40,22);



   gClient->GetColorByName("#ffffff",ucolor);

   // combo box
   TGComboBox *fSelectCanvas = new TGComboBox(fGroupCanvas,-1,kHorizontalFrame | kSunkenFrame | kDoubleBorder | kOwnBackground);
   fSelectCanvas->Resize(160,22);
   fSelectCanvas->Select(-1);
   fGroupCanvas->AddFrame(fSelectCanvas, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fSelectCanvas->MoveResize(64,200,91,22);

   TGNumberEntry *fUpdateRate = new TGNumberEntry(fGroupCanvas, (Double_t) 0,6,-1,(TGNumberFormat::EStyle) 5);
   fGroupCanvas->AddFrame(fUpdateRate, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fUpdateRate->MoveResize(176,40,59,22);

   gClient->GetColorByName("#d4cf87",ucolor);
   TGLabel *fLabelRate = new TGLabel(fGroupCanvas,"Rate [sec.]",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);
   fLabelRate->SetTextJustify(36);
   fLabelRate->SetMargins(0,0,0,0);
   fLabelRate->SetWrapLength(-1);
   fGroupCanvas->AddFrame(fLabelRate, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelRate->MoveResize(240,40,64,18);

   TGLabel *fLabelChangeSelected = new TGLabel(fGroupCanvas,"Change Selected",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);
   fLabelChangeSelected->SetTextJustify(kTextTop | kTextLeft);
   fLabelChangeSelected->SetMargins(0,0,0,0);
   fLabelChangeSelected->SetWrapLength(-1);
   fGroupCanvas->AddFrame(fLabelChangeSelected, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelChangeSelected->MoveResize(64,177,104,18);

   gClient->GetColorByName("#00ff00",ucolor);
   TGTextButton *fStartRefresh = new TGTextButton(fGroupCanvas,"Start Refresh");
   fStartRefresh->SetTextJustify(36);
   fStartRefresh->SetMargins(0,0,0,0);
   fStartRefresh->SetWrapLength(-1);
   fStartRefresh->Resize(91,24);

   fStartRefresh->ChangeBackground(ucolor);
   fGroupCanvas->AddFrame(fStartRefresh, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fStartRefresh->MoveResize(64,40,91,24);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valButton2930;
   valButton2930.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valButton2930.fForeground);
   gClient->GetColorByName("#e0e0e0",valButton2930.fBackground);
   valButton2930.fFillStyle = kFillSolid;
   valButton2930.fFont = ufont->GetFontHandle();
   valButton2930.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valButton2930, kTRUE);

   gClient->GetColorByName("#0000ff",ucolor);
   TGTextButton *fRefreshCurrent = new TGTextButton(fGroupCanvas,"Refresh Current");
   fRefreshCurrent->SetTextJustify(36);
   fRefreshCurrent->SetMargins(0,0,0,0);
   fRefreshCurrent->SetWrapLength(-1);
   fRefreshCurrent->Resize(91,24);

//   fRefreshCurrent->ChangeBackground(ucolor);
   fGroupCanvas->AddFrame(fRefreshCurrent, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fRefreshCurrent->MoveResize(64,80,91,24);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valButton2931;
   valButton2931.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valButton2931.fForeground);
   gClient->GetColorByName("#e0e0e0",valButton2931.fBackground);
   valButton2931.fFillStyle = kFillSolid;
   valButton2931.fFont = ufont->GetFontHandle();
   valButton2931.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valButton2931, kTRUE);

   TGTextButton *fRefreshAll = new TGTextButton(fGroupCanvas,"Refresh All");
   fRefreshAll->SetTextJustify(36);
   fRefreshAll->SetMargins(0,0,0,0);
   fRefreshAll->SetWrapLength(-1);
   fRefreshAll->Resize(91,24);

//   fRefreshAll->ChangeBackground(ucolor);
   fGroupCanvas->AddFrame(fRefreshAll, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fRefreshAll->MoveResize(176,80,91,24);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valButton2932;
   valButton2932.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valButton2932.fForeground);
   gClient->GetColorByName("#e0e0e0",valButton2932.fBackground);
   valButton2932.fFillStyle = kFillSolid;
   valButton2932.fFont = ufont->GetFontHandle();
   valButton2932.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valButton2932, kTRUE);

   TGTextButton *fZeroCurrent = new TGTextButton(fGroupCanvas,"Zero Current");
   fZeroCurrent->SetTextJustify(36);
   fZeroCurrent->SetMargins(0,0,0,0);
   fZeroCurrent->SetWrapLength(-1);
   fZeroCurrent->Resize(91,24);

//   fZeroCurrent->ChangeBackground(ucolor);
   fGroupCanvas->AddFrame(fZeroCurrent, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fZeroCurrent->MoveResize(64,112,91,24);

   fGroupCanvas->SetLayoutManager(new TGVerticalLayout(fGroupCanvas));
   fGroupCanvas->Resize(336,240);
   fMainFrame6310->AddFrame(fGroupCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fGroupCanvas->MoveResize(16,256,336,240);

   fMainFrame1877->AddFrame(fMainFrame6310, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame6310->MoveResize(0,0,370,496);

   fMainFrame2710->AddFrame(fMainFrame1877, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame1877->MoveResize(0,0,369,506);

   fRbeerFrame->AddFrame(fMainFrame2710, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame2710->MoveResize(0,0,366,503);

   fRbeerFrame->SetMWMHints(kMWMDecorAll,
                        kMWMFuncAll,
                        kMWMInputModeless);
   fRbeerFrame->MapSubwindows();

   fRbeerFrame->Resize(fRbeerFrame->GetDefaultSize());
	 fRbeerFrame->Move(100,100);
	 fRbeerFrame->SetWMPosition(100,100);
   fRbeerFrame->MapWindow();
   fRbeerFrame->Resize(361,495);
}  
