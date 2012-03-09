//! \file Formula.hxx
//! \brief Defines a thread safe wrapper class for TTreeFormulas.
#ifndef FORMULA_HXX
#define FORMULA_HXX
#include <string>
#include "utils/boost_scoped_ptr.h"

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
    boost::scoped_ptr<volatile TTreeFormula> fTreeFormulae[4];
    std::string fFormulaArgs[4];
  public:
    TreeFormulae(): kEventCode(-1001) {}
    TreeFormulae(Int_t npar, const char* params, const char* gate, Int_t event_code);
    /// Get parameter values
    std::string Get(Int_t index);
    Double_t Eval(Int_t index);
    Double_t EvalUnlocked(Int_t index);
    Bool_t Change(Int_t index, std::string new_formula);
  private:
    void ThrowBad(const char* formula, Int_t index);
    Bool_t Init(Int_t index, std::string new_formula);
    Bool_t Init(boost::scoped_ptr<volatile TTreeFormula>& formula, const std::string& formula_arg);
    TreeFormulae(const TreeFormulae& other): kEventCode(-1001) {}
    TreeFormulae& operator= (const TreeFormulae& other) { return *this; }
  };
}

#endif
