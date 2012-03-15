#include "CanvasGui.hxx"
#include "Rootbeer.hxx"


rb::gui::Canvas::Canvas(Int_t w, Int_t h):
	Frame(w, h) , fUpdateRate(5) {
	AddWidget<ClearCurrent>();
	AddWidget<ClearAll>();
	AddWidget<UpdateCurrent>();
	AddWidget<UpdateAll>();
	AddWidget<UpdateRateLabel>();
	AddWidget<SetUpdateRate>();
	AddWidget<StartStopUpdate>();
	InitFrames();
}
	
void rb::gui::StartStopUpdate::operator() () {
	if(!fCanvas->kIsUpdating) {
		rb::canvas::StartUpdate(fCanvas->fUpdateRate);
		SetText("Stop Update");
		SetBackgroundColor(kStopColor);
		fCanvas->kIsUpdating = true;
	}
	else {
		rb::canvas::StopUpdate();
		SetText("Start Update");
		SetBackgroundColor(kStartColor);
		fCanvas->kIsUpdating = false;
	}		
}

void rb::gui::SetUpdateRate::operator() () {
	fOwner->fUpdateRate = GetIntNumber();
	if(fOwner->kIsUpdating) {
		StartStopUpdate(0, -1, fOwner)();
	}
}

void rb::gui::UpdateAll::operator() () {
	rb::canvas::UpdateAll();
}

void rb::gui::UpdateCurrent::operator() () {
	rb::canvas::UpdateCurrent();
}

void rb::gui::ClearAll::operator() () {
	rb::canvas::ClearAll();
}

void rb::gui::ClearCurrent::operator() () {
	rb::canvas::ClearCurrent();
}
