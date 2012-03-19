#include <sstream>
#include <TCanvas.h>
#include "CanvasGui.hxx"
#include "Rootbeer.hxx"
#include "boost/bind.hpp"

#define ADD_WIDGET(CLASS, ...)					\
	fWidgets.Add((CLASS*)0, this, this, __VA_ARGS__)

#define ADD_WIDGET_FRAME(CLASS, FRAME, LAYOUT_TYPE, ...)								\
	fWidgets.Add((CLASS*)0, FRAME, new TGLayoutHints(LAYOUT_TYPE, 2, 2, 2, 2), this, __VA_ARGS__)

#define ADD_FRAME(NAME, CLASS, W, H, TYPE)													\
	fWidgets.AddFrame(NAME, new CLASS (this, W, H, TYPE, pix::Pink));

namespace {
const Pixel_t kStartColor = pix::Blue;
const Pixel_t kStopColor  = pix::Red;

}
/*
enum ELayoutHints {
   kLHintsNoHints = 0,
   kLHintsLeft    = BIT(0),
   kLHintsCenterX = BIT(1),
   kLHintsRight   = BIT(2),
   kLHintsTop     = BIT(3),
   kLHintsCenterY = BIT(4),
   kLHintsBottom  = BIT(5),
   kLHintsExpandX = BIT(6),
   kLHintsExpandY = BIT(7),
   kLHintsNormal  = (kLHintsLeft | kLHintsTop)
};
*/
rb::gui::Canvas::Canvas():
	MainFrame(800, 400) , fUpdateRate(5), kIsUpdating(false) {

	fLayout.reset(new TGLayoutHints(kLHintsCenterX | kLHintsCenterY | kLHintsExpandX));

	const Int_t WWW = 400;
	const Int_t HHH = 50;

	TGCompositeFrame* newCanvas = AddFrame<Composite<TGHorizontalFrame> > (WWW, HHH, kFixedWidth);
 	ADD_WIDGET_FRAME(TextEntry<Canvas>,  newCanvas, kLHintsNormal, &Canvas::Null, "CanvasName", "c1");
	ADD_WIDGET_FRAME(TextButton<Canvas>, newCanvas, kLHintsNormal, &Canvas::CreateCanvas, "CreateCanvas", "New Canvas");
	ADD_WIDGET_FRAME(Label<Canvas>,      newCanvas, kLHintsLeft | kLHintsBottom, "CanvasNameLabel", "   Name:");


	TGCompositeFrame* allCanvas = AddFrame<Composite<TGHorizontalFrame> > (WWW, HHH, kFixedWidth);
	ADD_WIDGET_FRAME(TextButton<Canvas>, allCanvas, kLHintsExpandX, &Canvas::ClearCurrent, "ClearCurrent", "Clear Current");
	ADD_WIDGET_FRAME(TextButton<Canvas>, allCanvas, kLHintsExpandX, &Canvas::ClearAll, "ClearAll", "Clear All");

	TGCompositeFrame* currentCanvas = AddFrame<Composite<TGHorizontalFrame> > (WWW, HHH, kFixedWidth);
	ADD_WIDGET_FRAME(TextButton<Canvas>, currentCanvas, kLHintsExpandX, &Canvas::UpdateCurrent, "UpdateCurrent", "Update Current");
	ADD_WIDGET_FRAME(TextButton<Canvas>, currentCanvas, kLHintsExpandX, &Canvas::UpdateAll, "UpdateAll", "Update All");

	TGCompositeFrame* changeCanvas = AddFrame<Composite<TGHorizontalFrame> > (WWW, HHH, kFixedWidth);
	ADD_WIDGET_FRAME(TextButton<Canvas>, changeCanvas, kLHintsExpandX, &Canvas::DivideCanvas, "DivideCanvas", "Divide Current");
	ADD_WIDGET_FRAME(TextButton<Canvas>, changeCanvas, kLHintsExpandX, &Canvas::ToDo, "ChangeCanvas", "Change Canvas");

	TGCompositeFrame* updateRate = AddFrame<Composite<TGHorizontalFrame> > (WWW, HHH, kFixedWidth);
	ADD_WIDGET_FRAME(TextButton<Canvas>, updateRate, kLHintsNormal, &Canvas::StartStop, "StartStop", "Start Update", kStartColor, pix::White);
	ADD_WIDGET_FRAME(Label<Canvas>, updateRate, kLHintsLeft | kLHintsBottom,"UpdateRateLabel", "    Rate (s): ");
	ADD_WIDGET_FRAME(NumberEntry<Canvas>, updateRate, kLHintsNormal, &Canvas::SetUpdateRate, "SetUpdateRate", 5, 1, 600, 5);

	InitFrames(80,80,600,600,1,1);
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

namespace rb { namespace gui { namespace {
class DividePopup: public rb::gui::Popup
{
public:
	 DividePopup();
	 ~DividePopup() { }
	 void DoAction();
};
DividePopup::DividePopup(): Popup(100, 150) {

	TGCompositeFrame* size = AddFrame<Composite<TGHorizontalFrame> > (100, 50, kFixedWidth);
	ADD_WIDGET_FRAME(NumberEntryField<DividePopup>, size, kLHintsCenterX,  &DividePopup::Null, "Horiz", 2, 1, 100);
	ADD_WIDGET_FRAME(Label<DividePopup>, size, kLHintsCenterX, "hLabel", " x ");
	ADD_WIDGET_FRAME(NumberEntryField<DividePopup>, size, kLHintsCenterX,  &DividePopup::Null, "Vert", 2, 1, 100);

	TGCompositeFrame* okc = AddFrame<Composite<TGHorizontalFrame> > (100, 50, kFixedWidth);
	ADD_WIDGET_FRAME(TextButton<DividePopup>, okc, kLHintsCenterX, &DividePopup::Okay, "OK", "OK");           
	ADD_WIDGET_FRAME(TextButton<DividePopup>, okc, kLHintsCenterX, &DividePopup::Cancel, "Cancel", "Cancel");

	Find<NumberEntryField<DividePopup> >("Horiz")->SetDefaultSize(30,20);
	Find<NumberEntryField<DividePopup> >("Vert")->SetDefaultSize(30,20);
	InitFrames(80,80,320,320,1,1);
}
void DividePopup::DoAction() {
	if(gPad) {
		gPad->Clear();
		Int_t w = Find<NumberEntryField<DividePopup> >("Horiz")->GetIntNumber(),
			 h = Find<NumberEntryField<DividePopup> >("Vert")->GetIntNumber();
		gPad->Divide(w, h);
	}
}

class SelectCanvas: public rb::gui::Popup
{
public:
	 SelectCanvas();
	 ~SelectCanvas() { }
	 void DoAction();
};
SelectCanvas::SelectCanvas(): Popup(100,150) {
	
}
void SelectCanvas::DoAction() {
}

} } }


void rb::gui::Canvas::DivideCanvas() {
	DividePopup * pop_up = new DividePopup();
	if(pop_up);
}
