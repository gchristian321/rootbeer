//! \file Data.cxx
//! \brief Implements Data.hxx
#include <algorithm>
#include <TROOT.h>
#include <TTreeFormula.h>
#include "Data.hxx"


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Internal Helper Functions & Classes                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  // void_pointer_add                                      //
  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  // Add an offset to a void* pointer
  void void_pointer_add(volatile void*& initial, Int_t offset) {
    volatile char* temp =  reinterpret_cast<volatile char*>(initial);
    temp += offset;
    initial = reinterpret_cast<volatile void*>(temp);
  }

  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  // remove_duplicate_spaces                               //
  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  // Remove adjacent whitespace from a \c std::string
  void remove_duplicate_spaces(std::string& str) {
    for(int i=str.size()-1; i > 0; --i) {
      if(str[i] == ' ' && str[i-1] == ' ')
	str.erase(i,1);
    }
  }

  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  // double2str                                            //
  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  // Convert a double to a std::string
  inline std::string double2str(Double_t d) {
    std::stringstream sstr;
    sstr << d;
    return sstr.str();
  }

  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  // string_len_compare                                    //
  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  // Tell is a string is longer than the other
  bool string_len_compare (const std::string& lhs, const std::string& rhs) {
    return rhs.size() > lhs.size();
  }

  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
  // Helper Classes                                        //
  //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

  ////////////////////////////////////////////////////
  ///// MBasicData::ArrayConverter Implementation ////
  ////////////////////////////////////////////////////
  MBasicData::ArrayConverter::ArrayConverter(TStreamerElement* element) :
    fElement(element) {
    std::stringstream sstr;
    Int_t ndim = fElement->GetArrayDim();
    std::vector<UInt_t> index(4, 0);

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

  std::string MBasicData::ArrayConverter::GetFullName(const char* baseName, UInt_t index) {
    if(index < fIndices.size()) {
      std::string out = baseName;
      out += fIndices[index];
      return out;
    }
    else {
      fprintf(stderr, "Invalid index %d\n", index);
      return "";
    }
  }


  ////////////////////////////////////
  ///// MBasicData Implementation ////
  ////////////////////////////////////
#define CHECK_TYPE(type)						\
  else if (!strcmp(basic_type_name, #type))				\
    return new BasicData<type> (name, addr, basic_type_name, element)
  MBasicData* MBasicData::New(const char* name, volatile void* addr, const char* basic_type_name, TStreamerElement* element) {
    if(0);
    CHECK_TYPE(Double_t);
    CHECK_TYPE(Float_t);
    CHECK_TYPE(Long64_t);
    CHECK_TYPE(Long_t);
    CHECK_TYPE(Int_t);
    CHECK_TYPE(Short_t);
    CHECK_TYPE(Char_t);
    CHECK_TYPE(Bool_t);
    CHECK_TYPE(ULong64_t);
    CHECK_TYPE(ULong_t);
    CHECK_TYPE(UInt_t);
    CHECK_TYPE(UShort_t);
    CHECK_TYPE(double);
    CHECK_TYPE(float);
    CHECK_TYPE(long long);
    CHECK_TYPE(long);
    CHECK_TYPE(int);
    CHECK_TYPE(short);
    CHECK_TYPE(char);
    CHECK_TYPE(bool);
    CHECK_TYPE(unsigned long long);
    CHECK_TYPE(unsigned long);
    CHECK_TYPE(unsigned int);
    CHECK_TYPE(unsigned);
    CHECK_TYPE(unsigned short);
    else return 0;
  }
#undef CHECK_TYPE


MBasicData::MBasicData(const char* branch_name, const char* basic_type_name) :
  kBranchName(branch_name), kBasicClassName(basic_type_name) {}

  MBasicData::Map_t& MBasicData::fgAll() {
    static MBasicData::Map_t* m = new MBasicData::Map_t();
    return *m;
  }

  MBasicData* MBasicData::Find(const char* name) {
    MBasicData::Map_t::iterator it = fgAll().find(std::string(name));
    return it != fgAll().end() ? it->second : 0;
  }

  void MBasicData::SavePrimitive(std::ostream& strm) {
    if(fgAll().empty()) return;
    MBasicData::Map_t::iterator it;
    for(it = fgAll().begin(); it != fgAll().end(); ++it) {
      strm << "  rb::Data::SetValue(\"" << it->first << "\", " << it->second->GetValue() << ");\n";
    }
  }

  void MBasicData::PrintAll() {
    if(fgAll().empty()) return;
    std::vector<std::string> names, values, classes;

    MBasicData::Map_t::iterator it = fgAll().begin();
    while(it != fgAll().end()) {
      names.push_back(it->first);
      values.push_back(double2str(it->second->GetValue()));
      classes.push_back(it->second->kBasicClassName);
      ++it;
    }
    Int_t maxName  = max_element(names.begin(), names.end(), string_len_compare)->size();
    maxName = maxName > 4 ? maxName : 4;

    printf("\n%-*s\t%s\n", maxName, "Name", "Value [class]");
    printf("%-*s\t%s\n", maxName, "----", "-------------");
    for(Int_t i=0; i< names.size(); ++i) {
      if(atoi(values.at(i).c_str()) < 0)
	printf("%-*s\t%s [%s]\n", maxName, names.at(i).c_str(), values.at(i).c_str(), classes.at(i).c_str());
      else
	printf("%-*s\t %s [%s]\n", maxName, names.at(i).c_str(), values.at(i).c_str(), classes.at(i).c_str());
    } printf("\n");
  }

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// static rb::MData::MapData                             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t MBasicData::MapData(const char* name, TStreamerElement* element, volatile void* base_address) {
  std::string typeName = element->GetTypeName(); // the type of this specific element
  remove_duplicate_spaces(typeName); // in case someone did 'unsigned    short' or whatever

  volatile void* address = base_address; // address of base pointer to the whole class containing this element
  void_pointer_add(address, element->GetOffset()); // increment to address of this specific element
  MBasicData* basicData = MBasicData::New(name, address, typeName.c_str(), element); // try to create an MBasicData derived instance

  return (basicData == 0);
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::MData::MapClass                                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void MBasicData::MapClass(const char* name, const char* classname, volatile void* address) {

  TClass* cl = TClass::GetClass(classname);
  if(!cl) return;

  TStreamerInfo* sinfo = static_cast<TStreamerInfo*>(cl->GetStreamerInfo());
  TObjArray* elems = sinfo->GetElements();

  for(Int_t i=0; i< elems->GetEntries(); ++i) {
    TStreamerElement* element = reinterpret_cast <TStreamerElement*>(elems->At(i));
    std::stringstream ssName;
    ssName << name << "." << element->GetName();
    Bool_t basic = MapData(ssName.str().c_str(), element, address);
    if(!basic) {
      void_pointer_add(address, element->GetOffset());
      MapClass(name, element->GetTypeName(), address);
    }
  }
}
