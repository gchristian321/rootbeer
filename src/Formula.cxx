//! \file Formula.cxx
//! \brief Implements Formula.hxx
#include <cassert>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <TTree.h>
#include <TString.h>
#include <TTreeFormula.h>
#include "Formula.hxx"

typedef std::vector<std::string> StringVector_t;
namespace form = rb::formula;



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Helper Functions                                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace {
  inline StringVector_t tokenize(const char* str, char token) {
    std::istringstream iss(str);
    std::string entry;
    StringVector_t out;
    while(std::getline(iss, entry, token))
      out.push_back(entry);
    return out;
  }
  inline StringVector_t get_param_vector(const char* str, int& ntokens) {
    StringVector_t out = tokenize(str, ':');
    ntokens = out.size();
    for(int i = out.size(); i < 3; ++i) out.push_back("");
    return out;
  }
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                                 //
// HistWrapper                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
form::HistWrapper::HistWrapper(TTree* tree, rb::Mutex* mutex, Int_t npar, const char* params, const char* gate) :
  fMutex(mutex), Get(this, mutex), Eval(this, mutex), Change(this, mutex), Init(this, mutex) {
  // Initialize gate
  GateModifier gateMod;
  bool good_gate = Init(GATE, gate, tree, gateMod);
  if (!good_gate) ThrowBad(gate, GATE);

  // Initialize parameters
  Int_t npar_found;
  StringVector_t pars = get_param_vector(params, npar_found);
  if(npar_found != npar) {
    std::stringstream err;
    err << "Invalid parameter specification (\"" << params << "\") for a " << npar << "-dimensional histogram";
    throw(std::invalid_argument(err.str().c_str()));
  }
  ParamModifier paramMod;
  for(Int_t I = 0; I < 3; ++I) {
    bool good_param = Init(I, pars[I].c_str(), tree, paramMod);
    if(!good_param) ThrowBad(pars[I].c_str(), I);
  }
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void ThrowBad()                                       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void form::HistWrapper::ThrowBad(const char* formula, Int_t index) {
  std::stringstream message;
  message << "Bad TTreeFormula: \"" << formula << "\" (index: " << index << ")";
  std::invalid_argument exception(message.str().c_str());
  throw exception;
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Functor Classes                                       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// GateModifier::operator() () <void>                    //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void form::GateModifier::operator() (std::string& formula) {
  std::string gate = TString(formula).ReplaceAll(" ","").Data();
  if     (gate ==  "") formula = "1";  // Null field means no gate, i.e. always true.
  else if(gate == "0") formula ="!1";  // Somehow "0" evaluates to true, should be false.
  else; // don't modify
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::formula::Init <bool>                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t form::InitFunctor::DoOperation(Int_t index, std::string formula_arg, TTree* tree, Modifier& modifier) {
  assert(index <= GATE);
  assert(index >= 0);
  boost::scoped_ptr<TTreeFormula>& formula = fOwner->fTreeFormulae[index];
  modifier(formula_arg);
  if(formula_arg != "") {
    formula.reset(new TTreeFormula(formula_arg.c_str(), formula_arg.c_str(), tree));
    return formula->GetNdim() != 0;
  } else {
    formula.reset(0);
    return true;
  }
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::formula::Change <bool>                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t form::ChangeFunctor::DoOperation(Int_t index, const char* new_formula) {
  TTreeFormula* formula = fOwner->GetFormula(index);
  if(!formula) return false;

  TTree* tree = formula->GetTree();
  // check that new gate formula is valid
  GateModifier gateMod;
  boost::scoped_ptr<TTreeFormula> temp;
  bool is_valid = fOwner->Init.OperateUnlocked(index, new_formula, tree, gateMod);
  if(!is_valid) return false;
  // It's valid, so set the new gate
  else fOwner->fTreeFormulae[index].reset(temp.get());
  return true;
}
