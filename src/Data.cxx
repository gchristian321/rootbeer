//! \file Data.cxx
//! \brief Implements Data.hxx
#include <algorithm>
#include "Data.hxx"
#include "Rint.hxx"


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                 //
// rb::data::MBasic Implementation       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
#define CHECK_TYPE(type)						\
  else if (!strcmp(basic_type_name, #type))				\
    return new rb::data::Basic<type> (name, addr, basic_type_name, element)
rb::data::MBasic* rb::data::MBasic::New(const char* name, volatile void* addr, const char* basic_type_name, TStreamerElement* element) {
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
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::MBasic::fgAll() [static]     //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::data::MBasic::Map_t& rb::data::MBasic::fgAll() {
  static rb::data::MBasic::Map_t* m = new rb::data::MBasic::Map_t();
  return *m;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// MBasic* rb::data::MBasic::Find() [static]   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::data::MBasic* rb::data::MBasic::Find(const char* name) {
  rb::data::MBasic::Map_t::iterator it = fgAll().find(std::string(name));
  return it != fgAll().end() ? it->second : 0;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Sub Class                             //
// rb::data::MBasic::Printer             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::MBasic::Printer::SavePrimitive()   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::data::MBasic::Printer::SavePrimitive(std::ostream& strm) {
  if(fgAll().empty()) return;
  rb::data::MBasic::Map_t::iterator it;
  for(it = fgAll().begin(); it != fgAll().end(); ++it) {
    strm << "  rb::Rb::Data::SetValue(\"" << it->first << "\", " << it->second->GetValue() << ");\n";
  }
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::MBasic::Printer::PrintAll()        //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::data::MBasic::Printer::PrintAll() {
  if(fgAll().empty()) return;
  std::vector<std::string> names, values, classes;

  rb::data::MBasic::Map_t::iterator it = fgAll().begin();
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


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                 //
// rb::data::Mapper Implementation       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Bool_t rb::data::Mapper::MapElement()  //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t rb::data::Mapper::MapElement(const char* name, TStreamerElement* element, volatile void* base_address) {
  std::string typeName = element->GetTypeName(); // the type of this specific element
  remove_duplicate_spaces(typeName); // in case someone did 'unsigned    short' or whatever

  volatile void* address = base_address; // address of base pointer to the whole class containing this element
  void_pointer_add(address, element->GetOffset()); // increment to address of this specific element
  rb::data::MBasic* basicData = rb::data::MBasic::New(name, address, typeName.c_str(), element); // try to create an data::MBasic derived instance

  return (basicData == 0);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::Mapper::MapClass()      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::data::Mapper::MapClass(const char* name, const char* classname, volatile void* address) {

  TClass* cl = TClass::GetClass(classname);
  if(!cl) return;

  TStreamerInfo* sinfo = static_cast<TStreamerInfo*>(cl->GetStreamerInfo());
  TObjArray* elems = sinfo->GetElements();

  for(Int_t i=0; i< elems->GetEntries(); ++i) {
    TStreamerElement* element = reinterpret_cast<TStreamerElement*>(elems->At(i));
    std::stringstream ssName;
    ssName << name << "." << element->GetName();
    Bool_t basic = MapElement(ssName.str().c_str(), element, address);
    if(!basic) {
      void_pointer_add(address, element->GetOffset());
      MapClass(name, element->GetTypeName(), address);
    }
  }
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::Mapper::Message()       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::data::Mapper::Message(const char* brName, const char* clName) {
  std::stringstream sstr;
  static Bool_t printHeader(kTRUE);
  if(printHeader) {
    sstr << "\nMapping the address of user data objects:\n"
	 << "      Name\t\t\tClass Name\n"
	 << "      ----\t\t\t----------\n";
    printHeader = kFALSE;
  }
  sstr << "      " << brName << "\t\t\t" << clName << "\n";
  rb::gApp()->AddMessage(sstr.str());
}
