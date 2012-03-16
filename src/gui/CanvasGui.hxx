#ifndef CANVAS_GUI_HXX
#define CANVAS_GUI_HXX
#include "Frame.hxx"


namespace rb { namespace gui {

class Canvas: public Frame
{
private:
	 Int_t fUpdateRate;
	 Bool_t kIsUpdating;
public:
	 Canvas(Int_t w = 200, Int_t h = 400);
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
