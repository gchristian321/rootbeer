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


namespace form = rb::formula;

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Helper Functions                                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace {
  inline Bool_t is_valid_index(Int_t index) {
    Bool_t ret = index >= 0 && index <= GATE;
    if(!ret) err::Error("Formula") << "Invalid index: " << index;
    return ret;
  }
  inline std::vector<std::string> tokenize(const char* str, char token) {
    std::istringstream iss(str);
    std::string entry;
    std::vector<std::string> out;
    while(std::getline(iss, entry, token))
      out.push_back(entry);
    return out;
  }
  inline std::vector<std::string> get_param_vector(const char* str, int& ntokens) {
    std::vector<std::string> out = tokenize(str, ':');
    ntokens = out.size();
    for(int i = out.size(); i < 3; ++i) out.push_back("");
    return out;
  }
  inline Bool_t init_formula(boost::scoped_ptr<TTreeFormula>& formula, TTree* tree, const std::string& formula_arg) {
    if(formula_arg != "") {
      formula.reset(new TTreeFormula(formula_arg.c_str(), formula_arg.c_str(), tree));
      return formula->GetNdim() != 0;
    } else {
      formula.reset(0);
      return true;
    }
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
  fMutex(mutex), Eval(this, mutex), EvalMultiple(this, mutex), Change(this, mutex), Init(this, mutex) {
  // Initialize gate
  fFormulaArgs[GATE] = gate;
  GateModifier gm; gm(fFormulaArgs[GATE]);
  bool good_gate = Init(GATE, fFormulaArgs[GATE], tree);
  if(!good_gate) ThrowBad(gate, GATE);
  
  // Initialize parameters
  Int_t npar_found;
  std::vector<std::string> pars = get_param_vector(params, npar_found);
  if(npar_found != npar) {
    std::stringstream err;
    err << "Invalid parameter specification (\"" << params << "\") for a " << npar << "-dimensional histogram";
    throw(std::invalid_argument(err.str().c_str()));
  }
  for(Int_t I = 0; I < 3; ++I) {
    fFormulaArgs[I] = pars[I].c_str();
    ParamModifier pm; pm(fFormulaArgs[I]);
    bool good_param = Init(I, fFormulaArgs[I].c_str(), tree);
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
  else;                                // don't modify
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::formula::Init <bool>                              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t form::InitFunctor::DoOperation(Int_t index, std::string formula_arg, TTree* tree) {
  assert(is_valid_index(index));
  boost::scoped_ptr<TTreeFormula>& formula = fOwner->fTreeFormulae[index];
  return init_formula(formula, tree, formula_arg);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::formula::Change <bool>                            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t form::ChangeFunctor::DoOperation(Int_t index, std::string new_formula) {
  if(!is_valid_index(index)) return false;

  // Modify formula if necessary
  TTree* tree = fOwner->fTreeFormulae[index]->GetTree();
  boost::scoped_ptr<Modifier> modifier(new GateModifier());
  if(index != GATE) modifier.reset(new ParamModifier());
  modifier->operator()(new_formula);

  // check that new gate formula is valid
  boost::scoped_ptr<TTreeFormula> temp;
  if(!init_formula(temp, tree, new_formula))
    return false;
  else {
    fOwner->fFormulaArgs[index] = new_formula;
    return init_formula(fOwner->fTreeFormulae[index], tree, new_formula);
  }
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// rb::formula::Eval <Double_t>                          //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Double_t rb::formula::EvalFunctor::DoOperation(Int_t index) {
  return is_valid_index(index) ? fOwner->fTreeFormulae[index]->EvalInstance(0) : -1.;
}
