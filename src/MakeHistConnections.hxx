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
#define RB_SIGNALS static_cast<rb::Rint*>(gApplication)->GetSignals()

void MakeHistConnections() {
   // Define actions to take when buttons are pressed:
   //     fMainFramePointer->Connect(fTextButtonPointer, "Pressed()", 0, 0, "some_function()");
	fHistVarFrame->MoveResize(380,20,fHistVarFrame->GetWidth(),fHistVarFrame->GetHeight());


// ==== Hist Creation ==== //
	fNameEntry->SetText("hist");
	fBinsX->SetNumber(100); fHighX->SetNumber(100);
	fBinsY->SetNumber(100);	fHighY->SetNumber(100);
	fBinsZ->SetNumber(100);	fHighZ->SetNumber(100);
	fEventEntry->Connect("Selected(Int_t)", "rb::Signals", RB_SIGNALS, "PopulateParameters(Int_t)");
	RB_SIGNALS->PopulateEvents();
	fTypeEntry->Connect("Selected(Int_t)", "rb::Signals", RB_SIGNALS, "EnableHistFields(Int_t)");
	fTypeEntry->Selected(0);

	fHistCreateButton->Connect("Pressed()", "rb::Signals", RB_SIGNALS, "CreateHistogram()");
	fHistTree->Connect("DoubleClicked(TGListTreeItem*, Int_t)", "rb::Signals", RB_SIGNALS, "HistTreeItemSelect(TGListTreeItem*, Int_t)");
	fHistTree->Connect("ReturnPressed(TGListTreeItem*)", "rb::Signals", RB_SIGNALS, "HistTreeItemSelect(TGListTreeItem*)");
	fHistTree->Connect("Clicked(TGListTreeItem*, Int_t)", "rb::Signals", RB_SIGNALS, "HistTreeItemClicked(TGListTreeItem*, Int_t)");
	fDrawButton->Connect("Pressed()", "rb::Signals", RB_SIGNALS, "DrawHist()");
	fDeleteButton->Connect("Pressed()", "rb::Signals", RB_SIGNALS, "DeleteHist()");
	fCommandOk->Connect("Pressed()", "rb::Signals", RB_SIGNALS, "HistMemberFn()");
	fCommandEntry->Connect("ReturnPressed()", "rb::Signals", RB_SIGNALS, "HistMemberFn()");
	fMkdirButton->Connect("Pressed()",  "rb::Signals", RB_SIGNALS, "Mkdir()");

	RB_SIGNALS->Connect("NewOrDeleteHist()", "rb::Signals", RB_SIGNALS, "SyncHistTree()");

	RB_SIGNALS->SyncHistTree();

	fQuit->Connect("Pressed()", "rb::Signals", RB_SIGNALS, "Quit()");


// ===== Variables/Config ===== //
	fConfigLoadMethodReset->Connect("Clicked()", "rb::Signals", RB_SIGNALS, "ClickedLoadButton(=0)");
	fConfigLoadMethodOverwrite->Connect("Clicked()", "rb::Signals", RB_SIGNALS, "ClickedLoadButton(=1)");
	fConfigLoadMethodCumulate->Connect("Clicked()", "rb::Signals", RB_SIGNALS, "ClickedLoadButton(=2)");
	RB_SIGNALS->ClickedLoadButton(0);

	std::stringstream write_connections[4];
	for(int i=0; i< 4; ++i)
		 write_connections[i] << "WriteConfig(=" << i << ")";
	fConfigHistSaveButton->Connect("Pressed()", "rb::Signals", RB_SIGNALS, write_connections[rb::Signals::WRITE_HIST].str().c_str());
	fHistSaveButton->Connect("Pressed()", "rb::Signals", RB_SIGNALS, write_connections[rb::Signals::WRITE_HIST].str().c_str());
	fConfigVarSaveButton->Connect("Pressed()", "rb::Signals", RB_SIGNALS, write_connections[rb::Signals::WRITE_VAR].str().c_str());
	fConfigCanvasSaveButton->Connect("Pressed()", "rb::Signals", RB_SIGNALS, write_connections[rb::Signals::WRITE_CANVAS].str().c_str());
	fConfigAllSaveButton->Connect("Pressed()", "rb::Signals", RB_SIGNALS, write_connections[rb::Signals::WRITE_ALL].str().c_str());

  fConfigHistSaveButton->Connect("Pressed()", "TGTextButton",   fConfigHistSaveButton, "SetDown(=false)");
  fHistSaveButton->Connect("Pressed()", "TGTextButton",         fHistSaveButton, "SetDown(=false)");
  fConfigVarSaveButton->Connect("Pressed()", "TGTextButton",    fConfigVarSaveButton, "SetDown(=false)");
	fConfigCanvasSaveButton->Connect("Pressed()", "TGTextButton", fConfigCanvasSaveButton, "SetDown(=false)");
  fConfigAllSaveButton->Connect("Pressed()", "TGTextButton",    fConfigAllSaveButton, "SetDown(=false)");

	fConfigAllLoadButton->Connect("Pressed()", "rb::Signals", RB_SIGNALS, "ReadConfig(=false)");
	fHistLoadButton->Connect("Pressed()", "rb::Signals", RB_SIGNALS, "ReadConfig(=true)");
	fConfigAllLoadButton->Connect("Pressed()", "TGTextButton",    fConfigAllLoadButton, "SetDown(=false)");
	fHistLoadButton->Connect("Pressed()", "TGTextButton",    fHistLoadButton, "SetDown(=false)");

	fVariablesTree->Connect("DoubleClicked(TGListTreeItem*, Int_t)", "rb::Signals", RB_SIGNALS, "ReadVariable(TGListTreeItem*, Int_t)");
	fReadVarButton->Connect("Pressed()", "rb::Signals", RB_SIGNALS, "ReadVariable()");
	fSetVarButton->Connect("Pressed()", "rb::Signals", RB_SIGNALS, "SetVariable()");
	RB_SIGNALS->SyncVariables();



}

#undef RB_SIGNALS
#undef RB_BUTTON_CONNECT
