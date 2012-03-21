#define RB_SIGNALS static_cast<rb::Rint*>(gApplication)->GetSignals()

void MakeConnections() {
   // Define actions to take when buttons are pressed:
   //     fMainFramePointer->Connect(fTextButtonPointer, "Pressed()", 0, 0, "some_function()");


	fUnattach->SetEnabled(false);

	fRbeerFrame->Connect(fAttachOnline, "Pressed()", "rb::Signals", RB_SIGNALS, "AttachOnline()");
	fRbeerFrame->Connect(fAttachFile, "Pressed()", "rb::Signals", RB_SIGNALS, "AttachFile()");
	fRbeerFrame->Connect(fAttachList, "Pressed()", "rb::Signals", RB_SIGNALS, "AttachList()");
	fRbeerFrame->Connect(fUnattach, "Pressed()", "rb::Signals", RB_SIGNALS, "Unattach()");

	RB_SIGNALS->Connect("Unattaching()", "TGTextButton", fUnattach, "ChangeBackground(=0xe0e0e0)");	
	RB_SIGNALS->Connect("Unattaching()", "TGTextButton", fUnattach, "SetEnabled(=false)");
	RB_SIGNALS->Connect("Unattaching()", "TGTextButton", fIsContinuous, "SetEnabled(=true)");
	RB_SIGNALS->Connect("Attaching()", "TGTextButton", fUnattach, "ChangeBackground(=0x00ff00)");
	RB_SIGNALS->Connect("Attaching()", "TGTextButton", fUnattach, "SetEnabled(=true)");
	RB_SIGNALS->Connect("Attaching()", "TGTextButton", fIsContinuous, "SetEnabled(=false)");
}

#undef RB_SIGNALS
