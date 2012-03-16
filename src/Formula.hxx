//! \file Formula.hxx
//! \brief Defines a thread safe wrapper class for TTreeFormulas.
#ifndef FORMULA_HXX
#define FORMULA_HXX
#include <string>
#include "utils/boost_ptr_vector.h"
#include "utils/Critical.hxx"

// =========== Forward Declarations =========== //
class TTree;
class TTreeFormula;

// =========== Enums =========== //
enum AxisIndices { X, Y, Z, GATE };


namespace rb
{
  // =========== Class Definitions ============ //

  /// \brief Wrapper for histogram TTreeFormulae
  class TreeFormulae
  {
  private:
    const Int_t kEventCode;
    rb::Critical<boost::ptr_vector<TTreeFormula> > fTreeFormulae;
    std::vector<std::string> fFormulaArgs;
  public:
    TreeFormulae(): kEventCode(-1001), fTreeFormulae(0, gDataMutex) {}
    TreeFormulae(std::vector<std::string>& params, Int_t event_code);
    Int_t GetN() { return fTreeFormulae->size(); }
    std::string Get(Int_t index);
    Double_t Eval(Int_t index);
    Double_t EvalUnlocked(Int_t index);
    void EvalAll(std::vector<Double_t>& out);
    void EvalAllUnlocked(std::vector<Double_t>& out);
    Bool_t Change(Int_t index, std::string new_formula);
  private:
    void ThrowBad(const char* formula, Int_t index);
    TreeFormulae(const TreeFormulae& other): kEventCode(-1001), fTreeFormulae(0, gDataMutex) {}
    TreeFormulae& operator= (const TreeFormulae& other) { return *this; }
  };
}

#endif
