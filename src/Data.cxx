//! \file Data.cxx
//! \brief Implements Data.hxx
//! \todo Handle arrays at a level lower than the final one.
#include <set>
#include <list>
#include <deque>
#include <vector>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <TRealData.h>
#include "Rint.hxx"
#include "Data.hxx"
#include "utils/Error.hxx"
#include "utils/ANSort.hxx"

#define ADD_STL(CONTAINER, TYPE) do {																		\
		std::stringstream typeStr;																					\
		typeStr << #CONTAINER << "<"	<< #TYPE << ">";											\
		TClass* cl = TClass::GetClass(typeStr.str().c_str());								\
		if(!cl) err::Error("ADD_STL") << "No TClass for " << typeStr.str();	\
		assert(cl);																													\
		fAddMap.insert(std::make_pair(cl, AddSTL<std::CONTAINER<TYPE>, TYPE>)); \
		fAddrMap.insert(std::make_pair(cl, AddrSTL<std::CONTAINER<TYPE> >)); \
		fLengthMap.insert(std::make_pair(cl, STLLength<std::CONTAINER<TYPE> >)); \
		fBasicTypeMap.insert(std::make_pair(cl, #TYPE));										\
	} while(0)

#define ADD_CONTAINERS(TYPE) do {								\
		ADD_STL(set, TYPE);													\
		ADD_STL(list, TYPE);												\
		ADD_STL(deque, TYPE);												\
		ADD_STL(vector, TYPE);											\
		ADD_STL(multiset, TYPE);										\
	} while(0)

namespace {

typedef void   (*STLAdd_t)(const char*, volatile void*, TDataMember*);
typedef Long_t (*STLAddr_t)(volatile void*, size_t);
typedef size_t (*STLLength_t)(volatile void*);
typedef std::map<TClass*, STLAdd_t> STLAddMap_t;
typedef std::map<TClass*, STLAddr_t> STLAddrMap_t;
typedef std::map<TClass*, STLLength_t> STLLengthMap_t;
typedef std::map<TClass*, std::string> STLBasicTypeMap_t;

/// Add all elements of an STL container to MBasic::fgData()
template <class T, class B>
void AddSTL(const char* name, volatile void* addr, TDataMember* d) {
	T* pSTL = (T*)addr;
	int i = 0;
	for(typename T::iterator it = pSTL->begin(); it != pSTL->end(); ++it) {
		Long_t addrThis = reinterpret_cast<Long_t>(&(*it));
		std::stringstream fullName;
		fullName << name << "[" << i++ << "]";
		rb::data::MBasic* m =
			new rb::data::Basic<B>(fullName.str().c_str(), (volatile void*)addrThis, d);
		if(!m) err::Error("AddSTL") << "Constructor returned a NULL pointer" << ERR_FILE_LINE;
	}
}

/// Get the length of an STL container at address addr
template <class T>
size_t STLLength(volatile void* addr) {
	T* p = (T*)addr;
	return p->size();
}

/// Get the address of element i in STL container at address addr
template <class T>
Long_t AddrSTL(volatile void* addr, size_t i) {
  T* p = (T*)addr;
  typename T::iterator it = p->begin();
  size_t indx = 0;
  while(indx != i && it != p->end()) {
    it++; indx++;
  }
  return it == p->end() ? 0 :  reinterpret_cast<Long_t>(&(*it));
}


class STLMaps
{
private:
	STLAddMap_t fAddMap;
	STLAddrMap_t fAddrMap;
	STLLengthMap_t fLengthMap;
	STLBasicTypeMap_t fBasicTypeMap;
public:
	STLAddMap_t* GetAddMap() { return &fAddMap; }
	STLAddrMap_t* GetAddrMap() { return &fAddrMap; }
	STLLengthMap_t* GetLengthMap() { return &fLengthMap; }
	STLBasicTypeMap_t* GetBasicTypeMap() { return &fBasicTypeMap; }
	STLMaps() {
		ADD_CONTAINERS(double);
		ADD_CONTAINERS(float);
		ADD_CONTAINERS(long long);
		ADD_CONTAINERS(long);
		ADD_CONTAINERS(int);
		ADD_CONTAINERS(short);
		ADD_CONTAINERS(char);
		ADD_CONTAINERS(unsigned long long);
		ADD_CONTAINERS(unsigned long);
		ADD_CONTAINERS(unsigned int);
		ADD_CONTAINERS(unsigned short);
		ADD_CONTAINERS(unsigned char);
	}
};

} // namespace

#undef ADD_CONTAINERS
#undef ADD_STL


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                 //
// rb::data::MReader Implementation      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

#define GET_READER(addr, type)																					\
  else if (!strcmp(typeName, #type))																		\
    retval = new rb::data::Reader<type> (addr)
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// MReader* rb::data::MReader::New() [static]  //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::data::MReader* rb::data::MReader::New(const char* typeName, Long_t dataAddress) {
	MReader* retval = 0;
	if(!dataAddress) {}
	GET_READER(dataAddress, double);
	GET_READER(dataAddress, float);
	GET_READER(dataAddress, long long);
	GET_READER(dataAddress, long);
	GET_READER(dataAddress, int);
	GET_READER(dataAddress, short);
	GET_READER(dataAddress, char);
	GET_READER(dataAddress, bool);
	GET_READER(dataAddress, unsigned long long);
	GET_READER(dataAddress, unsigned long);
	GET_READER(dataAddress, unsigned int);
	GET_READER(dataAddress, unsigned short);
	GET_READER(dataAddress, unsigned char);
	else {}
	return retval;
}
#undef GET_READER

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                 //
// rb::data::MBasic Implementation       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// MBasic* rb::data::MBasic::New() [static]    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::data::MBasic::New(const char* name, volatile void* addr, TDataMember* d) {

#define CHECK_TYPE(type) do {																						\
		if (!strcmp(d->GetTrueTypeName(), #type)) {													\
			rb::data::MBasic* m = new rb::data::Basic<type> (name, addr, d);	\
			if(!m) err::Error("data::MBasic::New") << "Constructor returned a NULL pointer"; \
			return;																														\
		}	 } while(0)


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
	CHECK_TYPE(unsigned short);
	CHECK_TYPE(unsigned char);

	static STLMaps stlMaps;
	STLAddMap_t::iterator it = stlMaps.GetAddMap()->find(TClass::GetClass(d->GetTrueTypeName()));
	if(it == stlMaps.GetAddMap()->end()) { // no support for the requested STL container
		err::Error("MReader::New") << "No support for STL class: \"" << d->GetTrueTypeName()
															 << "\"." << ERR_FILE_LINE;
		return;
	}
	else { // call "adder" function
		it->second(name, addr, d);
		return;
	}

#undef CHECK_TYPE
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::data::MBasic::GetAll() [static]         //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
std::vector<std::string> rb::data::MBasic::GetAll() {
	std::vector<std::string> out;
	for(Map_t::iterator it = fgAll().begin(); it != fgAll().end(); ++it) {
		out.push_back(it->first);
	}
	return out;
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
namespace { ANSort ansort; }
void rb::data::MBasic::Printer::SavePrimitive(std::ostream& strm) {
  if(fgAll().empty()) return;
  rb::data::MBasic::Map_t::iterator it;
	std::vector<std::string> names;
  for(it = fgAll().begin(); it != fgAll().end(); ++it) {
		names.push_back(it->first);
	}
	std::sort(names.begin(), names.end(), ansort);
	for(UInt_t i=0; i< names.size(); ++i) {
		it = fgAll().find(names[i]);
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
} }
void rb::data::MBasic::Printer::PrintAll() {
  if(fgAll().empty()) return;
  std::vector<std::string> names, values, classes;

  rb::data::MBasic::Map_t::iterator it = fgAll().begin();
  while(it != fgAll().end()) {
    names.push_back(it->first); ++it;
	}
	std::sort(names.begin(), names.end(), ansort);

	for(UInt_t i=0; i< names.size(); ++i) {
		it = fgAll().find(names[i]);
    values.push_back(double2str(it->second->GetValue()));
    classes.push_back(it->second->fDataMember->GetTrueTypeName());
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
	if(exclude_hash) return !(title[0] == '#');
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
  else if(d->GetArrayDim() > 4) { // too big
		err::Warning("MapData")
			<< "No support for arrays > 4 dimensions. The array " << name
			<< " is " << d->GetArrayDim() << " dimensions and will not be mapped!"
			<< ERR_FILE_LINE;
	}
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
// void rb::data::Mapper::HandleSTL()     //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::data::Mapper::HandleSTL(TDataMember* d, const char* name) {
  Long_t addr = kBase + d->GetOffset();
  Int_t nDim = d->GetArrayDim();
  if(nDim == 0) { // not an array
    rb::data::MBasic::New(name, reinterpret_cast<void*>(addr), d);
  }
  else if(d->GetArrayDim() > 4) { // too big
		err::Warning("MapData")
			<< "No support for arrays > 4 dimensions. The array " << name
			<< " is " << d->GetArrayDim() << " dimensions and will not be mapped!"
			<< ERR_FILE_LINE;
	}
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
  else if(d->GetArrayDim() > 4) { // too big
		err::Warning("MapData")
			<< "No support for arrays > 4 dimensions. The array " << name
			<< " is " << d->GetArrayDim() << " dimensions and will not be mapped!"
			<< ERR_FILE_LINE;
	}
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
// void rb::data::Mapper::InsertSTL()     //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::data::Mapper::InsertSTL(TDataMember* d, std::vector<std::string>& v_names, const char* name) {
	static STLMaps stlMaps;

  Long_t addr = kBase + d->GetOffset();
  Int_t nDim = d->GetArrayDim();
  if(nDim == 0) { // not an array
		v_names.push_back(name);
  }
  else if(d->GetArrayDim() > 4) { // too big
		err::Warning("MapData")
			<< "No support for arrays > 4 dimensions. The array " << name
			<< " is " << d->GetArrayDim() << " dimensions and will not be mapped!"
			<< ERR_FILE_LINE;
	}
  else {
    ArrayConverter ac(d);
    Int_t arrayLen = ac.GetArrayLength();
		Int_t size = d->GetUnitSize();

		// Loop over array elements
    for(Int_t i=0; i< arrayLen; ++i) {
			addr += size*(i>0); // address of the STL container
			size_t lengthSTL = 0; // STL container length
			STLLengthMap_t::iterator it =
				stlMaps.GetLengthMap()->find(TClass::GetClass(d->GetTrueTypeName()));

			if(it == stlMaps.GetLengthMap()->end()) { // no support for the requested STL container
				err::Error("MReader::New") << "No support for STL class: \"" << d->GetTrueTypeName()
																	 << "\"." << ERR_FILE_LINE;
				return;
			}

			lengthSTL = it->second((volatile void*)addr);
			for(size_t j=0; j< lengthSTL; ++j) {
				std::stringstream fullname;
				fullname << ac.GetFullName(name, i) << "[" << j << "]";
				v_names.push_back(fullname.str());
			}
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
  if(!cl) {
		err::Warning("MapClass")
			<< "Unable to map the class \"" << kClassName << "\" because it doesn not "
			<< "have a ROOT Dictionary." << ERR_FILE_LINE;
		return;
	}
  TList* dataMembers = cl->GetListOfDataMembers();
  for(Int_t i=0; i< dataMembers->GetEntries(); ++i) {
    TDataMember* d = reinterpret_cast<TDataMember*>(dataMembers->At(i));
    if(!ShouldBeMapped(d)) continue;
    std::string newName = append_name(kBranchName, d->GetName());
    if(d->IsBasic())
			HandleBasic(d, newName.c_str());
		else if(d->IsSTLContainer())
			HandleSTL(d, newName.c_str());
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
		else if(d->IsSTLContainer())
			InsertSTL(d, branches, newName.c_str());
    else {
      Long_t addr = kBase + d->GetOffset();
      Mapper sub_mapper(newName.c_str(), d->GetTrueTypeName(), addr, false);
      sub_mapper.ReadBranches(branches);
    }
  }
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Long_t rb::data::Mapper::FindBasicAddr()   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace {
inline Long_t FindSTLAddr(const char* name, Long_t baseAddr, TDataMember* dataMember, TRealData* realData) {
	Long_t retval = 0;
	Int_t nDim = dataMember->GetArrayDim();
	static STLMaps stlMaps;

	if(nDim > 4) { // too big
		Warning("FindSTLAddr",
						"No support for arrays > 4 dimensions. The array %s is %d and will not be mapped!",
						name, dataMember->GetArrayDim());
	}
	else if (nDim == 0) { // single element
		STLAddrMap_t::iterator it = stlMaps.GetAddrMap()->find(TClass::GetClass(dataMember->GetTrueTypeName()));
		if (it == stlMaps.GetAddrMap()->end()) {
			err::Error("FindSTLAddr")
				<< "No support for the STL class: " << dataMember->GetTrueTypeName() << ERR_FILE_LINE;
		}
		else {
			Long_t thisAddr = baseAddr + realData->GetThisOffset();
			std::string strName(name);
			int indx =
				atoi( strName.substr(strName.find("[")+1, strName.find("]") - strName.find("[")-1).c_str() );
			retval = it->second((volatile void*)thisAddr, indx);
		}
	}
	else { // valid aray
		ArrayConverter ac(dataMember);
		Int_t arrayLen = ac.GetArrayLength();
		Int_t size = dataMember->GetUnitSize();
		Long_t thisAddr = baseAddr + realData->GetThisOffset();

		std::string name0 = name;
		std::string strName = name0.substr(0, name0.rfind("["));
		int indx =
			atoi ( name0.substr(name0.rfind("[") + 1, name0.rfind("]") - name0.rfind("[") - 1).c_str() );

		for(Int_t i=0; i< arrayLen; ++i) {
			thisAddr += size*(i>0);
			std::string nnn = name0.rfind(".") < name0.size() ? name0.substr(0, name0.rfind(".") + 1) : "";
			nnn += dataMember->GetName();

			if(ac.GetFullName(nnn.c_str(), i) == strName) {
				STLAddrMap_t::iterator it = stlMaps.GetAddrMap()->find(TClass::GetClass(dataMember->GetTrueTypeName()));
				if (it == stlMaps.GetAddrMap()->end()) {
					err::Error("FindSTLAddr")
						<< "No support for the STL class: " << dataMember->GetTrueTypeName() << ERR_FILE_LINE;
				}
				else {
					retval = it->second((volatile void*)thisAddr, indx);
					break;
				}
			}
		}
	}
	return retval;
}

} // namespace

Long_t rb::data::Mapper::FindBasicAddr(const char* name, TDataMember** data_member) {
  TClass* cl = TClass::GetClass(kClassName.c_str());
	TRealData* realData = cl ? cl->GetRealData(name) : 0;
	if(!realData && cl) { // try w/o brackets
		std::string name2 = std::string(name).substr(0, std::string(name).find("["));
		realData = cl->GetRealData(name2.c_str());
	}
	TDataMember* dataMember = realData ? realData->GetDataMember() : 0;
	if(!dataMember) return 0;

	if(data_member) *data_member = dataMember;
	Long_t retval = 0;

	if(dataMember->IsBasic()) { // Handle Basic Data Types inline
		Int_t nDim = dataMember->GetArrayDim();
		if(nDim == 0) { // not an array
			retval = kBase + realData->GetThisOffset();
		}
		else if(dataMember->GetArrayDim() > 4) { // too big
			Warning("FindBasic",
							"No support for arrays > 4 dimensions. The array %s is %d and will not be mapped!",
							name, dataMember->GetArrayDim());
		}
		else {
			ArrayConverter ac(dataMember);
			Int_t arrayLen = ac.GetArrayLength();
			Int_t size = dataMember->GetUnitSize();
			Long_t addr = kBase + realData->GetThisOffset();
			for(Int_t i=0; i< arrayLen; ++i) {
				addr += size*(i>0);
				std::string strName(name);
				std::string nnn = strName.rfind(".") < strName.size() ? strName.substr(0, strName.rfind(".") + 1) : "";
				nnn += dataMember->GetName();
				if(ac.GetFullName(nnn.c_str(), i) == strName) {
					retval = addr;
					break;
				}
			}
		}
	}

	else if (dataMember->IsSTLContainer()) { // delegate to FindSTLAddr function
		retval = FindSTLAddr(name, kBase, dataMember, realData);
	}

	else { // bail out
		err::Error("FindBasicAddr") << "Unsupported \"final\" data type: "
																<< dataMember->GetTrueTypeName() << ERR_FILE_LINE;
	}

	return retval;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::data::MReader* rb::data::Mapper::FindBasicReader()   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::data::MReader* rb::data::Mapper::FindBasicReader(const char* name, TDataMember** data_member) {
	TDataMember* d = 0;
	Long_t retval = FindBasicAddr(name, &d);
	if(data_member) *data_member = d;
	if(d && d->IsBasic())
		return retval ? rb::data::MReader::New(d->GetTrueTypeName(), retval) : 0;
	else {
		static STLMaps stlMaps;
		STLBasicTypeMap_t::iterator it = stlMaps.GetBasicTypeMap()->find(TClass::GetClass(d->GetTrueTypeName()));
		if(it == stlMaps.GetBasicTypeMap()->end()) {
			err::Error("FindBasicReader")
				<< "Couldn't figure out the basic type stored in container: \""
				<< d->GetTrueTypeName() << "\"" << ERR_FILE_LINE;
			return 0;
		}
		std::string strType = it->second;
		return retval ? rb::data::MReader::New(strType.c_str(), retval) : 0;
	}
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::data::Mapper::Message()       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::data::Mapper::Message() {
	char buf[4096];
  std::stringstream sstr;
  static Bool_t printHeader(kTRUE);
  if(printHeader) {
		sprintf(buf, "\nMapping the address of user data objects:\n"); sstr << buf;
		sprintf(buf, "%-30s%-30s\n", "      Name",  "Class Name");     sstr << buf;
		sprintf(buf, "%-30s%-30s\n", "      ----",  "----------");     sstr << buf;
    printHeader = kFALSE;
  }
	sprintf(buf, "      %-24s%-30s\n", kBranchName.c_str(), kClassName.c_str()); sstr << buf;
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
