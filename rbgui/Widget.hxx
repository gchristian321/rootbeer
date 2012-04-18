#ifndef WIDGET_HXX
#define WIDGET_HXX
#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>
#include <TROOT.h>
#include <TGClient.h>
#include <TGButton.h>
#include <TGComboBox.h>
#include <TGNumberEntry.h>
#include "../src/utils/boost_ptr_vector.h"
#include "../src/utils/boost_scoped_ptr.h"

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

class AWidget;
typedef std::vector<AWidget*> WidgetVector_t;

class AWidget
{
public:
	 AWidget() { }
	 virtual ~AWidget() { }
	 virtual void operator() () = 0;
};

class WidgetFactory
{
private:
	 WidgetVector_t fVector;
public:
	 WidgetFactory() { }
	 ~WidgetFactory () {
		 for(Int_t i=0; i< fVector.size(); ++i) {
			 delete fVector[i];
		 }
	 }
	 void AddFrames(TGMainFrame* mainframe, boost::scoped_ptr<TGLayoutHints>& layout) {
	 	 for(Int_t i=0; i< fVector.size(); ++i) {
			 TGFrame* button = dynamic_cast<TGFrame*>(fVector[i]);
			 assert(button != 0); 
	 		 mainframe->AddFrame(button, layout.get());
	 	 }
	 }
	 void operator() (Int_t index) {
		 try {
			 fVector.at(index)->operator() ();
		 } catch (std::exception& e) {
			 std::cerr << "Error: Invalid index " << index << "\n";
		 }
	 }
	 template <class DER>
	 void Add(DER* der, TGMainFrame* mainframe) {
		 AWidget* widget = new DER(mainframe, fVector.size());
		 fVector.push_back(widget);
	 }
	 template <class DER, class A1>
	 void Add(DER* der, TGMainFrame* mainframe, A1 a1) {
		 AWidget* widget = new DER(mainframe, fVector.size(), a1);
		 fVector.push_back(widget);
	 }
	 template <class DER, class A1, class A2>
	 void Add(DER* der, TGMainFrame* mainframe, A1 a1, A2 a2) {
		 AWidget* widget = new DER(mainframe, fVector.size(), a1, a2);

		 fVector.push_back(widget);
	 }
	 template <class DER, class A1, class A2, class A3>
	 void Add(DER* der, TGMainFrame* mainframe, A1 a1, A2 a2, A3 a3) {
		 AWidget* widget = new DER(mainframe, fVector.size(), a1, a2, a3);
		 fVector.push_back(widget);
	 }
};

} }



#endif
