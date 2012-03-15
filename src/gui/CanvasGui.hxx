#ifndef CANVAS_GUI_HXX
#define CANVAS_GUI_HXX
#include <TGLabel.h>
#include "Frame.hxx"


namespace rb { namespace gui {

class Canvas;

class UpdateAll: public rb::gui::AWidget, public TGTextButton
{
private:
	 rb::gui::Canvas* const fCanvas;
public:
	 UpdateAll(TGMainFrame* mainframe, Int_t id, rb::gui::Canvas* canvas):
		 TGTextButton(mainframe, "Update All", id), fCanvas(canvas) {
		 SetBackgroundColor(pix::White);
		 SetFont(kDefaultFont);
	 }
	 void operator() ();
};

class UpdateCurrent: public rb::gui::AWidget, public TGTextButton
{
private:
	 rb::gui::Canvas* const fCanvas;
public:
	 UpdateCurrent(TGMainFrame* mainframe, Int_t id, rb::gui::Canvas* canvas):
		 TGTextButton(mainframe, "Update Current", id), fCanvas(canvas) {
		 SetBackgroundColor(pix::White);
		 SetFont(kDefaultFont);
	 }
	 void operator() ();
};

class ClearAll: public rb::gui::AWidget, public TGTextButton
{
private:
	 rb::gui::Canvas* const fCanvas;
public:
	 ClearAll(TGMainFrame* mainframe, Int_t id, rb::gui::Canvas* canvas):
		 TGTextButton(mainframe, "Clear All", id), fCanvas(canvas) {
		 SetBackgroundColor(pix::White);
		 SetFont(kDefaultFont);
	 }
	 void operator() ();
};

class ClearCurrent: public rb::gui::AWidget, public TGTextButton
{
private:
	 rb::gui::Canvas* const fCanvas;
public:
	 ClearCurrent(TGMainFrame* mainframe, Int_t id, rb::gui::Canvas* canvas):
		 TGTextButton(mainframe, "Clear Current", id), fCanvas(canvas) {
		 SetBackgroundColor(pix::White);
		 SetFont(kDefaultFont);
	 }
	 void operator() ();
};

class StartStopUpdate: public rb::gui::AWidget, public TGTextButton
{
private:
	 static const Pixel_t kStartColor = pix::Blue;
	 static const Pixel_t kStopColor  = pix::Red;
	 rb::gui::Canvas* const fCanvas;
public:
	 StartStopUpdate(TGMainFrame* mainframe, Int_t id, rb::gui::Canvas* canvas):
		 TGTextButton(mainframe, "Start Update", id), fCanvas(canvas) {
		 SetBackgroundColor(kStartColor);
		 SetTextColor(pix::White);
		 SetFont(kDefaultFont);
	 }
	 void operator() ();
};

class UpdateRateLabel: public rb::gui::AWidget, public TGLabel
{
private:
	 rb::gui::Canvas* const fCanvas;
public:
	 UpdateRateLabel(TGMainFrame* mainframe, Int_t id, rb::gui::Canvas* canvas):
		 TGLabel(mainframe, "UpdateRate"), fCanvas(canvas) {
		 SetTextFont(kDefaultFont);
		 SetBackgroundColor(pix::White);
	 }
	 void operator() () {}
};

class SetUpdateRate: public rb::gui::TWidget<rb::gui::Canvas>, public TGNumberEntry
{
public:
	 SetUpdateRate(TGMainFrame* mainframe, Int_t id, rb::gui::Canvas* canvas):
		 TWidget<Canvas>(canvas), TGNumberEntry(mainframe, 5, 5, id, TGNumberFormat::kNESReal, TGNumberFormat::kNEAPositive) {
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
	 Canvas(Int_t w = 200, Int_t h = 400);
	 virtual ~Canvas() { }
private:
	 template <class T> void AddWidget()
			{ fWidgets.Add((T*)0, this, this); }
public:
	 friend class SetUpdateRate;
	 friend class StartStopUpdate;
	 friend class UpdateAll;
	 friend class UpdateCurrent;
	 friend class ClearAll;
	 friend class ClearCurrent;
	 ClassDef(rb::gui::Canvas, 0);
};

} }



#endif
