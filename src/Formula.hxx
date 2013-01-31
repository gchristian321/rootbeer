//! \file Formula.hxx
//! \brief Defines a thread safe wrapper class for TTreeFormulas.
#ifndef FORMULA_HXX
#define FORMULA_HXX
#include <string>
#include "utils/boost_scoped_ptr.h"
#include "utils/boost_ptr_vector.h"
#include "utils/Critical.hxx"


// =========== Forward Declarations =========== //
class TTree;
class TTreeFormula;

// =========== Enums =========== //
enum AxisIndices { X, Y, Z, GATE };


namespace rb
{
// =========== Forward Declarations =========== //
namespace data { class MReader; }

// =========== Class Definitions ============ //

/// \brief abstract class for accessing class data via a string.
//! \details This provides a very basic interface for accessing
//! event-by-event data from a class which is known to ROOT (i.e. has a dictionary).
//! Derived classes decide exactly how to access the data (e.g. directly vs. using TTReeFormula).
class DataFormula
{
public:
	/// \brief Empty constructor.
	DataFormula() { }
	/// \brief Empty (virtual) destructor.
	virtual ~DataFormula() { }
	/// \brief Evaluates the current state of the formula data.
	virtual Double_t Evaluate() = 0;
	/// \brief Tells if the current object is in a "zombie" (invalid) state.
	/// \attention In general, the Evaluate() function will do no checking for validity of the
	/// internal components. Ths IsZombie() function should provide all needed checks and be
	/// called by the user before evaluating.
	virtual Bool_t IsZombie() = 0;
};

/// \brief Derived class of DataFormula making use of ROOT's TTreeFormula to evaluate the data.
//! \details This is a convenient way to do things, but suffers performance-wise. The intent is
//! for this to be the implementation when calculation is needed to evaluate the full formula
//! expression.
class TTreeDataFormula : public DataFormula
{
private:
	/// \brief TTreeFormula object to evaluate the string
	boost::scoped_ptr<TTreeFormula> fTTreeFormula;
public:
	/// \brief Creates internal TTreeFormula
	//! \details Parameters are the same as for ROOT's TTreeFormula.
	TTreeDataFormula(const char* name, const char* formula, TTree* tree):
		fTTreeFormula(new TTreeFormula(name, formula, tree)) { }
	/// \brief Calls fFormula->EvalInstance(0)
	virtual Double_t Evaluate() { return fTTreeFormula->EvalInstance(0); }
	/// \brief Returns true if GetNdim() == 0
	virtual Bool_t IsZombie() { return fTTreeFormula->GetNdim() == 0; }
};

/// \brief Derived class of DataFormula making use of our rb::data::Mapper functionality.
//! \details This class stores directly the address of the data to be evaluated. It's intended
//! use is when the formula corresponds to a single basic piece of data within a class.
class DirectDataFormula : public DataFormula
{
private:
	/// "Reader" for the values stored at the address corresponding to the desired formula
	rb::data::MReader* fReader;
public:
	/// \brief Creates fReader
	//! \param branchName name of the "top level" branch in which the formula's data is contained
	//! \param className name of the class of which the formula's data is a member
	//! \param addr Pointer to the class instance storing the formula's data
	//! \param formula The formula to evaluate.
	DirectDataFormula(const char* branchName, const char* className, void* addr, const char* formula);
	/// Deletes fReader
	virtual ~DirectDataFormula();
	/// \brief Calls fReader->ReadValue();
	virtual Double_t Evaluate();
	/// \brief Returns true if fReader == 0
	virtual Bool_t IsZombie() { return fReader == 0; }
};

/// \brief Derived class of DataFormula evaluating a constant
class ConstantDataFormula : public DataFormula
{
private:
	/// The constant value
	Double_t fValue;
	/// Tells whether or not the passed value was actually a constant
	Bool_t fIsFormulaConstant;
public:
	/// \brief Fills fValue with the correct amount, sets fIsFormulaConstant
	//! \param formula The formula to evaluate.
	ConstantDataFormula(const char* formula);
	/// \brief Empty
	virtual ~ConstantDataFormula() { }
	/// \brief Returns fValue
	virtual Double_t Evaluate() { return fValue; }
	/// \brief Returns true if fIsFormulaConstant == false
	virtual Bool_t IsZombie() { return fIsFormulaConstant == false; }
};


/// \brief Wrapper for histogram TTreeFormulae
class TreeFormulae
{
private:
	const Int_t kEventCode;
	rb::Critical<boost::ptr_vector<rb::DataFormula> > fDataFormulae;
	std::vector<std::string> fFormulaArgs;
public:
	TreeFormulae(): kEventCode(-1001), fDataFormulae(0, gDataMutex) {}
	TreeFormulae(std::vector<std::string>& params, Int_t event_code);
	Int_t GetN() { return fDataFormulae->size(); }
	std::string Get(Int_t index);
	Double_t Eval(Int_t index);
	Double_t EvalUnlocked(Int_t index);
	void EvalAll(std::vector<Double_t>& out);
	void EvalAllUnlocked(std::vector<Double_t>& out);
	Bool_t Change(Int_t index, std::string new_formula);
private:
	void ThrowBad(const char* formula, Int_t index);
	TreeFormulae(const TreeFormulae& other): kEventCode(-1001), fDataFormulae(0, gDataMutex) {}
	TreeFormulae& operator= (const TreeFormulae& other) { return *this; }
};

} // namespace rb

#endif
