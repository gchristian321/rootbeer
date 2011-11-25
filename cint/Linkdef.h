/** @file rbeer_utils_linkdef.hxx */
/*!
 *  This file is where CINT is told to generate dictionaries
 *  for the various classes we create.
 */

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

#pragma link C++ namespace rb;
#pragma link C++ namespace rb::canvas;

#pragma link C++ class sData+;
#pragma link C++ class sVariables+;

#pragma link C++ class rb::Hist+;
#pragma link C++ class rb::H1D+;
#pragma link C++ class rb::H2D+;
#pragma link C++ class rb::H3D+;

#pragma link C++ class UserDataABC+;
#pragma link C++ class UserData<sData>+;

#pragma link C++ class std::vector<TTreeFormula*>;
#pragma link C++ class std::map<std::string, UserDataABC*>;

#endif // #ifdef __CINT__
