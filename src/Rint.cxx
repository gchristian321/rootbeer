//! \file Rint.cxx
//! \brief Implements Rint.hxx
#include <set>
#include "Rint.hxx"
#include "Rootbeer.hxx"
#include "gui/CanvasGui.hxx"
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
  TRint(appClassName, argc, argv, options, numOptions, kTRUE) {
  RegisterEvents();
  SetPrompt("rootbeer [%d] ");
  PrintLogo(liteLogo);
  std::cout << fMessage.str() << std::endl;

	std::set<std::string> flags(argv, argv + *argc);
	if(flags.count("-ng") || !gClient) fFrames = 0;
	else { fFrames = new rb::gui::MainFrameFactory(); InitGui(); }
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Rint::Terminate()                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Rint::Terminate(Int_t status) {
  rb::canvas::StopUpdate();
  rb::Unattach();
	if(fFrames) delete fFrames;
  EventMap_t::iterator it;
  for(it = fEvents.begin(); it != fEvents.end(); ++it) {
    rb::Event* event = it->second.first;
    rb::Event::Destructor::Operate(event);
  }
  TRint::Terminate(status);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Rint::Terminate()                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Rint::InitGui() {
//	fFrames->Add (new gui::Canvas() );
	GuiLayout();
	HistGuiLayout();
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Frame* rb::Rint::GetGuiFrame()                        //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Rint::SyncAll() {
	if(!fFrames) return;
	for(UInt_t i=0; i< fFrames->Size(); ++i)
		 fFrames->Get(i)->Sync();
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
	EventMap_t::iterator it = fEvents.begin();
	EventVector_t out; if(!fEvents.size()) return out;
	while(it != fEvents.end()) {
		out.push_back(std::make_pair(it->first, it->second.second)); ++it;
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
// void rb::Rint::PrintLogo()                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Rint::PrintLogo(Bool_t lite) {
  if(!lite)
    std::cout << "                                                           \n"
	      << "                            ___                                  \n"
	      << "                          .'   '.                                \n"
	      << "                         /       \\           oOoOo              \n"
	      << "                        |         |       ,==|||||               \n"
	      << "                         \\       /       _|| |||||              \n"
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
