//! \file Data.cxx
//! \brief Implements Data.hxx
#include <TROOT.h>
#include <TClass.h>
#include <TStreamerInfo.h>
#include <TStreamerElement.h>
#include <TTreeFormula.h>
#include "Data.hxx"
using namespace std;



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Helper Functions                                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void_pointer_add                                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
/// Add an offset to a void* pointer
void void_pointer_add(volatile void*& initial, Int_t offset) {
  volatile char* temp =  reinterpret_cast<volatile char*>(initial);
  temp += offset;
  initial = reinterpret_cast<volatile void*>(temp);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// remove_duplicate_spaces                               //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
/// Remove adjacent whitespace from a \c std::string
void remove_duplicate_spaces(string& str) {
  for(int i=str.size()-1; i > 0; --i) {
    if(str[i] == ' ' && str[i-1] == ' ')
      str.erase(i,1);
  }
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Helper Classes                                        //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// This is just a quick and dirty class to reconstruct the original indecex
// of a multi-dimensional array that has been flattened by TStreamerElemens.
// All we really need out is just strings with the original indices in brackets.
class ArrayConverter
{
private:
  TStreamerElement* fElement;
  vector<string> fIndices;

public:
  ArrayConverter(TStreamerElement* element) :
    fElement(element) {
    stringstream sstr;
    Int_t ndim = fElement->GetArrayDim();
    vector<UInt_t> index(4, 0);

    // Lazy way of "flattening" the array, just use nested for loops.  TStreamerElement
    // only supports up to 4-dimensional arrays anyway so there's no loss. Though really this
    // should be done recursively withouth the for loops (or some other clever way).
    for(index[0] = 0; index[0] < fElement->GetMaxIndex(0); ++index[0]) {
      for(index[1] = 0; index[1] < (ndim > 1 ? fElement->GetMaxIndex(1) : 1) ; ++index[1]) {
	for(index[2] = 0; index[2] < (ndim > 2 ? fElement->GetMaxIndex(2) : 1) ; ++index[2]) {
	  for(index[3] = 0; index[3] < (ndim > 3 ? fElement->GetMaxIndex(3) : 1) ; ++index[3]) {
	    sstr.str("");
	    for(Int_t i=0; i< ndim; ++i) {
	      sstr << "[" << index[i] << "]";
	    }
	    fIndices.push_back(sstr.str());
	  }
	}
      }
    }
  }
  // Return the original indices.
  string GetFullName(const char* baseName, UInt_t index) {
    if(index < fIndices.size()) {
      string out = baseName;
      out += fIndices[index];
      return out;
    }
    else {
      fprintf(stderr, "Invalid index %d\n", index);
      return "";
    }
  }
};


    
  



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                                 //
// rb::Data                                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Static data member initialization                     //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::Data::Map_t       rb::Data::fgMap;
rb::Data::SetMap_t    rb::Data::fgSetFunctionMap;
rb::Data::GetMap_t    rb::Data::fgGetFunctionMap;
rb::Data::ObjectMap_t rb::Data::fgObjectMap;
rb::Data::DeleteMap_t rb::Data::fgDeleteMap;


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::Data::Data(const char* name, const char* class_name, volatile void* data, Bool_t createPointer):
  kName(name), kClassName(class_name), kMapClass(createPointer) {
      fData = data;
      fgMap[kName] = this;


#define FMAP_INSERT(KEY)						\
      fgSetFunctionMap.insert(std::make_pair(#KEY , &SetDataValue<KEY>)); \
      fgGetFunctionMap.insert(std::make_pair(#KEY , &GetDataValue<KEY>));

      if(fgSetFunctionMap.empty()) {
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
#undef FMAP_INSERT
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::Data::Getvalue                                    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Double_t rb::Data::GetValue(const char* name) {
  ObjectMapIterator_t it = fgObjectMap.find(name);
  if(it == fgObjectMap.end()) {
    Error("GetValue", "%s not found.", name);
    return -1.;
  }
  string typeName = it->second.second;

  GetMapIterator_t itGet = fgGetFunctionMap.find(typeName);
  if(itGet == fgGetFunctionMap.end()) {
    Error("GetValue", "Invalid type %s", typeName.c_str());
    return -1.;
  }
  return Double_t(itGet->second(name));
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// static rb::Data::SetValue                             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Data::SetValue(const char* name, Double_t newvalue) {
  ObjectMapIterator_t itObject = fgObjectMap.find(name);
  if(itObject == fgObjectMap.end()) {
    Error("SetData", "Data object: %s not found.", name);
    return;
  }

  volatile void* objectAddress = itObject->second.first;
  string type = itObject->second.second;
  SetMapIterator_t itCast = fgSetFunctionMap.find(type);
  if(itCast == fgSetFunctionMap.end()) {
    Error("SetData", "Invalid type: %s", type.c_str());
    return;
  }

  void_cast castFunction = itCast->second;
  castFunction(objectAddress, newvalue);
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// static branch adding function                         //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Data::AddBranches() {
  MapIterator_t it = fgMap.begin();
  while(it != rb::Data::fgMap.end()) {
    rb::Data* data = (*it++).second;
    std::string brName = data->kName; brName += ".";
    LockingPointer<char> pData (reinterpret_cast<volatile char*>(data->fData), data->fMutex);
    void* v = reinterpret_cast<void*>(pData.Get());
    rb::Hist::AddBranch(brName.c_str(), data->kClassName.c_str(), &v);
  }
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// static class mapping function                         //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Data::MapClasses() {
  vector<string> vPrint; /// For message printing
  vPrint.push_back("\nMapping the address of user data objects:\n");
  vPrint.push_back("      Name\t\tClass Name\n");
  vPrint.push_back("      ----\t\t----------\n");
  Int_t printMinSize = vPrint.size();

  for(MapIterator_t it = fgMap.begin(); it != fgMap.end(); ++it) {
    if(it->second->kMapClass) {
      string name = it->first;
      string className = it->second->kClassName;

      stringstream sPrint;
      sPrint << "      " << name << "\t\t" << className << endl;
      vPrint.push_back(sPrint.str());

      MapClass(name.c_str(), className.c_str(), it->second->fData);
    }
  }

  if(vPrint.size() > printMinSize) {
    for(UInt_t u=0; u< vPrint.size(); ++u) {
      cout << vPrint[u];
    }
    cout << "\n\n";
  }
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// static rb::Data::SavePrimitive                        //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Data::SavePrimitive(ostream& strm) {
  ObjectMapIterator_t it;
  for(it = fgObjectMap.begin(); it != fgObjectMap.end(); ++it) {
    strm << "  rb::Data::SetValue(\"" << it->first << "\", " << GetValue(it->first.c_str()) << ");\n";
  }
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// static rb::Data::MapData                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t rb::Data::MapData(const char* name, TStreamerElement* element, volatile void* base_address) {

  string typeName = element->GetTypeName();
  remove_duplicate_spaces(typeName); // in case someone did 'unsigned    short' or whatever

  SetMapIterator_t it = fgSetFunctionMap.find(typeName);
  if(it == fgSetFunctionMap.end()) return kFALSE;

  volatile void* address = base_address;
  void_pointer_add(address, element->GetOffset());
  Int_t arrLen = element->GetArrayLength();

  if(!arrLen) {
    fgObjectMap.insert(std::make_pair(name, std::make_pair(address, typeName)));
  }
  else {
    Int_t size = element->GetSize() / arrLen;
    ArrayConverter arrayConvert(element);
    for(Int_t i=0; i< arrLen; ++i) {
      void_pointer_add(address, size*(i>0));
      fgObjectMap.insert(std::make_pair(arrayConvert.GetFullName(name, i), std::make_pair(address, typeName)));
    }
  }
  return kTRUE;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// static rb::Data::MapClass                             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Data::MapClass(const char* name, const char* classname, volatile void* address) {

  TClass* cl = TClass::GetClass(classname);
  if(!cl) return;

  TStreamerInfo* sinfo = static_cast<TStreamerInfo*>(cl->GetStreamerInfo());
  TObjArray* elems = sinfo->GetElements();

  for( Int_t i=0; i< elems->GetEntries(); ++i) {
    TStreamerElement* element =
      reinterpret_cast <TStreamerElement*>(elems->At(i));
    stringstream ssName;
    ssName << name << "." << element->GetName();
    Bool_t basic = MapData(ssName.str().c_str(), element, address);
    if(!basic) {
      void_pointer_add(address, element->GetOffset());
      MapClass(ssName.str().c_str(), element->GetTypeName(), address);
    }
  }
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// static rb::Data::PrintAll                             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::Data::PrintAll() {
  ObjectMapIterator_t it = fgObjectMap.begin();
  while(it != fgObjectMap.end()) {
    string name = (*it++).first;
    cout << name << " = " << GetValue(name.c_str()) << endl;
  }
}
