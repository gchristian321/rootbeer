#ifndef __HISTVIEWER_HH
#define __HISTVIEWER_HH
// c++ library includes
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
// ROOT library includes
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

using namespace std;

class HistMaker;
class HistViewer {
    RQ_OBJECT("HistViewer")
    private:
        TGMainFrame *fMain;  // main window
        TGTab* fTab;         // controls the tabs
        std::vector<TGCompositeFrame*> tabs;
        std::vector<TRootEmbeddedCanvas*> ecanvases;
        std::vector<std::string> availablehists;
        Int_t cindex; // current tab / canvas index
        HistMaker* hmaker;
        char drawoption[500];
    public:
        TFile* infile; // possible input file
        TTree* intree; // possible input tree
        HistViewer(const TGWindow *p,UInt_t w,UInt_t h,HistMaker* histmaker);
        //HistViewer(const TGWindow *p,UInt_t w,UInt_t h);
        virtual ~HistViewer();
        void createtab(const char* name="tab");
        void DoTab(Int_t id);
        //void DoTab(Int_t id);
        void HandleAttachMenu(Int_t id);
        void DoHistSelect();
        void HandleHistSelectMenu(TGFrame* f,Int_t btn);
        void DoTextEntry(const char* text);
};
#endif
