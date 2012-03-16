#ifndef FRAME_HXX
#define FRAME_HXX
#include "utils/boost_scoped_ptr.h"
#include "utils/Error.hxx"
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
	 virtual void Sync() { }
	 void Null() { }
protected:
	 AWidget* Find(const char* name);
	 template <class T> T* Find(const char* name);
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
	 UInt_t Size();
	 Frame* Get(Int_t index);
};

} }



// ===== Inline Function Implementations ===== //

// ===== Class Frame ===== //

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

inline rb::gui::AWidget* rb::gui::Frame::Find(const char* name) {
	return fWidgets.Find(name);
}

template <class T> T* rb::gui::Frame::Find(const char* name) {
	return fWidgets.Find<T>(name);
}


// ===== Class FrameFactory ===== //

inline rb::gui::FrameFactory::~FrameFactory() {
	for(UInt_t i=0; i< fFrames.size(); ++i)
		 delete fFrames[i];
}

inline void rb::gui::FrameFactory::Add(Frame* frame) {
	fFrames.push_back(frame);
}

inline UInt_t rb::gui::FrameFactory::Size() {
	return fFrames.size();
}

inline rb::gui::Frame* rb::gui::FrameFactory::Get(Int_t index) {
	try { return fFrames.at(index); }
	catch (std::exception& e) {
		err::Error("FrameFactory::Get") << "Invalid index " << index << " (max = " << fFrames.size() << ").\n";
		return 0;
	}
}


#endif
