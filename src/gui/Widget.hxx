#ifndef WIDGET_HXX
#define WIDGET_HXX
#include <cassert>
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <TROOT.h>
#include <TGLabel.h>
#include <TGClient.h>
#include <TGButton.h>
#include <TGComboBox.h>
#include <TGNumberEntry.h>
#include "utils/Error.hxx"
#include "utils/boost_scoped_ptr.h"

namespace pix { enum Color_t {
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
}

namespace rb { namespace gui {

namespace {
const Pixel_t kDefaultBackground = pix::White;
const char* const kDefaultFont = "Arial 10 bold";
}

class AWidget;
typedef std::vector<AWidget*> WidgetVector_t;
typedef std::set<TGCompositeFrame*> FrameSet_t;

class AWidget
{
private:
	 const std::string fWidgetName;
public:
	 AWidget(const char* name): fWidgetName(name) { }
	 virtual ~AWidget() { }
	 std::string GetWidgetName() { return fWidgetName; }
	 virtual void operator() () = 0;
};

template <class OWNER>
class TWidget: public AWidget
{
public:
	 typedef void (OWNER::* MemFn_t)() ;
protected:
	 OWNER* fOwner;
	 MemFn_t fFunction;
public:
	 TWidget(OWNER* owner, MemFn_t function, const char* name):
		 AWidget(name), fOwner(owner), fFunction(function) { }
	 virtual ~TWidget() { }
	 virtual void operator() ();
};

class WidgetFactory
{
private:
	 FrameSet_t fFrames;
	 WidgetVector_t fWidgets;
public:
	 WidgetFactory() { }
	 ~WidgetFactory () {
		 for(UInt_t i=0; i< fWidgets.size(); ++i) {
			 delete fWidgets[i];
		 }
		 for(FrameSet_t::iterator it = fFrames.begin(); it != fFrames.end(); ++it) {
			 delete *it;
		 }
	 }
	 void AddFrame(TGCompositeFrame* frame) {
		 fFrames.insert(frame);
	 }
	 void AddToMainframe(TGMainFrame* mainframe, TGLayoutHints& layout) {
		 for(FrameSet_t::iterator it = fFrames.begin(); it != fFrames.end(); ++it) {
			 mainframe->AddFrame(*it, &layout);
		 }
	 }
	 AWidget* At(Int_t index) {
		 try {
			 return fWidgets.at(index);
		 } catch (std::exception& e) {
			 std::cerr << "Error: Invalid index " << index << "\n";
		 }
	 }
	 AWidget* Find(const char* name) {
		 AWidget* out = 0;
		 for(UInt_t i=0; i< fWidgets.size(); ++i) {
			 if(fWidgets[i]->GetWidgetName() == std::string(name)) {
				 out = fWidgets[i]; break;
			 }
		 }
		 if(!out) err::Error("WidgetFactory::Find") << name << " wasn't found!\n";
		 return out;
	 }
	 template <class T> T* Find(const char* name) {
		 T* out = 0;
		 for(UInt_t i=0; i< fWidgets.size(); ++i) {
			 if(fWidgets[i]->GetWidgetName() == std::string(name)) {
				 out = dynamic_cast<T*>(fWidgets[i]);
				 break;
			 }
		 }
		 if(!out) err::Error("WidgetFactory::Find") << name << " wasn't found!\n";
		 return out;
	 }
	 void operator() (Int_t index) {
		 try {
			 fWidgets.at(index)->operator() ();
		 } catch (std::exception& e) {
			 std::cerr << "Error: Invalid index " << index << "\n";
		 }
	 }
	 template <class DER>
	 void Add(DER* der, TGWindow* mainframe) {
		 AWidget* widget = new DER(mainframe, fWidgets.size());
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1>
	 void Add(DER* der, TGWindow* mainframe, A1 a1) {
		 AWidget* widget = new DER(mainframe, fWidgets.size(), a1);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1, class A2>
	 void Add(DER* der, TGWindow* mainframe, A1 a1, A2 a2) {
		 AWidget* widget = new DER(mainframe, fWidgets.size(), a1, a2);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1, class A2, class A3>
	 void Add(DER* der, TGWindow* mainframe, A1 a1, A2 a2, A3 a3) {
		 AWidget* widget = new DER(mainframe, fWidgets.size(), a1, a2, a3);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1, class A2, class A3, class A4>
	 void Add(DER* der, TGWindow* mainframe, A1 a1, A2 a2, A3 a3, A4 a4) {
		 AWidget* widget = new DER(mainframe, fWidgets.size(), a1, a2, a3, a4);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1, class A2, class A3, class A4, class A5>
	 void Add(DER* der, TGWindow* mainframe, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) {
		 AWidget* widget = new DER(mainframe, fWidgets.size(), a1, a2, a3, a4, a5);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1, class A2, class A3, class A4, class A5, class A6>
	 void Add(DER* der, TGWindow* mainframe, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) {
		 AWidget* widget = new DER(mainframe, fWidgets.size(), a1, a2, a3, a4, a5, a6);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
	 void Add(DER* der, TGWindow* mainframe, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) {
		 AWidget* widget = new DER(mainframe, fWidgets.size(), a1, a2, a3, a4, a5, a6, a7);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
	 void Add(DER* der, TGWindow* mainframe, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) {
		 AWidget* widget = new DER(mainframe, fWidgets.size(), a1, a2, a3, a4, a5, a6, a7, a8);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
	 void Add(DER* der, TGWindow* mainframe, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9) {
		 AWidget* widget = new DER(mainframe, fWidgets.size(), a1, a2, a3, a4, a5, a6, a7, a8, a9);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
	 void Add(DER* der, TGWindow* mainframe, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10) {
		 AWidget* widget = new DER(mainframe, fWidgets.size(), a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
		 fWidgets.push_back(widget);
	 }
	 template <class DER>
	 void Add(DER* der, TGCompositeFrame* frame, TGLayoutHints* layout) {
		 assert(fFrames.count(frame));
		 AWidget* widget = new DER(frame, fWidgets.size());
		 frame->AddFrame(&dynamic_cast<TGFrame&>(*widget), layout);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1>
	 void Add(DER* der, TGCompositeFrame* frame, TGLayoutHints* layout, A1 a1) {
		 assert(fFrames.count(frame));
		 AWidget* widget = new DER(frame, fWidgets.size(), a1);
		 frame->AddFrame(&dynamic_cast<TGFrame&>(*widget), layout);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1, class A2>
	 void Add(DER* der, TGCompositeFrame* frame, TGLayoutHints* layout, A1 a1, A2 a2) {
		 assert(fFrames.count(frame));
		 AWidget* widget = new DER(frame, fWidgets.size(), a1, a2);
		 frame->AddFrame(&dynamic_cast<TGFrame&>(*widget), layout);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1, class A2, class A3>
	 void Add(DER* der, TGCompositeFrame* frame, TGLayoutHints* layout, A1 a1, A2 a2, A3 a3) {
		 assert(fFrames.count(frame));
		 AWidget* widget = new DER(frame, fWidgets.size(), a1, a2, a3);
		 frame->AddFrame(&dynamic_cast<TGFrame&>(*widget), layout);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1, class A2, class A3, class A4>
	 void Add(DER* der, TGCompositeFrame* frame, TGLayoutHints* layout, A1 a1, A2 a2, A3 a3, A4 a4) {
		 assert(fFrames.count(frame));
		 AWidget* widget = new DER(frame, fWidgets.size(), a1, a2, a3, a4);
		 frame->AddFrame(&dynamic_cast<TGFrame&>(*widget), layout);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1, class A2, class A3, class A4, class A5>
	 void Add(DER* der, TGCompositeFrame* frame, TGLayoutHints* layout, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) {
		 assert(fFrames.count(frame));
		 AWidget* widget = new DER(frame, fWidgets.size(), a1, a2, a3, a4, a5);
		 frame->AddFrame(&dynamic_cast<TGFrame&>(*widget), layout);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1, class A2, class A3, class A4, class A5, class A6>
	 void Add(DER* der, TGCompositeFrame* frame, TGLayoutHints* layout, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) {
		 assert(fFrames.count(frame));
		 AWidget* widget = new DER(frame, fWidgets.size(), a1, a2, a3, a4, a5, a6);
		 frame->AddFrame(&dynamic_cast<TGFrame&>(*widget), layout);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
	 void Add(DER* der, TGCompositeFrame* frame, TGLayoutHints* layout, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) {
		 assert(fFrames.count(frame));
		 AWidget* widget = new DER(frame, fWidgets.size(), a1, a2, a3, a4, a5, a6, a7);
		 frame->AddFrame(&dynamic_cast<TGFrame&>(*widget), layout);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
	 void Add(DER* der, TGCompositeFrame* frame, TGLayoutHints* layout, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) {
		 assert(fFrames.count(frame));
		 AWidget* widget = new DER(frame, fWidgets.size(), a1, a2, a3, a4, a5, a6, a7, a8);
		 frame->AddFrame(&dynamic_cast<TGFrame&>(*widget), layout);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
	 void Add(DER* der, TGCompositeFrame* frame, TGLayoutHints* layout, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9) {
		 assert(fFrames.count(frame));
		 AWidget* widget = new DER(frame, fWidgets.size(), a1, a2, a3, a4, a5, a6, a7, a8, a9);
		 frame->AddFrame(&dynamic_cast<TGFrame&>(*widget), layout);
		 fWidgets.push_back(widget);
	 }
	 template <class DER, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
	 void Add(DER* der, TGCompositeFrame* frame, TGLayoutHints* layout, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10) {
		 assert(fFrames.count(frame));
		 AWidget* widget = new DER(frame, fWidgets.size(), a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
		 frame->AddFrame(&dynamic_cast<TGFrame&>(*widget), layout);
		 fWidgets.push_back(widget);
	 }
};


template <class T>
class TextButton: public TWidget<T>, public TGTextButton
{
public:
	 TextButton(TGWindow* mainframe, Int_t id, T* owner, void (T::* f)(),
							const char* name, const char* text, Pixel_t color = pix::White, Pixel_t text_color = pix::Black):
		 TWidget<T> (owner, f, name), TGTextButton(mainframe, text, id) {
		 SetBackgroundColor(color);
		 SetTextColor(text_color);
		 SetFont(kDefaultFont);
	 }
};

template <class T>
class Label: public TWidget<T>, public TGLabel
{
public:
	 Label(TGWindow* mainframe, Int_t id, T* owner, const char* name, const char* text):
		 TWidget<T> (owner, &T::Null, name), TGLabel(mainframe, text) {
		 SetTextFont(kDefaultFont);
		 SetBackgroundColor(pix::White);
	 }
};

template <class T>
class TextEntry: public TWidget<T>, public TGTextEntry
{
public:
	 TextEntry(TGWindow* mainframe, Int_t id, T* owner, void(T::* f)(), const char* name, const char* init):
		 TWidget<T> (owner, f, name), TGTextEntry(mainframe, init, id) { }
};

template <class T>
class NumberEntry: public TWidget<T>, public TGNumberEntry
{
public:
	 NumberEntry(TGWindow* mainframe, Int_t id, T* owner, void(T::* f)(),
							 const char* name, Int_t init, Int_t low = -1, Int_t high = -1, Int_t digitwidth = 5,
							 TGNumberFormat::EStyle style = TGNumberFormat::kNESReal,
							 TGNumberFormat::EAttribute attr =TGNumberFormat::kNEAAnyNumber):
		 TWidget<T> (owner, f, name),
		 TGNumberEntry(mainframe, init, digitwidth, id, style, attr) {
		 if(low < high) SetLimits(TGNumberFormat::kNELLimitMinMax, low, high);
	 }
};

template <class T>
class NumberEntryField: public TWidget<T>, public TGNumberEntryField
{
public:
	 NumberEntryField(TGWindow* mainframe, Int_t id, T* owner, void (T::*f)(),
										const char* name, Int_t init, Int_t low = -1, Int_t high = -1,
										TGNumberFormat::EStyle style = TGNumberFormat::kNESReal,
										TGNumberFormat::EAttribute attr =TGNumberFormat::kNEAAnyNumber):
		 TWidget<T> (owner, f, name),
		 TGNumberEntryField(mainframe, id, init, style, attr) {
		 if(low < high) SetLimits(TGNumberFormat::kNELLimitMinMax, low, high);
	 }
};

} }


#ifndef __MAKECINT__
#include "boost/bind.hpp"
template <class OWNER>
void rb::gui::TWidget<OWNER>::operator() () {
	boost::bind(fFunction, fOwner)();
}
#endif


#endif
