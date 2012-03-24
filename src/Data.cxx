//! \file Data.cxx
//! \brief Implements Data.hxx
#include <algorithm>
#include "Rint.hxx"
#include "Data.hxx"


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                 //
// rb::data::MBasic Implementation       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// MBasic* rb::data::MBasic::New() [static]    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::data::MBasic::New(const char* name, volatile void* addr, TDataMember* d) {
#define CHECK_TYPE(type)																								\
  else if (!strcmp(d->GetTrueTypeName(), #type)) {											\
    rb::data::MBasic* m = new rb::data::Basic<type> (name, addr, d);		\
    if(!m) err::Error("data::MBasic::New") << "Constructor returned a NULL pointer"; }
  if(0);
  CHECK_TYPE(double)
		 CHECK_TYPE(float)
		 CHECK_TYPE(long long)
		 CHECK_TYPE(long)
		 CHECK_TYPE(int)
		 CHECK_TYPE(short)
		 CHECK_TYPE(char)
		 CHECK_TYPE(bool)
		 CHECK_TYPE(unsigned long long)
		 CHECK_TYPE(unsigned long)
		 CHECK_TYPE(unsigned int)
		 CHECK_TYPE(unsigned short)
		 CHECK_TYPE(unsigned char)
  else;
#undef CHECK_TYPE
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
namespace { // Helper functions for PrintAll()
inline bool string_len_compare (const std::string& lhs, const std::string& rhs) {
	return rhs.size() > lhs.size();
}
inline std::string double2str(Double_t d) {
	std::stringstream sstr;
	sstr << d;
	return sstr.str();
}
}
void rb::data::MBasic::Printer::PrintAll() {
  if(fgAll().empty()) return;
  std::vector<std::string> names, values, classes;

  rb::data::MBasic::Map_t::iterator it = fgAll().begin();
  while(it != fgAll().end()) {
    names.push_back(it->first);
    values.push_back(double2str(it->second->GetValue()));
    classes.push_back(it->second->fDataMember->GetTrueTypeName());
    ++it;
  }
  Int_t maxName  = max_element(names.begin(), names.end(), string_len_compare)->size();
  maxName = maxName > 4 ? maxName : 4;

  printf("\n%-*s\t%s\n", maxName, "Name", "Value [type]");
  printf("%-*s\t%s\n", maxName, "----", "-------------");
  for(UInt_t i=0; i< names.size(); ++i) {
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
namespace { // Helper Functions & Class //
inline Bool_t ShouldBeMapped(TDataMember* d, bool exclude_hash = true) {
	std::string title = d->GetTitle();
	if(title.size() == 0) return true;
	if(exclude_hash) return !(title[0] == '!' || title[0] == '#');
	else             return !(title[0] == '!');
}
class ArrayConverter // Class to reconstruct the original indices				       
{		       // of a multi-dimensional array that has been flattened by TStreamerElements.
public:	       // Gives us strings with the original indices in brackets.
	 ArrayConverter(TDataMember* d);
	 // Return the original indices.
	 std::string GetFullName(const char* baseName, UInt_t index);
	 Int_t GetArrayLength() { return fArrayLength; }
private:
	 TDataMember* fElement;
	 std::vector<std::string> fIndices;
	 Int_t fArrayLength;
}; // class ArrayConverter
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::Mapper::HandleBasic()   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::data::Mapper::HandleBasic(TDataMember* d, const char* name) {
  Long_t addr = kBase + d->GetOffset();
  Int_t nDim = d->GetArrayDim();
  if(nDim == 0) { // not an array
    rb::data::MBasic::New(name, reinterpret_cast<void*>(addr), d);
  }
  else if(d->GetArrayDim() > 4) // too big
		 Warning("MapData",
						 "No support for arrays > 4 dimensions. The array %s is %d and will not be mapped!",
						 name, d->GetArrayDim());
  else {
    ArrayConverter ac(d);
    Int_t arrayLen = ac.GetArrayLength();
    Int_t size = d->GetUnitSize();
    for(Int_t i=0; i< arrayLen; ++i) {
      addr += size*(i>0);
      rb::data::MBasic::New(ac.GetFullName(name, i).c_str(), reinterpret_cast<void*>(addr), d);
    }
  }  
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::Mapper::InsertBasic()   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::data::Mapper::InsertBasic(TDataMember* d, std::vector<std::string>& v_names, const char* name) {
  Long_t addr = kBase + d->GetOffset();
  Int_t nDim = d->GetArrayDim();
  if(nDim == 0) { // not an array
		v_names.push_back(name);
  }
  else if(d->GetArrayDim() > 4) // too big
		 Warning("MapData",
						 "No support for arrays > 4 dimensions. The array %s is %d and will not be mapped!",
						 name, d->GetArrayDim());
  else {
    ArrayConverter ac(d);
    Int_t arrayLen = ac.GetArrayLength();
    Int_t size = d->GetUnitSize();
    for(Int_t i=0; i< arrayLen; ++i) {
      addr += size*(i>0);
			v_names.push_back(ac.GetFullName(name, i).c_str());
    }
  }  
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::Mapper::MapClass()      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace {
inline std::string append_name(const std::string& base, const char* toAppend) {
  std::stringstream out;
  out << base << "." << toAppend;
  return out.str();
}
}
void rb::data::Mapper::MapClass() {
  TClass* cl = TClass::GetClass(kClassName.c_str());
  if(!cl) return;
  TList* dataMembers = cl->GetListOfDataMembers();
  for(Int_t i=0; i< dataMembers->GetEntries(); ++i) {
    TDataMember* d = reinterpret_cast<TDataMember*>(dataMembers->At(i));
    if(!ShouldBeMapped(d)) continue;

    std::string newName = append_name(kBranchName, d->GetName());
    if(d->IsBasic())
			 HandleBasic(d, newName.c_str());
    else {
      Long_t addr = kBase + d->GetOffset();
      Mapper sub_mapper(newName.c_str(), d->GetTrueTypeName(), addr, false);
      sub_mapper.MapClass();
    }
  }
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::Mapper::ReadBranches()  //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::data::Mapper::ReadBranches(std::vector<std::string>& branches) {
  TClass* cl = TClass::GetClass(kClassName.c_str());
  if(!cl) return;
  TList* dataMembers = cl->GetListOfDataMembers();
  for(Int_t i=0; i< dataMembers->GetEntries(); ++i) {
    TDataMember* d = reinterpret_cast<TDataMember*>(dataMembers->At(i));
    if(!ShouldBeMapped(d, false)) continue;

    std::string newName = append_name(kBranchName, d->GetName());
    if(d->IsBasic())
			 InsertBasic(d, branches, newName.c_str());
    else {
      Long_t addr = kBase + d->GetOffset();
      Mapper sub_mapper(newName.c_str(), d->GetTrueTypeName(), addr, false);
      sub_mapper.ReadBranches(branches);
    }
  }
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::Mapper::Message()       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::data::Mapper::Message() {
  std::stringstream sstr;
  static Bool_t printHeader(kTRUE);
  if(printHeader) {
    sstr << "\nMapping the address of user data objects:\n"
				 << "      Name\t\t\tClass Name\n"
				 << "      ----\t\t\t----------\n";
    printHeader = kFALSE;
  }
  sstr << "      " << kBranchName << "\t\t\t" << kClassName << "\n";
  rb::gApp()->AddMessage(sstr.str());
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class ArrayConverter                   //
// Implementation                         //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
ArrayConverter::ArrayConverter(TDataMember* d) : fElement(d), fArrayLength(0) {
  for(Int_t j=0; j< d->GetArrayDim(); ++j)
		 fArrayLength += d->GetMaxIndex(j);

  std::stringstream sstr;
  Int_t ndim = fElement->GetArrayDim();
  std::vector<Int_t> index(4, 0);

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
std::string ArrayConverter::GetFullName(const char* baseName, UInt_t index)  {
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
