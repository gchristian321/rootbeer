#ifndef RB_CLASS_FORMULA_HEADER
#define RB_CLASS_FORMULA_HEADER
#include <memory>
#include <vector>
#include <TString.h>
#include <TFormula.h>
#include <TObjArray.h>


namespace rb
{

namespace data { class MReader; }


/// Helper class to parse string expressions specifying data members.
class ClassMemberParser {
private:
	/// One token for each level of indirection, e.g `a.b.c` -> fTokens = `("a","b","c")`
	std::auto_ptr<TObjArray> fTokens;
	/// Array indices belonging to each level of indirection
	std::vector<std::vector<Int_t> > fArrayIndices;
public:
	/// Empty contstructor, no action
	ClassMemberParser();
	/// Parses _member_ argument into indirection tokens an array indices.
	ClassMemberParser(const char* member);
	/// Get the array index of a given indirection level and dimension
	Int_t GetIndex(Int_t indir, Int_t dim) const;
	/// Returns the number of levels of indirection
	Int_t GetNindir() const { return fTokens.get() ? fTokens->GetEntries() : 0; }
	/// Returns the string specifying a single level of indirection
	TString& GetToken(Int_t index) const;
	/// Prints a summary of all members
	void Print() const;
	/// Explicitly parse a string
	void Parse(const char* member);
private:
	/// Figure out the array indices at all indirection levels.
	void ParseArrayIndices();
};


/// Class to evaluate furmula expressions containing class members.
/*!
 *  Basically, we just overload a couple of virtual methods in TFormula so that
 *  it treats recognizable members of classes as inputs to the formula. When possible,
 *  it reads the values of these parameters directly from their address, so the evaluation
 *  is fast compared to something like TTreeFormula.
 */
class ClassFormula: public TFormula
{
protected:
	/// "Branch" name assigned to the class in the rb::data::Basic<T> constructor.
	const char* fBranchname;
	/// Name of the class from which to take data
	const char* fClassname;
	/// Base addres of the class from which to read data
	void* fAddress;
	/// Vector of rb::data::MReader pointers, to extract data needed to evaluate the formula.
	std::vector<rb::data::MReader*> fReaders;

public:
	/// Constructor
	ClassFormula(const char* name, const char* formula, const char* branchName, const char* className, void* classAddr);
	/// Copy constructor
	ClassFormula(const ClassFormula& lhs);
	/// Free memory allocated to fReaders
	virtual ~ClassFormula();
	/// Evaluates the formula using the current value of class members.
	virtual Double_t Eval() const;
	/// Return the number of class members used as variables in the formula.
	Int_t GetNvar() const;
	/// Provide a parenthesis operator to evaluate the formula.
	Double_t operator() () const;

protected:
	/// Overload the virtual TFormula::DefinedVariable() method to recognize user class data.
	virtual Int_t DefinedVariable(TString &name, Int_t &action);
	/// Make Eval(x,y,z,t) private.
	virtual Double_t Eval(Double_t, Double_t = 0, Double_t = 0, Double_t = 0) const;
	/// Overload virtual TFormula::EvalPar() method to use internal data for evaluation.
	virtual Double_t EvalPar(const Double_t* x, const Double_t* params = 0);
private:
	/// Helper function for the constructor, same as TFormula constructor.
	void TFormulaConstructor(const char* name, const char* formula);

public:
	ClassDef(ClassFormula, 0);
};

} // namespace rb


inline Double_t rb::ClassFormula::Eval(Double_t x, Double_t y, Double_t z, Double_t t) const {
	/*!
	 * No change from parent class Eval(x,y,z,t). Simply included to move it out of the public interface.
	 * See Eval() for more information.
	 */
	return TFormula::Eval(x,y,z,t);
}
inline Double_t rb::ClassFormula::Eval() const {
	/*!
	 * Since the formula is evaluated only from internal data, the default Eval(x,y,z,t) method,
	 * which takes arguments, doesn't make sense. So instead, put a null-argument Eval()
	 * in the public interface and make Eval(x,y,z,t) protected.
	 */
	return Eval(0);
}

inline Int_t rb::ClassFormula::GetNvar() const {
	return static_cast<Int_t>(fReaders.size());
}

inline Double_t rb::ClassFormula::operator() () const {
	/*! `	return Eval();` */
	return Eval();
}


#ifdef CLASS_FORMULA_TEST
#include "Data.hxx"
#include "Event.hxx"
struct Data_t {
	int i;
	float f;
	double d;
	int i2;
	float f2;
	double d2;
};
struct DataEvent: public rb::Event {
	rb::data::Wrapper<Data_t> fData;
	DataEvent(): fData("data", this, true, "") {}
	Bool_t DoProcess(const void*,Int_t){return true;}
	void HandleBadEvent(){}
};
#endif

#endif
