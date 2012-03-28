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
#define RB_BUTTON_CONNECT(button, function) fRbeerFrame->Connect(button, "Pressed()", "rb::Signals", RB_SIGNALS, function);

void MakeConnections() {
   // Define actions to take when buttons are pressed:
   //     fMainFramePointer->Connect(fTextButtonPointer, "Pressed()", 0, 0, "some_function()");

	fRbeerFrame->MoveResize(20,20,fRbeerFrame->GetWidth(), fRbeerFrame->GetHeight());
	fUnattach->SetEnabled(false);
	fUpdateRate->SetLimits(TGNumberFormat::kNELLimitMinMax, 1, 600);
	fUpdateRate->SetNumber(5);

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
	RB_BUTTON_CONNECT(fSelectCanvas, "CdCanvas()");
	RB_BUTTON_CONNECT(fClearCurrent, "ClearCurrent()");

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

	fConfigSave->Connect("Pressed()", "rb::Signals", RB_SIGNALS, "WriteConfig(=1)");
	fConfigLoad->Connect("Pressed()", "rb::Signals", RB_SIGNALS, "ReadCanvasConfig()");
	fConfigSave->Connect("Pressed()", "TGTextButton", fConfigSave, "SetDown(=false)");
	fConfigLoad->Connect("Pressed()", "TGTextButton", fConfigLoad, "SetDown(=false)");

	fSaveData->Connect("Clicked()", "rb::Signals", RB_SIGNALS, "SaveData()");
	fSaveHist->Connect("Clicked()", "rb::Signals", RB_SIGNALS, "SaveHists()");
	RB_SIGNALS->Connect("Attaching()", "TGTextButton", fSaveData, "SetEnabled(=false)");
	RB_SIGNALS->Connect("Attaching()", "TGTextButton", fSaveHist, "SetEnabled(=false)");
	RB_SIGNALS->Connect("Unattaching()", "TGTextButton", fSaveData, "SetEnabled(=true)");
	RB_SIGNALS->Connect("Unattaching()", "rb::Signals", RB_SIGNALS, "EnableSaveHists()");
	fSaveData->Clicked();

}

#undef RB_SIGNALS
#undef RB_BUTTON_CONNECT
