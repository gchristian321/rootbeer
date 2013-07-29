#include <map>
#include <string>
#include <iostream>
#include <TClass.h>
#include <TDataType.h>
#include <TRealData.h>
#include <TDataMember.h>
#include "ClassData.hxx"
#include "ClassFormula.hxx"
#include "utils/Error.hxx"

class Matrix2Vector
{
private:
	std::vector<Int_t> fMax;
public:
	Matrix2Vector() {}
	void AddDim(Int_t max) {
		fMax.push_back(max);
	}
	Int_t GetNdim() const { return fMax.size(); }
	Int_t GetVectorIndex(const std::vector<Int_t>& matrixIndices) {
		if((Int_t)matrixIndices.size() != GetNdim()) return -1;

		Int_t retval = 0;
		for(UInt_t i=0; i< matrixIndices.size(); ++i) {
			if(matrixIndices.at(i) >= fMax.at(i)) return -1;
			Int_t thisSize = 1;
			for(UInt_t j=i+1; j< matrixIndices.size(); ++j) {
				thisSize *= fMax.at(j);
			}
			retval += thisSize * matrixIndices.at(i);
		}
		return retval;
	}

	Int_t GetVectorIndex(Int_t i1, Int_t i2=-1, Int_t i3=-1, Int_t i4=-1) {
		std::vector<Int_t> v;
		v.push_back(i1);
		if(i2>=0) v.push_back(i2);
		if(i3>=0) v.push_back(i3);
		if(i4>=0) v.push_back(i4);
		return GetVectorIndex(v);
	}
};

rb::ClassData::ClassData():
	fClass(0), fDataMember(0), fAddr(0), fIndex(0) {
}

rb::ClassData::ClassData(const char* name, TClass* tclass, void* base) {
	fClass = tclass;
	Init(name, (Long_t)base);
}

void rb::ClassData::CloneInto(rb::ClassData* other) {
	other->fClass = fClass;
	other->fDataMember = fDataMember;
	other->fAddr = fAddr;
	other->fIndex = fIndex;
}

rb::ClassData* rb::ClassData::Create(const char* name, const char* classname, void* base) {
	TClass* tclass = TClass::GetClass(classname);
	if(!tclass) err::Error("ClassData::Create") << "Invalid class: \"" <<  classname << "\".";
	return Create(name, tclass, base);
}

namespace {

typedef void(*ClassDataSetter_t)(rb::ClassData*&);
typedef std::pair<Int_t, ClassDataSetter_t> IdSetterKey_t;
typedef std::pair<std::string, ClassDataSetter_t> NameSetterKey_t;

template <class T>
void set_basic(rb::ClassData*& out) {
	out = new rb::ClassDataBasic<T>();
}

template <class T>
void set_vector(rb::ClassData*& out) {
	out = new rb::ClassDataVector<T>();
}

IdSetterKey_t id_setters_ [] = {
	std::make_pair(kBool_t,     set_basic<Bool_t>),
	std::make_pair(kChar_t,     set_basic<Char_t>),
	std::make_pair(kUChar_t,    set_basic<UChar_t>),
	std::make_pair(kShort_t,    set_basic<Short_t>),
	std::make_pair(kUShort_t,   set_basic<UShort_t>),
	std::make_pair(kInt_t,      set_basic<Int_t>),
	std::make_pair(kUInt_t,     set_basic<UInt_t>),
	std::make_pair(kLong_t,     set_basic<Long_t>),
	std::make_pair(kULong_t,    set_basic<ULong_t>),
	std::make_pair(kLong64_t,   set_basic<Long64_t>),
	std::make_pair(kULong64_t,  set_basic<ULong64_t>),
	std::make_pair(kFloat_t,    set_basic<Float_t>),
	std::make_pair(kFloat16_t,  set_basic<Float16_t>),
	std::make_pair(kDouble_t,   set_basic<Double_t>),
	std::make_pair(kDouble32_t, set_basic<Double32_t>)
};

NameSetterKey_t type_setters_ [] = {
	std::make_pair("vector<double,allocator<double> >",                         set_vector<double>),
	std::make_pair("vector<float,allocator<float> >",                           set_vector<float>),
	std::make_pair("vector<long long,allocator<long long> >",                   set_vector<long long>),
	std::make_pair("vector<long,allocator<long> >",                             set_vector<long>),
	std::make_pair("vector<int,allocator<int> >",                               set_vector<int>),
	std::make_pair("vector<short,allocator<short> >",                           set_vector<short>),
	std::make_pair("vector<char,allocator<char> >",                             set_vector<char>),
	std::make_pair("vector<bool,allocator<bool> >",                             set_vector<bool>),
	std::make_pair("vector<unsigned long long,allocator<unsigned long long> >", set_vector<unsigned long long>),
	std::make_pair("vector<unsigned long,allocator<unsigned long> >",           set_vector<unsigned long>),
	std::make_pair("vector<unsigned int,allocator<unsigned int> >",             set_vector<unsigned int>),
	std::make_pair("vector<unsigned short,allocator<unsigned short> >",         set_vector<unsigned short>),
	std::make_pair("vector<unsigned char,allocator<unsigned char> >",           set_vector<unsigned char>),
};

ClassDataSetter_t find_setter(Int_t key) {
	static std::map<Int_t, ClassDataSetter_t>
		id_setters_map (id_setters_, id_setters_ + sizeof(id_setters_) / sizeof(IdSetterKey_t));
	std::map<Int_t, ClassDataSetter_t>::iterator it = id_setters_map.find(key);
	return it != id_setters_map.end() ? it->second : 0;
}

ClassDataSetter_t find_setter(const std::string& key) {
	static std::map<std::string, ClassDataSetter_t> type_setters_map
		(type_setters_, type_setters_ + sizeof(type_setters_) / sizeof(NameSetterKey_t));
	std::map<std::string, ClassDataSetter_t>::iterator it = type_setters_map.find(key);
	return it != type_setters_map.end() ? it->second : 0;
}

}

rb::ClassData* rb::ClassData::Create(const char* name, TClass* tclass, void* base) {
	if(tclass == 0 || base == 0)
		return 0;
	ClassData classdata (name, tclass, base);
	if(!classdata.fDataMember) {
		err::Error("ClassData::Create")
			<< "Couldn't find member \"" << name << "\" in class \"" << tclass->GetName() << "\".";
		return 0;
	}

	ClassData* out = 0;
	std::string type = classdata.fDataMember->GetTrueTypeName();
	//
	// Basic data
	if(classdata.fDataMember->IsBasic()) {
		ClassDataSetter_t setter = 0;
		Int_t typeId = classdata.fDataMember->GetDataType()->GetType();

		if(typeId == kDouble_t && type == "long double") // special handline of long double
			setter = set_basic<long double>;
		else
			setter = find_setter(typeId);

		if(setter)
			setter(out);
		else
			err::Error("ClassData::Create") << "No support for basic data type: \"" << type << "\"";
	}
	//
	// Vector of basic data
	else if (classdata.fDataMember->IsSTLContainer() == TDictionary::kVector) {
		ClassDataSetter_t setter = find_setter(type);
		if(setter)
			setter(out);
		else
			err::Error("ClassData::Create") << "No support for stl vector type: \"" << type << "\"";
	}
	//
	// Not supported
	else {
		err::Error("ClassData::Create") << "No support for class: \"" << type << "\"";
	}

	if(out) classdata.CloneInto(out);
	return out;
}

namespace {
Int_t check_stl_at(std::string& strname) {
	const std::string key = "_stl_vector_at";
	size_t pos = strname.find(key);
	if(pos > strname.size()) return -1;

	std::string indxstr = strname.substr(pos+key.size()+1);
	indxstr = indxstr.substr(0, indxstr.size() - 1);
	Int_t indx = atoi(indxstr.c_str());
	strname = strname.substr(0, pos);
	return indx;
} }

void rb::ClassData::Init(const char* name, Long_t base) {
	fAddr = 0; fDataMember = 0; fIndex = 0;
	if(!fClass) return;

	std::string strname = "";
	rb::ClassMemberParser parser(name);
	for(Int_t i=0; i< parser.GetNindir(); ++i) {
		Bool_t isLast = (i == parser.GetNindir() - 1);

		strname = parser.GetToken(i);
		Int_t stlat = check_stl_at(strname);
		if(stlat >= 0) fIndex = stlat;

		fDataMember = fClass->GetDataMember(strname.c_str());
		if(!fDataMember) {
			err::Error("ClassData::Init")
				<< "fClass->GetDataMember(\"" << strname << "\") failed";
			return;
		}

		Int_t ndim = fDataMember->GetArrayDim();
		Long_t noff = fDataMember->GetOffset();
		if(ndim == 0) {
			if(!isLast) {
				noff += fDataMember->GetUnitSize();
				//			strname += ".";
				fClass = TClass::GetClass(fDataMember->GetTrueTypeName());
				if(!fClass) {
					err::Error("ClassData::Init")
						<< "TClass::GetClass(\"" << fDataMember->GetTrueTypeName() << "\") failed";
					return;
				}
			}
		}
		else {
			Matrix2Vector m2v;
			for(Int_t j=0; j< ndim; ++j) {
				m2v.AddDim(fDataMember->GetMaxIndex(j));
			}
			std::vector<Int_t> indices;
			for(Int_t j=0; j< ndim; ++j) {
				Int_t indx = parser.GetIndex(j, 0);
				if(indx < 0 || indx >=  fDataMember->GetMaxIndex(j)) {
					err::Error("ClassData::Init")
						<< "Invalid index! " << indx << ", max = " << fDataMember->GetMaxIndex(j) << ERR_FILE_LINE;
					return;
				}
				indices.push_back(indx);
			}
			Int_t indxv = m2v.GetVectorIndex(indices);
			
			noff += indxv * fDataMember->GetUnitSize();
			if(!isLast) {
				fClass = TClass::GetClass(fDataMember->GetTrueTypeName());
				if(!fClass) {
					err::Error("ClassData::Init")
						<< "TClass::GetClass(\"" << fDataMember->GetTrueTypeName() << "\") failed";
					return;
				}
			}
		}

		base += noff;
	}

	fAddr = (void*)base;
}

struct Str0 { int i; };
struct Str1 {
	Str0 str0;
	Str0 str0arr[6];
	int a[45];
	double b;
	float c;
	std::vector<float> vf;
	long double ld;
} str1;
