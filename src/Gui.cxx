//! \file Gui.cxx
//! \brief Implements Gui.hxx
#include "Gui.hxx"
#include "Rint.hxx"

void TGRbeerFrame::GuiLayout()
{

   TGFont *ufont;         // will reflect user font changes
   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // main frame
   // /* TGMainFrame* */ fRbeerFrame = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
   this->SetName("fRbeerFrame");
	 this->SetWindowName("rootbeer");
   this->SetLayoutBroken(kTRUE);
   this->ChangeBackground((Pixel_t)0xd4cf87);

	 
   // composite frame
   /* TGCompositeFrame* */ fMainFrame1596 = new TGCompositeFrame(this,349,426,kVerticalFrame);   fMainFrame1596->SetName("fMainFrame1596");
   fMainFrame1596->SetLayoutBroken(kTRUE);
   fMainFrame1596->ChangeBackground((Pixel_t)0xd4cf87);

   ULong_t ucolor = 0;        // will reflect user color changes

   // composite frame
   /* TGCompositeFrame* */ fMainFrame6310 = new TGCompositeFrame(fMainFrame1596,352,428,kVerticalFrame,ucolor);   fMainFrame6310->SetName("fMainFrame6310");
   fMainFrame6310->SetLayoutBroken(kTRUE);
   fMainFrame6310->ChangeBackground((Pixel_t)0xd4cf87);

   // "Canvas" group frame
   /* TGGroupFrame* */ fGroupCanvas = new TGGroupFrame(fMainFrame6310,"Canvas",kVerticalFrame,TGGroupFrame::GetDefaultGC()(),TGGroupFrame::GetDefaultFontStruct(),ucolor);   fGroupCanvas->SetTitlePos(TGGroupFrame::kCenter);
   fGroupCanvas->SetLayoutBroken(kTRUE);
   fGroupCanvas->ChangeBackground((Pixel_t)0xd4cf87);
   /* TGTextButton* */ fZeroAll = new TGTextButton(fGroupCanvas,"Zero All");
   fZeroAll->SetFont(ufont->GetFontStruct());
   fZeroAll->SetTextJustify(36);
   fZeroAll->SetMargins(0,0,0,0);
   fZeroAll->SetWrapLength(-1);
   fZeroAll->Resize(91,24);
   fGroupCanvas->AddFrame(fZeroAll, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fZeroAll->MoveResize(110,100,95,24);
   /* TGTextButton* */ fDivideCurrent = new TGTextButton(fGroupCanvas,"Divide Current");
   fDivideCurrent->SetFont(ufont->GetFontStruct());
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

   /* TGTextEntry* */ fEntryName = new TGTextEntry(fGroupCanvas, new TGTextBuffer(14),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);   fEntryName->SetMaxLength(4096);
   fEntryName->SetAlignment(kTextLeft);
   fEntryName->SetText("");
   fEntryName->Resize(80,fEntryName->GetDefaultHeight());
   fGroupCanvas->AddFrame(fEntryName, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fEntryName->MoveResize(208,167,80,22);
   
   gClient->GetColorByName("#00ff00",ucolor);
   /* TGTextButton* */ fCreateNew = new TGTextButton(fGroupCanvas,"Create New");
   fCreateNew->SetFont(ufont->GetFontStruct());
   fCreateNew->SetTextJustify(36);
   fCreateNew->SetMargins(0,0,0,0);
   fCreateNew->SetWrapLength(-1);
   fCreateNew->Resize(91,24);

   fCreateNew->ChangeBackground(ucolor);
   fGroupCanvas->AddFrame(fCreateNew, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fCreateNew->MoveResize(110,165,95,24);

   gClient->GetColorByName("#d4cf87",ucolor);
   /* TGLabel* */ fLabelName = new TGLabel(fGroupCanvas,"Name:",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);   fLabelName->SetTextJustify(33);
   fLabelName->SetMargins(0,0,0,0);
   fLabelName->SetWrapLength(-1);
   fGroupCanvas->AddFrame(fLabelName, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelName->MoveResize(210,145,35,22);

   /* TGTextButton* */ fLogy = new TGCheckButton(fGroupCanvas, "Log y"); fLogy->ChangeBackground(ucolor);
   fLogy->SetFont(ufont->GetFontStruct());
   fLogy->SetTextJustify(36);
   fLogy->SetMargins(0,0,0,0);
   fLogy->SetWrapLength(-1);
   fGroupCanvas->AddFrame(fLogy, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLogy->MoveResize(110,132,95,14);

   /* TGTextButton* */ fLogz = new TGCheckButton(fGroupCanvas, "Log z"); fLogz->ChangeBackground(ucolor);
   fLogz->SetFont(ufont->GetFontStruct());
   fLogz->SetTextJustify(36);
   fLogz->SetMargins(0,0,0,0);
   fLogz->SetWrapLength(-1);
   fGroupCanvas->AddFrame(fLogz, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLogz->MoveResize(165,132,65,14);

   /* TGNumberEntry* */ fUpdateRate = new TGNumberEntry(fGroupCanvas, (Double_t) 0,6,-1,(TGNumberFormat::EStyle) 5);   fGroupCanvas->AddFrame(fUpdateRate, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fUpdateRate->MoveResize(110,28,59,22);


   /* TGLabel* */ fLabelRate = new TGLabel(fGroupCanvas,"Rate [sec.]",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);   fLabelRate->SetTextJustify(36);
   fLabelRate->SetMargins(0,0,0,0);
   fLabelRate->SetWrapLength(-1);
   fGroupCanvas->AddFrame(fLabelRate, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelRate->MoveResize(174,28,64,18);

   gClient->GetColorByName("#00ff00",ucolor);
   /* TGTextButton* */ fStartRefresh = new TGTextButton(fGroupCanvas,"Start Refresh");
   fStartRefresh->SetFont(ufont->GetFontStruct());
   fStartRefresh->SetTextJustify(36);
   fStartRefresh->SetMargins(0,0,0,0);
   fStartRefresh->SetWrapLength(-1);
   fStartRefresh->Resize(91,24);

   fStartRefresh->ChangeBackground(ucolor);
   fGroupCanvas->AddFrame(fStartRefresh, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fStartRefresh->MoveResize(8,28,95,24);
   /* TGTextButton* */ fRefreshCurrent = new TGTextButton(fGroupCanvas,"Refresh Current");
   fRefreshCurrent->SetFont(ufont->GetFontStruct());
   fRefreshCurrent->SetTextJustify(36);
   fRefreshCurrent->SetMargins(0,0,0,0);
   fRefreshCurrent->SetWrapLength(-1);
   fRefreshCurrent->Resize(91,24);
   fGroupCanvas->AddFrame(fRefreshCurrent, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fRefreshCurrent->MoveResize(8,68,95,24);
   /* TGTextButton* */ fRefreshAll = new TGTextButton(fGroupCanvas,"Refresh All");
   fRefreshAll->SetFont(ufont->GetFontStruct());
   fRefreshAll->SetTextJustify(36);
   fRefreshAll->SetMargins(0,0,0,0);
   fRefreshAll->SetWrapLength(-1);
   fRefreshAll->Resize(91,24);
   fGroupCanvas->AddFrame(fRefreshAll, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fRefreshAll->MoveResize(110,68,95,24);
   /* TGTextButton* */ fZeroCurrent = new TGTextButton(fGroupCanvas,"Zero Current");
   fZeroCurrent->SetFont(ufont->GetFontStruct());
   fZeroCurrent->SetTextJustify(36);
   fZeroCurrent->SetMargins(0,0,0,0);
   fZeroCurrent->SetWrapLength(-1);
   fZeroCurrent->Resize(91,24);
   fGroupCanvas->AddFrame(fZeroCurrent, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fZeroCurrent->MoveResize(8,100,95,24);
   /* TGTextButton* */ fClearCurrent = new TGTextButton(fGroupCanvas,"Clear Canvas");
   fClearCurrent->SetFont(ufont->GetFontStruct());
   fClearCurrent->SetTextJustify(36);
   fClearCurrent->SetMargins(0,0,0,0);
   fClearCurrent->SetWrapLength(-1);
   fClearCurrent->Resize(91,24);
   fGroupCanvas->AddFrame(fClearCurrent, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fClearCurrent->MoveResize(8,165,95,24);

   gClient->GetColorByName("#d4cf87",ucolor);

   // "Configuration" group frame
   /* TGGroupFrame* */ fGroupConfig = new TGGroupFrame(fGroupCanvas,"Configuration",kVerticalFrame,TGGroupFrame::GetDefaultGC()(),TGGroupFrame::GetDefaultFontStruct(),ucolor);   fGroupConfig->SetTitlePos(TGGroupFrame::kCenter);
   fGroupConfig->SetLayoutBroken(kTRUE);

   gClient->GetColorByName("#ffcc00",ucolor);
   /* TGTextButton* */ fConfigLoad = new TGTextButton(fGroupConfig,"Load Canvases");
   fConfigLoad->SetFont(ufont->GetFontStruct());
   fConfigLoad->SetTextJustify(36);
   fConfigLoad->SetMargins(0,0,0,0);
   fConfigLoad->SetWrapLength(-1);
   fConfigLoad->Resize(64,24);

   fConfigLoad->ChangeBackground(ucolor);
   fGroupConfig->AddFrame(fConfigLoad, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fConfigLoad->MoveResize(12-5,56,64+30,24);

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
   /* TGTextButton* */ fConfigSave = new TGTextButton(fGroupConfig,"Save Canvases",-1,uGC->GetGC());
   fConfigSave->SetFont(ufont->GetFontStruct());
   fConfigSave->SetTextJustify(36);
   fConfigSave->SetMargins(0,0,0,0);
   fConfigSave->SetWrapLength(-1);
   fConfigSave->Resize(64,24);

   fConfigSave->ChangeBackground(ucolor);
   fGroupConfig->AddFrame(fConfigSave, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fConfigSave->MoveResize(12-5,24,64+30,24);

   fGroupConfig->SetLayoutManager(new TGVerticalLayout(fGroupConfig));
   fGroupConfig->Resize(88,96);
   fGroupCanvas->AddFrame(fGroupConfig, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fGroupConfig->MoveResize(228-9,55,88+18,96);

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
   /* TGGroupFrame* */ fGroupData = new TGGroupFrame(fMainFrame6310,"Data",kVerticalFrame | kRaisedFrame,uGC->GetGC(),ufont->GetFontStruct(),ucolor);   fGroupData->SetTitlePos(TGGroupFrame::kCenter);
   fGroupData->SetLayoutBroken(kTRUE);

   gClient->GetColorByName("#ffcc00",ucolor);
   /* TGTextButton* */ fAttachOnline = new TGTextButton(fGroupData,"Attach Online");
   fAttachOnline->SetFont(ufont->GetFontStruct());
   fAttachOnline->SetTextJustify(36);
   fAttachOnline->SetMargins(0,0,0,0);
   fAttachOnline->SetWrapLength(-1);
   fAttachOnline->Resize(91,24);

   fAttachOnline->ChangeBackground(ucolor);
   fGroupData->AddFrame(fAttachOnline, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fAttachOnline->MoveResize(8,32,95,24);

   /* TGTextButton* */ fAttachFile = new TGTextButton(fGroupData,"Attach File");
   fAttachFile->SetFont(ufont->GetFontStruct());
   fAttachFile->SetTextJustify(36);
   fAttachFile->SetMargins(0,0,0,0);
   fAttachFile->SetWrapLength(-1);
   fAttachFile->Resize(91,24);

   fAttachFile->ChangeBackground(ucolor);
   fGroupData->AddFrame(fAttachFile, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fAttachFile->MoveResize(8,72,95,24);

   /* TGTextButton* */ fAttachList = new TGTextButton(fGroupData,"Attach List");
   fAttachList->SetFont(ufont->GetFontStruct());
   fAttachList->SetTextJustify(36);
   fAttachList->SetMargins(0,0,0,0);
   fAttachList->SetWrapLength(-1);
   fAttachList->Resize(91,24);

   fAttachList->ChangeBackground(ucolor);
   fGroupData->AddFrame(fAttachList, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fAttachList->MoveResize(8,104,95,24);
   /* TGTextButton* */ fUnattach = new TGTextButton(fGroupData,"Unattach");
   fUnattach->SetFont(ufont->GetFontStruct());
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
   /* TGTextEntry* */ fEntryHost = new TGTextEntry(fGroupData, new TGTextBuffer(14),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);   fEntryHost->SetMaxLength(4096);
   fEntryHost->SetAlignment(kTextLeft);
#ifdef MIDAS_SERVER_HOST
   fEntryHost->SetText(MIDAS_SERVER_HOST);
#else
   fEntryHost->SetText("");
#endif
   fEntryHost->Resize(176,fEntryHost->GetDefaultHeight());
   fGroupData->AddFrame(fEntryHost, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fEntryHost->MoveResize(144,20,176,22);

   gClient->GetColorByName("#d4cf87",ucolor);
   /* TGLabel* */ fLabelHost = new TGLabel(fGroupData,"Host:",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);   fLabelHost->SetTextJustify(36);
   fLabelHost->SetMargins(0,0,0,0);
   fLabelHost->SetWrapLength(-1);
   fGroupData->AddFrame(fLabelHost, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelHost->MoveResize(110,20,32,18);

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
   /* TGTextEntry* */ fEntryPort = new TGTextEntry(fGroupData, new TGTextBuffer(14),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);   fEntryPort->SetMaxLength(4096);
   fEntryPort->SetAlignment(kTextLeft);
#ifdef MIDAS_EXPT_NAME
	 fEntryPort->SetText(MIDAS_EXPT_NAME);
#else
   fEntryPort->SetText("");
#endif
   fEntryPort->Resize(88,fEntryPort->GetDefaultHeight());
   fGroupData->AddFrame(fEntryPort, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fEntryPort->MoveResize(144,44,88,22);

   /* TGLabel* */ fLabelPort = new TGLabel(fGroupData,"Expt:",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);   fLabelPort->SetTextJustify(36);
   fLabelPort->SetMargins(0,0,0,0);
   fLabelPort->SetWrapLength(-1);
   fGroupData->AddFrame(fLabelPort, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelPort->MoveResize(110,44,32,18);

   /* TGCheckButton* */ fIsContinuous = new TGCheckButton(fGroupData,"Continuous");   fIsContinuous->ChangeBackground(ucolor);
   fIsContinuous->SetTextJustify(36);
   fIsContinuous->SetMargins(0,0,0,0);
   fIsContinuous->SetWrapLength(-1);
   fGroupData->AddFrame(fIsContinuous, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fIsContinuous->MoveResize(110,118,88,19);


   /* TGLabel* */ fFilterLabel = new TGLabel(fGroupData,"Filter:",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);   fLabelPort->SetTextJustify(36);
   fFilterLabel->SetMargins(0,0,0,0);
   fFilterLabel->SetWrapLength(-1);
   fGroupData->AddFrame(fFilterLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fFilterLabel->MoveResize(110,73,32,18);

   GCValues_t valEntry1879;
   valEntry1879.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valEntry1879.fForeground);
   gClient->GetColorByName("#e0e0e0",valEntry1879.fBackground);
   valEntry1879.fFillStyle = kFillSolid;
   valEntry1879.fFont = ufont->GetFontHandle();
   valEntry1879.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valEntry1879, kTRUE);
   /* TGTextEntry* */ fFilterEntry = new TGTextEntry(fGroupData, new TGTextBuffer(14),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);   fFilterEntry->SetMaxLength(4096);
   fFilterEntry->SetAlignment(kTextLeft);
   fFilterEntry->SetText("");
   fFilterEntry->Resize(88,fFilterEntry->GetDefaultHeight());
   fGroupData->AddFrame(fFilterEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fFilterEntry->MoveResize(144,73,186,22);


   /* TGLabel* */ fFilterTypeLabel = new TGLabel(fGroupData,"Type:",TGLabel::GetDefaultGC()(),TGLabel::GetDefaultFontStruct(),kChildFrame,ucolor);   fFilterTypeLabel->SetTextJustify(36);
   fFilterTypeLabel->SetMargins(0,0,0,0);
   fFilterTypeLabel->SetWrapLength(-1);
   fGroupData->AddFrame(fFilterTypeLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fFilterTypeLabel->MoveResize(110,95,32,18);


   // combo box
   /* TGComboBox* */ fFilterType = new TGComboBox(fGroupData,-1,kHorizontalFrame | kSunkenFrame | kDoubleBorder | kOwnBackground);   fFilterType->Resize(256,22);
   fFilterType->Select(-1);
   fGroupData->AddFrame(fFilterType, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fFilterType->MoveResize(144,95,186,22);

   ufont = gClient->GetFont("-*-helvetica--*-*-12-*-*-*-*-*-*-*");

   /* TGTextButton* */ fSetFilter = new TGTextButton(fGroupData,"Set Filter");
   fSetFilter->SetFont(ufont->GetFontStruct());
   fSetFilter->SetTextJustify(36);
   fSetFilter->SetMargins(0,0,0,0);
   fSetFilter->SetWrapLength(-1);
   fSetFilter->Resize(91,24);
   fGroupData->AddFrame(fSetFilter, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	 int WW = 80;
   fSetFilter->MoveResize(330-WW,125,WW,24);


   /* TGCheckButton* */ fSaveData = new TGCheckButton(fGroupData,"Save Data");   fSaveData->ChangeBackground(ucolor);
   fSaveData->SetTextJustify(36);
   fSaveData->SetMargins(0,0,0,0);
   fSaveData->SetWrapLength(-1);
   fGroupData->AddFrame(fSaveData, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fSaveData->MoveResize(110,134,88,19);

   /* TGCheckButton* */ fSaveHist = new TGCheckButton(fGroupData,"Save Histograms");   fSaveHist->ChangeBackground(ucolor);
   fSaveHist->SetTextJustify(36);
   fSaveHist->SetMargins(0,0,0,0);
   fSaveHist->SetWrapLength(-1);
   fGroupData->AddFrame(fSaveHist, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fSaveHist->MoveResize(110,150,118,19);


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

   /* TGLabel* */ fLabelSource = new TGLabel(fGroupData,"[none]",uGC->GetGC(),ufont->GetFontStruct(),kChildFrame,ucolor);   fLabelSource->SetTextJustify(17);
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

   /* TGLabel* */ fLabelDataSource = new TGLabel(fGroupData,"Data source:",uGC->GetGC(),ufont->GetFontStruct(),kChildFrame,ucolor);   fLabelDataSource->SetTextJustify(17);
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

   /* TGLabel* */ fNbuffersLabelDivider = new TGLabel(fGroupData," | ",uGC->GetGC(),ufont->GetFontStruct(),kChildFrame,ucolor);   fNbuffersLabelDivider->SetTextJustify(17);
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

   /* TGLabel* */ fNbuffersLabel = new TGLabel(fGroupData,"Buffers Analyzed:",uGC->GetGC(),ufont->GetFontStruct(),kChildFrame,ucolor);   fNbuffersLabel->SetTextJustify(17);
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

   /* TGLabel* */ fNbuffers = new TGLabel(fGroupData,"0",uGC->GetGC(),ufont->GetFontStruct(),kChildFrame,ucolor);   fNbuffers->SetTextJustify(17);
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

   this->AddFrame(fMainFrame1596, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame1596->MoveResize(0,0,349,426);

   this->SetMWMHints(kMWMDecorAll,
                        kMWMFuncAll,
                        kMWMInputModeless);
   this->MapSubwindows();

   this->Resize(this->GetDefaultSize());
   this->MapWindow();
   this->Resize(349,424);

   // Call MakeConnections function, defined in Gui.hxx
   MakeConnections(); 
}  

// TGMainFrame *fRbeerFrame = 0;TGCompositeFrame *fMainFrame1596 = 0;TGCompositeFrame *fMainFrame6310 = 0;TGGroupFrame *fGroupCanvas = 0;TGTextButton *fZeroAll = 0;TGTextButton *fDivideCurrent = 0;TGTextEntry *fEntryName = 0;TGTextButton *fCreateNew = 0;TGLabel *fLabelName = 0;TGTextButton *fLogy = 0;TGNumberEntry *fUpdateRate = 0;TGLabel *fLabelRate = 0;TGTextButton *fStartRefresh = 0;TGTextButton *fRefreshCurrent = 0;TGTextButton *fRefreshAll = 0;TGTextButton *fZeroCurrent = 0;TGTextButton *fClearCurrent = 0;TGGroupFrame *fGroupConfig = 0;TGTextButton *fConfigLoad = 0;TGTextButton *fConfigSave = 0;TGGroupFrame *fGroupData = 0;TGTextButton *fAttachOnline = 0;TGTextButton *fAttachFile = 0;TGTextButton *fAttachList = 0;TGTextButton *fUnattach = 0;TGTextEntry *fEntryHost = 0;TGLabel *fLabelHost = 0;TGTextEntry *fEntryPort = 0;TGTextEntry *fFilteFrEntry = 0;TGLabel *fLabelPort = 0;TGCheckButton *fIsContinuous = 0;TGCheckButton *fSaveData = 0;TGCheckButton *fSaveHist = 0;TGLabel *fLabelSource = 0;TGLabel *fLabelDataSource = 0;TGLabel *fNbuffersLabelDivider = 0;TGLabel *fNbuffersLabel = 0;TGLabel *fNbuffers = 0;
// ================================================== //

/*namespace pix { enum Color_t {
	White  = 0xffffff,
	Maroon = 0x800000,
	Red    = 0xff0000,
	Yellow = 0xffff00,
	Green  = 0x008000,
	Lime   = 0x00ff00,
	Teal   = 0x008080,
	Aqua   = 0x00ffff,
	Navy   = 0x000080,
	Blue   = 0x0000ff,
	Purple = 0x800080,
	Pink   = 0xff00ff,
	Orange = 0xff6600,
	Black  = 0x000000,
	Gray60 = 0x666666,
	Gray40 = 0x999999,
	Gray20 = 0xcccccc
	};
	B.G. Color = 0xd4cf87
	}*/


#define RB_SIGNALS rb::Rint::gApp()->GetSignals()
#define RB_BUTTON_CONNECT(button, function) this->Connect(button, "Pressed()", "rb::Signals", RB_SIGNALS, function);

void TGRbeerFrame::MakeConnections() {
   // Define actions to take when buttons are pressed:
   //     fMainFramePointer->Connect(fTextButtonPointer, "Pressed()", 0, 0, "some_function()");

	this->MoveResize(20,20,this->GetWidth(), this->GetHeight());
	fUnattach->SetEnabled(false);
	fUpdateRate->SetLimits(TGNumberFormat::kNELLimitMinMax, 1, 600);
	fUpdateRate->SetNumber(5);

	RB_SIGNALS->PopulateEvents();

	RB_BUTTON_CONNECT(fAttachOnline, "AttachOnline()");
	RB_BUTTON_CONNECT(fAttachFile, "AttachFile()");
	RB_BUTTON_CONNECT(fAttachList, "AttachList()");
	RB_BUTTON_CONNECT(fUnattach, "Unattach()");
	RB_BUTTON_CONNECT(fRefreshAll, "UpdateAll()");
	RB_BUTTON_CONNECT(fRefreshCurrent, "UpdateCurrent()");
	RB_BUTTON_CONNECT(fZeroAll, "ZeroAll()");
	RB_BUTTON_CONNECT(fZeroCurrent, "ZeroCurrent()");
	RB_BUTTON_CONNECT(fCreateNew, "CreateNew()");
	RB_BUTTON_CONNECT(fDivideCurrent, "DivideCurrent()");
	RB_BUTTON_CONNECT(fStartRefresh, "Update()");
	RB_BUTTON_CONNECT(fLogy, "ToggleLog(=1)");
	RB_BUTTON_CONNECT(fLogz, "ToggleLog(=2)");
	RB_BUTTON_CONNECT(fClearCurrent, "ClearCurrent()");
	RB_BUTTON_CONNECT(fSetFilter, "SetFilter()");

	RB_SIGNALS->Connect("Unattaching()", "TGTextButton", fUnattach, "ChangeBackground(=0xe0e0e0)");	
	RB_SIGNALS->Connect("Unattaching()", "TGTextButton", fUnattach, "SetEnabled(=false)");
	RB_SIGNALS->Connect("Unattaching()", "TGTextButton", fIsContinuous, "SetEnabled(=true)");
	RB_SIGNALS->Connect("Attaching()", "TGTextButton", fUnattach, "ChangeBackground(=0x00ff00)");
	RB_SIGNALS->Connect("Attaching()", "TGTextButton", fUnattach, "SetEnabled(=true)");
	RB_SIGNALS->Connect("Attaching()", "TGTextButton", fIsContinuous, "SetEnabled(=false)");

	RB_SIGNALS->Connect("StartingUpdate()", "TGTextButton", fStartRefresh, "ChangeBackground(=0xff0000)");
	RB_SIGNALS->Connect("StartingUpdate()", "TGTextButton", fStartRefresh, "ChangeText(=\"Stop Refresh\")");
	RB_SIGNALS->Connect("StoppingUpdate()", "TGTextButton", fStartRefresh, "ChangeBackground(=0x00ff00)");
	RB_SIGNALS->Connect("StoppingUpdate()", "TGTextButton", fStartRefresh, "ChangeText(=\"Start Refresh\")");

	RB_SIGNALS->Connect("StartingUpdate(Int_t)", "TGNumberEntry", fUpdateRate, "SetNumber(Int_t)");
  fUpdateRate->Connect("ValueSet(Long_t)", "rb::Signals", RB_SIGNALS, "ChangeUpdateRate(Long_t)");

	RB_SIGNALS->Connect("AttachedFile(const char*)", "TGLabel", fLabelSource, "SetText(const char*)");   
	RB_SIGNALS->Connect("AttachedOnline(const char*)", "TGLabel", fLabelSource, "SetText(const char*)");
	RB_SIGNALS->Connect("Unattaching()", "TGLabel", fLabelSource, "SetText(=\"[none]\")");

	fConfigSave->Connect("Pressed()", "rb::HistSignals", RB_SIGNALS, "WriteConfig(=1)");
	fConfigLoad->Connect("Pressed()", "rb::HistSignals", RB_SIGNALS, "ReadCanvasConfig()");
	fConfigSave->Connect("Pressed()", "TGTextButton", fConfigSave, "SetDown(=false)");
	fConfigLoad->Connect("Pressed()", "TGTextButton", fConfigLoad, "SetDown(=false)");

	fSaveData->Connect("Clicked()", "rb::Signals", RB_SIGNALS, "SaveData()");
	fSaveHist->Connect("Clicked()", "rb::Signals", RB_SIGNALS, "SaveHists()");
	RB_SIGNALS->Connect("Attaching()", "TGTextButton", fSaveData, "SetEnabled(=false)");
	RB_SIGNALS->Connect("Attaching()", "TGTextButton", fSaveHist, "SetEnabled(=false)");
	RB_SIGNALS->Connect("Unattaching()", "TGTextButton", fSaveData, "SetEnabled(=true)");
	RB_SIGNALS->Connect("Unattaching()", "rb::Signals", RB_SIGNALS, "EnableSaveHists()");
	fSaveData->Clicked();

	Connect("TCanvas", "ProcessedEvent(Int_t, Int_t, Int_t, TObject*)",
					"rb::Signals", RB_SIGNALS, "DoubleClickCanvas(Int_t, Int_t, Int_t, TObject*)");	

	Connect("TCanvas", "Modified()", "rb::Signals", RB_SIGNALS, "SyncWithGpad()");
}
 
TGRbeerFrame::~TGRbeerFrame() {
	rb::Rint::gApp()->DeleteSignals();
}


#undef RB_SIGNALS
#undef RB_BUTTON_CONNECT
