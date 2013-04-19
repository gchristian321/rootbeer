//! \file HistGui.cxx
//! \brief Implements HistGui.hxx
#include "HistGui.hxx"
#include "Rint.hxx"

void TGHistVarFrame::HistGuiLayout()
{

   // main frame
   // /* TGMainFrame* */ this = new TGMainFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
	 this->SetName("fHistVarFrame");
	 this->SetWindowName("rootbeer histograms");
   this->SetLayoutBroken(kTRUE);

   // composite frame
   /* TGCompositeFrame* */ fMainFrame2592 = new TGCompositeFrame(this,754,487,kVerticalFrame);   fMainFrame2592->SetName("fMainFrame2592");
   fMainFrame2592->SetLayoutBroken(kTRUE);

   // composite frame
   /* TGCompositeFrame* */ fMainFrame2438 = new TGCompositeFrame(fMainFrame2592,749,484,kVerticalFrame);   fMainFrame2438->SetName("fMainFrame2438");
   fMainFrame2438->SetLayoutBroken(kTRUE);

   // composite frame
   /* TGCompositeFrame* */ fMainFrame1180 = new TGCompositeFrame(fMainFrame2438,746,479,kVerticalFrame);   fMainFrame1180->SetName("fMainFrame1180");
   fMainFrame1180->SetLayoutBroken(kTRUE);

   // composite frame
   /* TGCompositeFrame* */ fMainFrame2078 = new TGCompositeFrame(fMainFrame1180,754,492,kVerticalFrame);   fMainFrame2078->SetName("fMainFrame2078");
   fMainFrame2078->SetLayoutBroken(kTRUE);

   // composite frame
   /* TGCompositeFrame* */ fMainFrame4698 = new TGCompositeFrame(fMainFrame2078,787,504,kVerticalFrame);   fMainFrame4698->SetName("fMainFrame4698");
   fMainFrame4698->SetLayoutBroken(kTRUE);

   // composite frame
   /* TGCompositeFrame* */ fMainFrame1001 = new TGCompositeFrame(fMainFrame4698,746,473,kVerticalFrame);   fMainFrame1001->SetName("fMainFrame1001");
   fMainFrame1001->SetLayoutBroken(kTRUE);

   // composite frame
   /* TGCompositeFrame* */ fMainFrame942 = new TGCompositeFrame(fMainFrame1001,744,475,kVerticalFrame);   fMainFrame942->SetName("fMainFrame942");
   fMainFrame942->SetLayoutBroken(kTRUE);

   // composite frame
   /* TGCompositeFrame* */ fMainFrame1678 = new TGCompositeFrame(fMainFrame942,745,477,kVerticalFrame);   fMainFrame1678->SetName("fMainFrame1678");
   fMainFrame1678->SetLayoutBroken(kTRUE);

   // composite frame
   /* TGCompositeFrame* */ fMainFrame909 = new TGCompositeFrame(fMainFrame1678,742,471,kVerticalFrame);   fMainFrame909->SetName("fMainFrame909");
   fMainFrame909->SetLayoutBroken(kTRUE);

   // composite frame
   /* TGCompositeFrame* */ fMainFrame1598 = new TGCompositeFrame(fMainFrame909,743,471,kVerticalFrame);   fMainFrame1598->SetName("fMainFrame1598");
   fMainFrame1598->SetLayoutBroken(kTRUE);

   // composite frame
   /* TGCompositeFrame* */ fMainFrame2824 = new TGCompositeFrame(fMainFrame1598,744,473,kVerticalFrame);   fMainFrame2824->SetName("fMainFrame2824");
   fMainFrame2824->SetLayoutBroken(kTRUE);

   // tab widget
   /* TGTab* */ fHistTab = new TGTab(fMainFrame2824,737,465);
   // container of "Histograms"
   TGCompositeFrame *fCompositeFrame717;
   fCompositeFrame717 = fHistTab->AddTab("Histograms");
   fCompositeFrame717->SetLayoutManager(new TGVerticalLayout(fCompositeFrame717));
   fCompositeFrame717->SetLayoutBroken(kTRUE);

   // canvas widget
   /* TGCanvas* */ fHistCanvas = new TGCanvas(fCompositeFrame717,216,320);
   // canvas viewport
   TGViewPort *fViewPort719 = fHistCanvas->GetViewPort();

   // list tree
   /* TGListTree* */ fHistTree = new TGListTree(fHistCanvas, kHorizontalFrame);
   // const TGPicture *popen = gClient->GetPicture("ofolder_t.xpm");       //used for list tree items
   // const TGPicture *pclose= gClient->GetPicture("folder_t.xpm");      //used for list tree items
	 fHistTree->AddRoot(gROOT->GetName());
	 fHistTree->FindItemByPathname(gROOT->GetName())->SetOpen(true);



   fViewPort719->AddFrame(fHistTree);
   fHistTree->SetLayoutManager(new TGHorizontalLayout(fHistTree));
   fHistTree->MapSubwindows();
   fHistCanvas->SetContainer(fHistTree);
   fHistCanvas->MapSubwindows();
   fCompositeFrame717->AddFrame(fHistCanvas, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fHistCanvas->MoveResize(2,2,216,320);

   TGFont *ufont;         // will reflect user font changes
   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   TGGC   *uGC;           // will reflect user GC changes
   // graphics context changes
   GCValues_t valButton729;
   valButton729.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valButton729.fForeground);
   gClient->GetColorByName("#e0e0e0",valButton729.fBackground);
   valButton729.fFillStyle = kFillSolid;
   valButton729.fFont = ufont->GetFontHandle();
   valButton729.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valButton729, kTRUE);

   ULong_t ucolor;        // will reflect user color changes
   gClient->GetColorByName("#000000",ucolor);
   /* TGTextButton* */ fCommandOk = new TGTextButton(fCompositeFrame717,"OK",-1,uGC->GetGC());
fCommandOk->SetFont(ufont->GetFontStruct());
   fCommandOk->SetTextJustify(36);
   fCommandOk->SetMargins(0,0,0,0);
   fCommandOk->SetWrapLength(-1);
   fCommandOk->Resize(96,22);

   fCommandOk->ChangeBackground(ucolor);
   fCompositeFrame717->AddFrame(fCommandOk, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fCommandOk->MoveResize(426,411,96,22);

   ufont = gClient->GetFont("-*-helvetica-bold-r-*-*-12-*-*-*-*-*-iso8859-1");
   /* TGTextButton* */ fQuit = new TGTextButton(fCompositeFrame717,"Quit");
	 fQuit->SetFont(ufont->GetFontStruct());
   fQuit->SetTextJustify(36);
   fQuit->SetMargins(0,0,0,0);
   fQuit->SetWrapLength(-1);
   fQuit->Resize(91,24);

   fQuit->ChangeBackground(0xff0000);
	 fQuit->SetForegroundColor(0xffffff);
	 fQuit->SetFont(ufont->GetFontStruct());
   fCompositeFrame717->AddFrame(fQuit, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fQuit->MoveResize(620,411,96,22);


   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valEntry730;
   valEntry730.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valEntry730.fForeground);
   gClient->GetColorByName("#e0e0e0",valEntry730.fBackground);
   valEntry730.fFillStyle = kFillSolid;
   valEntry730.fFont = ufont->GetFontHandle();
   valEntry730.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valEntry730, kTRUE);
   /* TGTextEntry* */ fCommandEntry = new TGTextEntry(fCompositeFrame717, new TGTextBuffer(14),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);   fCommandEntry->SetMaxLength(4096);
   fCommandEntry->SetAlignment(kTextLeft);
   fCommandEntry->SetText("");
   fCommandEntry->Resize(412,fCommandEntry->GetDefaultHeight());
   fCompositeFrame717->AddFrame(fCommandEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fCommandEntry->MoveResize(5,411,412,22);

   ufont = gClient->GetFont("-*-helvetica--*-*-12-*-*-*-*-*-*-*");

   // graphics context changes
   GCValues_t vall731;
   vall731.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",vall731.fForeground);
   gClient->GetColorByName("#e0e0e0",vall731.fBackground);
   vall731.fFillStyle = kFillSolid;
   vall731.fFont = ufont->GetFontHandle();
   vall731.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall731, kTRUE);
   /* TGLabel* */ fCommandLabel = new TGLabel(fCompositeFrame717,"Command (member function)",uGC->GetGC(),ufont->GetFontStruct());   fCommandLabel->SetTextJustify(33);
   fCommandLabel->SetMargins(0,0,0,0);
   fCommandLabel->SetWrapLength(-1);
   fCompositeFrame717->AddFrame(fCommandLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fCommandLabel->MoveResize(7,390,192,18);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valButton732;
   valButton732.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valButton732.fForeground);
   gClient->GetColorByName("#e0e0e0",valButton732.fBackground);
   valButton732.fFillStyle = kFillSolid;
   valButton732.fFont = ufont->GetFontHandle();
   valButton732.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valButton732, kTRUE);

   gClient->GetColorByName("#0000ff",ucolor);
   /* TGTextButton* */ fDrawButton = new TGTextButton(fCompositeFrame717,"Draw",-1,uGC->GetGC());
fDrawButton->SetFont(ufont->GetFontStruct());
   fDrawButton->SetTextJustify(36);
   fDrawButton->SetMargins(0,0,0,0);
   fDrawButton->SetWrapLength(-1);
   fDrawButton->Resize(98,24);

   fDrawButton->ChangeBackground(ucolor);
   fCompositeFrame717->AddFrame(fDrawButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fDrawButton->MoveResize(118,326,98,24);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valButton733;
   valButton733.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valButton733.fForeground);
   gClient->GetColorByName("#e0e0e0",valButton733.fBackground);
   valButton733.fFillStyle = kFillSolid;
   valButton733.fFont = ufont->GetFontHandle();
   valButton733.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valButton733, kTRUE);

   gClient->GetColorByName("#ff0000",ucolor);
   /* TGTextButton* */ fDeleteButton = new TGTextButton(fCompositeFrame717,"Delete",-1,uGC->GetGC());
fDeleteButton->SetFont(ufont->GetFontStruct());
   fDeleteButton->SetTextJustify(36);
   fDeleteButton->SetMargins(0,0,0,0);
   fDeleteButton->SetWrapLength(-1);
   fDeleteButton->Resize(98,24);

   fDeleteButton->ChangeBackground(ucolor);
   fCompositeFrame717->AddFrame(fDeleteButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fDeleteButton->MoveResize(2,326,98,24);

   // "Create New" group frame
   /* TGGroupFrame* */ fHistCreateFrame = new TGGroupFrame(fCompositeFrame717,"Create New");   fHistCreateFrame->SetLayoutBroken(kTRUE);

   /* TGNumberEntryField* */ fHighX = new TGNumberEntryField(fHistCreateFrame, 6, (Double_t) 0);   fHistCreateFrame->AddFrame(fHighX, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fHighX->MoveResize(436,103,59,22);
   /* TGNumberEntryField* */ fLowX = new TGNumberEntryField(fHistCreateFrame, 5, (Double_t) 0);   fHistCreateFrame->AddFrame(fLowX, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLowX->MoveResize(372,103,59,22);
   /* TGNumberEntryField* */ fBinsX = new TGNumberEntryField(fHistCreateFrame, 4, (Double_t) 0);   fHistCreateFrame->AddFrame(fBinsX, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fBinsX->MoveResize(308,103,59,22);

	 fBinsX->Associate(this);
	 fLowX->Associate(this);
	 fHighX->Associate(this);

   /* TGNumberEntryField* */ fBinsY = new TGNumberEntryField(fHistCreateFrame, 8, (Double_t) 0);   fHistCreateFrame->AddFrame(fBinsY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fBinsY->MoveResize(308,135,59,22);
   /* TGNumberEntryField* */ fLowY = new TGNumberEntryField(fHistCreateFrame, 9, (Double_t) 0);   fHistCreateFrame->AddFrame(fLowY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLowY->MoveResize(372,135,59,22);
   /* TGNumberEntryField* */ fHighY = new TGNumberEntryField(fHistCreateFrame, 10, (Double_t) 0);   fHistCreateFrame->AddFrame(fHighY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fHighY->MoveResize(436,135,59,22);

	 fBinsY->Associate(this);
	 fLowY->Associate(this);
	 fHighY->Associate(this);

   /* TGNumberEntryField* */ fBinsZ = new TGNumberEntryField(fHistCreateFrame, 12, (Double_t) 0);   fHistCreateFrame->AddFrame(fBinsZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fBinsZ->MoveResize(308,167,59,22);
   /* TGNumberEntryField* */ fLowZ = new TGNumberEntryField(fHistCreateFrame, 13, (Double_t) 0);   fHistCreateFrame->AddFrame(fLowZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLowZ->MoveResize(372,167,59,22);
   /* TGNumberEntryField* */ fHighZ = new TGNumberEntryField(fHistCreateFrame, 14, (Double_t) 0);   fHistCreateFrame->AddFrame(fHighZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fHighZ->MoveResize(436,167,59,22);

	 fBinsZ->Associate(this);
	 fLowZ->Associate(this);
	 fHighZ->Associate(this);

	 fBinsX->SetFormat(TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative);
	 fBinsY->SetFormat(TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative);
	 fBinsZ->SetFormat(TGNumberFormat::kNESInteger, TGNumberFormat::kNEANonNegative);

   ufont = gClient->GetFont("-*-helvetica--*-*-12-*-*-*-*-*-*-*");

   // graphics context changes
   GCValues_t vall771;
   vall771.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",vall771.fForeground);
   gClient->GetColorByName("#e0e0e0",vall771.fBackground);
   vall771.fFillStyle = kFillSolid;
   vall771.fFont = ufont->GetFontHandle();
   vall771.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall771, kTRUE);
   /* TGLabel* */ fLabelX = new TGLabel(fHistCreateFrame,"x",uGC->GetGC(),ufont->GetFontStruct());   fLabelX->SetTextJustify(36);
   fLabelX->SetMargins(0,0,0,0);
   fLabelX->SetWrapLength(-1);
   fHistCreateFrame->AddFrame(fLabelX, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelX->MoveResize(8,105,24,18);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t vall772;
   vall772.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",vall772.fForeground);
   gClient->GetColorByName("#e0e0e0",vall772.fBackground);
   vall772.fFillStyle = kFillSolid;
   vall772.fFont = ufont->GetFontHandle();
   vall772.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall772, kTRUE);
   /* TGLabel* */ fLabelY = new TGLabel(fHistCreateFrame,"y",uGC->GetGC());   fLabelY->SetTextJustify(36);
   fLabelY->SetMargins(0,0,0,0);
   fLabelY->SetWrapLength(-1);
   fHistCreateFrame->AddFrame(fLabelY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelY->MoveResize(8,137,24,18);

   ufont = gClient->GetFont("-*-helvetica--*-*-12-*-*-*-*-*-*-*");

   // graphics context changes
   GCValues_t vall773;
   vall773.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",vall773.fForeground);
   gClient->GetColorByName("#e0e0e0",vall773.fBackground);
   vall773.fFillStyle = kFillSolid;
   vall773.fFont = ufont->GetFontHandle();
   vall773.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall773, kTRUE);
   /* TGLabel* */ fLabelZ = new TGLabel(fHistCreateFrame,"z",uGC->GetGC(),ufont->GetFontStruct());   fLabelZ->SetTextJustify(36);
   fLabelZ->SetMargins(0,0,0,0);
   fLabelZ->SetWrapLength(-1);
   fHistCreateFrame->AddFrame(fLabelZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelZ->MoveResize(8,169,24,18);

   ufont = gClient->GetFont("-*-helvetica--*-*-12-*-*-*-*-*-*-*");

   // graphics context changes
   GCValues_t vall774;
   vall774.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",vall774.fForeground);
   gClient->GetColorByName("#e0e0e0",vall774.fBackground);
   vall774.fFillStyle = kFillSolid;
   vall774.fFont = ufont->GetFontHandle();
   vall774.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall774, kTRUE);
   /* TGLabel* */ fLabelParameter = new TGLabel(fHistCreateFrame,"Parameter",uGC->GetGC(),ufont->GetFontStruct());   fLabelParameter->SetTextJustify(33);
   fLabelParameter->SetMargins(0,0,0,0);
   fLabelParameter->SetWrapLength(-1);
   fHistCreateFrame->AddFrame(fLabelParameter, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelParameter->MoveResize(43,79,63,18);

   ufont = gClient->GetFont("-*-helvetica--*-*-12-*-*-*-*-*-*-*");

   // graphics context changes
   GCValues_t vall775;
   vall775.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",vall775.fForeground);
   gClient->GetColorByName("#e0e0e0",vall775.fBackground);
   vall775.fFillStyle = kFillSolid;
   vall775.fFont = ufont->GetFontHandle();
   vall775.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall775, kTRUE);
   /* TGLabel* */ fLabelBins = new TGLabel(fHistCreateFrame,"Bins",uGC->GetGC(),ufont->GetFontStruct());   fLabelBins->SetTextJustify(33);
   fLabelBins->SetMargins(0,0,0,0);
   fLabelBins->SetWrapLength(-1);
   fHistCreateFrame->AddFrame(fLabelBins, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelBins->MoveResize(308,79,32,18);

   ufont = gClient->GetFont("-*-helvetica--*-*-12-*-*-*-*-*-*-*");

   // graphics context changes
   GCValues_t vall776;
   vall776.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",vall776.fForeground);
   gClient->GetColorByName("#e0e0e0",vall776.fBackground);
   vall776.fFillStyle = kFillSolid;
   vall776.fFont = ufont->GetFontHandle();
   vall776.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall776, kTRUE);
   /* TGLabel* */ fLabelLow = new TGLabel(fHistCreateFrame,"Low",uGC->GetGC(),ufont->GetFontStruct());   fLabelLow->SetTextJustify(33);
   fLabelLow->SetMargins(0,0,0,0);
   fLabelLow->SetWrapLength(-1);
   fHistCreateFrame->AddFrame(fLabelLow, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelLow->MoveResize(372,79,32,18);

   ufont = gClient->GetFont("-*-helvetica--*-*-12-*-*-*-*-*-*-*");

   // graphics context changes
   GCValues_t vall777;
   vall777.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",vall777.fForeground);
   gClient->GetColorByName("#e0e0e0",vall777.fBackground);
   vall777.fFillStyle = kFillSolid;
   vall777.fFont = ufont->GetFontHandle();
   vall777.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall777, kTRUE);
   /* TGLabel* */ fLabelHigh = new TGLabel(fHistCreateFrame,"High",uGC->GetGC(),ufont->GetFontStruct());   fLabelHigh->SetTextJustify(33);
   fLabelHigh->SetMargins(0,0,0,0);
   fLabelHigh->SetWrapLength(-1);
   fHistCreateFrame->AddFrame(fLabelHigh, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fLabelHigh->MoveResize(436,79,32,18);

   gClient->GetColorByName("#ffffff",ucolor);

   // combo box
   /* TGComboBox* */ fTypeEntry = new TGComboBox(fHistCreateFrame,-1,kHorizontalFrame | kSunkenFrame | kDoubleBorder | kOwnBackground);
	 int ne = 0;
	 fTypeEntry->AddEntry("1D ", ne++); //0
   fTypeEntry->AddEntry("2D ", ne++); //1
   fTypeEntry->AddEntry("3D ", ne++); //2
	 fTypeEntry->AddEntry("Scaler", ne++); //3
   fTypeEntry->AddEntry("Summary [h]", ne++); //4
   fTypeEntry->AddEntry("Summary [v]", ne++); //5
   fTypeEntry->AddEntry("1D gamma ", ne++); //6
   fTypeEntry->AddEntry("2D gamma ", ne++); //7
   fTypeEntry->AddEntry("3D gamma ", ne++); //8
   fTypeEntry->AddEntry("Bit ", ne++); //9
   fTypeEntry->Resize(102,22);
   fTypeEntry->Select(0);
   fHistCreateFrame->AddFrame(fTypeEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTypeEntry->MoveResize(39,48,102,22);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valEntry798;
   valEntry798.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valEntry798.fForeground);
   gClient->GetColorByName("#e0e0e0",valEntry798.fBackground);
   valEntry798.fFillStyle = kFillSolid;
   valEntry798.fFont = ufont->GetFontHandle();
   valEntry798.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valEntry798, kTRUE);
   /* TGTextEntry* */ fGateEntry = new TGTextEntry(fHistCreateFrame, new TGTextBuffer(14),15,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);   fGateEntry->SetMaxLength(4096);
   fGateEntry->SetAlignment(kTextLeft);
   fGateEntry->SetText("");
   fGateEntry->Resize(256,fGateEntry->GetDefaultHeight());
   fHistCreateFrame->AddFrame(fGateEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fGateEntry->MoveResize(39,226,256,22);
	 fGateEntry->Associate(this);

   ufont = gClient->GetFont("-*-helvetica--*-*-12-*-*-*-*-*-*-*");

   // graphics context changes
   GCValues_t vall799;
   vall799.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",vall799.fForeground);
   gClient->GetColorByName("#e0e0e0",vall799.fBackground);
   vall799.fFillStyle = kFillSolid;
   vall799.fFont = ufont->GetFontHandle();
   vall799.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall799, kTRUE);
   /* TGLabel* */ fGateLabel = new TGLabel(fHistCreateFrame,"Gate",uGC->GetGC(),ufont->GetFontStruct());   fGateLabel->SetTextJustify(33);
   fGateLabel->SetMargins(0,0,0,0);
   fGateLabel->SetWrapLength(-1);
   fHistCreateFrame->AddFrame(fGateLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fGateLabel->MoveResize(39,201,32,18);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valEntry800;
   valEntry800.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valEntry800.fForeground);
   gClient->GetColorByName("#e0e0e0",valEntry800.fBackground);
   valEntry800.fFillStyle = kFillSolid;
   valEntry800.fFont = ufont->GetFontHandle();
   valEntry800.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valEntry800, kTRUE);
   /* TGTextEntry* */ fNameEntry = new TGTextEntry(fHistCreateFrame, new TGTextBuffer(14),1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);   fNameEntry->SetMaxLength(4096);
   fNameEntry->SetAlignment(kTextLeft);
   fNameEntry->SetText("");
   fNameEntry->Resize(112,fNameEntry->GetDefaultHeight());
   fHistCreateFrame->AddFrame(fNameEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fNameEntry->MoveResize(151,48,112,22);
	 fNameEntry->Associate(this);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valEntry801;
   valEntry801.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valEntry801.fForeground);
   gClient->GetColorByName("#e0e0e0",valEntry801.fBackground);
   valEntry801.fFillStyle = kFillSolid;
   valEntry801.fFont = ufont->GetFontHandle();
   valEntry801.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valEntry801, kTRUE);
   /* TGTextEntry* */ fTitleEntry = new TGTextEntry(fHistCreateFrame, new TGTextBuffer(14),2,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);   fTitleEntry->SetMaxLength(4096);
   fTitleEntry->SetAlignment(kTextLeft);
   fTitleEntry->SetText("");
   fTitleEntry->Resize(216,fTitleEntry->GetDefaultHeight());
   fHistCreateFrame->AddFrame(fTitleEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTitleEntry->MoveResize(272,48,216,22);
	 fTitleEntry->Associate(this);

   ufont = gClient->GetFont("-*-helvetica--*-*-12-*-*-*-*-*-*-*");

   // graphics context changes
   GCValues_t vall802;
   vall802.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",vall802.fForeground);
   gClient->GetColorByName("#e0e0e0",vall802.fBackground);
   vall802.fFillStyle = kFillSolid;
   vall802.fFont = ufont->GetFontHandle();
   vall802.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall802, kTRUE);
   /* TGLabel* */ fTypeLabel = new TGLabel(fHistCreateFrame,"Type",uGC->GetGC(),ufont->GetFontStruct());   fTypeLabel->SetTextJustify(33);
   fTypeLabel->SetMargins(0,0,0,0);
   fTypeLabel->SetWrapLength(-1);
   fHistCreateFrame->AddFrame(fTypeLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTypeLabel->MoveResize(39,24,40,18);

   gClient->GetColorByName("#ffffff",ucolor);

   // combo box
   /* TGComboBox* */ fParamZ = new TGComboBox(fHistCreateFrame,"",11,kHorizontalFrame | kSunkenFrame | kDoubleBorder | kOwnBackground);   fParamZ->Resize(264,22);
   fParamZ->Select(-1);
   fHistCreateFrame->AddFrame(fParamZ, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fParamZ->MoveResize(39,167,264,22);
//	 fParamZ->Associate(this);
	 fParamZ->GetTextEntry()->Associate(this);

   gClient->GetColorByName("#ffffff",ucolor);

   // combo box
   /* TGComboBox* */ fParamY = new TGComboBox(fHistCreateFrame,"",7,kHorizontalFrame | kSunkenFrame | kDoubleBorder | kOwnBackground);   fParamY->Resize(264,22);
   fParamY->Select(-1);
   fHistCreateFrame->AddFrame(fParamY, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fParamY->MoveResize(39,135,264,22);
//	 fParamY->Associate(this);
	 fParamY->GetTextEntry()->Associate(this);

   gClient->GetColorByName("#ffffff",ucolor);

   // combo box
   /* TGComboBox* */ fParamX = new TGComboBox(fHistCreateFrame,"",3,kHorizontalFrame | kSunkenFrame | kDoubleBorder | kOwnBackground);   fParamX->Resize(264,22);
   fParamX->Select(0);
   fHistCreateFrame->AddFrame(fParamX, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fParamX->MoveResize(39,103,264,22);
//	 fParamX->Associate(this);
	 fParamX->GetTextEntry()->Associate(this);

   gClient->GetColorByName("#ffffff",ucolor);

   // combo box
   /* TGComboBox* */ fEventEntry = new TGComboBox(fHistCreateFrame,-1,kHorizontalFrame | kSunkenFrame | kDoubleBorder | kOwnBackground);   fEventEntry->Resize(256,22);
   fEventEntry->Select(-1);
   fHistCreateFrame->AddFrame(fEventEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fEventEntry->MoveResize(40,280,256,22);

   ufont = gClient->GetFont("-*-helvetica--*-*-12-*-*-*-*-*-*-*");

   // graphics context changes
   GCValues_t vall847;
   vall847.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",vall847.fForeground);
   gClient->GetColorByName("#e0e0e0",vall847.fBackground);
   vall847.fFillStyle = kFillSolid;
   vall847.fFont = ufont->GetFontHandle();
   vall847.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall847, kTRUE);
   /* TGLabel* */ fEventLabel = new TGLabel(fHistCreateFrame,"Event type",uGC->GetGC(),ufont->GetFontStruct());   fEventLabel->SetTextJustify(33);
   fEventLabel->SetMargins(0,0,0,0);
   fEventLabel->SetWrapLength(-1);
   fHistCreateFrame->AddFrame(fEventLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fEventLabel->MoveResize(40,256,64,18);

   ufont = gClient->GetFont("-*-helvetica--*-*-12-*-*-*-*-*-*-*");

   // graphics context changes
   GCValues_t vall848;
   vall848.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",vall848.fForeground);
   gClient->GetColorByName("#e0e0e0",vall848.fBackground);
   vall848.fFillStyle = kFillSolid;
   vall848.fFont = ufont->GetFontHandle();
   vall848.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall848, kTRUE);
   /* TGLabel* */ fNameLabel = new TGLabel(fHistCreateFrame,"Name",uGC->GetGC(),ufont->GetFontStruct());   fNameLabel->SetTextJustify(33);
   fNameLabel->SetMargins(0,0,0,0);
   fNameLabel->SetWrapLength(-1);
   fHistCreateFrame->AddFrame(fNameLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fNameLabel->MoveResize(151,24,40,18);

   ufont = gClient->GetFont("-*-helvetica--*-*-12-*-*-*-*-*-*-*");

   // graphics context changes
   GCValues_t vall849;
   vall849.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",vall849.fForeground);
   gClient->GetColorByName("#e0e0e0",vall849.fBackground);
   vall849.fFillStyle = kFillSolid;
   vall849.fFont = ufont->GetFontHandle();
   vall849.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall849, kTRUE);
   /* TGLabel* */ fTitleLabel = new TGLabel(fHistCreateFrame,"Title",uGC->GetGC(),ufont->GetFontStruct());   fTitleLabel->SetTextJustify(33);
   fTitleLabel->SetMargins(0,0,0,0);
   fTitleLabel->SetWrapLength(-1);
   fHistCreateFrame->AddFrame(fTitleLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTitleLabel->MoveResize(272,24,40,18);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valButton850;
   valButton850.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valButton850.fForeground);
   gClient->GetColorByName("#e0e0e0",valButton850.fBackground);
   valButton850.fFillStyle = kFillSolid;
   valButton850.fFont = ufont->GetFontHandle();
   valButton850.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valButton850, kTRUE);

   gClient->GetColorByName("#00ff00",ucolor);
   /* TGTextButton* */ fHistCreateButton = new TGTextButton(fHistCreateFrame,"Create/Replace",-1,uGC->GetGC());
fHistCreateButton->SetFont(ufont->GetFontStruct());
   fHistCreateButton->SetTextJustify(36);
   fHistCreateButton->SetMargins(0,0,0,0);
   fHistCreateButton->SetWrapLength(-1);
   fHistCreateButton->Resize(98,24);

   fHistCreateButton->ChangeBackground(ucolor);
   fHistCreateFrame->AddFrame(fHistCreateButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fHistCreateButton->MoveResize(40,315,98,24);
   fHistCreateButton->ChangeBackground(ucolor);
   fHistCreateFrame->AddFrame(fHistCreateButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fHistCreateButton->MoveResize(40,315,98,24);

   gClient->GetColorByName("#ffcc00",ucolor);
   /* TGTextButton* */ fHistRegateButton = new TGTextButton(fHistCreateFrame,"Regate",-1,uGC->GetGC());
fHistRegateButton->SetFont(ufont->GetFontStruct());
   fHistRegateButton->SetTextJustify(36);
   fHistRegateButton->SetMargins(0,0,0,0);
   fHistRegateButton->SetWrapLength(-1);
   fHistRegateButton->Resize(98,24);

   fHistRegateButton->ChangeBackground(ucolor);
   fHistCreateFrame->AddFrame(fHistRegateButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fHistRegateButton->MoveResize(40,315,98,24);
   fHistRegateButton->ChangeBackground(ucolor);
   fHistCreateFrame->AddFrame(fHistRegateButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fHistRegateButton->MoveResize(150,315,98,24);

	 fHistReplaceButton = new TGCheckButton(fHistCreateFrame, "Replace");
	 fHistCreateFrame->AddFrame(fHistReplaceButton,  new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	 fHistReplaceButton->MoveResize(150+98+10,320,70,24);
	 fHistReplaceButton->SetDown(kFALSE);


   // "Configuration File" group frame
   /* TGGroupFrame* */ fHistConfigFrame = new TGGroupFrame(fHistCreateFrame,"Configuration File");   fHistConfigFrame->SetLayoutBroken(kTRUE);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valButton852;
   valButton852.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valButton852.fForeground);
   gClient->GetColorByName("#e0e0e0",valButton852.fBackground);
   valButton852.fFillStyle = kFillSolid;
   valButton852.fFont = ufont->GetFontHandle();
   valButton852.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valButton852, kTRUE);

   gClient->GetColorByName("#0000ff",ucolor);
   /* TGTextButton* */ fHistSaveButton = new TGTextButton(fHistConfigFrame,"Save",-1,uGC->GetGC());
fHistSaveButton->SetFont(ufont->GetFontStruct());
   fHistSaveButton->SetTextJustify(36);
   fHistSaveButton->SetMargins(0,0,0,0);
   fHistSaveButton->SetWrapLength(-1);
   fHistSaveButton->Resize(92,24);

   fHistSaveButton->ChangeBackground(ucolor);
   fHistConfigFrame->AddFrame(fHistSaveButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fHistSaveButton->MoveResize(17,25,92,24);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valButton853;
   valButton853.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valButton853.fForeground);
   gClient->GetColorByName("#e0e0e0",valButton853.fBackground);
   valButton853.fFillStyle = kFillSolid;
   valButton853.fFont = ufont->GetFontHandle();
   valButton853.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valButton853, kTRUE);

   gClient->GetColorByName("#ffcc00",ucolor);
   /* TGTextButton* */ fHistLoadButton = new TGTextButton(fHistConfigFrame,"Load",-1,uGC->GetGC());
fHistLoadButton->SetFont(ufont->GetFontStruct());
   fHistLoadButton->SetTextJustify(36);
   fHistLoadButton->SetMargins(0,0,0,0);
   fHistLoadButton->SetWrapLength(-1);
   fHistLoadButton->Resize(92,24);

   fHistLoadButton->ChangeBackground(ucolor);
   fHistConfigFrame->AddFrame(fHistLoadButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fHistLoadButton->MoveResize(17,63,92,24);

   fHistConfigFrame->SetLayoutManager(new TGVerticalLayout(fHistConfigFrame));
   fHistConfigFrame->Resize(128,112);
   fHistCreateFrame->AddFrame(fHistConfigFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fHistConfigFrame->MoveResize(360,232,128,112);

   fHistCreateFrame->SetLayoutManager(new TGVerticalLayout(fHistCreateFrame));
   fHistCreateFrame->Resize(503,360);
   fCompositeFrame717->AddFrame(fHistCreateFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fHistCreateFrame->MoveResize(224,0,503,360);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valEntry854;
   valEntry854.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valEntry854.fForeground);
   gClient->GetColorByName("#e0e0e0",valEntry854.fBackground);
   valEntry854.fFillStyle = kFillSolid;
   valEntry854.fFont = ufont->GetFontHandle();
   valEntry854.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valEntry854, kTRUE);
   /* TGTextEntry* */ fDrawOptionEntry = new TGTextEntry(fCompositeFrame717, new TGTextBuffer(14),-1,uGC->GetGC(),ufont->GetFontStruct(),kSunkenFrame | kDoubleBorder | kOwnBackground);   fDrawOptionEntry->SetMaxLength(4096);
   fDrawOptionEntry->SetAlignment(kTextLeft);
   fDrawOptionEntry->SetText("");
   fDrawOptionEntry->Resize(74,fDrawOptionEntry->GetDefaultHeight());
   fCompositeFrame717->AddFrame(fDrawOptionEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fDrawOptionEntry->MoveResize(142,361,74,22);

   ufont = gClient->GetFont("-*-helvetica--*-*-12-*-*-*-*-*-*-*");

   /* TGTextButton* */ fMkdirButton = new TGTextButton(fCompositeFrame717,"New Directory");
fMkdirButton->SetFont(ufont->GetFontStruct());
   fMkdirButton->SetTextJustify(36);
   fMkdirButton->SetMargins(0,0,0,0);
   fMkdirButton->SetWrapLength(-1);
   fMkdirButton->Resize(96,22);

   fMkdirButton->ChangeBackground(ucolor);
   fCompositeFrame717->AddFrame(fMkdirButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fMkdirButton->MoveResize(230,361,96,22);


   // graphics context changes
   GCValues_t vall855;
   vall855.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",vall855.fForeground);
   gClient->GetColorByName("#e0e0e0",vall855.fBackground);
   vall855.fFillStyle = kFillSolid;
   vall855.fFont = ufont->GetFontHandle();
   vall855.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&vall855, kTRUE);
   /* TGLabel* */ fDrawOptionLabel = new TGLabel(fCompositeFrame717,"Draw option:",uGC->GetGC(),ufont->GetFontStruct());   fDrawOptionLabel->SetTextJustify(17);
   fDrawOptionLabel->SetMargins(0,0,0,0);
   fDrawOptionLabel->SetWrapLength(-1);
   fCompositeFrame717->AddFrame(fDrawOptionLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fDrawOptionLabel->MoveResize(60,358,80,18);


   // container of "Variables / Configuration"
   TGCompositeFrame *fCompositeFrame857;
   fCompositeFrame857 = fHistTab->AddTab("Variables / Configuration");
   fCompositeFrame857->SetLayoutManager(new TGVerticalLayout(fCompositeFrame857));
   fCompositeFrame857->SetLayoutBroken(kTRUE);

   // "Variables" group frame
   /* TGGroupFrame* */ fVariablesFrame = new TGGroupFrame(fCompositeFrame857,"Variables");   fVariablesFrame->SetLayoutBroken(kTRUE);


   ufont = gClient->GetFont("-*-helvetica-bold-r-*-*-12-*-*-*-*-*-iso8859-1");

   /* TGTextButton* */ fVarQuit = new TGTextButton(fCompositeFrame857,"Quit");
fVarQuit->SetFont(ufont->GetFontStruct());
   fVarQuit->SetTextJustify(36);
   fVarQuit->SetMargins(0,0,0,0);
   fVarQuit->SetWrapLength(-1);
   fVarQuit->Resize(91,24);

   fVarQuit->ChangeBackground(0xff0000);
	 fVarQuit->SetForegroundColor(0xffffff);
	 fVarQuit->SetFont(ufont->GetFontStruct());
   fCompositeFrame857->AddFrame(fVarQuit, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fVarQuit->MoveResize(620,411,96,22);

   ufont = gClient->GetFont("-*-helvetica--*-*-12-*-*-*-*-*-*-*");


   // canvas widget
   /* TGCanvas* */ fVariablesCanvas = new TGCanvas(fVariablesFrame,336,352);
   // canvas viewport
   TGViewPort *fViewPort860 = fVariablesCanvas->GetViewPort();

   // list tree
   /* TGListTree* */ fVariablesTree = new TGListTree(fVariablesCanvas,kHorizontalFrame);

   fViewPort860->AddFrame(fVariablesTree);
   fVariablesTree->SetLayoutManager(new TGHorizontalLayout(fVariablesTree));
   fVariablesTree->MapSubwindows();
   fVariablesCanvas->SetContainer(fVariablesTree);
   fVariablesCanvas->MapSubwindows();
   fVariablesFrame->AddFrame(fVariablesCanvas, new TGLayoutHints(kLHintsNormal));
   fVariablesCanvas->MoveResize(16,24,336,352);

   /* TGTextButton* */ fReadVarButton = new TGTextButton(fVariablesFrame,"Read Value");
fReadVarButton->SetFont(ufont->GetFontStruct());
   fReadVarButton->SetTextJustify(36);
   fReadVarButton->SetMargins(0,0,0,0);
   fReadVarButton->SetWrapLength(-1);
   fReadVarButton->Resize(98,24);

   fReadVarButton->ChangeBackground(ucolor);
   fVariablesFrame->AddFrame(fReadVarButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fReadVarButton->MoveResize(16,384,98,24);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valButton871;
   valButton871.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valButton871.fForeground);
   gClient->GetColorByName("#e0e0e0",valButton871.fBackground);
   valButton871.fFillStyle = kFillSolid;
   valButton871.fFont = ufont->GetFontHandle();
   valButton871.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valButton871, kTRUE);

   gClient->GetColorByName("#0000ff",ucolor);
   /* TGTextButton* */ fSetVarButton = new TGTextButton(fVariablesFrame,"Set Value",-1,uGC->GetGC());
fSetVarButton->SetFont(ufont->GetFontStruct());
   fSetVarButton->SetTextJustify(36);
   fSetVarButton->SetMargins(0,0,0,0);
   fSetVarButton->SetWrapLength(-1);
   fSetVarButton->Resize(98,24);

   fSetVarButton->ChangeBackground(ucolor);
   fVariablesFrame->AddFrame(fSetVarButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fSetVarButton->MoveResize(131,384,98,24);
   /* TGNumberEntryField* */ fVarEntry = new TGNumberEntryField(fVariablesFrame, 9, (Double_t) 0);   fVariablesFrame->AddFrame(fVarEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fVarEntry->MoveResize(248,384,88,24);

   fVariablesFrame->SetLayoutManager(new TGVerticalLayout(fVariablesFrame));
   fVariablesFrame->Resize(368,430);
   fCompositeFrame857->AddFrame(fVariablesFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fVariablesFrame->MoveResize(5,8,368,430);

   // "Configuration Files" group frame
   /* TGGroupFrame* */ fConfigAllFrame = new TGGroupFrame(fCompositeFrame857,"Configuration Files");   fConfigAllFrame->SetLayoutBroken(kTRUE);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valButton877;
   valButton877.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valButton877.fForeground);
   gClient->GetColorByName("#e0e0e0",valButton877.fBackground);
   valButton877.fFillStyle = kFillSolid;
   valButton877.fFont = ufont->GetFontHandle();
   valButton877.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valButton877, kTRUE);

   /* TGTextButton* */ fConfigHistSaveButton = new TGTextButton(fConfigAllFrame,"Save Histograms",-1,uGC->GetGC());
fConfigHistSaveButton->SetFont(ufont->GetFontStruct());
   fConfigHistSaveButton->SetTextJustify(36);
   fConfigHistSaveButton->SetMargins(0,0,0,0);
   fConfigHistSaveButton->SetWrapLength(-1);
   fConfigHistSaveButton->Resize(105,24);

   fConfigHistSaveButton->ChangeBackground(ucolor);
   fConfigAllFrame->AddFrame(fConfigHistSaveButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fConfigHistSaveButton->MoveResize(16,27,105,24);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valButton878;
   valButton878.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valButton878.fForeground);
   gClient->GetColorByName("#e0e0e0",valButton878.fBackground);
   valButton878.fFillStyle = kFillSolid;
   valButton878.fFont = ufont->GetFontHandle();
   valButton878.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valButton878, kTRUE);

   /* TGTextButton* */ fConfigVarSaveButton = new TGTextButton(fConfigAllFrame,"Save Variables",-1,uGC->GetGC());
fConfigVarSaveButton->SetFont(ufont->GetFontStruct());
   fConfigVarSaveButton->SetTextJustify(36);
   fConfigVarSaveButton->SetMargins(0,0,0,0);
   fConfigVarSaveButton->SetWrapLength(-1);
   fConfigVarSaveButton->Resize(105,24);

   fConfigVarSaveButton->ChangeBackground(ucolor);
   fConfigAllFrame->AddFrame(fConfigVarSaveButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fConfigVarSaveButton->MoveResize(135,27,105,24);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valButton879;
   valButton879.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#ffffff",valButton879.fForeground);
   gClient->GetColorByName("#e0e0e0",valButton879.fBackground);
   valButton879.fFillStyle = kFillSolid;
   valButton879.fFont = ufont->GetFontHandle();
   valButton879.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valButton879, kTRUE);

   /* TGTextButton* */ fConfigCanvasSaveButton = new TGTextButton(fConfigAllFrame,"Save Canvases",-1,uGC->GetGC());
fConfigCanvasSaveButton->SetFont(ufont->GetFontStruct());
   fConfigCanvasSaveButton->SetTextJustify(36);
   fConfigCanvasSaveButton->SetMargins(0,0,0,0);
   fConfigCanvasSaveButton->SetWrapLength(-1);
   fConfigCanvasSaveButton->Resize(105,24);

   fConfigCanvasSaveButton->ChangeBackground(ucolor);
   fConfigAllFrame->AddFrame(fConfigCanvasSaveButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fConfigCanvasSaveButton->MoveResize(16,63,105,24);

   ufont = gClient->GetFont("-*-helvetica-medium-r-*-*-12-*-*-*-*-*-iso8859-1");

   // graphics context changes
   GCValues_t valButton880;
   valButton880.fMask = kGCForeground | kGCBackground | kGCFillStyle | kGCFont | kGCGraphicsExposures;
   gClient->GetColorByName("#000000",valButton880.fForeground);
   gClient->GetColorByName("#e0e0e0",valButton880.fBackground);
   valButton880.fFillStyle = kFillSolid;
   valButton880.fFont = ufont->GetFontHandle();
   valButton880.fGraphicsExposures = kFALSE;
   uGC = gClient->GetGC(&valButton880, kTRUE);

   gClient->GetColorByName("#00ff00",ucolor);
   /* TGTextButton* */ fConfigAllSaveButton = new TGTextButton(fConfigAllFrame,"Save All",-1,uGC->GetGC());
fConfigAllSaveButton->SetFont(ufont->GetFontStruct());
   fConfigAllSaveButton->SetTextJustify(36);
   fConfigAllSaveButton->SetMargins(0,0,0,0);
   fConfigAllSaveButton->SetWrapLength(-1);
   fConfigAllSaveButton->Resize(105,24);

   fConfigAllSaveButton->ChangeBackground(ucolor);
   fConfigAllFrame->AddFrame(fConfigAllSaveButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fConfigAllSaveButton->MoveResize(135,63,105,24);

   gClient->GetColorByName("#ffcc00",ucolor);
   /* TGTextButton* */ fConfigAllLoadButton = new TGTextButton(fConfigAllFrame,"Load");
fConfigAllLoadButton->SetFont(ufont->GetFontStruct());
   fConfigAllLoadButton->SetTextJustify(36);
   fConfigAllLoadButton->SetMargins(0,0,0,0);
   fConfigAllLoadButton->SetWrapLength(-1);
   fConfigAllLoadButton->Resize(105,24);

   fConfigAllLoadButton->ChangeBackground(ucolor);
   fConfigAllFrame->AddFrame(fConfigAllLoadButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fConfigAllLoadButton->MoveResize(136,100,105,24);

   // "Load Method" group frame
   /* TGGroupFrame* */ fConfigLoadMethodFrame = new TGGroupFrame(fConfigAllFrame,"Load Method");   fConfigLoadMethodFrame->SetLayoutBroken(kTRUE);
   /* TGRadioButton* */ fConfigLoadMethodReset = new TGRadioButton(fConfigLoadMethodFrame,"Reset");   fConfigLoadMethodReset->SetTextJustify(36);
   fConfigLoadMethodReset->SetMargins(0,0,0,0);
   fConfigLoadMethodReset->SetWrapLength(-1);
   fConfigLoadMethodFrame->AddFrame(fConfigLoadMethodReset, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fConfigLoadMethodReset->MoveResize(8,22,64,19);
   /* TGRadioButton* */ fConfigLoadMethodOverwrite = new TGRadioButton(fConfigLoadMethodFrame,"Overwrite");   fConfigLoadMethodOverwrite->SetTextJustify(36);
   fConfigLoadMethodOverwrite->SetMargins(0,0,0,0);
   fConfigLoadMethodOverwrite->SetWrapLength(-1);
   fConfigLoadMethodFrame->AddFrame(fConfigLoadMethodOverwrite, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fConfigLoadMethodOverwrite->MoveResize(8,42,72,19);
   /* TGRadioButton* */ fConfigLoadMethodCumulate = new TGRadioButton(fConfigLoadMethodFrame,"Cumulate");   fConfigLoadMethodCumulate->SetTextJustify(36);
   fConfigLoadMethodCumulate->SetMargins(0,0,0,0);
   fConfigLoadMethodCumulate->SetWrapLength(-1);
   fConfigLoadMethodFrame->AddFrame(fConfigLoadMethodCumulate, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fConfigLoadMethodCumulate->MoveResize(8,62,72,19);

   fConfigLoadMethodFrame->SetLayoutManager(new TGVerticalLayout(fConfigLoadMethodFrame));
   fConfigLoadMethodFrame->Resize(96,96);
   fConfigAllFrame->AddFrame(fConfigLoadMethodFrame, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fConfigLoadMethodFrame->MoveResize(17,95,96,96);

   fConfigAllFrame->SetLayoutManager(new TGVerticalLayout(fConfigAllFrame));
   fConfigAllFrame->Resize(256,200);
   fCompositeFrame857->AddFrame(fConfigAllFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fConfigAllFrame->MoveResize(377,8,256,200);


   fHistTab->SetTab(0);

   fHistTab->Resize(fHistTab->GetDefaultSize());
   fMainFrame2824->AddFrame(fHistTab, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fHistTab->MoveResize(0,0,737,465);

   fMainFrame1598->AddFrame(fMainFrame2824, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame2824->MoveResize(0,0,744,473);

   fMainFrame909->AddFrame(fMainFrame1598, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame1598->MoveResize(0,0,743,471);

   fMainFrame1678->AddFrame(fMainFrame909, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame909->MoveResize(0,0,742,471);

   fMainFrame942->AddFrame(fMainFrame1678, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame1678->MoveResize(0,0,745,477);

   fMainFrame1001->AddFrame(fMainFrame942, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame942->MoveResize(0,0,744,475);

   fMainFrame4698->AddFrame(fMainFrame1001, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame1001->MoveResize(0,0,746,473);

   fMainFrame2078->AddFrame(fMainFrame4698, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame4698->MoveResize(0,0,787,504);

   fMainFrame1180->AddFrame(fMainFrame2078, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame2078->MoveResize(0,0,754,492);

   fMainFrame2438->AddFrame(fMainFrame1180, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame1180->MoveResize(0,0,746,479);

   fMainFrame2592->AddFrame(fMainFrame2438, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame2438->MoveResize(0,0,749,484);

   this->AddFrame(fMainFrame2592, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fMainFrame2592->MoveResize(0,0,754,487);

   this->SetMWMHints(kMWMDecorAll,
                        kMWMFuncAll,
                        kMWMInputModeless);
   this->MapSubwindows();

   this->Resize(this->GetDefaultSize());
   this->MapWindow();
   this->Resize(741,471);


   // Call MakeHistConnections function, defined in HistGui.hxx
   MakeHistConnections();
}

//TGMainFrame *fHistVarFrame = 0;TGCompositeFrame *fMainFrame2592 = 0;TGCompositeFrame *fMainFrame2438 = 0;TGCompositeFrame *fMainFrame1180 = 0;TGCompositeFrame *fMainFrame2078 = 0;TGCompositeFrame *fMainFrame4698 = 0;TGCompositeFrame *fMainFrame1001 = 0;TGCompositeFrame *fMainFrame942 = 0;TGCompositeFrame *fMainFrame1678 = 0;TGCompositeFrame *fMainFrame909 = 0;TGCompositeFrame *fMainFrame1598 = 0;TGCompositeFrame *fMainFrame2824 = 0;TGTab *fHistTab = 0;TGCanvas *fHistCanvas = 0;TGListTree *fHistTree = 0;TGTextButton *fCommandOk = 0;TGTextButton *fQuit = 0;TGTextEntry *fCommandEntry = 0;TGLabel *fCommandLabel = 0;TGTextButton *fDrawButton = 0;TGTextButton *fDeleteButton = 0;TGGroupFrame *fHistCreateFrame = 0;TGNumberEntryField *fHighX = 0;TGNumberEntryField *fLowX = 0;TGNumberEntryField *fBinsX = 0;TGNumberEntryField *fBinsY = 0;TGNumberEntryField *fLowY = 0;TGNumberEntryField *fHighY = 0;TGNumberEntryField *fBinsZ = 0;TGNumberEntryField *fLowZ = 0;TGNumberEntryField *fHighZ = 0;TGLabel *fLabelX = 0;TGLabel *fLabelY = 0;TGLabel *fLabelZ = 0;TGLabel *fLabelParameter = 0;TGLabel *fLabelBins = 0;TGLabel *fLabelLow = 0;TGLabel *fLabelHigh = 0;TGComboBox *fTypeEntry = 0;TGTextEntry *fGateEntry = 0;TGLabel *fGateLabel = 0;TGTextEntry *fNameEntry = 0;TGTextEntry *fTitleEntry = 0;TGLabel *fTypeLabel = 0;TGComboBox *fParamZ = 0;TGComboBox *fParamY = 0;TGComboBox *fParamX = 0;TGComboBox *fEventEntry = 0;TGLabel *fEventLabel = 0;TGLabel *fNameLabel = 0;TGLabel *fTitleLabel = 0;TGTextButton *fHistCreateButton = 0;TGTextButton *fHistRegateButton = 0;TGGroupFrame *fHistConfigFrame = 0;TGTextButton *fHistSaveButton = 0;TGTextButton *fHistLoadButton = 0;TGTextEntry *fDrawOptionEntry = 0;TGTextButton *fMkdirButton = 0;TGLabel *fDrawOptionLabel = 0;TGGroupFrame *fVariablesFrame = 0;TGCanvas *fVariablesCanvas = 0;TGListTree *fVariablesTree = 0;TGTextButton *fReadVarButton = 0;TGTextButton *fSetVarButton = 0;TGNumberEntryField *fVarEntry = 0;TGGroupFrame *fConfigAllFrame = 0;TGTextButton *fConfigHistSaveButton = 0;TGTextButton *fConfigVarSaveButton = 0;TGTextButton *fConfigCanvasSaveButton = 0;TGTextButton *fConfigAllSaveButton = 0;TGTextButton *fConfigAllLoadButton = 0;TGGroupFrame *fConfigLoadMethodFrame = 0;TGRadioButton *fConfigLoadMethodReset = 0;TGRadioButton *fConfigLoadMethodOverwrite = 0;TGRadioButton *fConfigLoadMethodCumulate = 0;
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
#define RB_HIST_SIGNALS rb::Rint::gApp()->GetHistSignals()

void TGHistVarFrame::MakeHistConnections() {
   // Define actions to take when buttons are pressed:
   //     fMainFramePointer->Connect(fTextButtonPointer, "Pressed()", 0, 0, "some_function()");
	this->MoveResize(380,20,this->GetWidth(),this->GetHeight());


// ==== Hist Creation ==== //
	fNameEntry->SetText("hist");
	fBinsX->SetNumber(100); fHighX->SetNumber(100);
	fBinsY->SetNumber(100);	fHighY->SetNumber(100);
	fBinsZ->SetNumber(100);	fHighZ->SetNumber(100);
	fEventEntry->Connect("Selected(Int_t)", "rb::HistSignals", RB_HIST_SIGNALS, "PopulateParameters(Int_t)");
	RB_HIST_SIGNALS->PopulateEvents();
	fTypeEntry->Connect("Selected(Int_t)", "rb::HistSignals", RB_HIST_SIGNALS, "EnableHistFields(Int_t)");
	fTypeEntry->Selected(0);

	fHistCreateButton->Connect("Pressed()", "rb::HistSignals", RB_HIST_SIGNALS, "CreateHistogram()");
	fHistTree->Connect("DoubleClicked(TGListTreeItem*, Int_t)", "rb::HistSignals", RB_HIST_SIGNALS, "HistTreeItemSelect(TGListTreeItem*, Int_t)");
	fHistTree->Connect("KeyPressed(TGListTreeItem*, UInt_t, UInt_t)", "rb::HistSignals", RB_HIST_SIGNALS, "HistTreeKeyPressed(TGListTreeItem*, UInt_t, UInt_t)");
	fHistTree->Connect("ReturnPressed(TGListTreeItem*)", "rb::HistSignals", RB_HIST_SIGNALS, "HistTreeItemSelect(TGListTreeItem*)");
	fHistTree->Connect("Clicked(TGListTreeItem*, Int_t)", "rb::HistSignals", RB_HIST_SIGNALS, "HistTreeItemClicked(TGListTreeItem*, Int_t)");
	fDrawButton->Connect("Pressed()", "rb::HistSignals", RB_HIST_SIGNALS, "DrawHist()");
	fDeleteButton->Connect("Pressed()", "rb::HistSignals", RB_HIST_SIGNALS, "DeleteHist()");
	fCommandOk->Connect("Pressed()", "rb::HistSignals", RB_HIST_SIGNALS, "HistMemberFn()");
	fCommandEntry->Connect("ReturnPressed()", "rb::HistSignals", RB_HIST_SIGNALS, "HistMemberFn()");
	fMkdirButton->Connect("Pressed()",  "rb::HistSignals", RB_HIST_SIGNALS, "Mkdir()");
	fHistRegateButton->Connect("Pressed()", "rb::HistSignals", RB_HIST_SIGNALS, "RegateHist()");
	fHistReplaceButton->Connect("Pressed()", "rb::HistSignals", RB_HIST_SIGNALS, "ToggleCreateReplace()");


	RB_HIST_SIGNALS->Connect("NewOrDeleteHist()", "rb::HistSignals", RB_HIST_SIGNALS, "SyncHistTree()");

	RB_HIST_SIGNALS->SyncHistTree();

	fQuit->Connect("Pressed()", "rb::HistSignals", RB_HIST_SIGNALS, "Quit()");
	fVarQuit->Connect("Pressed()", "rb::HistSignals", RB_HIST_SIGNALS, "Quit()");


// ===== Variables/Config ===== //
	fConfigLoadMethodReset->Connect("Clicked()", "rb::HistSignals", RB_HIST_SIGNALS, "ClickedLoadButton(=0)");
	fConfigLoadMethodOverwrite->Connect("Clicked()", "rb::HistSignals", RB_HIST_SIGNALS, "ClickedLoadButton(=1)");
	fConfigLoadMethodCumulate->Connect("Clicked()", "rb::HistSignals", RB_HIST_SIGNALS, "ClickedLoadButton(=2)");
	RB_HIST_SIGNALS->ClickedLoadButton(0);

	std::stringstream write_connections[4];
	for(int i=0; i< 4; ++i)
		 write_connections[i] << "WriteConfig(=" << i << ")";
	fConfigHistSaveButton->Connect("Pressed()", "rb::HistSignals", RB_HIST_SIGNALS, write_connections[rb::HistSignals::WRITE_HIST].str().c_str());
	fHistSaveButton->Connect("Pressed()", "rb::HistSignals", RB_HIST_SIGNALS, write_connections[rb::HistSignals::WRITE_HIST].str().c_str());
	fConfigVarSaveButton->Connect("Pressed()", "rb::HistSignals", RB_HIST_SIGNALS, write_connections[rb::HistSignals::WRITE_VAR].str().c_str());
	fConfigCanvasSaveButton->Connect("Pressed()", "rb::HistSignals", RB_HIST_SIGNALS, write_connections[rb::HistSignals::WRITE_CANVAS].str().c_str());
	fConfigAllSaveButton->Connect("Pressed()", "rb::HistSignals", RB_HIST_SIGNALS, write_connections[rb::HistSignals::WRITE_ALL].str().c_str());

  fConfigHistSaveButton->Connect("Pressed()", "TGTextButton",   fConfigHistSaveButton, "SetDown(=false)");
  fHistSaveButton->Connect("Pressed()", "TGTextButton",         fHistSaveButton, "SetDown(=false)");
  fConfigVarSaveButton->Connect("Pressed()", "TGTextButton",    fConfigVarSaveButton, "SetDown(=false)");
	fConfigCanvasSaveButton->Connect("Pressed()", "TGTextButton", fConfigCanvasSaveButton, "SetDown(=false)");
  fConfigAllSaveButton->Connect("Pressed()", "TGTextButton",    fConfigAllSaveButton, "SetDown(=false)");

	fConfigAllLoadButton->Connect("Pressed()", "rb::HistSignals", RB_HIST_SIGNALS, "ReadConfig(=false)");
	fHistLoadButton->Connect("Pressed()", "rb::HistSignals", RB_HIST_SIGNALS, "ReadConfig(=true)");
	fConfigAllLoadButton->Connect("Pressed()", "TGTextButton",    fConfigAllLoadButton, "SetDown(=false)");
	fHistLoadButton->Connect("Pressed()", "TGTextButton",    fHistLoadButton, "SetDown(=false)");

	fVariablesTree->Connect("DoubleClicked(TGListTreeItem*, Int_t)", "rb::HistSignals", RB_HIST_SIGNALS, "ReadVariable(TGListTreeItem*, Int_t)");
	fReadVarButton->Connect("Pressed()", "rb::HistSignals", RB_HIST_SIGNALS, "ReadVariable()");
	fSetVarButton->Connect("Pressed()", "rb::HistSignals", RB_HIST_SIGNALS, "SetVariable()");
	RB_HIST_SIGNALS->SyncVariables();

	fHistTab->Connect("ProcessedEvent(Event_t*)", "rb::HistSignals", RB_HIST_SIGNALS, "HandleEvent(Event_t*)");

}


TGHistVarFrame::~TGHistVarFrame() {
	rb::Rint::gApp()->DeleteHistSignals();
	DeleteMembers();
}

void TGHistVarFrame::DeleteMembers() {
	delete fMainFrame2592;
	delete fMainFrame2438;
	delete fMainFrame1180;
	delete fMainFrame2078;
	delete fMainFrame4698;
	delete fMainFrame1001;
	delete fMainFrame942;
	delete fMainFrame1678;
	delete fMainFrame909;
	delete fMainFrame1598;
	delete fMainFrame2824;
	delete fHistTab;
	delete fHistCanvas;
	delete fHistTree;
	delete fCommandOk;
	delete fQuit;
	delete fVarQuit;
	delete fCommandEntry;
	delete fCommandLabel;
	delete fDrawButton;
	delete fDeleteButton;
	delete fHistCreateFrame;
	delete fHighX;
	delete fLowX;
	delete fBinsX;
	delete fBinsY;
	delete fLowY;
	delete fHighY;
	delete fBinsZ;
	delete fLowZ;
	delete fHighZ;
	delete fLabelX;
	delete fLabelY;
	delete fLabelZ;
	delete fLabelParameter;
	delete fLabelBins;
	delete fLabelLow;
	delete fLabelHigh;
	delete fTypeEntry;
	delete fGateEntry;
	delete fGateLabel;
	delete fNameEntry;
	delete fTitleEntry;
	delete fTypeLabel;
	delete fParamZ;
	delete fParamY;
	delete fParamX;
	delete fEventEntry;
	delete fEventLabel;
	delete fNameLabel;
	delete fTitleLabel;
	delete fHistCreateButton;
	delete fHistRegateButton;
	delete fHistConfigFrame;
	delete fHistSaveButton;
	delete fHistLoadButton;
	delete fDrawOptionEntry;
	delete fMkdirButton;
	delete fDrawOptionLabel;
	delete fVariablesFrame;
	delete fVariablesCanvas;
	delete fVariablesTree;
	delete fReadVarButton;
	delete fSetVarButton;
	delete fVarEntry;
	delete fConfigAllFrame;
	delete fConfigHistSaveButton;
	delete fConfigVarSaveButton;
	delete fConfigCanvasSaveButton;
	delete fConfigAllSaveButton;
	delete fConfigAllLoadButton;
	delete fConfigLoadMethodFrame;
	delete fConfigLoadMethodReset;
	delete fConfigLoadMethodOverwrite;
	delete fConfigLoadMethodCumulate;
}

namespace { void next_incr(Int_t& n) { if(n<14) ++n; else n = 0; } }
Bool_t TGHistVarFrame::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
	switch (GET_MSG(msg)) {
	case kC_TEXTENTRY:
		switch (GET_SUBMSG(msg)) {
		case kTE_TAB:
			{
				TGFrame* widgets[15] = {
					fNameEntry, fTitleEntry,
					fParamX, fBinsX, fLowX, fHighX,
					fParamY, fBinsY, fLowY, fHighY,
					fParamZ, fBinsZ, fLowZ, fHighZ,
					fGateEntry
				};
				Int_t next = parm1 < 15 ? parm1 : 0;

				TGTextEntry* enext = 0;
				enext1:
				if(widgets[next]->InheritsFrom(TGTextEntry::Class())) {
					if(static_cast<TGTextEntry*>(widgets[next])->IsEnabled() == kTRUE) {
						enext = static_cast<TGTextEntry*>(widgets[next]);
					}
				}
				else if(widgets[next]->InheritsFrom(TGComboBox::Class())) {
					if(static_cast<TGComboBox*>(widgets[next])->IsEnabled() == kTRUE) {
						enext = static_cast<TGComboBox*>(widgets[next])->GetTextEntry();
					}
				}
				else {
					std::cout << "Wrong widget type! " << ERR_FILE_LINE << "\n";
					return false;
				}

				if(enext == 0) {
					next_incr(next);
					goto enext1;
				}
				enext->SetFocus();
				break;
			}
		default:
			break;
		}

	default:
		break;
	}

	return kTRUE;
}


#undef RB_HIST_SIGNALS
#undef RB_BUTTON_CONNECT
