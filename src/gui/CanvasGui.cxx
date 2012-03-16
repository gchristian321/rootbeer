#include <sstream>
#include <TCanvas.h>
#include "CanvasGui.hxx"
#include "Rootbeer.hxx"
#include "boost/bind.hpp"

#define ADD_WIDGET(CLASS, ...)					\
	fWidgets.Add((CLASS*)0, this, this, __VA_ARGS__)

namespace {
const Pixel_t kStartColor = pix::Blue;
const Pixel_t kStopColor  = pix::Red;
}

rb::gui::Canvas::Canvas(Int_t w, Int_t h):
	Frame(w, h) , fUpdateRate(5), kIsUpdating(false) {

	ADD_WIDGET(TextEntry<Canvas>,  &Canvas::Null, "CanvasName", "c1");

	ADD_WIDGET(TextButton<Canvas>, &Canvas::CreateCanvas, "CreateCanvas", "New Canvas");
	ADD_WIDGET(TextButton<Canvas>, &Canvas::DivideCanvas, "DivideCanvas", "Divide Current");
	ADD_WIDGET(TextButton<Canvas>, &Canvas::ClearCurrent, "ClearCurrent", "Clear Current");
	ADD_WIDGET(TextButton<Canvas>, &Canvas::ClearAll, "ClearAll", "Clear All");
	ADD_WIDGET(TextButton<Canvas>, &Canvas::UpdateCurrent, "UpdateCurrent", "Update Current");
	ADD_WIDGET(TextButton<Canvas>, &Canvas::UpdateAll, "UpdateAll", "Update All");

	ADD_WIDGET(Label<Canvas>, "UpdateRateLabel", "Update Rate");

	ADD_WIDGET(NumberEntry<Canvas>, &Canvas::SetUpdateRate, "SetUpdateRate", 5, 1, 600, 5 /*digitwidth*/ );

	ADD_WIDGET(TextButton<Canvas>,  &Canvas::StartStop, "StartStop", "Start Update", kStartColor, pix::White);

	InitFrames();
	Sync();
}

void rb::gui::Canvas::Sync() {
	Int_t rate = rb::canvas::GetUpdateRate();
	TextButton<Canvas>* button = Find<TextButton<Canvas> >("StartStop");
	assert(button);
	if(rate) {
		button->SetText("Stop Update");
		button->SetBackgroundColor(kStopColor);
		kIsUpdating = true;
		fUpdateRate = rate;
		Find<NumberEntry<Canvas> >("SetUpdateRate")->SetNumber(rate);
	}
	else {
		button->SetText("Start Update");
		button->SetBackgroundColor(kStartColor);
		kIsUpdating = false;
	}
}
		

void rb::gui::Canvas::StartStop() {
	TextButton<Canvas>* button = Find<TextButton<Canvas> >("StartStop");
	assert(button);
	if(!kIsUpdating) {
		rb::canvas::StartUpdate(fUpdateRate);
		button->SetText("Stop Update");
		button->SetBackgroundColor(kStopColor);
		kIsUpdating = true;
	}
	else {
		rb::canvas::StopUpdate();
		button->SetText("Start Update");
		button->SetBackgroundColor(kStartColor);
		kIsUpdating = false;
	}		
}

void rb::gui::Canvas::SetUpdateRate() {
	NumberEntry<Canvas>* box = Find<NumberEntry<Canvas> >("SetUpdateRate");
	assert(box);
	fUpdateRate = box->GetIntNumber();
	if(kIsUpdating) {
		rb::canvas::StartUpdate(fUpdateRate);
	}
}

void rb::gui::Canvas::UpdateAll() {
	rb::canvas::UpdateAll();
}

void rb::gui::Canvas::UpdateCurrent() {
	rb::canvas::UpdateCurrent();
}

void rb::gui::Canvas::ClearAll() {
	rb::canvas::ClearAll();
}

void rb::gui::Canvas::ClearCurrent() {
	rb::canvas::ClearCurrent();
}


void rb::gui::Canvas::CreateCanvas() {
	std::string name = Find<TextEntry<Canvas> >("CanvasName")->GetText();
	TCanvas* canvas (0);
	if(name == "") canvas = new TCanvas();
	else {
		std::stringstream ssname(name);
		int nn = 1;
		while(gROOT->GetListOfCanvases()->FindObject(ssname.str().c_str())) {
			ssname.str(""); ssname << name << "_" << nn++;
		}
		canvas = new TCanvas(ssname.str().c_str(), ssname.str().c_str());
	}
}

namespace {
class DividePopUp: public rb::gui::Frame
{
public:
	 DividePopUp();
	 ~DividePopUp() { }
	 void DoAction();
	 void Erase();
};
void DividePopUp::Erase() { delete this; }
DividePopUp::DividePopUp(): rb::gui::Frame(50, 150) {
	ADD_WIDGET(rb::gui::NumberEntryField<DividePopUp>, &DividePopUp::Null, "Width", 2, 1, 100);
	ADD_WIDGET(rb::gui::NumberEntryField<DividePopUp>, &DividePopUp::Null, "Height", 2, 1, 100);
	ADD_WIDGET(rb::gui::TextButton<DividePopUp>, &DividePopUp::Erase, "Cancel", "Cancel");
	ADD_WIDGET(rb::gui::TextButton<DividePopUp>, &DividePopUp::DoAction, "OK", "OK");

	Find<rb::gui::NumberEntryField<DividePopUp> >("Width")->SetDefaultSize(30,20);
	Find<rb::gui::NumberEntryField<DividePopUp> >("Height")->SetDefaultSize(30,20);

	InitFrames();
} }

void DividePopUp::DoAction() {
	if(gPad) {
		gPad->Clear();
		Int_t w = Find<rb::gui::NumberEntryField<DividePopUp> >("Width")->GetIntNumber(),
			 h = Find<rb::gui::NumberEntryField<DividePopUp> >("Height")->GetIntNumber();
		gPad->Divide(w, h);
	}
	Erase();
}

void rb::gui::Canvas::DivideCanvas() {
	DividePopUp * pop_up = new DividePopUp();
	if(pop_up);
}
