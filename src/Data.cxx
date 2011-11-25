/*! \file Data.cxx
 *  \brief Implements Data.hxx
 */
#include "Data.hxx"
using namespace std;


// Some helper functions.

/// Figure out the lowest-level names of subbranches of a TBranch and print to a stream.
void branch_parse(TBranch* branch, Bool_t is_top, ostream& strm) {
  // Store the top level branch name for later use.
  static string top_name;
  if(is_top) top_name = branch->GetName();

  Int_t nBranches = branch->GetListOfBranches()->GetEntries();
  if(branch->IsFolder()) { // this branch contains subbranches, recurse into them
    for(Int_t i=0; i< nBranches; ++i) {
      branch_parse(static_cast<TBranch*>(branch->GetListOfBranches()->At(i)), kFALSE, strm);
    }
  }
  else { // at a lowest-level branch, print the name and value into strm
    stringstream sstr;
    sstr << top_name << "->" << branch->GetName();
    TTreeFormula f("f", sstr.str().c_str(), branch->GetTree()); // for figuring out the value
    if(f.GetTree()) { // forula compiled OK
      strm << sstr.str() << " = " << f.EvalInstance() << ";\n";
    }
  }
}

void class_parse(const char* name, const char* class_name, void* v, ostream& strm) {
  TTree tParse("tParse", "Temp tree for parsing a class");
  tParse.Branch(name, class_name, &v);
  for(Int_t i=0; i< tParse.GetListOfBranches()->GetEntries(); ++i) {
    branch_parse(static_cast<TBranch*>(tParse.GetListOfBranches()->At(i)), kTRUE, strm);
  }
}



// Class UserDataABC //

// Static branch adding function
void UserDataABC::AddBranches() {
  MapIterator_t it = fgMap.begin();
  while(it != UserDataABC::fgMap.end())
    (*it++).second->AddBranch();
}

// Static CINT pointer creation function
void UserDataABC::CreatePointers() {
  vector<string> vPrint; /// For message printing
  vPrint.push_back("\nCreating pointers to user data objects:\n");

  for(MapIterator_t it = fgMap.begin(); it != fgMap.end(); ++it) {
    if(it->second->kCintPointer) {
      string name = it->first;
      string className = it->second->fClassName;

      stringstream sPrint;
      sPrint << "        " << className << "* " << name << endl;
      vPrint.push_back(sPrint.str());

      stringstream  sExecute;
      sExecute << className << "* " << name << " = "
	       << "UserData<" << className << ">::Get "
	       << "(\"" << name << "\")";

      gROOT->ProcessLine(sExecute.str().c_str());
    }
  }

  if(vPrint.size() > 1) { // At least one pointer was created
    for(UInt_t u=0; u< vPrint.size(); ++u) {
      cout << vPrint[u];
    }
    cout << "\n\n";
  }
}


// Static parsing function
void UserDataABC::ParseAllInstances(ostream& strm) {
   MapIterator_t it = fgMap.begin();
   while(it != fgMap.end()) {
     if(it->second->kCintPointer) {
       class_parse(it->second->fName.c_str(),
		   it->second->fClassName.c_str(),
		   it->second->fData,
		   strm);
       ++it;
     }
   }
 }
