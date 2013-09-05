//! \file WriteConfig.cxx
//! \brief Implements methods related to saving configuration files.
//! \details Put in a separate file because they're verbose and we want to avoid cluttering
//! Rootbeer.cxx
#include <vector>
#include <fstream>
#include <iostream>
#include <TCanvas.h>
#include <TCutG.h>
#include "Rootbeer.hxx"
#include "Rint.hxx"
#include "Data.hxx"
#include "Signals.hxx"
#include "utils/Assorted.hxx"
#include "hist/Hist.hxx"
#include "mxml/mxml.hxx"



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// GENERIC HELPER FUNCTIONS               //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

namespace {

//
// Check for existance of a file, returns true if okay to proceed
// (and overwrite if existing), false otherwise.
Bool_t check_file(const char* name) {
	Bool_t retval = true;
	std::ifstream ifs(name);
	if(ifs.good()) {
		std::string answer;
		std::cout << "The file " << name << " exists. Overwrite (y/n)?\n";
		std::cin  >> answer;
		if(!(answer == "y" || answer == "Y")) {
			std::cout << "Aborting...\n";
			retval = false;
		}
	}
	return retval;
}

//
// Create an XML writer into a new file
rb::XmlWriter* get_xml_writer(const char* filename, bool prompt, int& code) {
	code = 0;
	TString filename1 = filename;
	{
		TErrorIgnore ignore(5001);
		gSystem->ExpandPathName(filename1);
	}
	if(prompt && check_file(filename1) == false) {
		code = 1;
		return 0;
	}
	
	rb::XmlWriter* writer = rb::mxml_open_file(filename1);
	if(!writer) code = -1;
	return writer;
}

//
// Find the XML tree in a file
rb::XmlNode* get_xml_tree(const char* filename, TString& errmsg) {
	TString filename1 = filename;
	{
		TErrorIgnore ignore(5001);
		gSystem->ExpandPathName(filename1);
	}
	char err[4096]; int errcode;
	rb::XmlNode *tree = rb::mxml_parse_file(filename1, err, sizeof(err), &errcode);
	if(!tree) errmsg = err;
	return tree;
} }

		

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// XML VARIABLES                        //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// WRITE                                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

namespace {
void write_variable_nodes(rb::XmlWriter* writer) {
	mxml_write_comment(writer, "ROOTBEER Variables");
	mxml_start_element(writer, "variables");
	rb::data::MBasic::Printer p;
  p.SaveXML(writer);
	mxml_end_element(writer);
} }

Int_t rb::WriteVariablesXML(const char* filename, Bool_t prompt) {
	int code;
	XmlWriter* writer = get_xml_writer(filename, prompt, code);
	if(!writer) return code;
	
	write_variable_nodes(writer);
	mxml_close_file(writer);
	return 0;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// READ                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

namespace {
void read_variables_tree(rb::XmlNode* tree, bool quiet) {
	rb::XmlNode* varsnode = rb::mxml_find_node(tree, "variables");
	if(!varsnode) {
		if(!quiet) rb::err::Error("ReadVariablesXML") << "No \"variables\" node";
		return;
	}
	
	for(int i=0; i< rb::mxml_get_number_of_children(varsnode); ++i) {
		rb::XmlNode* child = rb::mxml_subnode(varsnode, i);
		if(!strcmp(rb::mxml_get_name(child), "var")) {
			TString name  = rb::mxml_get_attribute(child, "name");
			TString type  = rb::mxml_get_attribute(child, "type");
			TString value = rb::mxml_get_value(child);			

			if(!type.Contains("const"))
				rb::data::SetValue(name, value.Atof());
		}
	}
} }

void rb::ReadVariablesXML(const char* filename) {
	TString err;
	XmlNode *tree = get_xml_tree(filename, err);
	if(!tree) {
		rb::err::Error("ReadVariablesXML") << err << "\n";
	} else {
		read_variables_tree(tree, false);
	}

	if(tree) rb::mxml_free_tree(tree);
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// XML HISTOGRAMS                       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// WRITE                                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace {
void write_dir(rb::XmlWriter *w, TDirectory* dir) {
	mxml_start_element(w, "TDirectory");
	mxml_write_attribute(w, "name", dir->GetName());
	mxml_write_attribute(w, "title", dir->GetTitle());
}

void recurse_dir(rb::XmlWriter *w, TDirectory* dir, bool writeHead = true) {
	if(writeHead) write_dir(w, dir);
	for(int i=0; i< dir->GetList()->GetEntries(); ++i) {
		TObject* obj = dir->GetList()->At(i); 
		if(obj->InheritsFrom(TDirectory::Class())) {
			recurse_dir(w, (TDirectory*)obj);
		}
		else if (obj->InheritsFrom(rb::hist::Base::Class())) {
			static_cast<rb::hist::Base*>(obj)->WriteXML(w);
		}
	}
	mxml_end_element(w);
}

void write_cuts_xml(rb::XmlWriter* w) {
	for(Int_t i=0; i< gROOT->GetListOfSpecials()->GetEntries(); ++i) {
		TObject* special = gROOT->GetListOfSpecials()->At(i);
		if(special->InheritsFrom(TCutG::Class())) {
			TCutG* cut = (TCutG*)special;
			rb::mxml_start_element(w, "TCutG");
			rb::mxml_write_attribute(w, "name",  cut->GetName());
			rb::mxml_write_attribute(w, "title", cut->GetTitle());
			rb::mxml_write_attribute(w, "varx",  cut->GetVarX());
			rb::mxml_write_attribute(w, "vary",  cut->GetVarY());
			rb::mxml_write_attribute(w, "lw", Form("%d", cut->GetLineWidth()));
			rb::mxml_write_attribute(w, "lc", Form("%d", cut->GetLineColor()));

			for(int i=0; i< cut->GetN(); ++i) {
				rb::mxml_start_element(w, "point");
				Double_t x, y; cut->GetPoint(i, x, y);
				rb::mxml_write_value(w, Form("%lf %lf", x, y));
				rb::mxml_end_element(w);
			}

			rb::mxml_end_element(w);
		}
	}
} 

void write_hist_nodes(rb::XmlWriter* writer) {
	mxml_write_comment(writer, "Graphical Cuts");
	mxml_start_element(writer, "cuts");
	write_cuts_xml(writer);
	mxml_end_element(writer);

	mxml_write_comment(writer, "ROOTBEER Histograms");
	mxml_start_element(writer, "histograms");
	recurse_dir(writer, gROOT, false);
	mxml_end_element(writer);
} }


Int_t rb::WriteHistXML(const char* filename, Bool_t prompt) {
	int code;
	XmlWriter* writer = get_xml_writer(filename, prompt, code);
	if(!writer) return code;

	write_hist_nodes(writer);
	mxml_close_file(writer);
	return 0;
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// READ                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace {
void recurse_dir_read(rb::XmlNode* node, TDirectory* parent, bool replace) {
	if(!parent) {
		std::cerr << "Error: parent == 0\n";
		return;
	}
	parent->cd();
	TDirectory* dir = parent;
	if(!strcmp(rb::mxml_get_name(node), "TDirectory")) {
		string name = rb::mxml_get_attribute(node, "name");
		string title = rb::mxml_get_attribute(node, "title");

		std::stringstream cmd;
		cmd << "rb::Mkdir(\"" << name << "\", \"" << title << "\");";
		dir = (TDirectory*)gROOT->ProcessLineFast(cmd.str().c_str());
	}
	for(int i=0; i< rb::mxml_get_number_of_children(node); ++i) {
		rb::mxml_struct *node1 = rb::mxml_subnode(node, i);
		if(!node1)
			continue;
		else if(!strcmp(rb::mxml_get_name(node1), "TDirectory"))
			recurse_dir_read(node1, dir, replace);
		else {
			rb::hist::Base::ConstructXML(node1, replace);
		}
	}
}

void delete_all_hists_and_dirs() {
	gROOT->cd();
	for(int i=0; i< gROOT->GetList()->GetEntries(); ++i) {
		delete gROOT->GetList()->At(i);
	}
	rb::Rint::gApp()->GetHistSignals()->SyncHistTree(); 
}

void read_cut(rb::XmlNode* node, bool replace) {
	std::vector<Double_t> xpoints, ypoints;
	for(int i=0; i< mxml_get_number_of_children(node); ++i) {
		rb::XmlNode* point = mxml_subnode(node, i);
		if(!strcmp(mxml_get_name(point), "point")) {
			Double_t x, y;
			char* val = mxml_get_value(point);
			sscanf(val, "%lf %lf", &x, &y);

			xpoints.push_back(x);
			ypoints.push_back(y);
		}
	}

	TString name  = mxml_get_attribute(node, "name");
	TString title = mxml_get_attribute(node, "title");
	TString varx  = mxml_get_attribute(node, "varx");
	TString vary  = mxml_get_attribute(node, "vary");
	Width_t lw = atoi(mxml_get_attribute(node, "lw"));
	Color_t lc = atoi(mxml_get_attribute(node, "lc"));

	TObject* existing = gROOT->GetListOfSpecials()->FindObject(name);
	if(existing && existing->InheritsFrom(TCutG::Class())) {
		if (replace) existing->Delete();
		else return;
	}

	TCutG* cut = new TCutG(name, xpoints.size(), &xpoints[0], &ypoints[0]);
	cut->SetVarX(varx);
	cut->SetVarY(vary);	
	cut->SetTitle(title);
	cut->SetLineWidth(lw);
	cut->SetLineColor(lc);
} 

void read_hist_tree(rb::XmlNode* tree, Option_t* option, bool quiet) {
	// figure out option
	Int_t replace;
	TString opt1(option);
	opt1.ToLower();
	if     (opt1 == "r") replace = 2; // reset
	else if(opt1 == "o") replace = 1; // overwrite
	else if(opt1 == "c") replace = 0; // cumulate
	else {
		rb::err::Warning("ReadHistXML")
			<< "Option " << option << " not valid, defaulting to \"reset\".";
		replace = 2;
	}

	if(replace == 2) { // reset
		// delete cuts
		for(int i=0; i< gROOT->GetListOfSpecials()->GetEntries(); ++i) {
			if(gROOT->GetListOfSpecials()->At(i)->InheritsFrom(TCutG::Class()))
				gROOT->GetListOfSpecials()->At(i)->Delete();
		}
		// delete hists
		delete_all_hists_and_dirs();
		replace = 0;
	}

	rb::XmlNode* histnode = rb::mxml_find_node(tree, "histograms");
	if(!histnode) {
		if(!quiet) rb::err::Error("ReadHistXML") << "No \"histograms\" node";
		return;
	}

	//
	// read cuts
	rb::XmlNode* cutnode = rb::mxml_find_node(tree, "cuts");
	if(cutnode) {
		for(int i=0; i< rb::mxml_get_number_of_children(cutnode); ++i) {
			rb::XmlNode* child = rb::mxml_subnode(cutnode, i);
			if(!strcmp(rb::mxml_get_name(child), "TCutG")) read_cut(child, replace);
		}
	}

	//
	// read histograms
	recurse_dir_read(histnode, gROOT, replace);
} }

void rb::ReadHistXML(const char* filename, Option_t* option)
{
	TString err;
	XmlNode *tree = get_xml_tree(filename, err);
	if(!tree) {
		rb::err::Error("ReadHistXML") << err << "\n";
	} else {
		read_hist_tree(tree, option, false);
	}

	if(tree) rb::mxml_free_tree(tree);
}



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// XML CANVAS                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// WRITE                                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace {
void write_canvas_hist(rb::XmlWriter* w, TH1* hst) {
	rb::hist::Base* hrb = (rb::hist::Base*)gROOT->ProcessLineFast(hst->GetName());
	if(!hrb) return;

	TString path = "";
	TDirectory* dir = hrb->GetDirectory();
	while(dir->GetMotherDir()) {
		path.Insert(0, "/");
		path.Insert(0, dir->GetName());
		dir = dir->GetMotherDir();
	}
	
	mxml_start_element(w, "rb_hist_Base");
	mxml_write_attribute(w, "path", path);
	mxml_write_attribute(w, "name", hrb->GetName());
	mxml_write_attribute(w, "opt",  hrb->GetDrawOption());
	mxml_end_element(w);
}

void recurse_pad(rb::XmlWriter* w, TPad* pad) {
	pad->cd();
	if(!pad->InheritsFrom(TCanvas::Class())) {
		mxml_start_element(w, "TPad");
		mxml_write_attribute(w, "name",   pad->GetName());
		mxml_write_attribute(w, "title",  pad->GetTitle());
		mxml_write_attribute(w, "xlow",   Form("%f", pad->GetXlowNDC()));
		mxml_write_attribute(w, "ylow",   Form("%f", pad->GetYlowNDC()));
		mxml_write_attribute(w, "xhigh",  Form("%f", pad->GetXlowNDC() + pad->GetWNDC()));
		mxml_write_attribute(w, "yhigh",  Form("%f", pad->GetYlowNDC() + pad->GetHNDC()));
		mxml_write_attribute(w, "color",  Form("%d", pad->GetFillColor()));
	}
	for(Int_t i=0; i< pad->GetListOfPrimitives()->GetEntries(); ++i) {
		TObject* obj =  pad->GetListOfPrimitives()->At(i);
		if(obj->InheritsFrom(TPad::Class()))
			recurse_pad(w, (TPad*)obj);
		else if(obj->InheritsFrom(TH1::Class())) {
			if( std::string(obj->GetName()).substr(0, std::string("(rb::hist::Base*)").size()) == "(rb::hist::Base*)")
				write_canvas_hist(w, (TH1*)obj);
		}
	}
	if(!pad->InheritsFrom(TCanvas::Class()))
		mxml_end_element(w);
} 

void write_canvas_nodes(rb::XmlWriter* writer) {
	TVirtualPad* pcur = gPad;
	mxml_write_comment(writer, "ROOTBEER Canvas Configuration");
	mxml_start_element(writer, "canvas");

	for(Int_t i=0; i< gROOT->GetListOfCanvases()->GetEntries(); ++i) {
		TCanvas* canvas = (TCanvas*)gROOT->GetListOfCanvases()->At(i);
		mxml_start_element(writer, "TCanvas");
		mxml_write_attribute(writer, "name",  canvas->GetName());
		mxml_write_attribute(writer, "title", canvas->GetTitle());
		mxml_write_attribute(writer, "width",  Form("%d", canvas->GetWw()));
		mxml_write_attribute(writer, "height", Form("%d", canvas->GetWh()));
		mxml_write_attribute(writer, "xpos",   Form("%d", canvas->GetWindowTopX()));
		mxml_write_attribute(writer, "ypos",   Form("%d", canvas->GetWindowTopY()));
		
		recurse_pad(writer, canvas);
		
		mxml_end_element(writer);
	}

	mxml_end_element(writer);	
	if(pcur) pcur->cd();
} }

Int_t rb::WriteCanvasXML(const char* filename, Bool_t prompt) {
	int code;
	XmlWriter* writer = get_xml_writer(filename, prompt, code);
	if(!writer) return code;
	
	write_canvas_nodes(writer);
	mxml_close_file(writer);
	return 0;
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// READ                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace {
TPad* create_pad(rb::XmlNode* child, Int_t& nsub) {
	TString name   =      rb::mxml_get_attribute(child, "name");
	TString title  =      rb::mxml_get_attribute(child, "title");
	Double_t xlow  = atof(rb::mxml_get_attribute(child, "xlow"));
	Double_t xhigh = atof(rb::mxml_get_attribute(child, "xhigh"));
	Double_t ylow  = atof(rb::mxml_get_attribute(child, "ylow"));
	Double_t yhigh = atof(rb::mxml_get_attribute(child, "yhigh"));
	Color_t  color = atoi(rb::mxml_get_attribute(child, "color"));

	TPad* p = new TPad(name, title, xlow, ylow, xhigh, yhigh, color);
	p->SetNumber(++nsub);
	p->Draw();
	
	return p;
}

void draw_hist_pad(rb::XmlNode* child) {
	TString dirpath = rb::mxml_get_attribute(child, "path");
	TString hstname = rb::mxml_get_attribute(child, "name");

	TDirectory* current = gDirectory;
	Bool_t cd = gROOT->cd(dirpath);
	if(!cd) {
		rb::err::Warning("ReadCanvasXML")
			<< "Specified directory \"" << dirpath << "\" for the histogram \""
			<< hstname << "\" does not exist.";
	} else {
		TObject* hhh = gDirectory->Get(hstname);
		if(!hhh || !hhh->InheritsFrom(rb::hist::Base::Class())) {
			rb::err::Warning("ReadCanvasXML")
				<< "Specified histogram \"" << hstname << "\" does not exist in the directory \""
				<< dirpath << "\".";
		}
		const char* option  = rb::mxml_get_attribute(child, "opt");
		const char* opt = option ? option : "";
		static_cast<rb::hist::Base*>(hhh)->Draw(opt);
	}			
	current->cd();
}

void recurse_canvas_read(rb::XmlNode* node, TPad* parent) {
	if(!parent) {
		std::cerr << "Error: parent == 0\n";
		return;
	}
	parent->cd();
	TPad* pad = parent;
	Int_t nsub = 0;

	for(int i=0; i< rb::mxml_get_number_of_children(node); ++i) {
		parent->cd();
		rb::XmlNode* child = rb::mxml_subnode(node, i);
		if(!child)
			continue;
		else if(!strcmp(rb::mxml_get_name(child), "TPad")) {
			TPad* p = create_pad(child, nsub);
			recurse_canvas_read(child, p);
		}
		else if (!strcmp(rb::mxml_get_name(child), "rb_hist_Base")) {
			draw_hist_pad(child);
		}
		else {
			;
		}
	}
}

void read_canvas_tree(rb::XmlNode* tree, bool quiet) {
	rb::XmlNode* canvases = mxml_find_node(tree, "canvas");
	if(!canvases) {
		if(!quiet) rb::err::Error("ReadCanvasXML") << "No \"canvas\" node";
		return;
	}

	for(Int_t i=0; i< gROOT->GetListOfCanvases()->GetEntries(); ++i) {
		static_cast<TCanvas*>(gROOT->GetListOfCanvases()->At(i))->Close();
	}

	for(int i=0; i< rb::mxml_get_number_of_children(canvases); ++i) {
		rb::XmlNode* child = rb::mxml_subnode(canvases, i);

		if(!strcmp(rb::mxml_get_name(child), "TCanvas")) {
			TString name  = rb::mxml_get_attribute(child, "name");
			TString title = rb::mxml_get_attribute(child, "title");
			Int_t ww = atoi(rb::mxml_get_attribute(child, "width"));
			Int_t hh = atoi(rb::mxml_get_attribute(child, "height"));
			Int_t xx = atoi(rb::mxml_get_attribute(child, "xpos"));
			Int_t yy = atoi(rb::mxml_get_attribute(child, "ypos"));

			TCanvas* c = new TCanvas(name, title, ww, hh);
			Int_t diffx = ww - c->GetWw(), diffy = hh - c->GetWh();
			c->SetWindowSize(ww + diffx, hh + diffy);
			c->SetWindowPosition(xx - diffx, yy - diffy);

			recurse_canvas_read(child,  c);
		}
	}
} }

void rb::ReadCanvasXML(const char* filename) {
	TString filename1 = filename;
	{
		TErrorIgnore ignore(5001);
		gSystem->ExpandPathName(filename1);
	}
	TString err;
	XmlNode *tree = get_xml_tree(filename, err);
	if(!tree) {
		rb::err::Error("ReadCanvasXML") << err << "\n";
	} else {
		read_canvas_tree(tree, false);
	}

	if(tree) rb::mxml_free_tree(tree);
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// XML ALL                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// WRITE                                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

Int_t rb::WriteConfigXML(const char* filename, Bool_t prompt) {
	int code;
	XmlWriter* writer = get_xml_writer(filename, prompt, code);
	if(!writer) return code;
	
	write_hist_nodes(writer);
	write_canvas_nodes(writer);
	write_variable_nodes(writer);

	mxml_close_file(writer);
	return 0;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// READ                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

void rb::ReadConfigXML(const char* filename, Option_t* option) {
	TString err;
	XmlNode *tree = get_xml_tree(filename, err);
	if(!tree) {
		rb::err::Error("ReadConfigXML") << err << "\n";
	} else {
		read_hist_tree(tree, option, true);
		read_canvas_tree(tree, true);
		read_variables_tree(tree, true);
	}

	if(tree) rb::mxml_free_tree(tree);
}
