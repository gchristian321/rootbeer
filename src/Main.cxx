//! \file Main.cxx
//! \brief Implements the \c main symbol for export
#include <cassert>
#include <set>
#include <string>
#include <TROOT.h>
#include "boost/scoped_ptr.hpp"
#include "Rint.hxx"
#include "Attach.hxx"
#include "Rootbeer.hxx"
#include "Main.hxx"

namespace {
void usage(const char* arg0) {
	std::string progname(arg0);
	unsigned long slashPos = progname.rfind('/');
	if (slashPos < progname.size())
		progname = progname.substr (slashPos + 1);
	std::cout << "usage: " << progname << " --unpack <input file>\n\n";
	exit(1);
}
void handle_args(int argc, char** argv, std::string& fin) {
	if(argc != 3) usage(argv[0]);
	fin  = argv[2];
} }

/// \brief The \c main ROOTBEER function.
//! \details Creates an instance of \c rb::Rint and runs it.
int rb::Main::Run(int argc, char** argv)
{

 if (argc > 1 && !strcmp(argv[1], "--unpack")) { // 'rbunpack'

	 std::string fin;
	 handle_args(argc, argv, fin);
	 int argc2 = argc + 1;
	 char** argv2 = (char**)malloc(argc2*sizeof(char*));
	 for(int i=0; i< argc; ++i) {
		 argv2[i] = (char*)malloc(strlen(argv[i]+1));
		 strcpy(argv2[i], argv[i]);
	 }
	 argv2[argc] = (char*)malloc(4);
	 strcpy(argv2[argc], "-ng");

	 rb::Rint rbApp("Rbunpack", &argc2, argv2, 0, 0, true);
	 rbApp.StartSave(false);
	 rb::AttachFile(fin.c_str());
	 gSystem->Sleep(1e2);
	 while(rb::FileAttached())
		 assert(0);
	 rbApp.Terminate(0);
	 for(int i=0; i< argc2; ++i)
		 free(argv2[i]);
	 free(argv2);
	 return 0;

 } else { // Standard ROOTBEER

	 std::set<std::string> args(argv, argv+argc);
	 Bool_t lite = args.count("-l");
	 rb::Rint rbApp("Rootbeer", &argc, argv, 0, 0, lite);
	 gROOT->ProcessLine("gStyle->SetOptTitle(kTRUE)");
	 gROOT->ProcessLine("gStyle->SetOptStat(kTRUE)");
	 rbApp.Run();
	 return 0;

 }
}


int main(int argc, char** argv)
{
	boost::scoped_ptr<rb::Main> m (rb::GetMain());
	return m->Run(argc, argv);
}
