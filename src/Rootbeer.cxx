/*! \file Rootbeer.cxx 
 *  \brief Implements the user interface functions.
 */
#include <iostream>
#include "Rootbeer.hxx"
#include "Hist.hxx"
using namespace std;



/// User functions

void rb::AddHist(const char* name, const char* title,
		 Int_t nbinsx, Double_t xlow, Double_t xhigh,
		 const char* param, const char* gate) {

  rb::H1D * hst = new rb::H1D(name, title,
			      nbinsx, xlow, xhigh,
			      param, gate);
}

void rb::AddHist(const char* name, const char* title,
		 Int_t nbinsx, Double_t xlow, Double_t xhigh,
		 Int_t nbinsy, Double_t ylow, Double_t yhigh,
		 const char* param, const char* gate) {

  rb::H2D * hst = new rb::H2D(name, title,
			      nbinsx, xlow, xhigh,
			      nbinsy, ylow, yhigh,
			      param, gate);
}

void rb::AddHist(const char* name, const char* title,
		 Int_t nbinsx, Double_t xlow, Double_t xhigh,
		 Int_t nbinsy, Double_t ylow, Double_t yhigh,
		 Int_t nbinsz, Double_t zlow, Double_t zhigh,
		 const char* param, const char* gate) {

  rb::H3D * hst = new rb::H3D(name, title,
  			      nbinsx, xlow, xhigh,
  			      nbinsy, ylow, yhigh,
  			      nbinsz, zlow, zhigh,
  			      param, gate);
}


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

