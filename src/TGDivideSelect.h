#ifndef ROOT_TGDivideSelect
#define ROOT_TGDivideSelect

#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif

class TGLabel;
class TGComboBox;
class TGTextEntry;
class TGTextButton;
class TGNumberEntryField;


class TGDivideSelect : public TGTransientFrame {

private:
   TGLabel             *fLabelH;  // horiz. label
   TGLabel             *fLabelV;  // vert. label
   TGLabel             *fX;       // 'x' denoting 'by'
   TGNumberEntryField  *fNEH;     // number entry widget
   TGNumberEntryField  *fNEV;     // number entry widget
   TGTextButton        *fOk;      // ok button
   TGTextButton        *fCancel;  // cancel button
	 int                 *fRetH;    // horizontal divisions
	 int                 *fRetV;    // vertical divisions

   TGDivideSelect(const TGDivideSelect&);  // Not implemented
   TGDivideSelect &operator= (const TGDivideSelect&); // Not implemented

public:
   TGDivideSelect(const TGWindow *p = 0, const TGWindow *main = 0,
									int* ret_h = 0, int* ret_v = 0, UInt_t options = kVerticalFrame);
   ~TGDivideSelect();
   virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);

   ClassDef(TGDivideSelect, 0)  // Simple input dialog

};

#endif
