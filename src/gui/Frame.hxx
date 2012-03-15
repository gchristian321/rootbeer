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
	 ClassDef(rb::gui::Frame, 0);
};

class FrameFactory
{
private:
	 std::vector<Frame*> fFrames;
public:
	 FrameFactory() { }
	 ~FrameFactory();
	 void Add(Frame* frame);
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
	return true;
}

inline rb::gui::FrameFactory::~FrameFactory() {
	for(UInt_t i=0; i< fFrames.size(); ++i)
		 delete fFrames[i];
}

inline void rb::gui::FrameFactory::Add(Frame* frame) {
	fFrames.push_back(frame);
}


#endif
