#include "HistViewer.h"
#include "HistMaker.h"
#include "TGFileDialog.h"
#include "TCanvas.h"
#include <stdlib.h>

const char *filetypes[] = { 
                            "ROOT files",    "*.root",
                            "All files",     "*",
                            0,               0 };

#if 0
// now defined in HistMaker.h
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
#endif


void 
HistViewer::DoTextEntry(const char* text)
{
    // handle text input
    TGTextEntry *te = (TGTextEntry *) gTQSender;
    Int_t id = te->WidgetId();
    std::cout << id << endl;
    switch (id)
    {
        case DRAW_OPTION:
            {
                sprintf(drawoption,"%s",text);
            }
            break;
    }
}
void 
HistViewer::HandleAttachMenu(Int_t id)
{
    switch (id)
    {
        case ATTACH_ONLINE:
            {
            }
            break;
        case ATTACH_TFILE:
            {
                static TString dir(".");
                TGFileInfo fi;
                fi.fFileTypes = filetypes;
                fi.fIniDir    = StrDup(dir);
                printf("fIniDir = %s\n", fi.fIniDir);
                new TGFileDialog(gClient->GetRoot(), fMain, kFDOpen, &fi);
                printf("Open file: %s (dir: %s)\n", fi.fFilename, fi.fIniDir);
                // TODO: this is dangerous and could cause issues if you dont open a root file
                infile = new TFile(fi.fFilename);
                intree = (TTree*)infile->Get("t");
                //cout << intree << " " << hmaker<<endl;
                hmaker->GenerateTreePopups(intree);
                // Map all subwindows of main frame
                hmaker->fMain->MapSubwindows();
                //dir = fi.fIniDir;
            }
            break;
        case ATTACH_TCHAIN:
            {
            }
            break;
        case ATTACH_PIPE:
            {
            }
            break;
        default:
            break;
    }
};
void
HistViewer::createtab(const char* name)
{
    tabs.push_back(fTab->AddTab(name));
    ecanvases.push_back(new TRootEmbeddedCanvas(name,tabs.back(),400,400));
    tabs.back()->AddFrame(ecanvases.back(), new TGLayoutHints(kLHintsExpandX| kLHintsExpandY,1,1,1,1));
};

void HistViewer::DoTab(Int_t id)
{
   // make new tab's canvas the active one
    cindex = id;
    ecanvases[id]->GetCanvas()->cd(); // note that the vector indexing has to remain 
                                       // consistent with the tab indexing.
    ecanvases[id]->GetCanvas()->cd(1); // to make sure that if we've subdivided the canvas,
                                       // we are on the first pad. this is better than drawing
                                       // over the whole thing
}
HistViewer::~HistViewer() {
    // Clean up used widgets: frames, buttons, layouthints
    fMain->Cleanup();
    delete fMain;
}
HistViewer::HistViewer(const TGWindow *p,UInt_t w,UInt_t h,HistMaker *histmaker)
{
    // Create a main frame
    fMain = new TGMainFrame(p,w,h);

    // comment smm
    hmaker = histmaker;

    // here are the tabs for the embedded canvases
    fTab = new TGTab(fMain, 800, 500);
    fMain->AddFrame(fTab,new TGLayoutHints(kLHintsExpandX| kLHintsExpandY,4,4,4,4));
    fTab->Connect("Selected(Int_t)", "HistViewer", this, "DoTab(Int_t)");
    createtab("demo");
    createtab("zoomed");
    DoTab(0);

    // button hints
    ULong_t bhints = kLHintsExpandX|kLHintsExpandY |kLHintsShrinkX|kLHintsShrinkY | kLHintsFillX|kLHintsFillY;
    TGLayoutHints* mlayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX );


    // buttons for the bottom
    TGHorizontalFrame* quickcmd = new TGHorizontalFrame(fMain,200,40);
    quickcmd->SetLayoutManager(new TGTableLayout(quickcmd, 2, 5));

    TGMenuBar* mptr = new TGMenuBar(quickcmd,1,1,kHorizontalFrame);
    TGPopupMenu* amptr = new TGPopupMenu(gClient->GetRoot());
    amptr->AddEntry("Online",ATTACH_ONLINE);
    amptr->AddEntry("TFile",ATTACH_TFILE);
    amptr->AddEntry("TChain",ATTACH_TCHAIN);
    amptr->AddEntry("Pipe",ATTACH_PIPE);


    amptr->Connect("Activated(Int_t)", "HistViewer", this,"HandleAttachMenu(Int_t)");

    mptr->AddPopup("Attach", amptr,mlayout);
    

    //bptr = new TGTextButton(quickcmd, "Attach");
    quickcmd->AddFrame(mptr, new TGTableLayoutHints(0,1,0,1));

    TGTextButton *bptr = new TGTextButton(quickcmd, "Geometry");
    quickcmd->AddFrame(bptr, new TGTableLayoutHints(1,2,0,1,bhints));

    bptr = new TGTextButton(quickcmd, "Update Selected");
    quickcmd->AddFrame(bptr, new TGTableLayoutHints(2,3,0,1,bhints));

    bptr = new TGTextButton(quickcmd, "Clear Selected");
    quickcmd->AddFrame(bptr, new TGTableLayoutHints(3,4,0,1,bhints));

    bptr = new TGTextButton(quickcmd, "Undock");
    quickcmd->AddFrame(bptr, new TGTableLayoutHints(4,5,0,1,bhints));

    bptr = new TGTextButton(quickcmd, "Start");
    quickcmd->AddFrame(bptr, new TGTableLayoutHints(0,1,1,2,bhints));

    bptr = new TGTextButton(quickcmd, "Hist Display");
    bptr->Connect("Clicked()","HistViewer",this,"DoHistSelect()");
    quickcmd->AddFrame(bptr, new TGTableLayoutHints(1,2,1,2,bhints));

    bptr = new TGTextButton(quickcmd, "Update All");
    quickcmd->AddFrame(bptr, new TGTableLayoutHints(2,3,1,2,bhints));

    bptr = new TGTextButton(quickcmd, "Clear All");
    quickcmd->AddFrame(bptr, new TGTableLayoutHints(3,4,1,2,bhints));

    bptr = new TGTextButton(quickcmd, "New Tab");
    quickcmd->AddFrame(bptr, new TGTableLayoutHints(3,4,1,2,bhints));

    fMain->AddFrame(quickcmd,new TGLayoutHints(kLHintsCenterX,1,1,1,1));

    quickcmd->Resize();

    // Set a name to the main frame
    fMain->SetWindowName("Histogram Viewer");
    // Map all subwindows of main frame
    fMain->MapSubwindows();
    // Initialize the layout algorithm
    //fMain->Resize(fMain->GetDefaultSize());
    fMain->Resize();
    // Map main frame
    fMain->MapWindow();

};
void
HistViewer::HandleHistSelectMenu(TGFrame* f,Int_t btn)
{
       //std::cout << "Processing selection " << id << " " << availablehists[id] << (*(hmaker->h1ds))[availablehists[id-1]] <<  endl;
    //(*(hmaker->h1ds))[availablehists[id-1]]->hist->Draw();
    // HandleContainer slot
    TGTextLBEntry *entry = (TGTextLBEntry *)f;
    Printf("Slot Handling double click (%s, %d)", entry->GetTitle(), btn);
    std::string key = string(entry->GetTitle());
    if (hmaker->h1ds->count(key)>0)
    {
        (*hmaker->h1ds)[key]->hist->Draw(drawoption);
    }
    else if (hmaker->h2ds->count(key)>0)
    {
        (*hmaker->h2ds)[key]->hist->Draw(drawoption);
    }
    else if (hmaker->h3ds->count(key)>0)
    {
        (*hmaker->h3ds)[key]->hist->Draw(drawoption);
    } else
    {
        cout << "failed to find the histogram to draw\n";
    }
    //[entry->GetTitle()]->hist->Draw(drawoption);
    gROOT->GetSelectedPad()->Update();
}
void
HistViewer::DoHistSelect()
{
    std::cout  << "making transient frame for histogram selection\n";
    TGTransientFrame* histselectframe = new TGTransientFrame(gClient->GetRoot(), fMain, 400, 400);
    // now lay out the guts
    TGHorizontalFrame* panel = new TGHorizontalFrame(histselectframe);
    histselectframe->AddFrame(panel, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

    // list window
    TGListBox *fListBox = new TGListBox(panel);
    Int_t i =0;
    std::cout << "i = " << i << endl;
    availablehists.clear();
    for(TH1DMap::iterator it = hmaker->h1ds->begin(); it != hmaker->h1ds->end(); ++it)
    {
        std::string title = it->first;
        std::cout << title << " " << i <<  endl;
        fListBox->AddEntry(title.c_str(),i);
        availablehists.push_back(title); // need something to correlate the menu ID with something real
        i++;
    }
    for(TH2DMap::iterator it = hmaker->h2ds->begin(); it != hmaker->h2ds->end(); ++it)
    {
        std::string title = it->first;
        std::cout << title << " " << i <<  endl;
        fListBox->AddEntry(title.c_str(),i);
        availablehists.push_back(title); // need something to correlate the menu ID with something real
        i++;
    }
    for(TH3DMap::iterator it = hmaker->h3ds->begin(); it != hmaker->h3ds->end(); ++it)
    {
        std::string title = it->first;
        std::cout << title << " " << i <<  endl;
        fListBox->AddEntry(title.c_str(),i);
        availablehists.push_back(title); // need something to correlate the menu ID with something real
        i++;
    }
    std::cout << " length of vector " << availablehists.size() << endl;
    //std::cout << availablehists[0] << endl;
    fListBox->Resize(80,120);
    fListBox->GetContainer()->Connect("DoubleClicked(TGFrame*,Int_t)", "HistViewer", this,"HandleHistSelectMenu(TGFrame*,Int_t)");
    panel->AddFrame(fListBox, new TGLayoutHints(kLHintsTop | kLHintsLeft, 1, 1, 1, 1));
    //panel->Resize();
    // now i need a vertical frame to house the rest of the options
    TGVerticalFrame *sidebar = new TGVerticalFrame(panel);
    panel->AddFrame(sidebar, new TGLayoutHints(kLHintsTop | kLHintsLeft, 1, 1, 1, 1));

    // at the top we have a text entry for draw options
    TGLabel* dlabel = new TGLabel(sidebar,new TGString("Draw Options:"));
    sidebar->AddFrame(dlabel);

    TGTextEntry* toption = new TGTextEntry(sidebar,"",DRAW_OPTION);
    sprintf(drawoption,"");
    toption->Connect("TextChanged(char*)", "HistViewer", this, "DoTextEntry(char*)");
    sidebar->AddFrame(toption, new TGTableLayoutHints(kLHintsTop | kLHintsLeft, 1, 1, 1, 1));
    toption->SetWidth(80);



    // generate the window...
    histselectframe->MapSubwindows();
    histselectframe->Resize();

    // position relative to the parent's window
    histselectframe->CenterOnParent();

    histselectframe->SetWindowName("Hist Select Dialogue");

    histselectframe->MapWindow();
}
