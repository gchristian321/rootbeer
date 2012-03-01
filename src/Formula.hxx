//! \file Formula.hxx
//! \brief Defines a thread safe wrapper class for TTreeFormulas.
#ifndef FORMULA_HXX
#define FORMULA_HXX
#include <string>
#include "utils/Mutex.hxx"
#include "utils/Error.hxx"
#include "utils/ThreadsafeFunctor.hxx"
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

enum AxisIndices { X, Y, Z, GATE };
namespace rb
{
  namespace formula
  {
    class HistWrapper;
    // FUNCTOR CLASSES //
    /// ABC for modifying formula string arguments
    struct Modifier { virtual void operator() (std::string& formula) = 0; };

    /// Modifier for gate formula arguments
    struct GateModifier: public Modifier { virtual void operator() (std::string& formula); };

    /// Modifier for Parameter formula arguments
    struct ParamModifier: public Modifier { virtual void operator() (std::string& formula); };

    /// Change a parameter formula argument
    struct ChangeFunctor: public ThreadsafeFunctor<HistWrapper, Bool_t, Int_t, std::string> {
      ChangeFunctor(HistWrapper* owner, rb::Mutex* mutex): ThreadsafeFunctor<HistWrapper, Bool_t, Int_t, std::string>(owner, mutex) {}
      Bool_t DoOperation(Int_t index, std::string new_formula);
    };
    /// Evaluate a formula
    struct EvalFunctor: public ThreadsafeFunctor<HistWrapper, Double_t, Int_t> {
      EvalFunctor(HistWrapper* owner, rb::Mutex* mutex): ThreadsafeFunctor<HistWrapper, Double_t, Int_t>(owner, mutex) {}
      Double_t DoOperation(Int_t index);
    };
    /// Initialize a TTreeParmeter
    struct InitFunctor: public ThreadsafeFunctor<HistWrapper, Bool_t, Int_t, std::string, TTree*> {
      InitFunctor(HistWrapper* owner, rb::Mutex* mutex): ThreadsafeFunctor<HistWrapper, Bool_t, Int_t, std::string, TTree*>(owner, mutex) {}
      Bool_t DoOperation(Int_t index, std::string formula_arg, TTree* tree);
    };
    class HistWrapper
    {
    private:
      boost::scoped_ptr<TTreeFormula> fTreeFormulae[4];
      std::string fFormulaArgs[4];
      rb::Mutex* fMutex;
    public:
      HistWrapper(): fMutex(0), Eval(this, 0), EvalMultiple(this, 0), Change(this, 0), Init(this, 0) {}
      HistWrapper(TTree* tree, rb::Mutex* mutex, Int_t npar, const char* params, const char* gate);
      /// Get parameter values
      std::string Get(Int_t index);
      EvalFunctor Eval;
      EvalFunctor EvalMultiple;
      ChangeFunctor Change;
    private:
      NoCopy fNoCopy;
      void ThrowBad(const char* formula, Int_t index);
      InitFunctor Init;
    public:
      friend class EvalFunctor;
      friend class EvalMultipleFunctor;
      friend class ChangeFunctor;
      friend class InitFunctor;
    };
  }
}

// INLINE FUNCTIONS //
inline std::string rb::formula::HistWrapper::Get(Int_t index) {
  if(index < 0 || index > GATE) {
    err::Info("Formula::Get") << "Invalid index: " << index;
    return "NULL";
  }
  return fFormulaArgs[index];
}
inline void rb::formula::ParamModifier::operator() (std::string& formula) {
  if(formula == "0") formula = "!1";
}
#endif
