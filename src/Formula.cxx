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
#include "Data.hxx"
#include "utils/Mutex.hxx"
#include "utils/Error.hxx"


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Helper Functions                                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
namespace
{
  inline void modify_formula_arg(std::string& formula) {
    std::string f = TString(formula).ReplaceAll(" ","").Data();
    if      (f ==  "") formula = "1";  // Null field means no gate, i.e. always true.
    else if (f == "0") formula ="!1";  // Somehow "0" evaluates to true, should be false.
    else;                              // don't modify
  }
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                                 //
// rb::DirectDataFormula                                 //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::DirectDataFormula::DirectDataFormula(const char* branchName, const char* className, void* addr, const char* formula) {
	rb::data::Mapper mapper (branchName, className, reinterpret_cast<Long_t>(addr), false);
	fReader = mapper.FindBasicReader(formula);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::DirectDataFormula::~DirectDataFormula() {
	if(fReader) delete fReader;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Double_t rb::DirectDataFormula::Evaluate()            //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Double_t rb::DirectDataFormula::Evaluate() {
	return fReader->ReadValue();
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Class                                                 //
// rb::TreeFormulae                                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Constructor                                           //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
rb::TreeFormulae::TreeFormulae(std::vector<std::string>& params, Int_t event_code):
  kEventCode(event_code), fDataFormulae(new boost::ptr_vector<rb::DataFormula>(), gDataMutex) {

  RB_LOCKGUARD(gDataMutex);
  std::vector<std::string>::iterator it;
  for(it = params.begin(); it != params.end(); ++it) {
    modify_formula_arg(*it);
    fFormulaArgs.push_back(*it);
    rb::DataFormula* formula = 
      rb::Event::InitFormula::Operate(rb::gApp()->GetEvent(kEventCode), it->c_str());

    if(formula->IsZombie()) ThrowBad(it->c_str(), it-params.begin());
    else fDataFormulae->push_back(formula);
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
// Bool_t rb::TreeFormulae::Change()                     //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Bool_t rb::TreeFormulae::Change(Int_t index, std::string new_formula) {

  // Modify formula if necessary
  modify_formula_arg(new_formula);

  // check that new gate formula is valid
  boost::scoped_ptr<rb::DataFormula>
    temp (rb::Event::InitFormula::Operate(rb::gApp()->GetEvent(kEventCode), new_formula.c_str()));

  if(temp->IsZombie())
    return false;
  else {
    try {
      RB_LOCKGUARD(gDataMutex);
      fDataFormulae->replace(index, rb::Event::InitFormula::Operate(rb::gApp()->GetEvent(kEventCode), new_formula.c_str()));
      fFormulaArgs.at(index) = new_formula;
    } catch(std::exception& e) {
      err::Error("rb::TreeFormulae::Change()") << "Invalid index " << index;
    }
    return true;
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
  rb::ScopedLock<rb::Mutex> LOCK(gDataMutex);
  return EvalUnlocked(index);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// Double_t rb::TreeFormulae::EvalUnlocked()             //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
Double_t rb::TreeFormulae::EvalUnlocked(Int_t index) {
  Double_t ret = -1;
  try { ret = fDataFormulae->at(index).Evaluate(); }
  catch (std::exception& e) {
    err::Error("rb::TreeFormulae::Eval") << "Invalid index " << index;
    ret = -1;
  }
  return ret;
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::TreeFormulae::EvalAll()                      //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::TreeFormulae::EvalAll(std::vector<Double_t>& out) {
  rb::ScopedLock<rb::Mutex> LOCK(gDataMutex);
  EvalAllUnlocked(out);
}
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
// void rb::TreeFormulae::EvalAllUnlocked()              //
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void rb::TreeFormulae::EvalAllUnlocked(std::vector<Double_t>& out) {
  boost::ptr_vector<rb::DataFormula>::iterator it;
  out.clear();
  for(it = fDataFormulae->begin(); it != fDataFormulae->end(); ++it)
    out.push_back(it->Evaluate());
}
