//! \file HistMaker.h
//! \brief Defines the class to make histograms from the GIU.

#ifndef __HISTMAKER_HH
#define __HISTMAKER_HH
// c++ library includes
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
// ROOT library includes
#include <TGListView.h>
#include <TQObject.h>
#include <RQ_OBJECT.h>
#include <TGFrame.h>
#include <TBranchObject.h>
#include <TGMenu.h>
#include <TTree.h>
#include <TFile.h>
#include <TGTab.h>
#include <TRootEmbeddedCanvas.h>
#include <TGButton.h>
#include <TGButtonGroup.h>
#include <TGLayout.h>
#include <TGTableLayout.h>
#include <TGTextEntry.h>
#include <TGLabel.h>
#include <TGPack.h>
#include <TBranchElement.h>
#include <TBranchElement.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TGListBox.h>
#include <TROOT.h>
// My includes
#include "Hist.hxx"

#include "TH1D_SF.h"
#include "TH2D_SF.h"
#include "TH3D_SF.h"
#include "HistViewer.h"
using namespace std;


/// Enum to id arguments to command boxes.
/// \remark Moved to the header file and gt rid of the
/// one in HistViewer.cc. -GC
enum CommandIdentifiers {
    ATTACH_ONLINE,
    ATTACH_TFILE,
    ATTACH_TCHAIN,
    ATTACH_PIPE,

    D1,
    D2,
    D3,
    SUMMARY,
    SUMMATION,
    BITMASK,

    FILL_TREE,
    FILL_ONLINE,
    FILL_TREE_ONLINE,

    PARAM_X,
    PARAM_Y,
    PARAM_Z,

    BINS_X,
    BINS_Y,
    BINS_Z,

    LOW_X,
    LOW_Y,
    LOW_Z,

    HIGH_X,
    HIGH_Y,
    HIGH_Z,

    NAME,
    GATE,

    DRAW_OPTION
};


class TGLVEntry_mod:public TGLVEntry{
    public:
        TGLVEntry_mod(const TGLVContainer* p, const TString& name, const TString& cname, TGString** subnames = 0, UInt_t options = kChildFrame, Pixel_t back = GetWhitePixel());

        void SetSubnames(const char* n1 = "", const char* n2 = "", const char* n3 = "", const char* n4 = "", const char* n5 = "", const char* n6 = "", const char* n7 = "", const char* n8 = "", const char* n9 = "", const char* n10 = "", const char* n11 = "", const char* n12 = "",const char* n13 = "", const char* n14 = "");

	/// \remark Added method allowing setting from a vector.
	void SetSubnames(const std::vector<std::string>& args);

	/// \remark Added method allowing setting from histogram.
	void SetSubnames(rb::Hist* hst);
};



//class HistViewer;

/// Class to make histograms from the GUI.
class HistMaker {
    RQ_OBJECT("HistMaker");
    private:
        TGListView* histlist;
        TGLVContainer*  cont;
        TGMenuBar* xmenubar,*ymenubar,*zmenubar;
        TGPopupMenu* xmenu,*ymenu,*zmenu;
        TGHorizontalFrame *top,*fbuttons,*fcbuttons;
        TGTextButton *loadFile,*saveFile,*create;

        // layout hints
        TGLayoutHints* fL3,*fL4,*fL5;

        // group frames
        TGGroupFrame* deffile;
        TGTextEntry* xptext,*yptext,*zptext,*binxtext,*binytext,*binztext,*loxtext,*loytext,*loztext,*hixtext,*hiytext,*hiztext,*histnametext,*tgate;

        // important for the fancy drop down menu
        std::vector<std::string> *namesx;
        std::vector<std::string> *namesy;
        std::vector<std::string> *namesz;

        /// collect info on buttons
	/// \remark Changed button info to strings. Convert to appropriate
	/// types when used. - GC
	std::map<Int_t, std::string> fInfo;

	std::string GetInfo(Int_t indx) {
	  static std::map<Int_t, std::string>::iterator it;
	  it = fInfo.find(indx);
	  if(it != fInfo.end()) return it->second;
	  Error("GetInfo", "Invalid key %d", indx);
	  return "";
	}

        Int_t htype,hfilloption,bins[3];
	Bool_t invalide;
        Double_t low[3],high[3];
        // these are pretty long... is that bad?
	//        char px[500],py[500],pz[500],hname[500],hgate[500];
  
	/// Histogram button box fields.
	std::string px, py, pz, hname, hgate;
    public:
        // public data members
            TGMainFrame *fMain;
            HistViewer* fView;
         TH1DMap* h1ds; // 1d histograms
         TH2DMap* h2ds; // 2d histograms
         TH3DMap* h3ds; // 3d histograms


    public:
        HistMaker(const TGWindow *p,UInt_t w,UInt_t h);
        virtual ~HistMaker();
        void DoTab(Int_t id);
        void MakeParamDropDown(TTree* t);

        // stuff for the fancy drop down
        void HandleTreeMenuX(Int_t id);
        void HandleTreeMenuY(Int_t id);
        void HandleTreeMenuZ(Int_t id);
        void MakePopupFromBranch(TBranchObject* branch,TGPopupMenu* menu,std::vector<std::string> *names, Int_t offset=0,std::string* currentname=NULL);
        TGPopupMenu* MakePopupFromTree(TTree* t,TGMenuBar* bar,std::vector<std::string> *names,const char* menuname,Int_t offset=0,std::string* currentname=NULL);
        void GenerateTreePopups(TTree* t);
        void MakeHistFromGui();
        void MakeHist(TTree* t,const char* hn,Int_t ht,Int_t foptions,const char* ngate="",const char* parx="",Int_t bx=0, Double_t lx=0, Double_t hx=0, const char* pary="", Int_t by=0, Double_t ly=0, Double_t hy=0,const char* parz="", Int_t bz=0, Double_t lz=0, Double_t hz=0, Bool_t invalidevt="FALSE");
        void DoTypeRadio();
        void DoFillRadio();
	void DoInvalidCheck();
        void DoTextEntry(const char* text);
        void ClearName(std::string hns);
        std::string ProcessFinalName(std::string* currentname,std::string flevelname);
        Int_t IsArray(std::string branchname,Int_t* index1,Int_t* index2);

};
#endif
