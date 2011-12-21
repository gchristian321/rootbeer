/*! \file Rootbeer.cxx 
 *  \brief Implements the user interface functions.
 */
#include <iostream>
#include "Rootbeer.hxx"
#include "Data.hxx"
using namespace std;



/// User functions


void rb::Logo() {
    cout << "                                                                 \n"
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
// Terminate function
void rb::Rint::Terminate(Int_t status) {
  rb::Canvas::StopUpdate();
  rb::Unattach();
  gSystem->Sleep(0.5e3);
  rb::Hist::DeleteAll();
  rb::Data::DeleteAll();
  TRint::Terminate(status);
}

