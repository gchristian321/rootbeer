//! \file Rootbeer.cxx 
//!  \brief Implements the user interface functions.
#include <iostream>
#include "Rootbeer.hxx"

#define RB_INIT
#include "ImportDataMacros.h"
rb::BufferSource* gBuffers = 0;



void rb::Logo() {
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
	    << "                    /_____                                       \n"
	    << "                                                                 \n"
	    << "      Welcome to ROOT BEER, the ROOT Basic Event ExtractoR       \n"
	    << "                                                                 \n"
    ;
}



// class rb::Rint //

rb::Rint::Rint(const char* appClassName, int* argc, char** argv,
	       void* options, int numOptions, Bool_t noLogo) :
  TRint(appClassName, argc, argv, options, numOptions, noLogo) {
  SetPrompt("rootbeer [%d] ");

#define RB_ALLOCATE
#include "ImportDataMacros.h"

}


// Terminate function
void rb::Rint::Terminate(Int_t status) {
  rb::Canvas::StopUpdate();
  rb::Unattach();
  gSystem->Sleep(0.5e3);

#define RB_DEALLOCATE
#include "ImportDataMacros.h"

  rb::Hist::DeleteAll();
  TRint::Terminate(status);
}

