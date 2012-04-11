#ifndef HISTGUI_HXX
#define HISTGUI_HXX


#ifndef ROOT_TGDockableFrame
#include "TGDockableFrame.h"
#endif
#ifndef ROOT_TGMenu
#include "TGMenu.h"
#endif
#ifndef ROOT_TGMdiDecorFrame
#include "TGMdiDecorFrame.h"
#endif
#ifndef ROOT_TG3DLine
#include "TG3DLine.h"
#endif
#ifndef ROOT_TGMdiFrame
#include "TGMdiFrame.h"
#endif
#ifndef ROOT_TGMdiMainFrame
#include "TGMdiMainFrame.h"
#endif
#ifndef ROOT_TGMdiMenu
#include "TGMdiMenu.h"
#endif
#ifndef ROOT_TGColorDialog
#include "TGColorDialog.h"
#endif
#ifndef ROOT_TGColorSelect
#include "TGColorSelect.h"
#endif
#ifndef ROOT_TGListBox
#include "TGListBox.h"
#endif
#ifndef ROOT_TGNumberEntry
#include "TGNumberEntry.h"
#endif
#ifndef ROOT_TGScrollBar
#include "TGScrollBar.h"
#endif
#ifndef ROOT_TGComboBox
#include "TGComboBox.h"
#endif
#ifndef ROOT_TGuiBldHintsEditor
#include "TGuiBldHintsEditor.h"
#endif
#ifndef ROOT_TGuiBldNameFrame
#include "TGuiBldNameFrame.h"
#endif
#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGFileDialog
#include "TGFileDialog.h"
#endif
#ifndef ROOT_TGShutter
#include "TGShutter.h"
#endif
#ifndef ROOT_TGButtonGroup
#include "TGButtonGroup.h"
#endif
#ifndef ROOT_TGCanvas
#include "TGCanvas.h"
#endif
#ifndef ROOT_TGFSContainer
#include "TGFSContainer.h"
#endif
#ifndef ROOT_TGFontDialog
#include "TGFontDialog.h"
#endif
#ifndef ROOT_TGuiBldEditor
#include "TGuiBldEditor.h"
#endif
#ifndef ROOT_TGProgressBar
#include "TGProgressBar.h"
#endif
#ifndef ROOT_TGTextEdit
#include "TGTextEdit.h"
#endif
#ifndef ROOT_TGButton
#include "TGButton.h"
#endif
#ifndef ROOT_TGFSComboBox
#include "TGFSComboBox.h"
#endif
#ifndef ROOT_TGLabel
#include "TGLabel.h"
#endif
#ifndef ROOT_TGMsgBox
#include "TGMsgBox.h"
#endif
#ifndef ROOT_TRootGuiBuilder
#include "TRootGuiBuilder.h"
#endif
#ifndef ROOT_TGTab
#include "TGTab.h"
#endif
#ifndef ROOT_TGListView
#include "TGListView.h"
#endif
#ifndef ROOT_TGSplitter
#include "TGSplitter.h"
#endif
#ifndef ROOT_TGStatusBar
#include "TGStatusBar.h"
#endif
#ifndef ROOT_TGListTree
#include "TGListTree.h"
#endif
#ifndef ROOT_TGuiBldGeometryFrame
#include "TGuiBldGeometryFrame.h"
#endif
#ifndef ROOT_TGToolTip
#include "TGToolTip.h"
#endif
#ifndef ROOT_TGToolBar
#include "TGToolBar.h"
#endif
#ifndef ROOT_TRootEmbeddedCanvas
#include "TRootEmbeddedCanvas.h"
#endif
#ifndef ROOT_TCanvas
#include "TCanvas.h"
#endif
#ifndef ROOT_TGuiBldDragManager
#include "TGuiBldDragManager.h"
#endif
#include "TROOT.h"
#include "Riostream.h"

namespace rb { class HistSignals; }
class TGHistVarFrame : public TGMainFrame
{
private:
	 TGCompositeFrame *fMainFrame2592;
	 TGCompositeFrame *fMainFrame2438;
	 TGCompositeFrame *fMainFrame1180;
	 TGCompositeFrame *fMainFrame2078;
	 TGCompositeFrame *fMainFrame4698;
	 TGCompositeFrame *fMainFrame1001;
	 TGCompositeFrame *fMainFrame942;
	 TGCompositeFrame *fMainFrame1678;
	 TGCompositeFrame *fMainFrame909;
	 TGCompositeFrame *fMainFrame1598;
	 TGCompositeFrame *fMainFrame2824;
	 TGTab *fHistTab;
	 TGCanvas *fHistCanvas;
	 TGListTree *fHistTree;
	 TGTextButton *fCommandOk; // "OK"
	 TGTextButton *fQuit; // "Quit"
	 TGTextEntry *fCommandEntry;
	 TGLabel *fCommandLabel; // "Command (member function)"
	 TGTextButton *fDrawButton; // "Draw"
	 TGTextButton *fDeleteButton; // "Delete"
	 TGGroupFrame *fHistCreateFrame; // "Create New"
	 TGNumberEntryField *fHighX;
	 TGNumberEntryField *fLowX;
	 TGNumberEntryField *fBinsX;
	 TGNumberEntryField *fBinsY;
	 TGNumberEntryField *fLowY;
	 TGNumberEntryField *fHighY;
	 TGNumberEntryField *fBinsZ;
	 TGNumberEntryField *fLowZ;
	 TGNumberEntryField *fHighZ;
	 TGLabel *fLabelX; // "x"
	 TGLabel *fLabelY; // "y"
	 TGLabel *fLabelZ; // "z"
	 TGLabel *fLabelParameter; // "Parameter"
	 TGLabel *fLabelBins; // "Bins"
	 TGLabel *fLabelLow; // "Low"
	 TGLabel *fLabelHigh; // "High"
	 TGComboBox *fTypeEntry;
	 TGTextEntry *fGateEntry;
	 TGLabel *fGateLabel; // "Gate"
	 TGTextEntry *fNameEntry;
	 TGTextEntry *fTitleEntry;
	 TGLabel *fTypeLabel; // "Type"
	 TGComboBox *fParamZ; // ""
	 TGComboBox *fParamY; // ""
	 TGComboBox *fParamX; // ""
	 TGComboBox *fEventEntry;
	 TGLabel *fEventLabel; // "Event type"
	 TGLabel *fNameLabel; // "Name"
	 TGLabel *fTitleLabel; // "Title"
	 TGTextButton *fHistCreateButton; // "Create/Replace"
	 TGTextButton *fHistRegateButton; // "Regate"
	 TGGroupFrame *fHistConfigFrame; // "Configuration File"
	 TGTextButton *fHistSaveButton; // "Save"
	 TGTextButton *fHistLoadButton; // "Load"
	 TGTextEntry *fDrawOptionEntry;
	 TGTextButton *fMkdirButton; // "New Directory"
	 TGLabel *fDrawOptionLabel; // "Draw option:"
	 TGGroupFrame *fVariablesFrame; // "Variables"
	 TGCanvas *fVariablesCanvas;
	 TGListTree *fVariablesTree;
	 TGTextButton *fReadVarButton; // "Read Value"
	 TGTextButton *fSetVarButton; // "Set Value"
	 TGNumberEntryField *fVarEntry;
	 TGGroupFrame *fConfigAllFrame; // "Configuration Files"
	 TGTextButton *fConfigHistSaveButton; // "Save Histograms"
	 TGTextButton *fConfigVarSaveButton; // "Save Variables"
	 TGTextButton *fConfigCanvasSaveButton; // "Save Canvases"
	 TGTextButton *fConfigAllSaveButton; // "Save All"
	 TGTextButton *fConfigAllLoadButton; // "Load"
	 TGGroupFrame *fConfigLoadMethodFrame; // "Load Method"
	 TGRadioButton *fConfigLoadMethodReset; // "Reset"
	 TGRadioButton *fConfigLoadMethodOverwrite; // "Overwrite"
	 TGRadioButton *fConfigLoadMethodCumulate; // "Cumulate"
public:
	 TGHistVarFrame(const TGWindow* p = 0, UInt_t w = 1, UInt_t h = 1, UInt_t options = kVerticalFrame) :
		 TGMainFrame(p, w, h, options) { }
	 ~TGHistVarFrame();

	 void HistGuiLayout();
	 void MakeHistConnections();
	 friend class rb::HistSignals;
};




#endif
