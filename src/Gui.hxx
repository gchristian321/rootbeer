//! \file Gui.hxx
//! \brief Defines the main rootbeer gui window.
//! \note The gui layout was originally created with root's TGuiBuilder, which generates source code
//! all contained within a single function. From here (once the layout was stable), the function was
//! broken down into parts and put inside of a class for better resource management.
#ifndef RB_GUI_HXX
#define RB_GUI_HXX

// ====== Automatically Generated (TGuiBuilder) #includes ======= //
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
#include "Riostream.h"

// ======== Forward Declarations ========= //
namespace rb { class Signals; }


// ======== Class Definitions ========= //

/// \brief Main rootbeer gui window.
/// \note Data members not documented individually; each corrsponds to a 
/// different gui element.
class TGRbeerFrame : public TGMainFrame
{
private:
	TGCompositeFrame *fMainFrame1596;
	TGCompositeFrame *fMainFrame6310;
	TGGroupFrame *fGroupCanvas; // "Canvas"
	TGTextButton *fZeroAll; // "Zero All"
	TGTextButton *fDivideCurrent; // "Divide Current"
	TGTextEntry *fEntryName;
	TGTextButton *fCreateNew; // "Create New"
	TGLabel *fLabelName; // "Name:"
	TGCheckButton *fLogy; // "Log/Linear"
	TGCheckButton *fLogz; // "Log/Linear"
	TGNumberEntry *fUpdateRate;
	TGLabel *fLabelRate; // "Rate [sec.]"
	TGTextButton *fStartRefresh; // "Start Refresh"
	TGTextButton *fRefreshCurrent; // "Refresh Current"
	TGTextButton *fRefreshAll; // "Refresh All"
	TGTextButton *fZeroCurrent; // "Zero Current"
	TGTextButton *fClearCurrent; // "Clear Canvas"
	TGGroupFrame *fGroupConfig; // "Configuration"
	TGTextButton *fConfigLoad; // "Load"
	TGTextButton *fConfigSave; // "Save"
	TGGroupFrame *fGroupData; // "Data"
	TGTextButton *fAttachOnline; // "Attach Online"
	TGTextButton *fAttachFile; // "Attach File"
	TGTextButton *fAttachList; // "Attach List"
	TGTextButton *fUnattach; // "Unattach"
	TGTextEntry *fEntryHost;
	TGLabel *fLabelHost; // "Host:"
	TGTextEntry *fEntryPort;
	TGLabel *fFilterLabel;
	TGTextEntry *fFilterEntry;
	TGLabel *fFilterTypeLabel;	 
	TGComboBox *fFilterType;
	TGTextButton *fSetFilter;
	TGLabel *fLabelPort; // "Expt:"
	TGCheckButton *fIsContinuous; // "Continuous"
	TGCheckButton *fSaveData; // "Save Data"
	TGCheckButton *fSaveHist; // "Save Histograms"
	TGLabel *fLabelSource; // "[none]"
	TGLabel *fLabelDataSource; // "Data source:"
	TGLabel *fNbuffersLabelDivider; // " | "
	TGLabel *fNbuffersLabel; // "Buffers Analyzed:"
	TGLabel *fNbuffers; // "0"

public:
	/// \brief Create a new rootbeer gui window
	/// \details Simply calls the TGMainFrame constructor
	TGRbeerFrame(const TGWindow* p = 0, UInt_t w = 1, UInt_t h = 1, UInt_t options = kVerticalFrame) :
		TGMainFrame(p, w, h, options) { }

	/// \brief Free all resources upon window destruction.
	/// \details Closes all signal/socket onnections (by calling destructors of TQObjects).
	/// \todo Currently, the destructor does \e not free any memory resources allocated to owned gui objects.
	/// Effectively, this means that the gui window is the source of a memory leak. In practice, this is of
	/// minor consequence since gui windows are typically created/destroyed rarely (if at all) within a program
	/// instance. However, it should be addressed at some point!
	~TGRbeerFrame();

	/// \brief Set up the layout of the gui components.
	/// \note Most of the code in this function was generated by TGuiBuilder, then modified by hand to tweak
	/// the look and to be made compatible with the class layout.
	void GuiLayout();

	/// \brief Create all TQObject connections between signals emitted from gui objects and rb::Signals
	void MakeConnections();

	/// Allow rb::Signals access to class data.
	friend class rb::Signals;
};



#endif
