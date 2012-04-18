#ifndef FRAME_HXX
#define FRAME_HXX
#include "../src/utils/boost_scoped_ptr.h"
#include "Widget.hxx"


namespace rb { namespace gui {

class Frame: public TGMainFrame
{
protected:
	 WidgetFactory fWidgets;
	 boost::scoped_ptr<TGLayoutHints> fLayout;
public:
	 Frame(UInt_t width, UInt_t height, Pixel_t color = pix::White);
	 virtual ~Frame() { }
	 virtual Bool_t ProcessMessage(Long_t msg, Long_t param1, Long_t param2);
protected:
	 void InitFrames();
};

} }

inline rb::gui::Frame::Frame(UInt_t w, UInt_t h, Pixel_t color):
	TGMainFrame(gClient->GetRoot(), w, h),
	fLayout(new TGLayoutHints(kLHintsCenterX | kLHintsCenterY)) {
	SetBackgroundColor(color);
}

inline void rb::gui::Frame::InitFrames() {
	fWidgets.AddFrames(this, fLayout);	
	MapSubwindows();
	Layout();
	MapWindow();
}

inline Bool_t rb::gui::Frame::ProcessMessage(Long_t msg, Long_t param1, Long_t param2) {
	fWidgets(param1);
}


namespace rb { namespace gui {

class Canvas;
class StartUpdateButton: public AWidget, public TGTextButton
{
private:
	 Canvas* const fCanvas;
public:
	 StartUpdateButton(TGMainFrame* mainframe, Int_t id, Canvas* canvas, Pixel_t color = pix::White):
		 TGTextButton(mainframe, "Start Update", id), fCanvas(canvas) {
		 SetBackgroundColor(color);
	 }
	 void operator() ();
};

class StopUpdateButton: public AWidget, public TGTextButton
{
private:
	 Canvas* const fCanvas;
public:
	 StopUpdateButton(TGMainFrame* mainframe, Int_t id, Canvas* canvas, Pixel_t color = pix::White):
		 TGTextButton(mainframe, "Stop Update", id), fCanvas(canvas) {
		 SetBackgroundColor(color);
	 }
	 void operator() ();
};

class SetUpdateRate: public AWidget, public TGNumberEntry
{
private:
	 Canvas* const fCanvas;
public:
	 SetUpdateRate(TGMainFrame* mainframe, Int_t id, Canvas* canvas):
		 TGNumberEntry(mainframe, 5, 5, id, TGNumberFormat::kNESReal, TGNumberFormat::kNEAPositive), fCanvas(canvas) {
		 SetLimits(TGNumberFormat::kNELLimitMinMax, 1, 600);
	 }
	 void operator() ();
};


class Canvas: public Frame
{
private:
	 Bool_t kIsUpdating;
	 Int_t fUpdateRate;
public:
	 Canvas(Int_t w = 100, Int_t h = 100);
	 virtual ~Canvas() { }

	 friend class SetUpdateRate;
	 friend class StartUpdateButton;
	 friend class StopUpdateButton;
};

} }

inline rb::gui::Canvas::Canvas(Int_t w, Int_t h):
	Frame(w, h) , fUpdateRate(5) {
	fWidgets.Add((StartUpdateButton*)0, this, this, pix::Blue);
	fWidgets.Add((StopUpdateButton*)0, this, this, pix::Red);
	fWidgets.Add((SetUpdateRate*)0, this, this);
	InitFrames();
}
	
inline void rb::gui::StartUpdateButton::operator() () {
	std::stringstream sstr;
	sstr << "rb::canvas::StartUpdate(" << fCanvas->fUpdateRate << ");";
	gROOT->ProcessLine(sstr.str().c_str());
	fCanvas->kIsUpdating = true;
}

inline void rb::gui::StopUpdateButton::operator() () {
	gROOT->ProcessLine("rb::canvas::StopUpdate();");
	fCanvas->kIsUpdating = false;
}

inline void rb::gui::SetUpdateRate::operator() () {
	fCanvas->fUpdateRate = GetIntNumber();
	if(fCanvas->kIsUpdating) {
		StartUpdateButton(0, -1, fCanvas)();
	}
}




#endif
