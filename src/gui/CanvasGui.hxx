#ifndef CANVAS_GUI_HXX
#define CANVAS_GUI_HXX
#include "MainFrame.hxx"


namespace rb { namespace gui {

class Canvas: public MainFrame
{
private:
	 Int_t fUpdateRate;
	 Bool_t kIsUpdating;
public:
	 Canvas();
	 virtual ~Canvas() { }
	 virtual void Sync();
private:
	 void UpdateAll();
	 void UpdateCurrent();
	 void ClearAll();
	 void ClearCurrent();
	 void CreateCanvas();
	 void DivideCanvas();
	 void StartStop();
	 void SetUpdateRate();
	 
public:
	 friend class TWidget<Canvas>;
	 ClassDef(rb::gui::Canvas, 0);
};

} }


#endif
