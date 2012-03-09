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
#include "Rint.hxx"
#include "utils/Mutex.hxx"
#include "utils/Error.hxx"


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Helper Functions                                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace
{
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
  inline void modify_formula_arg(Int_t index, std::string& formula) {
    assert(is_valid_index(index));
    if(index == GATE) {
      std::string gate = TString(formula).ReplaceAll(" ","").Data();
      if     (gate ==  "") formula = "1";  // Null field means no gate, i.e. always true.
      else if(gate == "0") formula ="!1";  // Somehow "0" evaluates to true, should be false.
      else;                                // don't modify
    }
    else {
      if(formula == "0") formula = "!1";
    }
  }
}
 
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                                 //
// rb::TreeFormulae                                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::TreeFormulae::TreeFormulae(Int_t npar, const char* params, const char* gate, Int_t event_code):
  kEventCode(event_code) {
  // Initialize gate
  fFormulaArgs[GATE] = gate;
  modify_formula_arg(GATE, fFormulaArgs[GATE]);
  bool good_gate = Init(GATE, fFormulaArgs[GATE]);
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
    modify_formula_arg(I, fFormulaArgs[I]);
    bool good_param = Init(I, fFormulaArgs[I].c_str());
    if(!good_param) ThrowBad(pars[I].c_str(), I);
  }
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void ThrowBad()                                       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::TreeFormulae::ThrowBad(const char* formula, Int_t index) {
  std::stringstream message;
  message << "Bad TTreeFormula: \"" << formula << "\" (index: " << index << ")";
  std::invalid_argument exception(message.str().c_str());
  throw exception;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Bool_t rb::TreeFormulae::Init()                       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t rb::TreeFormulae::Init(Int_t index, std::string formula_arg) {
  assert(is_valid_index(index));
  return Init(fTreeFormulae[index], formula_arg);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Bool_t rb::TreeFormulae::Init()                       //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t rb::TreeFormulae::Init(boost::scoped_ptr<volatile TTreeFormula>& formula, const std::string& formula_arg) {
  if(formula_arg != "") {
    return rb::Event::InitFormula::Operate(rb::gApp()->GetEvent(kEventCode), formula_arg.c_str(), formula);
  } else {
    formula.reset(0);
    return true;
  }
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Bool_t rb::TreeFormulae::Change()                     //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t rb::TreeFormulae::Change(Int_t index, std::string new_formula) {
  if(!is_valid_index(index)) return false;

  // Modify formula if necessary
  modify_formula_arg(index, new_formula);

  // check that new gate formula is valid
  boost::scoped_ptr<volatile TTreeFormula> temp;
  if(!Init(temp, new_formula))
    return false;
  else {
    Bool_t success = Init(fTreeFormulae[index], new_formula);
    fFormulaArgs[index] = new_formula;
    return success;
  }
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// std::string rb::TreeFormulae::Get()                   //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
std::string rb::TreeFormulae::Get(Int_t index) {
  if(index < 0 || index > GATE) {
    err::Info("Formula::Get") << "Invalid index: " << index;
    return "NULL";
  }
  return fFormulaArgs[index];
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Double_t rb::TreeFormulae::Eval()                     //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Double_t rb::TreeFormulae::Eval(Int_t index) {
  return is_valid_index(index) ?
    LockingPointer<TTreeFormula>(*fTreeFormulae[index], gDataMutex)->EvalInstance(0) : -1.;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Double_t rb::TreeFormulae::EvalUnlocked()             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Double_t rb::TreeFormulae::EvalUnlocked(Int_t index) {
  return is_valid_index(index) ?
    LockFreePointer<TTreeFormula>(*fTreeFormulae[index])->EvalInstance(0) : -1.;
}
