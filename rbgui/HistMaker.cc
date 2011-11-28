#include "HistMaker.h"
#include "TGFileDialog.h"
#include "TCanvas.h"
#include <stdlib.h>
#include <sstream>


/// Simple function to convert strings to basic data types.
template <typename basic>
basic basic_cast (const std::string& str) {
  std::stringstream sstr(str);
  basic out;
  sstr >> out;
  return out;
}

/// Simple function to convert basic data types to a string.
template <typename basic>
std::string string_cast (const basic& data) {
  std::stringstream sstr;
  sstr << data;
  return sstr.str();
}

typedef Double_t (*Atof)(const std::string&);
typedef Int_t    (*Atoi)(const std::string&);



TGLVEntry_mod
::TGLVEntry_mod(const TGLVContainer* p, const TString& name, const TString& cname, TGString** subnames, UInt_t options, Pixel_t back):TGLVEntry(p,name,cname,subnames,options,back)
{
    ;
}

void TGLVEntry_mod::SetSubnamesFromHist(rb::Hist* hst) {
  string arg[14] = { "", "", "", "", "", "", "", "", "", "", "", "", "", "" };
  // type
  // x param
  // x bins
  // x low
  // x high
  // y param
  // y bins
  // y low
  // y high
  // z param
  // z bins
  // z low
  // z high
  // gate
  Int_t naxes = hst->GetNdimensions();
  arg[0] = string_cast<Int_t>(naxes); arg[0] += "D";
  for(Int_t i=0; i< naxes; ++i) {
    TAxis* axis = hst->GetAxis(i);
    if(!axis) break;
    arg[1 + (i*4)] = hst->GetParam(i);
    arg[2 + (i*4)] = string_cast<Int_t>(axis->GetNbins());
    arg[3 + (i*4)] = string_cast<Double_t>(axis->GetBinLowEdge(1));
    arg[4 + (i*4)] = string_cast<Double_t>(axis->GetBinLowEdge(1+axis->GetNbins()));
  }
  arg[13] = hst->GetGate();

  SetSubnames(arg[0].c_str(), arg[1].c_str(), arg[2].c_str(), arg[3].c_str(), arg[4].c_str(), arg[5].c_str(), arg[6].c_str(), arg[7].c_str(), arg[8].c_str(), arg[9].c_str(), arg[10].c_str(), arg[11].c_str(), arg[12].c_str(), arg[13].c_str());
}

  

void TGLVEntry_mod::SetSubnames(const char* n1, const char* n2, const char* n3, const char* n4, const char* n5, const char* n6, const char* n7, const char* n8, const char* n9, const char* n10, const char* n11, const char* n12,const char* n13, const char* n14)
{
       // Sets new subnames.

   if (fSubnames) {
      for (Int_t i = 0; fSubnames[i] != 0; ++i) delete fSubnames[i];
      delete [] fSubnames;
      delete [] fCtw;
   }

   Int_t ncol = 0;
   fSubnames = 0;

   if (n14 && strlen(n14)) ncol=14;
   else if (n13 && strlen(n13)) ncol=13;
   else if (n12 && strlen(n12)) ncol=12;
   else if (n11 && strlen(n11)) ncol=11;
   else if (n10 && strlen(n10)) ncol=10;
   else if (n9 && strlen(n9)) ncol=9;
   else if (n8 && strlen(n8)) ncol=8;
   else if (n7 && strlen(n7)) ncol=7;
   else if (n6 && strlen(n6)) ncol=6;
   else if (n5 && strlen(n5)) ncol=5;
   else if (n4 && strlen(n4)) ncol=4;
   else if (n3 && strlen(n3)) ncol=3;
   else if (n2 && strlen(n2)) ncol=2;
   else if (n1 && strlen(n1)) ncol=1;

   if (!ncol) return;

   fSubnames = new TGString* [ncol+1];

   if (ncol>13) fSubnames[13] = new TGString(n14);
   if (ncol>12) fSubnames[12] = new TGString(n13);
   if (ncol>11) fSubnames[11] = new TGString(n12);
   if (ncol>10) fSubnames[10] = new TGString(n11);
   if (ncol>9) fSubnames[9] = new TGString(n10);
   if (ncol>8) fSubnames[8] = new TGString(n9);
   if (ncol>7) fSubnames[7] = new TGString(n8);
   if (ncol>6) fSubnames[6] = new TGString(n7);
   if (ncol>5) fSubnames[5] = new TGString(n6);
   if (ncol>4) fSubnames[4] = new TGString(n5);
   if (ncol>3) fSubnames[3] = new TGString(n4);
   if (ncol>2) fSubnames[2] = new TGString(n3);
   if (ncol>1) fSubnames[1] = new TGString(n2);
   if (ncol>0) fSubnames[0] = new TGString(n1);
   fSubnames[ncol] = 0;

   fCtw = new int[ncol];
   fCtw[ncol-1] = 0;

   for (int i = 0; i<ncol; i++) {
      fCtw[i] = gVirtualX->TextWidth(fFontStruct, fSubnames[i]->GetString(),
                                     fSubnames[i]->GetLength());
   }
}

void
HistMaker::DoTextEntry(const char* text)
{
  // handle text input
  TGTextEntry *te = (TGTextEntry *) gTQSender;
  Int_t id = te->WidgetId();
  //  cout << id << endl;
  fInfo[id] = text;
} ///< \remark Changed from switch...case to using a std::map
  /// Seems like a cleaner way to do it.

void
HistMaker::DoTypeRadio()
{
    // handle the histogram type radio buttons
    TGRadioButton *rad = (TGRadioButton *) gTQSender;
    htype = rad->WidgetId();
    cout << htype << endl;

}
void
HistMaker::DoFillRadio()
{
    // handle the fill option radio buttons
    TGRadioButton *rad = (TGRadioButton *) gTQSender;
    hfilloption = rad->WidgetId();
    cout << hfilloption << endl;
}
void
HistMaker::DoInvalidCheck()
{
    // handle the display invalid events button
    TGCheckButton *check = (TGCheckButton *) gTQSender;
    invalide = check->IsDown();
    cout << invalide << endl;
}
void
HistMaker::GenerateTreePopups(TTree* t)
{
    // generate the menus... i haven't yet found a successful way of only running though
    // the tree ones. Neither a copy constructor or using Clone() work.
    xmenu = MakePopupFromTree(t,xmenubar,namesx,"X");
    ymenu = MakePopupFromTree(t,ymenubar,namesy,"Y");
    zmenu = MakePopupFromTree(t,zmenubar,namesz,"Z");
    TGLayoutHints* mlayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX );
    xmenubar->RemovePopup("X");
    xmenubar->AddPopup("X",xmenu,mlayout);
    xmenu->Connect("Activated(Int_t)", "HistMaker", this,"HandleTreeMenuX(Int_t)");
    ymenubar->RemovePopup("Y");
    ymenubar->AddPopup("Y",ymenu,mlayout);
    ymenu->Connect("Activated(Int_t)", "HistMaker", this,"HandleTreeMenuY(Int_t)");
    zmenubar->RemovePopup("Z");
    zmenubar->AddPopup("Z",zmenu,mlayout);
    zmenu->Connect("Activated(Int_t)", "HistMaker", this,"HandleTreeMenuZ(Int_t)");
}

void HistMaker::DoTab(Int_t id)
{
   printf("Tab item %d activated\n", id);
}
void
HistMaker::ClearName(std::string hns)
{
    // idea is to make sure that we remove any duplicate keys
    if (h1ds->count(hns)>0)
    {
        // duplicate key .. remove these by default
        h1ds->erase(hns);
    }
    if (h2ds->count(hns)>0)
    {
        // duplicate key .. remove these by default
        h2ds->erase(hns);
    }
    if (h3ds->count(hns)>0)
    {
        // duplicate key .. remove these by default
        h3ds->erase(hns);
    }
}
void
HistMaker::MakeHist(TTree* t,const char* hn,Int_t ht,Int_t foptions,const char* ngate,const char* parx,Int_t bx, Double_t lx, Double_t hx, const char* pary, Int_t by, Double_t ly, Double_t hy,const char* parz, Int_t bz, Double_t lz, Double_t hz, Bool_t invalidevt)
{
    std::cout << "Making histogram\n";
    std::cout << "X parameter: " <<parx <<"\n";
    // point of this is to make a histogram... perhaps a command line interface
    // will be able to talk to this
    // Make the entry for the histogram table to be displayed, named after histogram
    TGLVEntry_mod* entry = new TGLVEntry_mod(cont, hn, hn);
    char hts[1000];
    char bxs[1000],lxs[1000],hxs[1000],bys[1000],lys[1000],hys[1000],bzs[1000],lzs[1000],hzs[1000];
    char bufgate[1000];
    switch (ht) // we do different things depending on which histogram type it is
    {
        case D1:
            {
                sprintf(hts,"1D");
                std::cout << "making  1d histogram\n";
                std::cout << "X parameter: " <<parx <<"\n";
		// let's try this one out for starters... just do a simple unsafe thing
                // which is having all the axes and no gate
                std::cout << "options " << bx << " " << hx << " " << lx << " " << foptions << endl;
		// add gate for valid events
                if (invalidevt)
		{
		    std::cout <<"show invalid" <<endl;
		    sprintf(bufgate,"%s",ngate);
		}
		else
		{
		    std::cout <<"don't show" <<endl;
		    if (ngate=="")
		    {
		    sprintf(bufgate,"%s!=-1",parx);
		    }
		    else
		    {
		    sprintf(bufgate,"%s && %s!=-1",ngate,parx);
		    }
		}
		if (bx>0&&hx-lx>0.&&foptions==FILL_TREE)
                {
                    // build the histogram
                    std::string hnstring = string(hn);
                    ClearName(hnstring);
                    // we chose to draw from tree, so that's what we'll do...
                    char buf[1000];
                    sprintf(buf,"%s>>%s(%i,%f,%f)",parx,hn,bx,lx,hx);
                    std::cout << "drawing " << buf << endl;
                    t->Draw(buf,bufgate,"goff"); // do the drawing
                    // and finally grab the histogram and put it in the dictionary
                    (*h1ds)[hnstring] = new TH1D_SF();
                    (*h1ds)[hnstring]->hist = (TH1D*)gDirectory->FindObject(hn);
                    std::cout << "yay histogram " << (*h1ds)[hnstring] << endl;
                }


                // TGLVEntry subnames must be strings, so we convert
                sprintf(bxs,"%i",bx); sprintf(lxs,"%.3f",lx); sprintf(hxs,"%.3f",hx);
                sprintf(bys,"-"); sprintf(lys,"-"); sprintf(hys,"-");
                sprintf(bzs,"-"); sprintf(lzs,"-"); sprintf(hzs,"-");
                entry->SetSubnames(hts,parx,bxs,lxs,hxs,"-",bys,lys,hys,"-",bzs,lzs,hzs,ngate);
                cont->AddItem(entry);
                histlist->Layout();
            }
            break;
        case D2:
            {
                sprintf(hts,"2D");
                std::cout << "making  2d histogram\n";
                // let's try this one out for starters... just do a simple unsafe thing
                // which is having all the axes and no gate
                std::cout << "options " << bx << " " << hx << " " << lx << " " << foptions << endl;
                std::cout << "options " << by << " " << hy << " " << ly << " " << foptions << endl;
		if (invalidevt)
		{
		    std::cout <<"show invalid" <<endl;
		    sprintf(bufgate,"%s",ngate);
		}
		else
		{
		    std::cout <<"don't show" <<endl;
		    if (ngate="")
		    {
		    sprintf(bufgate,"%s!=-1 && %s!=-1",parx,pary);
		    }
		    else
		    {
		    sprintf(bufgate,"%s && %s!=-1 && %s!=-1",ngate,parx,pary);
		    }
		}
                if (bx>0&&hx-lx>0.&&foptions==FILL_TREE&&by>0&&hy-ly>0.)
                {
                    // build the histogram
                    std::string hnstring = string(hn);
                    ClearName(hnstring);
                    // we chose to draw from tree, so that's what we'll do...
                    char buf[1000];
                    sprintf(buf,"%s:%s>>%s(%i,%f,%f,%i,%f,%f)",pary,parx,hn,bx,lx,hx,by,ly,hy);
                    std::cout << "drawing " << buf << endl;
                    t->Draw(buf,bufgate,"goff"); // do the drawing
                    // and finally grab the histogram and put it in the dictionary
                    (*h2ds)[hnstring] = new TH2D_SF();
                    (*h2ds)[hnstring]->hist = (TH2D*)gDirectory->FindObject(hn);
                    std::cout << "yay histogram " << (*h2ds)[hnstring] << endl;
                }
                // Make the entry for the histogram table to be displayed, named after histogram
                TGLVEntry_mod* entry = new TGLVEntry_mod(cont, hn, hn);

                // TGLVEntry subnames must be strings, so we convert
                sprintf(bxs,"%i",bx); sprintf(lxs,"%.3f",lx); sprintf(hxs,"%.3f",hx);
                sprintf(bys,"%i",by); sprintf(lys,"%.3f",ly); sprintf(hys,"%.3f",hy);
                sprintf(bzs,"-"); sprintf(lzs,"-"); sprintf(hzs,"-");
                entry->SetSubnames(hts,parx,bxs,lxs,hxs,pary,bys,lys,hys,"-",bzs,lzs,hzs,ngate);
                cont->AddItem(entry);
                histlist->Layout();
            }
            break;
        case D3:
            {
                sprintf(hts,"3D");
                std::cout << "making  3d histogram\n";
                // let's try this one out for starters... just do a simple unsafe thing
                // which is having all the axes and no gate
                std::cout << "options " << bx << " " << hx << " " << lx << " " << foptions << endl;
		if (invalidevt)
		{
		    std::cout <<"show invalid" <<endl;
		    sprintf(bufgate,"%s",ngate);
		}
		else
		{
		    std::cout <<"don't show" <<endl;
		    if (ngate="")
		    {
		    sprintf(bufgate,"%s!=-1 && %s!=-1 && %s!=-1",parx,pary,parz);
		    }
		    else
		    {
		    sprintf(bufgate,"%s && %s!=-1 && %s!=-1 && %s!=-1",ngate,parx,pary,parz);
		    }
		}
                if (bx>0&&hx-lx>0.&&foptions==FILL_TREE&&by>0&&hy-ly>0.&&bz>0&&hz-lz>0.)
                {
                    // build the histogram
                    std::string hnstring = string(hn);
                    ClearName(hnstring);
                    // we chose to draw from tree, so that's what we'll do...
                    char buf[1000];
                    sprintf(buf,"%s:%s:%s>>%s(%i,%f,%f,%i,%f,%f)",parz,parx,pary,hn,bx,lx,hx,by,ly,hy,bz,lz,hz);
                    std::cout << "drawing " << buf << endl;
                    t->Draw(buf,bufgate,"goff"); // do the drawing
                    // and finally grab the histogram and put it in the dictionary
                    (*h3ds)[hnstring] = new TH3D_SF();
                    (*h3ds)[hnstring]->hist = (TH3D*)gDirectory->FindObject(hn);
                    std::cout << "yay histogram " << (*h3ds)[hnstring] << endl;
                }
                // Make the entry for the histogram table to be displayed, named after histogram
                TGLVEntry_mod* entry = new TGLVEntry_mod(cont, hn, hn);

                // TGLVEntry subnames must be strings, so we convert
                sprintf(bxs,"%i",bx); sprintf(lxs,"%.3f",lx); sprintf(hxs,"%.3f",hx);
                sprintf(bys,"%i",by); sprintf(lys,"%.3f",ly); sprintf(hys,"%.3f",hy);
                sprintf(bzs,"%i",bz); sprintf(lzs,"%.3f",lz); sprintf(hzs,"%.3f",hz);
                entry->SetSubnames(hts,parx,bxs,lxs,hxs,pary,bys,lys,hys,parz,bzs,lzs,hzs,ngate);
                cont->AddItem(entry);
                histlist->Layout();
            }
            break;
        case SUMMARY:
            {
            }
                sprintf(hts,"Summary");
            break;
        case SUMMATION:
            {
                sprintf(hts,"Summation");
            }
            break;
        case BITMASK:
            {
                sprintf(hts,"Bitmask");
            }
            break;
        default:
                // clearly the histogram type wasn't specified properly
            break;
    }
    
    //entry->Activate(1);
}

/// Check if all fields are present for a given axis.
Int_t processAxis(Int_t param, Int_t bins, Int_t low, Int_t high, string& gparam, map<Int_t, string>& _map) {
  Int_t allFields[4] = {param, bins, low, high}, nFilled(0);
  for(Int_t i=0; i< 4; ++i) {
    if(_map[allFields[i]] != "") ++nFilled;
  }
  Int_t ret;
  switch(nFilled) {
  case 0: ret = 0;
    break;
  case 4: ret = 1;
    gparam += _map[param];
    gparam += ":";
    break;
  default: ret = -100;
    break;
  }
  return ret;
}

void
HistMaker::MakeHistFromGui()
{
  if(fInfo[NAME] == "") {
    Error("MakeHistFromGui", "Please specify a name");
    return;
  }

  //  Figure out no. of dimensions
  string par("");
  Int_t nAxes(0);
  nAxes += processAxis(PARAM_Z, BINS_Z, LOW_Z, HIGH_Z, par, fInfo);
  nAxes += processAxis(PARAM_Y, BINS_Y, LOW_Y, HIGH_Y, par, fInfo);
  nAxes += processAxis(PARAM_X, BINS_X, LOW_X, HIGH_X, par, fInfo);
  if(nAxes < 0) {
    Error("MakeHistFromGui", "Incomplete axis specification.");
    return;
  }
  string whatParShouldBe, params[3] = {fInfo[PARAM_X], fInfo[PARAM_Y], fInfo[PARAM_Z]};
  for(Int_t i = nAxes - 1; i >= 0; --i) {
    whatParShouldBe += params[i];
    whatParShouldBe += ":";
  }
  if(whatParShouldBe != par) {
    Error("MakeHistFromGui", "Invalid axis combination.");
    return;
  }
  par.erase(par.size() - 1);


  static Atoi a2i = basic_cast<Int_t>;
  static Atof a2d = basic_cast<Double_t>;
  TGLVEntry_mod* entry = 0;

  switch(nAxes) {
  case 1: {
    rb::H1D* hst = new rb::H1D (fInfo[NAME].c_str(), "",
				a2i(fInfo[BINS_X]), a2d(fInfo[LOW_X]), a2d(fInfo[HIGH_X]),
				par.c_str(), fInfo[GATE].c_str());

    entry = new TGLVEntry_mod(cont, static_cast<TH1*>(hst)->GetName(), static_cast<TH1*>(hst)->GetName());
    entry->SetSubnamesFromHist(hst);
    cont->AddItem(entry);
    histlist->Layout();

    break;
  }
  case 2: rb::Hist::New(fInfo[NAME].c_str(), "",
			a2i(fInfo[BINS_X]), a2d(fInfo[LOW_X]), a2d(fInfo[HIGH_X]),
			a2i(fInfo[BINS_Y]), a2d(fInfo[LOW_Y]), a2d(fInfo[HIGH_Y]),
			par.c_str(), fInfo[GATE].c_str()); break;
  case 3: rb::Hist::New(fInfo[NAME].c_str(), "",
			a2i(fInfo[BINS_X]), a2d(fInfo[LOW_X]), a2d(fInfo[HIGH_X]),
			a2i(fInfo[BINS_Y]), a2d(fInfo[LOW_Y]), a2d(fInfo[HIGH_Y]),
			a2i(fInfo[BINS_Z]), a2d(fInfo[LOW_Z]), a2d(fInfo[HIGH_Z]),
			par.c_str(), fInfo[GATE].c_str()); break;

  default: Error("MakeHistFromGui", "nAxes %d isn't valid", nAxes);
  }
}


HistMaker::HistMaker(const TGWindow *p,UInt_t w,UInt_t h) {
    // Initialize fInfo
  /// \warning This relies on the orderig of elements in CommandIdentifiers
  for(Int_t i = PARAM_X; i < GATE; ++i) fInfo[i] = "";


    // Create a main frame
    fMain = new TGMainFrame(p,w,h);
    // Create a horizontal frame widget with buttons
    TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain,800,40);
    TGTextButton *exit = new TGTextButton(hframe,"&Exit","gApplication->Terminate(0)");
    hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
    fMain->AddFrame(hframe, new TGLayoutHints(kLHintsBottom | kLHintsRight,2,2,2,2));

    // Create tabs for Histograms, Variables, and Gates
    // Thing that contains all the tabs
    TGTab* fTab = new TGTab(fMain, 800, 500);
    // function just to let me know I'm clicking ... probably pull it later
    fTab->Connect("Selected(Int_t)", "HistMaker", this, "DoTab(Int_t)");

    // now for the Histograms tab
    TGCompositeFrame* tf = fTab->AddTab("Histograms");


    //////////////////////////////////////////
    // BEGIN HISTOGRAM TYPE SELECTION BLOCK //
    //////////////////////////////////////////
    tf->SetLayoutManager(new TGVerticalLayout(tf)); // our tab is layed out into 3 vertical chunks

    // need a horizontal frame for the top level of the gui
    top = new TGHorizontalFrame(tf,800,400);
    tf->AddFrame(top,new TGLayoutHints(kLHintsExpandX,4,4,4,4));

    // group frame to contain the histogram options
    TGButtonGroup* histoptions = new TGButtonGroup(top,"Histogram Type");
    histoptions->SetTitlePos(TGGroupFrame::kCenter);
    histoptions->SetLayoutManager(new TGMatrixLayout(histoptions, 0, 2, 3));
    TGRadioButton* histobuttons[6];
    // order of options set because of how the matrix layout indexing works
    histobuttons[0] = new TGRadioButton(histoptions,new TGHotString("1D"),D1);
    histobuttons[0]->Connect("Clicked()","HistMaker",this,"DoTypeRadio()");
    histobuttons[3] = new TGRadioButton(histoptions,new TGHotString("Summary"),SUMMARY);
    histobuttons[3]->Connect("Clicked()","HistMaker",this,"DoTypeRadio()");
    histobuttons[1] = new TGRadioButton(histoptions,new TGHotString("2D"),D2);
    histobuttons[1]->Connect("Clicked()","HistMaker",this,"DoTypeRadio()");
    histobuttons[4] = new TGRadioButton(histoptions,new TGHotString("Summation"),SUMMATION);
    histobuttons[4]->Connect("Clicked()","HistMaker",this,"DoTypeRadio()");
    histobuttons[2] = new TGRadioButton(histoptions,new TGHotString("3D"),D3);
    histobuttons[2]->Connect("Clicked()","HistMaker",this,"DoTypeRadio()");
    histobuttons[5] = new TGRadioButton(histoptions,new TGHotString("Bitmask"),BITMASK);
    histobuttons[5]->Connect("Clicked()","HistMaker",this,"DoTypeRadio()");

    fL3 = new TGLayoutHints(kLHintsExpandY, 2, 2, 2, 2);
    top->AddFrame(histoptions,fL3);
    ////////////////////////////////////////
    // END HISTOGRAM TYPE SELECTION BLOCK //
    ////////////////////////////////////////

    ////////////////////////////////////////
    // BEGIN DRAW OPTIONS SELECTION BLOCK //
    ////////////////////////////////////////
    TGButtonGroup* drawoptions = new TGButtonGroup(top,"Fill Options");
    drawoptions->SetTitlePos(TGGroupFrame::kCenter);
    drawoptions->SetLayoutManager(new TGMatrixLayout(drawoptions, 0, 1, 3));
    TGRadioButton* drawbuttons[3];
    // want to be able to fill from the current tree, straight from online data,
    // or fill from the current tree and then keep adding online data

    drawbuttons[0] = new TGRadioButton(drawoptions,new TGHotString("Tree"),FILL_TREE);
    drawbuttons[0]->Connect("Clicked()","HistMaker",this,"DoFillRadio()");
    drawbuttons[1] = new TGRadioButton(drawoptions,new TGHotString("Online"),FILL_ONLINE);
    drawbuttons[1]->Connect("Clicked()","HistMaker",this,"DoFillRadio()");
    drawbuttons[2] = new TGRadioButton(drawoptions,new TGHotString("Tree+Online"),FILL_TREE_ONLINE);
    drawbuttons[2]->Connect("Clicked()","HistMaker",this,"DoFillRadio()");

    top->AddFrame(drawoptions,fL3);
    //////////////////////////////////////
    // END DRAW OPTIONS SELECTION BLOCK //
    //////////////////////////////////////

    /////////////////////////////////
    // BEGIN DEFINITION FILE BLOCK //
    /////////////////////////////////
    deffile = new TGGroupFrame((TGWindow*)top, "Definitions File");
    deffile ->SetTitlePos(TGGroupFrame::kCenter);
    deffile->SetLayoutManager(new TGMatrixLayout(deffile, 0, 1, 3));

    TGLabel* filename = new TGLabel(deffile,new TGString("----"));
    deffile->AddFrame(filename,new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    filename->Resize(140,filename->GetDefaultHeight());

    fbuttons = new TGHorizontalFrame(deffile,200,filename->GetDefaultHeight());
    //TGHorizontalFrame* fbuttons = new TGHorizontalFrame(deffile,200,filename->GetDefaultHeight());

    loadFile = new TGTextButton((TGWindow*)fbuttons, "Load");
    fbuttons->AddFrame(loadFile, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    loadFile->Resize(80,loadFile->GetDefaultHeight());

    saveFile = new TGTextButton((TGWindow*)fbuttons, "Save");
    fbuttons->AddFrame(saveFile, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
    saveFile->Resize(80,loadFile->GetDefaultHeight());

    fbuttons->Resize(140,loadFile->GetDefaultHeight()+4);

    deffile->AddFrame(fbuttons);

    fcbuttons = new TGHorizontalFrame(deffile,140,saveFile->GetDefaultHeight());

    TGCheckButton *cumulate = new TGCheckButton(fcbuttons, "Cumulate");
    //cumulate->Resize(5,cumulate->GetDefaultHeight());
    fcbuttons->AddFrame(cumulate);

    TGCheckButton *failsafe = new TGCheckButton(fcbuttons, "Failsafe");
    //failsafe->Resize(200,cumulate->GetDefaultHeight());
    fcbuttons->AddFrame(failsafe);

    deffile->AddFrame(fcbuttons);


    top->AddFrame(deffile, fL3);
    ///////////////////////////////
    // END DEFINITION FILE BLOCK //
    ///////////////////////////////


    // ADDED to top by SMM on 12/12/10 as a layout test
    TGHorizontalFrame* middle = new TGHorizontalFrame(top);
    top->AddFrame(middle, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

    ////////////////////////////
    // BEGIN AXIS SETUP BLOCK //
    ////////////////////////////
    ULong_t bhints = kLHintsExpandX|kLHintsExpandY |kLHintsShrinkX|kLHintsShrinkY | kLHintsFillX|kLHintsFillY;
    TGHorizontalFrame* axes = new TGHorizontalFrame(middle);
//    axes->SetLayoutManager(new TGTableLayout(axes, 4, 5)); - JKS, 7 June 2011
    axes->SetLayoutManager(new TGTableLayout(axes, 5, 5));

    // placeholders for the drop down menus for parameters
    //xlist = new TGTextButton(axes, "x");
    //axes->AddFrame(xlist, new TGTableLayoutHints(0,1,1,2,bhints));

    TGLayoutHints* mlayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX );
	xmenubar = new TGMenuBar(axes,1,1,kHorizontalFrame);
	xmenu = new TGPopupMenu(gClient->GetRoot());
    xmenubar->AddPopup("X", xmenu,mlayout);
    axes->AddFrame(xmenubar, new TGTableLayoutHints(0,1,1,2));

	ymenubar = new TGMenuBar(axes,1,1,kHorizontalFrame);
	ymenu = new TGPopupMenu(gClient->GetRoot());
    ymenubar->AddPopup("Y", ymenu,mlayout);
    axes->AddFrame(ymenubar, new TGTableLayoutHints(0,1,2,3));

	zmenubar = new TGMenuBar(axes,1,1,kHorizontalFrame);
	zmenu = new TGPopupMenu(gClient->GetRoot());
    zmenubar->AddPopup("Z", zmenu,mlayout);
    axes->AddFrame(zmenubar, new TGTableLayoutHints(0,1,3,4));

    /*
    TGTextButton* ylist = new TGTextButton(axes, "y");
    axes->AddFrame(ylist,new TGTableLayoutHints(0,1,2,3,bhints));
    ylist->Resize(40,ylist->GetDefaultHeight());

    TGTextButton* zlist = new TGTextButton(axes, "z");
    axes->AddFrame(zlist,new TGTableLayoutHints(0,1,3,4,bhints));
    zlist->Resize(40,zlist->GetDefaultHeight());
    */

    // the text entry field for the parameters
    TGLabel* plabel = new TGLabel(axes,new TGString("Parameter"));
    //plabel->Resize(150,zmenu->GetDefaultHeight());
    plabel->SetWidth(150);
    axes->AddFrame(plabel, new TGTableLayoutHints(1,2,0,1));

    xptext = new TGTextEntry(axes,"",PARAM_X);
    xptext->SetWidth(150);
    xptext->Connect("TextChanged(char*)", "HistMaker", this, "DoTextEntry(char*)");
    //xptext->Resize(150,zmenu->GetDefaultHeight());
    axes->AddFrame(xptext, new TGTableLayoutHints(1,2,1,2));

    yptext = new TGTextEntry(axes,"",PARAM_Y);
    yptext->SetWidth(150);
    yptext->Connect("TextChanged(char*)", "HistMaker", this, "DoTextEntry(char*)");
    //yptext->Resize(150,ymenu->GetDefaultHeight());
    axes->AddFrame(yptext, new TGTableLayoutHints(1,2,2,3));

    zptext = new TGTextEntry(axes,"",PARAM_Z);
    zptext->SetWidth(150);
    zptext->Connect("TextChanged(char*)", "HistMaker", this, "DoTextEntry(char*)");
    //zptext->Resize(150,zmenu->GetDefaultHeight());
    axes->AddFrame(zptext, new TGTableLayoutHints(1,2,3,4));

    // text entry for bins column
    TGLabel* binlabel = new TGLabel(axes,new TGString("bins"));
    //binlabel->Resize(40,zmenu->GetDefaultHeight());
    binlabel->SetWidth(40);
    axes->AddFrame(binlabel, new TGTableLayoutHints(2,3,0,1));

    binxtext = new TGTextEntry(axes,"",BINS_X);
    binxtext->SetWidth(40);
    binxtext->Connect("TextChanged(char*)", "HistMaker", this, "DoTextEntry(char*)");
    //binxtext->Resize(40,zmenu->GetDefaultHeight());
    axes->AddFrame(binxtext, new TGTableLayoutHints(2,3,1,2));

    binytext = new TGTextEntry(axes,"",BINS_Y);
    binytext->SetWidth(40);
    binytext->Connect("TextChanged(char*)", "HistMaker", this, "DoTextEntry(char*)");
    //binytext->Resize(40,ymenu->GetDefaultHeight());
    axes->AddFrame(binytext, new TGTableLayoutHints(2,3,2,3));

    binztext = new TGTextEntry(axes,"",BINS_Z);
    binztext->SetWidth(40);
    binztext->Connect("TextChanged(char*)", "HistMaker", this, "DoTextEntry(char*)");
    //binztext->Resize(40,zmenu->GetDefaultHeight());
    axes->AddFrame(binztext, new TGTableLayoutHints(2,3,3,4));

    // text entry for low column
    TGLabel* lolabel = new TGLabel(axes,new TGString("low"));
    lolabel->SetWidth(40);
    //lolabel->Resize(40,zmenu->GetDefaultHeight());
    axes->AddFrame(lolabel, new TGTableLayoutHints(3,4,0,1));

    loxtext = new TGTextEntry(axes,"",LOW_X);
    loxtext->SetWidth(40);
    loxtext->Connect("TextChanged(char*)", "HistMaker", this, "DoTextEntry(char*)");
    //loxtext->Resize(40,zmenu->GetDefaultHeight());
    axes->AddFrame(loxtext, new TGTableLayoutHints(3,4,1,2));

    loytext = new TGTextEntry(axes,"",LOW_Y);
    loytext->SetWidth(40);
    loytext->Connect("TextChanged(char*)", "HistMaker", this, "DoTextEntry(char*)");
    //loytext->Resize(40,ymenu->GetDefaultHeight());
    axes->AddFrame(loytext, new TGTableLayoutHints(3,4,2,3));

    loztext = new TGTextEntry(axes,"",LOW_Z);
    loztext->SetWidth(40);
    loztext->Connect("TextChanged(char*)", "HistMaker", this, "DoTextEntry(char*)");
    //loztext->Resize(40,zmenu->GetDefaultHeight());
    axes->AddFrame(loztext, new TGTableLayoutHints(3,4,3,4));

    // text entry for high column
    TGLabel* hilabel = new TGLabel(axes,new TGString("high"));
    hilabel->SetWidth(40);
    //hilabel->Resize(40,zmenu->GetDefaultHeight());
    axes->AddFrame(hilabel, new TGTableLayoutHints(4,5,0,1));

    hixtext = new TGTextEntry(axes,"",HIGH_X);
    //hixtext->Resize(40,zmenu->GetDefaultHeight());
    hixtext->SetWidth(40);
    hixtext->Connect("TextChanged(char*)", "HistMaker", this, "DoTextEntry(char*)");
    axes->AddFrame(hixtext, new TGTableLayoutHints(4,5,1,2));

    hiytext = new TGTextEntry(axes,"",HIGH_Y);
    hiytext->SetWidth(40);
    hiytext->Connect("TextChanged(char*)", "HistMaker", this, "DoTextEntry(char*)");
    //hiytext->Resize(40,ymenu->GetDefaultHeight());
    axes->AddFrame(hiytext, new TGTableLayoutHints(4,5,2,3));

    hiztext = new TGTextEntry(axes,"",HIGH_Z);
    hiztext->SetWidth(40);
    hiztext->Connect("TextChanged(char*)", "HistMaker", this, "DoTextEntry(char*)");
    //hiztext->Resize(40,zmenu->GetDefaultHeight());
    axes->AddFrame(hiztext, new TGTableLayoutHints(4,5,3,4));

    // Display invalid events button
    TGCheckButton *invalid = new TGCheckButton(axes, new TGHotString("Display invalid events"));
    invalid->Connect("Toggled(Bool_t)","HistMaker",this,"DoInvalidCheck()");
    axes->AddFrame(invalid, new TGTableLayoutHints(1,2,4,5));

    // add this to the gui
    middle->AddFrame(axes);

    fL4 = new TGLayoutHints(kLHintsLeft, 5, 5, 5, 5);
    fL5 = new TGLayoutHints(kLHintsTop, 5, 5, 5, 5);

    //////////////////////////
    // END AXIS SETUP BLOCK //
    //////////////////////////

    //////////////////////////////////////
    // BEGIN HISTOGRAM OPERATIONS BLOCK //
    //////////////////////////////////////
    ULong_t bhints2 = kLHintsExpandX|kLHintsShrinkX|kLHintsFillX;
    TGCompositeFrame* ops = new TGCompositeFrame(middle);

    ops->SetLayoutManager(new TGTableLayout(ops, 4, 3));

    TGLabel* histname = new TGLabel(ops,new TGString("Name:"));
    //histname->Resize(40,xlist->GetDefaultHeight());
    //histname->Resize(40,zmenu->GetDefaultHeight());
    ops->AddFrame(histname, new TGTableLayoutHints(0,1,0,1));

    histnametext = new TGTextEntry(ops,"",NAME);
    histnametext->Connect("TextChanged(char*)", "HistMaker", this, "DoTextEntry(char*)");
    ops->AddFrame(histnametext, new TGTableLayoutHints(1,3,0,1));
    histnametext->SetWidth(140);
    //histnametext->Resize(140,zmenu->GetDefaultHeight()-4);
    //histnametext->Resize(60,histname->GetDefaultHeight());

    create = new TGTextButton(ops, "Create/Replace");
    ops->AddFrame(create, new TGTableLayoutHints(0,1,1,2,bhints2));
    create->Connect("Clicked()","HistMaker",this,"MakeHistFromGui()");
    //create->Resize(60,zmenu->GetDefaultHeight());
    //create->Resize(60,create->GetDefaultHeight());

    TGTextButton* bdel = new TGTextButton(ops, "Delete");
    ops->AddFrame(bdel, new TGTableLayoutHints(2,3,1,2,bhints));
    //bdel->Resize(60,zmenu->GetDefaultHeight());
    //bdel->Resize(60,bdel->GetDefaultHeight());

    // placeholder button untiil i get the dropdown menu working
    TGTextButton* bgate = new TGTextButton(ops, "Gate");
    ops->AddFrame(bgate, new TGTableLayoutHints(1,2,2,3,bhints));
    //bgate->Resize(60,zmenu->GetDefaultHeight());
    //bgate->Resize(60,bgate->GetDefaultHeight());

    TGTextButton* bapply = new TGTextButton(ops, "Apply");
    ops->AddFrame(bapply, new TGTableLayoutHints(2,3,2,3,bhints));
    //bapply->Resize(60,zmenu->GetDefaultHeight());
    //bapply->Resize(60,bapply->GetDefaultHeight());

    TGTextButton* bclear = new TGTextButton(ops, "Clear");
    ops->AddFrame(bclear, new TGTableLayoutHints(0,1,2,3,bhints2));
    //bclear->Resize(60,zmenu->GetDefaultHeight());
    //bclear->Resize(60,bclear->GetDefaultHeight());

    TGTextButton* bduplicate = new TGTextButton(ops, "Duplicate");
    ops->AddFrame(bduplicate, new TGTableLayoutHints(1,2,1,2,bhints));
    //bduplicate->Resize(60,zmenu->GetDefaultHeight());
    //bduplicate->Resize(60,bduplicate->GetDefaultHeight());

    tgate = new TGTextEntry(ops,"",GATE);
    tgate->Connect("TextChanged(char*)", "HistMaker", this, "DoTextEntry(char*)");
    ops->AddFrame(tgate, new TGTableLayoutHints(1,2,3,4,bhints));
    tgate->SetWidth(60);
    //tgate->Resize(60,zmenu->GetDefaultHeight());
    //tgate->Resize(60,bduplicate->GetDefaultHeight());

    TGTextButton* bungate = new TGTextButton(ops, "Ungate");
    ops->AddFrame(bungate, new TGTableLayoutHints(2,3,3,4,bhints));
    //bungate->Resize(60,zmenu->GetDefaultHeight());

    TGCheckButton* clearall = new TGCheckButton(ops, "All");
    ops->AddFrame(clearall, new TGTableLayoutHints(0,1,3,4,bhints2));

    middle->AddFrame(ops);
    middle->Resize();
    ////////////////////////////////////
    // END HISTOGRAM OPERATIONS BLOCK //
    ////////////////////////////////////

    ////////////////////////////////
    // BEGIN HISTOGRAM LIST BLOCK //
    ////////////////////////////////

    //////////////////////
    // Begin TGListView histogram table test
    /////////////////////
    histlist = new TGListView(tf, 920, 500, kSunkenFrame);

    TString* heads[15];
    heads[0] = new TString("Name");
    heads[1] = new TString("Type");
    heads[2] = new TString("X Parameter");
    heads[3] = new TString("Bins");
    heads[4] = new TString("Lo Lim");
    heads[5] = new TString("Hi Lim");
    heads[6] = new TString("Y Parameter");
    heads[7] = new TString("Bins");
    heads[8] = new TString("Lo Lim");
    heads[9] = new TString("Hi Lim");
    heads[10] = new TString("Z Parameter");
    heads[11] = new TString("Bins");
    heads[12] = new TString("Lo Lim");
    heads[13] = new TString("Hi Lim");
    heads[14] = new TString("Gate");

    cont = new TGLVContainer(histlist, kSunkenFrame,(Pixel_t)0xFFFFFFFF);
    cont->SetHeaders(15);
    for (int eye=0;eye<15;++eye)
    {
        cont->SetHeader(heads[eye]->Data(), kTextCenterX, kTextLeft, eye);
    }
    //histlist->SetContainer(cont);
    cont->SetViewMode(kLVDetails);
    /*
    TString test("Test");
    TGLVEntry* entry = new TGLVEntry(cont, test, test);
    entry->SetSubnames("bloop","bloop","bloop","bloop","bloop","bloop","bloop","bloop","bloop","bloop","bloop","bloop");
    cont->AddItem(entry);
    */



    tf->AddFrame(histlist);
    //////////////////////
    // End TGListView histogram table test
    /////////////////////

    /////////////////////
    // Manual Crappy way to get table
    ////////////////////
    /*
    TGPack* histlist = new TGPack(tf); // need another horizontal frame for the bottom
    histlist->SetVertical(0);
    tf->AddFrame(histlist, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX,2,2,2,2));
    // now set up the frames to be added.
    TGVerticalFrame* histframes[15];
    histframes[0] = new TGVerticalFrame(histlist); // name
    histframes[1] = new TGVerticalFrame(histlist); // type
    histframes[2] = new TGVerticalFrame(histlist); // x parameter
    histframes[3] = new TGVerticalFrame(histlist); // x bins
    histframes[4] = new TGVerticalFrame(histlist); // x lo limit
    histframes[5] = new TGVerticalFrame(histlist); // x hi limit
    histframes[6] = new TGVerticalFrame(histlist); // y parameter
    histframes[7] = new TGVerticalFrame(histlist); // y bins
    histframes[8] = new TGVerticalFrame(histlist); // y lo limit
    histframes[9] = new TGVerticalFrame(histlist); // y hi limit
    histframes[10] = new TGVerticalFrame(histlist); // z parameter
    histframes[11] = new TGVerticalFrame(histlist); // z bins
    histframes[12] = new TGVerticalFrame(histlist); // z lo limit
    histframes[13] = new TGVerticalFrame(histlist); // z hi limit
    histframes[14] = new TGVerticalFrame(histlist); // gate

    // add labels
    TGLabel* histframelabels[15];

    //histframelabels[0] = new TGLabel(histframes[0],new TGString("Name"), TGLabel::GetDefaultGC()(), TGLabel::GetDefaultFontStruct(), kChildFrame | kRaisedFrame,TGLabel::GetDefaultFrameBackground());
    histframelabels[0] = new TGLabel(histframes[0],new TGString("Name"));
    histframelabels[1] = new TGLabel(histframes[1],new TGString("Type"));
    histframelabels[2] = new TGLabel(histframes[2],new TGString("X Parameter"));
    histframelabels[3] = new TGLabel(histframes[3],new TGString("Bins"));
    histframelabels[4] = new TGLabel(histframes[4],new TGString("Lo"));
    histframelabels[5] = new TGLabel(histframes[5],new TGString("Hi"));
    histframelabels[6] = new TGLabel(histframes[6],new TGString("Y Parameter"));
    histframelabels[7] = new TGLabel(histframes[7],new TGString("Bins"));
    histframelabels[8] = new TGLabel(histframes[8],new TGString("Lo"));
    histframelabels[9] = new TGLabel(histframes[9],new TGString("Hi"));
    histframelabels[10] = new TGLabel(histframes[10],new TGString("Z Parameter"));
    histframelabels[11] = new TGLabel(histframes[11],new TGString("Bins"));
    histframelabels[12] = new TGLabel(histframes[12],new TGString("Lo"));
    histframelabels[13] = new TGLabel(histframes[13],new TGString("Hi"));
    histframelabels[14] = new TGLabel(histframes[14],new TGString("Gate"));

    Float_t weights[15];
    weights[0]=1.3;
    weights[1]=0.5;
    weights[2]=2.;
    weights[3]=0.5;
    weights[4]=0.5;
    weights[5]=0.5;
    weights[6]=2.;
    weights[7]=0.5;
    weights[8]=0.5;
    weights[9]=0.5;
    weights[10]=0.2;
    weights[11]=0.3;
    weights[12]=0.3;
    weights[13]=0.3;
    weights[14]=1.;

    for (int i =0;i<15;++i)
    {
        histframelabels[i]->SetTextJustify(kTextLeft); // make sure all the label text is left justified
        histframelabels[i]->ChangeOptions(kChildFrame | kRaisedFrame); // raised frame
        //histframes[i]->Set3DStyle(kRaisedFrame);
        //histframelabels[i]->	SetBackgroundColor();
        histframes[i]->AddFrame(histframelabels[i], new TGLayoutHints(kLHintsExpandX,2,2,2,2));
        histlist->AddFrameWithWeight(histframes[i], new TGLayoutHints(kLHintsExpandX,1,1,1,1),weights[i]);
    }
    */
    /////////////////////
    // End Manual Crappy way to get table
    ////////////////////




    /*
    histframelabels[0] = new TGLabel(ops,new TGString("Name"));
    histframes[0]->AddFrame(histframelabels[0]);
    */

    //histlist->Resize();


    //////////////////////////////
    // END HISTOGRAM LIST BLOCK //
    //////////////////////////////

    // now for the Variables tab
    tf = fTab->AddTab("Variables");

    // now for the Gates tab
    tf = fTab->AddTab("Gates");

    // adding the tabs to the main frame
    TGLayoutHints *fL5 = new TGLayoutHints(kLHintsBottom | kLHintsExpandX | kLHintsExpandY, 2, 2, 5, 1);
    fMain->AddFrame(fTab, fL5);


    // Set a name to the main frame
    fMain->SetWindowName("Histogram Builder");
    // Map all subwindows of main frame
    fMain->MapSubwindows();
    // Initialize the layout algorithm
    //fMain->Resize(fMain->GetDefaultSize());
    fMain->Resize();
    // Map main frame
    fMain->MapWindow();
    histlist->ResizeColumns();

    // Set up the viewing window
    fView = new HistViewer(gClient->GetRoot(),800,300,this);
    namesx = new std::vector<string>;
    namesy = new std::vector<string>;
    namesz = new std::vector<string>;

    h1ds = new TH1DMap();  // 1d histograms
    h2ds = new TH2DMap();  // 2d histograms
    h3ds = new TH3DMap();  // 3d histograms
    cout << h1ds->size() << endl;
    cout << h2ds->size() << endl;
    cout << h3ds->size() << endl;
}

void 
HistMaker::MakeParamDropDown(TTree* t)
{
}
HistMaker::~HistMaker() {
    // Clean up used widgets: frames, buttons, layouthints
    fMain->Cleanup();
    delete fMain;
}

std::string 
HistMaker::ProcessFinalName(std::string* currentname,std::string flevelname)
{
    // idea is to check for the existence of the first part of flevelname in currentname
    // e.g. flevelname = "crdc1.x"
    //     currentname = "cal::sweeper.crdc1."
    std::string possible = "";
    Int_t pos = flevelname.find(".");
    if (pos !=-1)
    {
        // make the possible substring ... e.g. "crdc1"
        possible = flevelname.substr(0,pos);
        // now search for it in currentname
        if (currentname->find(possible) !=-1)
        {
            // if we found it, we want to not double count...
            return *currentname + flevelname.substr(pos+1,flevelname.length());
        }

    }
    return *currentname + flevelname;
}

Int_t
HistMaker::IsArray(std::string branchname,Int_t* index1,Int_t* index2)
{
    // idea is to determine if a branch is an array
    // in the branch structure, a bracket will indicate an array
    Int_t lastopen = branchname.rfind("[");
    Int_t lastclose = branchname.rfind("]");
    if (lastopen!=-1 && lastclose!=-1)
    {
        // we clearly have an array
        *index2 = atoi(branchname.substr(lastopen+1,lastclose-lastopen-1).c_str());
        std::string subbranch = branchname.substr(0,lastopen);
        std::cout << subbranch << endl;
        lastopen = subbranch.rfind("[");
        lastclose = subbranch.rfind("]");
        std::cout << "indiced " << *index2 << endl;
        if (lastopen!=-1&&lastclose!=-1)
        {
            *index1 = atoi(subbranch.substr(lastopen+1,lastclose-lastopen-1).c_str());
            std::cout << "indiced "<<*index1 << " "  << *index2 << endl;

            return 2;
        } else
        {
            *index1 = *index2;
            *index2 = 0;
        }
        return 1;
    }
    return 0;
}
void
HistMaker::HandleTreeMenuX(Int_t id)
{
    std::cout << "Handling X tree id " << id << endl;
    std::cout << "looking at branch " << (*namesx)[id] << endl;
    xptext->SetText((*namesx)[id].c_str());
}
void
HistMaker::HandleTreeMenuY(Int_t id)
{
    std::cout << "Handling Y tree id " << id << endl;
    std::cout << "looking at branch " << (*namesy)[id] << endl;
    yptext->SetText((*namesy)[id].c_str());
}
void
HistMaker::HandleTreeMenuZ(Int_t id)
{
    std::cout << "Handling Z tree id " << id << endl;
    std::cout << "looking at branch " << (*namesz)[id] << endl;
    zptext->SetText((*namesz)[id].c_str());
}
void
HistMaker::MakePopupFromBranch(TBranchObject* branch,TGPopupMenu* menu,std::vector<string> *names, Int_t offset,std::string* currentname)
{
    //std::cout << "inside makepopupfrombranch\n";
    // NOTE:
    // The TGPopupMenu is one logical level behind the TBranchObject, and we will look one logical level ahead!

    // want to list the branches inside of this branch
    //cout << "getting branch list\n";
    TObjArray* branchlist = branch->GetListOfBranches();
    // a check to see if there really are any branches inside this branch
    //cout << "getting size\n";
    Int_t size = branchlist->GetEntriesFast();
    std::cout << "looking at branch "<< branch->GetName()<< " " << size<<endl;
    if (size > 0)
    {
        //cout << "inside makepopupfrombranch recursion\n";
        // recursion
        // need to make a new popup menu and call this function on each member of the list
        TGPopupMenu* newmenu = new TGPopupMenu(gClient->GetRoot());
        menu->AddPopup(branch->GetName(),newmenu);
        // also need to update the currentname
        std::string* newname = new std::string();
        *newname = *currentname+string(branch->GetName())+".";
        for (int i=0;i<size;++i)
        {
            //cout << "inside loop of recursion\n";
            TBranchObject* newbranch = (TBranchObject*)branchlist->At(i);
            //cout << "======================\n===================\n";
            MakePopupFromBranch(newbranch,newmenu,names,offset,newname);
        }
    } else
    {
        // nothing else inside, let's add an entry to the menu and be done
        // TODO: handle arrays
        //cout << "adding new branch at end\n";
        std::string flevelname(string(branch->GetName()));
        std::string branchname = ProcessFinalName(currentname,flevelname);
        // look for if it's an array
        int i1,i2;
        i1 = 0;
        i2 = 0;
        Int_t arrayq = IsArray(branchname,&i1,&i2);

        if (arrayq == 0)
        {
            // no array
            menu->AddEntry(branch->GetName(),names->size()+offset); // note the offset just in case we need to push out
            names->push_back(branchname); // add name to vector
        } else if (arrayq == 1)
        {
            TGPopupMenu* newmenu = new TGPopupMenu(gClient->GetRoot());
            menu->AddPopup(branch->GetName(),newmenu);
            for (int i=0;i<i1;++i)
            {
                /*
                std::stringstream ss0;
                ss0 << i1;
                */

                std::stringstream ss;
                ss << i;
                newmenu->AddEntry(ss.str().c_str(),names->size()+offset);
                names->push_back(branchname.substr(0,branchname.find("[")+1)+ss.str() + "]");
            }
            // handle 1d arrays
        } else if (arrayq == 2)
        {
            // handle 2d arrays
            TGPopupMenu* newmenu = new TGPopupMenu(gClient->GetRoot());
            menu->AddPopup(branch->GetName(),newmenu);
            for (int i=0;i<i1;++i)
            {
                // make a new popup for each one...
                TGPopupMenu* temp = new TGPopupMenu(gClient->GetRoot());
                std::stringstream ss;
                ss << i;
                newmenu->AddPopup(ss.str().c_str(),temp);
                for (int j=0;j<i2;++j)
                {
                    std::stringstream ss2;
                    ss2 << j;
                    temp->AddEntry(ss2.str().c_str(),names->size()+offset);
                    names->push_back(branchname.substr(0,branchname.find("[")+1)+ss.str() + "]["+ss2.str()+"]");
                }
            }
        } else
        {
            std::cout << "Array sensing failed... moving on\n";
        }
        //cout << arrayq << " " << i1 << " " << i2 << endl;
        //names->push_back(*currentname+flevelname); // add name to vector


    }
}
TGPopupMenu*
HistMaker::MakePopupFromTree(TTree* t,TGMenuBar* bar,std::vector<string> *names,const char* menuname,Int_t offset,std::string* currentname)
{
    std::cout << "begin makepopupfromtree function\n";
    std::cout << "getting branch list\n";
    TObjArray *branchlist = t->GetListOfBranches();
    std::cout << "getting size\n";
    Int_t size = branchlist->GetEntries();
    if (currentname == NULL)
    {
        std::cout << "making new string\n";
        currentname = new std::string();
    }
    if (size > 0)
    {
        std::cout << "inside the recursion\n";
        // recursion
        // need to make a new popup menu and call this function on each member of the list
        std::cout << "making new popup\n";
        TGPopupMenu* newmenu = new TGPopupMenu(gClient->GetRoot());
        //newmenu->Connect("Activated(Int_t)", "HistMaker", this,"HandleTreeMenu(Int_t)");
        std::cout << "adding popup\n";
        //bar->AddPopup("puke",newmenu);
        TGLayoutHints* mlayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX );
        //bar->AddPopup(menuname,newmenu,mlayout);
        // also need to update the currentname
        std::cout << "making new name\n";
        std::string* newname = new std::string();
        std::cout << "adding to new name\n";
        *newname = *currentname;//+string(t->GetName());
        std::cout << "begin loop over branches\n";
        for (int i=0;i<size;++i)
        {
            std::cout << "getting branch\n";
            TBranchObject* newbranch = (TBranchObject*)branchlist->At(i);
            std::cout << "calling makepopupfrombranch\n";
            std::cout << "======================\n===================\n";
            //MakePopupFromBranch(newbranch,newmenu,names,offset,newname);
            MakePopupFromBranch(newbranch,newmenu,names,offset,newname);
        }
        return newmenu;
    } else
    {
        std::cout << "Epic Fail, your tree doesn't have any branches in it\n";
        return NULL;
    }
}
