//! \file Rint.cxx
//! \brief Implements Rint.hxx
#include <set>
#include "Rint.hxx"
#include "Rootbeer.hxx"
#include "Gui.hxx"
#include "HistGui.hxx"
#include "hist/Hist.hxx"

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class rb::Rint Implementation                         //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::Rint::Rint(const char* appClassName, int* argc, char** argv,
	       void* options, int numOptions, Bool_t liteLogo) :
  TRint(appClassName, argc, argv, options, numOptions, kTRUE),
	fSignals(0), fHistSignals(0),
	fSaveData(false), fSaveHists(false) {
        RegisterEvents();
        SetPrompt("rootbeer [%d] ");
        PrintLogo(liteLogo);
        std::cout << fMessage.str() << std::endl;

	std::set<std::string> flags(argv, argv + *argc);
	if(!(flags.count("-ng") || !gClient)) InitGui();

	for(EventMap_t::iterator it = fEvents.begin(); it != fEvents.end(); ++it) {
	  Int_t key = it->first;
	  fFilterCondition[key] = "";
	};
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Rint::Terminate()                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Rint::Terminate(Int_t status) {
  rb::canvas::StopUpdate();
  rb::Unattach();
  EventMap_t::iterator it;
  for(it = fEvents.begin(); it != fEvents.end(); ++it) {
    rb::Event* event = it->second.first;
    rb::Event::Destructor::Operate(event);
  }
	DeleteSignals();
	DeleteHistSignals();
  TRint::Terminate(status);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Rint::InitGui()                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Rint::InitGui() {
	DeleteSignals();
	fSignals = new rb::Signals();
	DeleteHistSignals();
	fHistSignals = new rb::HistSignals();
	fRbeerFrame = new TGRbeerFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
	fRbeerFrame->GuiLayout();
	fHistFrame = new TGHistVarFrame(gClient->GetRoot(),10,10,kMainFrame | kVerticalFrame);
	fHistFrame->HistGuiLayout();
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Events_t rb::Rint::GetEvent()                         //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::Event* rb::Rint::GetEvent(Int_t code) {
	return fEvents.count(code) ? fEvents.find(code)->second.first : 0;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// EventVector_t rb::Rint::GetEventVector()              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::EventVector_t rb::Rint::GetEventVector() {
	EventVector_t out;
	EventMap_t::iterator it = fEvents.begin();
	for (; it != fEvents.end(); ++it) {
		out.push_back(std::make_pair(it->first, it->second.second));
	}
	return out;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::hist::Base* rb::Rint::FindHistogram()             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::hist::Base* rb::Rint::FindHistogram(const char* name, TDirectory* directory) {
	EventVector_t events = GetEventVector();
	for(EventVector_t::iterator it = events.begin(); it != events.end(); ++it) {
		rb::hist::Base* hist = GetEvent(it->first)->FindHistogram(name, directory);
		if(hist) return hist;
	}
	return 0;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Rint::DeleteSignals()                        //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Rint::DeleteSignals() {
	if(fSignals) {
		delete fSignals;
		fSignals = 0;
	}
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Rint::DeleteHistSignals()                    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Rint::DeleteHistSignals() {
	if(fHistSignals) {
		delete fHistSignals;
		fHistSignals = 0;
	}
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Rint::PrintLogo()                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Rint::PrintLogo(Bool_t lite) {
  if(!lite)
    std::cout << "                                                           \n"
	      << "                            ___                                  \n"
	      << "                          .'   '.                                \n"
	      << "                         /       \\           oOoOo               \n"
	      << "                        |         |       ,==|||||               \n"
	      << "                         \\       /        || |||||               \n"
	      << "                          '.___.'    _.-'^|| |||||               \n"
	      << "                        __/_______.-'     '==HHHHH               \n"
	      << "                   _.-'` /                   \"\"\"\"\"          \n"
	      << "                .-'     /   oOoOo                                \n"
	      << "                `-._   / ,==|||||                                \n"
	      << "                    '-/._|| |||||                                \n"
	      << "                     /  ^|| |||||                                \n"
	      << "                    /    '==HHHHH                                \n"
	      << "                   /________\"\"\"\"\"                           \n"
	      << "                   `\\       `\\                                 \n"
	      << "                     \\        `\\   /                           \n"
	      << "                      \\         `\\/                            \n"
	      << "                      /                                          \n"
	      << "                     /                                           \n"
	      << "                    /_____                                       \n";
  std::cout   << "                                                           \n"
	      << "      Welcome to ROOT BEER, the ROOT Basic Event ExtractoR       \n"
	      << "                                                                 \n";
}
