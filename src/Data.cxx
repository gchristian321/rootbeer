/*! \file Data.cxx
 *  \brief Implements Data.hxx
 */
#include <TROOT.h>
#include <TClass.h>
#include <TStreamerInfo.h>
#include <TStreamerElement.h>
#include <TTreeFormula.h>
#include "Data.hxx"
using namespace std;


// Some helper functions.

/// Add an offset to a void* pointer
void void_pointer_add(void*& initial, Int_t offset) {
  char* temp =  reinterpret_cast<char*>(initial);
  temp += offset;
  initial = reinterpret_cast<void*>(temp);
}

/// Remove adjacent whitespace from a \c std::string
void remove_duplicate_spaces(string& str) {
  for(int i=str.size()-1; i > 0; --i) {
    if(str[i] == ' ' && str[i-1] == ' ')
      str.erase(i,1);
  }
}

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
rb::MData::Map_t       rb::MData::fgMap;
rb::MData::SetMap_t    rb::MData::fgSetFunctionMap;
rb::MData::GetMap_t    rb::MData::fgGetFunctionMap;
rb::MData::ObjectMap_t rb::MData::fgObjectMap;

// Constructor
rb::MData::MData(const char* name, const char* class_name, void* data, Bool_t createPointer):
  kName(name), kClassName(class_name), kCintPointer(createPointer) {
      fData = data;
      fgMap[kName] = this;

#define FMAP_INSERT(KEY)						\
      fgSetFunctionMap.insert(std::make_pair(#KEY , &SetDataValue<KEY>)); \
      fgGetFunctionMap.insert(std::make_pair(#KEY , &GetDataValue<KEY>));

      FMAP_INSERT(Double_t);
      FMAP_INSERT(Float_t);
      FMAP_INSERT(Long64_t);
      FMAP_INSERT(Long_t);
      FMAP_INSERT(Int_t);
      FMAP_INSERT(Short_t);
      FMAP_INSERT(Char_t);
      FMAP_INSERT(Bool_t);
      FMAP_INSERT(ULong64_t);
      FMAP_INSERT(ULong_t);
      FMAP_INSERT(UInt_t);
      FMAP_INSERT(UShort_t);
      FMAP_INSERT(double);
      FMAP_INSERT(float);
      FMAP_INSERT(long long);
      FMAP_INSERT(long);
      FMAP_INSERT(int);
      FMAP_INSERT(short);
      FMAP_INSERT(char);
      FMAP_INSERT(bool);
      FMAP_INSERT(unsigned long long);
      FMAP_INSERT(unsigned long);
      FMAP_INSERT(unsigned int);
      FMAP_INSERT(unsigned);
      FMAP_INSERT(unsigned short);
}

Double_t rb::MData::Get(const char* name) {
  ObjectMapIterator_t it = fgObjectMap.find(name);
  if(it == fgObjectMap.end()) {
    Error("Get", "%s not found.", name);
    return -1.;
  }
  string typeName = it->second.second;

  GetMapIterator_t itGet = fgGetFunctionMap.find(typeName);
  if(itGet == fgGetFunctionMap.end()) {
    Error("Get", "Invalid type %s", typeName.c_str());
    return -1.;
  }
  return Double_t(itGet->second(name));
}

void rb::MData::Set(const char* name, Double_t newvalue) {
  ObjectMapIterator_t itObject = fgObjectMap.find(name);
  if(itObject == fgObjectMap.end()) {
    Error("SetData", "Data object: %s not found.", name);
    return;
  }

  void* objectAddress = itObject->second.first;
  string type = itObject->second.second;
  SetMapIterator_t itCast = fgSetFunctionMap.find(type);
  if(itCast == fgSetFunctionMap.end()) {
    Error("SetData", "Invalid type: %s", type.c_str());
    return;
  }

  void_cast castFunction = itCast->second;
  castFunction(objectAddress, newvalue);
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

      //      gROOT->ProcessLine(sExecute.str().c_str());
      ParseClass(name.c_str(), className.c_str(), it->second->fData);
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

// Static save all function
void rb::MData::SaveAllPrimitive(ostream& strm) {
  MapIterator_t it;
  for(it = fgMap.begin(); it != fgMap.end(); ++it) {
    if(it->second->kCintPointer) {
      it->second->SavePrimitive(strm);
    }
  }
}


// Static class parsing function
Bool_t rb::MData::MapDataAddress(const char* name, TStreamerElement* element, void* base_address) {

  string typeName = element->GetTypeName();
  remove_duplicate_spaces(typeName); // in case someone did 'unsigned    short' or whatever

  SetMapIterator_t it = fgSetFunctionMap.find(typeName);
  if(it == fgSetFunctionMap.end()) return kFALSE;

  void* address = base_address;
  Int_t arrLen = element->GetArrayLength();
    void_pointer_add(address, element->GetOffset());

  if(!arrLen) {
    fgObjectMap.insert(std::make_pair(name, std::make_pair(address, typeName)));
    cout << name <<": " << address << endl;
  }
  else {
    Int_t size = element->GetSize() / arrLen;
    for(Int_t i=0; i< arrLen; ++i) {
      stringstream fullName;
      fullName << name << "[" << i << "]";
      void_pointer_add(address, size*(i>0));
      fgObjectMap.insert(std::make_pair(fullName.str(), std::make_pair(address, typeName)));
      cout << fullName.str() <<": " << address << endl;
    }
  }
  return kTRUE;
}

void rb::MData::ParseClass(const char* name, const char* classname, void* address) {

  TClass* cl = TClass::GetClass(classname);
  if(!cl) return;

  TStreamerInfo* sinfo = static_cast<TStreamerInfo*>(cl->GetStreamerInfo());
  TObjArray* elems = sinfo->GetElements();

  for( Int_t i=0; i< elems->GetEntries(); ++i) {
    TStreamerElement* element =
      reinterpret_cast <TStreamerElement*>(elems->At(i));
    stringstream ssName;
    ssName << name << "." << element->GetName();
    Bool_t basic = MapDataAddress(ssName.str().c_str(), element, address);
    if(!basic) {
      void_pointer_add(address, element->GetOffset());
      ParseClass(ssName.str().c_str(), element->GetTypeName(), address);
    }
  }
}

void rb::MData::list() {
  ObjectMapIterator_t it = fgObjectMap.begin();
  while(it != fgObjectMap.end()) {
    cout << (*it++).first << endl;
  }
}
