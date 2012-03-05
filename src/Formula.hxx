//! \file Formula.hxx
//! \brief Defines a thread safe wrapper class for TTreeFormulas.
#ifndef FORMULA_HXX
#define FORMULA_HXX
#include <string>
#ifndef __MAKECINT__
#include "boost/scoped_ptr.hpp"
#else
namespace boost { template <class T> class scoped_ptr<T>; }
#endif
class TTree;
class TTreeFormula;

class NoCopy
{
  NoCopy(const NoCopy& other) {}
  NoCopy& operator= (const NoCopy& other) {}
public:
  NoCopy() {}
};

// =========== Enums =========== //
enum AxisIndices { X, Y, Z, GATE };


namespace rb
{
  // =========== Class Definitions ============ //

  /// \brief Wrapper for histogram TTreeFormulae
  /// \details Implementation ensures that the data pointed to by the formulae
  /// are locked by using ThreadafeFunctors instead of member functions.
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
    NoCopy fNoCopy;
    void ThrowBad(const char* formula, Int_t index);
    Bool_t Init(Int_t index, std::string new_formula);
    Bool_t Init(boost::scoped_ptr<volatile TTreeFormula>& formula, const std::string& formula_arg);
  };
}

#endif
