#ifndef FRAME_HXX
#define FRAME_HXX
#include "utils/boost_scoped_ptr.h"
#include "utils/Error.hxx"
#include "Widget.hxx"


namespace rb { namespace gui {

class MainFrame: public TGMainFrame
{
protected:
	 WidgetFactory fWidgets;
	 boost::scoped_ptr<TGLayoutHints> fLayout;
public:
	 MainFrame(UInt_t width, UInt_t height);
	 virtual ~MainFrame() { }
	 virtual Bool_t ProcessMessage(Long_t msg, Long_t param1, Long_t param2);
	 virtual void Sync() { }
	 void Null() { }
	 void ToDo() { std::cerr << "TODO: Implement this button\n"; }
protected:
	 AWidget* Find(const char* name);
	 template <class T> T* Find(const char* name);
	 template <class T> TGCompositeFrame* AddFrame(Int_t w, Int_t h, UInt_t type);
	 void InitFrames(Int_t, Int_t, Int_t, Int_t, Int_t, Int_t);
	 ClassDef(rb::gui::MainFrame, 0);
};

class Popup: public MainFrame
{
public:
	 Popup(UInt_t w, UInt_t h);
	 virtual ~Popup() { }
	 void Cancel();
	 void Okay();
private:
	 virtual void DoAction() = 0;
};
	 
class MainFrameFactory
{
private:
	 std::vector<rb::gui::MainFrame*> fFrames;
public:
	 MainFrameFactory() { }
	 ~MainFrameFactory();
	 void Add(rb::gui::MainFrame* frame);
	 UInt_t Size();
	 MainFrame* Get(Int_t index);
};

} }



// ===== Inline Function Implementations ===== //

// ===== Class MainFrame ===== //

inline rb::gui::MainFrame::MainFrame(UInt_t w, UInt_t h):
	TGMainFrame(gClient->GetRoot(), w, h),
	fLayout(new TGLayoutHints(kLHintsCenterX | kLHintsCenterY)) {
	SetBackgroundColor(kDefaultBackground);
}

inline void rb::gui::MainFrame::InitFrames(Int_t a, Int_t b, Int_t c, Int_t d, Int_t e, Int_t f) {
	fWidgets.AddToMainframe(this, *fLayout);
	SetWMSizeHints(a,b,c,d,e,f);
	MapSubwindows();
	Resize(GetDefaultSize());
	MapWindow();
}

inline Bool_t rb::gui::MainFrame::ProcessMessage(Long_t msg, Long_t param1, Long_t param2) {
	fWidgets(param1);
	return true;
}

inline rb::gui::AWidget* rb::gui::MainFrame::Find(const char* name) {
	return fWidgets.Find(name);
}

template <class T> T* rb::gui::MainFrame::Find(const char* name) {
	return fWidgets.Find<T>(name);
}

template <class T>
TGCompositeFrame* rb::gui::MainFrame::AddFrame(Int_t w, Int_t h, UInt_t type) {
	TGCompositeFrame* frame = new T(this, w, h, type);
	fWidgets.AddFrame(frame);
	return frame;
}

// ===== Class Popup ===== //

inline rb::gui::Popup::Popup(UInt_t w, UInt_t h):
	MainFrame(w, h) { }

inline void rb::gui::Popup::Cancel() {
	delete this;
}

inline void rb::gui::Popup::Okay() {
	DoAction();
	Cancel();
}


// ===== Class MainFrameFactory ===== //

inline rb::gui::MainFrameFactory::~MainFrameFactory() {
	for(UInt_t i=0; i< fFrames.size(); ++i)
		 delete fFrames[i];
}

inline void rb::gui::MainFrameFactory::Add(rb::gui::MainFrame* frame) {
	fFrames.push_back(frame);
}

inline UInt_t rb::gui::MainFrameFactory::Size() {
	return fFrames.size();
}

inline rb::gui::MainFrame* rb::gui::MainFrameFactory::Get(Int_t index) {
	try { return fFrames.at(index); }
	catch (std::exception& e) {
		err::Error("MainFrameFactory::Get") << "Invalid index " << index << " (max = " << fFrames.size() << ").\n";
		return 0;
	}
}


// ====== Derived TGCompositeFrame Classes ====== //

namespace rb { namespace gui {

template <class T>
class Composite: public T
{
private:
	 rb::gui::MainFrame* fOwner;
public:
	 Composite(MainFrame* owner, Int_t w, Int_t h, UInt_t type):
		 T(owner, w, h, type, kDefaultBackground), fOwner(owner) { }
	 ~Composite() { }
	 Bool_t ProcessMessage(Long_t a, Long_t b, Long_t c) { return fOwner->ProcessMessage(a, b, c); }
};

} }


#endif
