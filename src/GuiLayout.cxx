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
   TGGroupFrame *fGroupFrame2889 = new TGGroupFrame(fMainFrame6310,"Data",kVerticalFrame | kRaisedFrame,uGC->GetGC(),ufont->GetFontStruct(),ucolor);
   fGroupFrame2889->SetTitlePos(TGGroupFrame::kCenter);
   fGroupFrame2889->SetLayoutBroken(kTRUE);

   gClient->GetColorByName("#ffcc00",ucolor);
   TGTextButton *fAttachOnline = new TGTextButton(fGroupFrame2889,"Attach Online");
   fAttachOnline->SetTextJustify(36);
   fAttachOnline->SetMargins(0,0,0,0);
   fAttachOnline->SetWrapLength(-1);
   fAttachOnline->Resize(91,24);

   fAttachOnline->ChangeBackground(ucolor);
   fGroupFrame2889->AddFrame(fAttachOnline, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fAttachOnline->MoveResize(8,32,91,24);

   TGTextButton *fAttachFile = new TGTextButton(fGroupFrame2889,"Attach File");
   fAttachFile->SetTextJustify(36);
   fAttachFile->SetMargins(0,0,0,0);
   fAttachFile->SetWrapLength(-1);
   fAttachFile->Resize(91,24);

   fAttachFile->ChangeBackground(ucolor);
   fGroupFrame2889->AddFrame(fAttachFile, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fAttachFile->MoveResize(8,72,91,24);

   TGTextButton *fAttachList = new TGTextButton(fGroupFrame2889,"Attach List");
   fAttachList->SetTextJustify(36);
   fAttachList->SetMargins(0,0,0,0);
   fAttachList->SetWrapLength(-1);
   fAttachList->Resize(91,24);

   fAttachList->ChangeBackground(ucolor);
   fGroupFrame2889->AddFrame(fAttachList, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fAttachList->MoveResize(8,104,91,24);
   TGTextButton *fUnattach = new TGTextButton(fGroupFrame2889,"Unattach");
   fUnattach->SetTextJustify(36);
   fUnattach->SetMargins(0,0,0,0);
   fUnattach->SetWrapLength(-1);
   fUnattach->Resize(91,24);
   fGroupFrame2889->AddFrame(fUnattach, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
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
   TGTextEntry *fTextEntry2894 = new TGTextEntry(fGroupFrame2889, new TGTextBuffer(14),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fTextEntry2894->SetMaxLength(4096);
   fTextEntry2894->SetAlignment(kTextLeft);
   fTextEntry2894->SetText("");
   fTextEntry2894->Resize(176,fTextEntry2894->GetDefaultHeight());
   fGroupFrame2889->AddFrame(fTextEntry2894, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextEntry2894->MoveResize(144,24,176,22);

   gClient->GetColorByName("#d4cf87",ucolor);
   TGLabel *fLabel2895 = new TGLabel(fGroupFrame2889,"Host:",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);
   fLabel2895->SetTextJustify(36);
   fLabel2895->SetMargins(0,0,0,0);
   fLabel2895->SetWrapLength(-1);
   fGroupFrame2889->AddFrame(fLabel2895, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabel2895->MoveResize(104,24,32,18);

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
   TGTextEntry *fTextEntry2896 = new TGTextEntry(fGroupFrame2889, new TGTextBuffer(14),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);
   fTextEntry2896->SetMaxLength(4096);
   fTextEntry2896->SetAlignment(kTextLeft);
   fTextEntry2896->SetText("");
   fTextEntry2896->Resize(88,fTextEntry2896->GetDefaultHeight());
   fGroupFrame2889->AddFrame(fTextEntry2896, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextEntry2896->MoveResize(144,48,88,22);

   TGLabel *fLabel2897 = new TGLabel(fGroupFrame2889,"Port:",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);
   fLabel2897->SetTextJustify(36);
   fLabel2897->SetMargins(0,0,0,0);
   fLabel2897->SetWrapLength(-1);
   fGroupFrame2889->AddFrame(fLabel2897, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabel2897->MoveResize(104,48,32,18);
   TGCheckButton *fIsContinuous = new TGCheckButton(fGroupFrame2889,"Continuous");

   fIsContinuous->ChangeBackground(ucolor);
   fIsContinuous->SetTextJustify(36);
   fIsContinuous->SetMargins(0,0,0,0);
   fIsContinuous->SetWrapLength(-1);
   fGroupFrame2889->AddFrame(fIsContinuous, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fIsContinuous->MoveResize(104,80,88,19);

   TGLabel *fLabel2899 = new TGLabel(fGroupFrame2889,"",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);
   fLabel2899->SetTextJustify(36);
   fLabel2899->SetMargins(0,0,0,0);
   fLabel2899->SetWrapLength(-1);
   fGroupFrame2889->AddFrame(fLabel2899, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabel2899->MoveResize(8,208,320,18);

   TGLabel *fLabel2900 = new TGLabel(fGroupFrame2889,"Data source:",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);
   fLabel2900->SetTextJustify(36);
   fLabel2900->SetMargins(0,0,0,0);
   fLabel2900->SetWrapLength(-1);
   fGroupFrame2889->AddFrame(fLabel2900, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabel2900->MoveResize(8,184,80,18);

   fGroupFrame2889->SetLayoutManager(new TGVerticalLayout(fGroupFrame2889));
   fGroupFrame2889->Resize(336,240);
   fMainFrame6310->AddFrame(fGroupFrame2889, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fGroupFrame2889->MoveResize(16,16,336,240);


   // "Canvas" group frame
   TGGroupFrame *fGroupFrame2901 = new TGGroupFrame(fMainFrame6310,"Canvas",kVerticalFrame,TGGroupFrame::GetDefaultGC()(),TGGroupFrame::GetDefaultFontStruct(),ucolor);
   fGroupFrame2901->SetTitlePos(TGGroupFrame::kCenter);
   fGroupFrame2901->SetLayoutBroken(kTRUE);

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
   TGTextButton *fTextButton2902 = new TGTextButton(fGroupFrame2901,"Zero All");
   fTextButton2902->SetTextJustify(36);
   fTextButton2902->SetMargins(0,0,0,0);
   fTextButton2902->SetWrapLength(-1);
   fTextButton2902->Resize(91,24);

//   fTextButton2902->ChangeBackground(ucolor);
   fGroupFrame2901->AddFrame(fTextButton2902, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButton2902->MoveResize(176,112,91,24);

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

   TGTextButton *fTextButton2903 = new TGTextButton(fGroupFrame2901,"Divide Current");
   fTextButton2903->SetTextJustify(36);
   fTextButton2903->SetMargins(0,0,0,0);
   fTextButton2903->SetWrapLength(-1);
   fTextButton2903->Resize(91,24);

//   fTextButton2903->ChangeBackground(ucolor);
   fGroupFrame2901->AddFrame(fTextButton2903, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButton2903->MoveResize(64,144,91,24);

   gClient->GetColorByName("#00ff00",ucolor);
   TGTextButton *fTextButton2904 = new TGTextButton(fGroupFrame2901,"Create New");
   fTextButton2904->SetTextJustify(36);
   fTextButton2904->SetMargins(0,0,0,0);
   fTextButton2904->SetWrapLength(-1);
   fTextButton2904->Resize(91,24);

	 fTextButton2904->ChangeBackground(ucolor);
   fGroupFrame2901->AddFrame(fTextButton2904, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButton2904->MoveResize(176,144,91,24);

   gClient->GetColorByName("#ffffff",ucolor);

   // combo box
   TGComboBox *fComboBox2905 = new TGComboBox(fGroupFrame2901,-1,kHorizontalFrame | kSunkenFrame | kDoubleBorder | kOwnBackground);
   fComboBox2905->AddEntry("Entry 1 ",0);
   fComboBox2905->AddEntry("Entry 2 ",1);
   fComboBox2905->AddEntry("Entry 3 ",2);
   fComboBox2905->AddEntry("Entry 4 ",3);
   fComboBox2905->AddEntry("Entry 5 ",4);
   fComboBox2905->AddEntry("Entry 6 ",5);
   fComboBox2905->AddEntry("Entry 7 ",6);
   fComboBox2905->Resize(160,22);
   fComboBox2905->Select(-1);
   fGroupFrame2901->AddFrame(fComboBox2905, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fComboBox2905->MoveResize(88,200,160,22);
   TGNumberEntry *fNumberEntry2923 = new TGNumberEntry(fGroupFrame2901, (Double_t) 0,6,-1,(TGNumberFormat::EStyle) 5);
   fGroupFrame2901->AddFrame(fNumberEntry2923, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fNumberEntry2923->MoveResize(176,40,59,22);

   gClient->GetColorByName("#d4cf87",ucolor);
   TGLabel *fLabel2927 = new TGLabel(fGroupFrame2901,"Rate [sec.]",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);
   fLabel2927->SetTextJustify(36);
   fLabel2927->SetMargins(0,0,0,0);
   fLabel2927->SetWrapLength(-1);
   fGroupFrame2901->AddFrame(fLabel2927, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabel2927->MoveResize(240,40,64,18);

   TGLabel *fLabel2928 = new TGLabel(fGroupFrame2901,"Change Selected",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);
   fLabel2928->SetTextJustify(36);
   fLabel2928->SetMargins(0,0,0,0);
   fLabel2928->SetWrapLength(-1);
   fGroupFrame2901->AddFrame(fLabel2928, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabel2928->MoveResize(112,176,104,18);

   gClient->GetColorByName("#00ff00",ucolor);
   TGTextButton *fTextButton2929 = new TGTextButton(fGroupFrame2901,"Start Refresh");
   fTextButton2929->SetTextJustify(36);
   fTextButton2929->SetMargins(0,0,0,0);
   fTextButton2929->SetWrapLength(-1);
   fTextButton2929->Resize(91,24);

   fTextButton2929->ChangeBackground(ucolor);
   fGroupFrame2901->AddFrame(fTextButton2929, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButton2929->MoveResize(64,40,91,24);

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
   TGTextButton *fTextButton2930 = new TGTextButton(fGroupFrame2901,"Refresh Current");
   fTextButton2930->SetTextJustify(36);
   fTextButton2930->SetMargins(0,0,0,0);
   fTextButton2930->SetWrapLength(-1);
   fTextButton2930->Resize(91,24);

//   fTextButton2930->ChangeBackground(ucolor);
   fGroupFrame2901->AddFrame(fTextButton2930, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButton2930->MoveResize(64,80,91,24);

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

   TGTextButton *fTextButton2931 = new TGTextButton(fGroupFrame2901,"Refresh All");
   fTextButton2931->SetTextJustify(36);
   fTextButton2931->SetMargins(0,0,0,0);
   fTextButton2931->SetWrapLength(-1);
   fTextButton2931->Resize(91,24);

//   fTextButton2931->ChangeBackground(ucolor);
   fGroupFrame2901->AddFrame(fTextButton2931, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButton2931->MoveResize(176,80,91,24);

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

   TGTextButton *fTextButton2932 = new TGTextButton(fGroupFrame2901,"Zero Current");
   fTextButton2932->SetTextJustify(36);
   fTextButton2932->SetMargins(0,0,0,0);
   fTextButton2932->SetWrapLength(-1);
   fTextButton2932->Resize(91,24);

//   fTextButton2932->ChangeBackground(ucolor);
   fGroupFrame2901->AddFrame(fTextButton2932, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButton2932->MoveResize(64,112,91,24);

   fGroupFrame2901->SetLayoutManager(new TGVerticalLayout(fGroupFrame2901));
   fGroupFrame2901->Resize(336,240);
   fMainFrame6310->AddFrame(fGroupFrame2901, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fGroupFrame2901->MoveResize(16,256,336,240);

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
