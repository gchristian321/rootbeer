//! \file Rint.cxx
//! \brief Implements Rint.hxx
#include "Rint.hxx"
#include "Rootbeer.hxx"


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
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Rint::Terminate()                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Rint::Terminate(Int_t status) {
  rb::canvas::StopUpdate();
  rb::Unattach();
  //  gSystem->Sleep(0.5e3);
  EventMap_t::iterator it;
  for(it = fEvents.begin(); it != fEvents.end(); ++it) {
    rb::Event* event = it->second;
    rb::Event::Destructor::Operate(event);
  }
  TRint::Terminate(status);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Events_t rb::Rint::GetEvent()                         //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::Event* rb::Rint::GetEvent(Int_t code) {
  EventMap_t::iterator itEvent = fEvents.find(code);
  return itEvent != fEvents.end() ? itEvent->second : NULL;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::Rint::PrintLogo()                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Rint::PrintLogo(Bool_t lite) {
  if(!lite)
    std::cout << "                                                                 \n"
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
  std::cout   << "                                                                 \n"
	      << "      Welcome to ROOT BEER, the ROOT Basic Event ExtractoR       \n"
	      << "                                                                 \n";
}
