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
      strm << "      " << sstr.str() << " = " << f.EvalInstance() << ";\n";
    }
  }
}


// Class rb::MData //

// Static data member initialization
rb::MData::Map_t rb::MData::fgMap;

// Constructor
rb::MData::MData(const char* name, const char* class_name, void* data, Bool_t createPointer):
  kName(name), kClassName(class_name), kCintPointer(createPointer) {
      fData = data;
      fgMap[kName] = this;
}

// Static branch adding function
void rb::MData::AddBranches() {
  MapIterator_t it = fgMap.begin();
  while(it != rb::MData::fgMap.end())
    (*it++).second->AddBranch();
}

// Static CINT pointer creation function
void rb::MData::CreatePointers() {
  vector<string> vPrint; /// For message printing
  vPrint.push_back("\nCreating pointers to user data objects:\n");

  for(MapIterator_t it = fgMap.begin(); it != fgMap.end(); ++it) {
    if(it->second->kCintPointer) {
      string name = it->first;
      string className = it->second->kClassName;

      stringstream sPrint;
      sPrint << "        " << className << "* " << name << endl;
      vPrint.push_back(sPrint.str());

      stringstream  sExecute;
      sExecute << className << "* " << name << " = "
	       << "rb::Data<" << className << ">::Get "
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

/// Parse a class and print it's member names to a stream.
void rb::MData::SavePrimitive(ostream& strm) {
  TTree tParse("tParse", "Temp tree for parsing a class");
  tParse.Branch(kName.c_str(), kClassName.c_str(), &fData);

  TIter it(tParse.GetListOfBranches());
  for(it = it.Begin(); it != it.End(); ++it) {
    branch_parse(static_cast<TBranch*>(*it), kTRUE, strm);
  }
}

// Static parsing function
void rb::MData::SaveAllPrimitive(ostream& strm) {
  MapIterator_t it;
  for(it = fgMap.begin(); it != fgMap.end(); ++it) {
    if(it->second->kCintPointer) {
      it->second->SavePrimitive(strm);
    }
  }
}
