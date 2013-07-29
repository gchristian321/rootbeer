#ifndef RB_CLASS_DATA_HEADER
#define RB_CLASS_DATA_HEADER
#include <cassert>
#include <TDataMember.h>

class TClass;

namespace rb
{

class ClassData : public TObject
{
protected:
	TClass* fClass;
	TDataMember* fDataMember;
	void* fAddr;
	Int_t fIndex;

protected:
	ClassData();
	ClassData(const char* name, TClass* tclass, void* base);
public:
	static ClassData* Create(const char* name, const char* classname, void* base);
	static ClassData* Create(const char* name, TClass* tclass, void* base);
	virtual ~ClassData() { }
	void* GetAddr() const { return fAddr; }
	TDataMember* GetDataMember() const { return fDataMember; }
	Int_t GetIndex() const { return fIndex; }
	void SetIndex(Int_t i) { fIndex = i; }
	virtual Double_t GetValue() { AbstractMethod("GetValue"); return 0; }
	operator double() { return GetValue(); }	
private:
	void Init(const char* name, Long_t base);
	void CloneInto(ClassData* other);
	ClassDef(ClassData, 0);
};

template <class T>
class ClassDataBasic : public ClassData
{
public:
	ClassDataBasic() { }
	virtual Double_t GetValue() {
		return *((T*)fAddr);
	}
	friend class ClassData;
	ClassDef(ClassDataBasic, 0);
};


template <class T>
class ClassDataVector : public ClassData
{
public:
	ClassDataVector() { }
	virtual Double_t GetValue() {
		try {  return ((std::vector<T>*)fAddr)->at(fIndex);  }
		catch (std::exception& e) { return -1; }
	}
	friend class ClassData;
	ClassDef(ClassDataVector, 0);
};

}

#ifdef __MAKECINT__

#pragma link C++ class rb::ClassDataBasic<Bool_t>+;
#pragma link C++ class rb::ClassDataBasic<Char_t>+;
#pragma link C++ class rb::ClassDataBasic<UChar_t>+;
#pragma link C++ class rb::ClassDataBasic<Short_t>+;
#pragma link C++ class rb::ClassDataBasic<UShort_t>+;
#pragma link C++ class rb::ClassDataBasic<Int_t>+;
#pragma link C++ class rb::ClassDataBasic<UInt_t>+;
#pragma link C++ class rb::ClassDataBasic<Long_t>+;
#pragma link C++ class rb::ClassDataBasic<ULong_t>+;
#pragma link C++ class rb::ClassDataBasic<Long64_t>+;
#pragma link C++ class rb::ClassDataBasic<ULong64_t>+;
#pragma link C++ class rb::ClassDataBasic<Float_t>+;
#pragma link C++ class rb::ClassDataBasic<Double_t>+;
#pragma link C++ class rb::ClassDataBasic<long double>+;

#pragma link C++ class rb::ClassDataVector<Bool_t>+;
#pragma link C++ class rb::ClassDataVector<Char_t>+;
#pragma link C++ class rb::ClassDataVector<UChar_t>+;
#pragma link C++ class rb::ClassDataVector<Short_t>+;
#pragma link C++ class rb::ClassDataVector<UShort_t>+;
#pragma link C++ class rb::ClassDataVector<Int_t>+;
#pragma link C++ class rb::ClassDataVector<UInt_t>+;
#pragma link C++ class rb::ClassDataVector<Long_t>+;
#pragma link C++ class rb::ClassDataVector<ULong_t>+;
#pragma link C++ class rb::ClassDataVector<Long64_t>+;
#pragma link C++ class rb::ClassDataVector<ULong64_t>+;
#pragma link C++ class rb::ClassDataVector<Float_t>+;
#pragma link C++ class rb::ClassDataVector<Double_t>+;

#endif


#endif
