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

enum { X, Y, Z, GATE };
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

    RB_THREADSAFE_FUNCTOR_1(HistWrapper, std::string, GetFunctor, Int_t, index)
    RB_THREADSAFE_FUNCTOR_1(HistWrapper, Double_t, EvalFunctor, Int_t, index)
    RB_THREADSAFE_FUNCTOR_2(HistWrapper, Bool_t, ChangeFunctor, Int_t, index, const char*, new_formula)
    RB_THREADSAFE_FUNCTOR_4(HistWrapper, Bool_t, InitFunctor, Int_t, index, std::string, formula_arg, TTree*, tree, Modifier&, modifier)
    class HistWrapper
    {
    private:
      boost::scoped_ptr<TTreeFormula> fTreeFormulae[4];
      rb::Mutex* fMutex;
    public:
      HistWrapper(): fMutex(0), Get(this, 0), Eval(this, 0), Change(this, 0), Init(this, 0) {}
      HistWrapper(TTree* tree, rb::Mutex* mutex, Int_t npar, const char* params, const char* gate);
      GetFunctor Get;
      EvalFunctor Eval;
      ChangeFunctor Change;
    private:
      NoCopy fNoCopy;
      void ThrowBad(const char* formula, Int_t index);
      TTreeFormula* GetFormula(Int_t index);
      InitFunctor Init;
    public:
      friend class GetFunctor;
      friend class EvalFunctor;
      friend class ChangeFunctor;
      friend class InitFunctor;
    };
  }
}

// INLINE FUNCTIONS //
inline TTreeFormula* rb::formula::HistWrapper::GetFormula(Int_t index) {
  if(index > GATE || index < 0) {
    err::Error("GetFormula") << "Invalid index " << index;
    return 0;
  }
  return fTreeFormulae[index].get();
}
inline std::string rb::formula::GetFunctor::DoOperation(Int_t index) {
  TTreeFormula* formula = fOwner->GetFormula(index); if(!formula) return "NULL";
  return formula->GetExpFormula().Data();
}
inline Double_t rb::formula::EvalFunctor::DoOperation(Int_t index) {
  TTreeFormula* formula = fOwner->GetFormula(index); if(!formula) return -1;
  return formula->EvalInstance(0);
}
inline void rb::formula::ParamModifier::operator() (std::string& formula) {
  if(formula == "0") formula = "!1";
}

#endif
