#include <iostream>
#include <TFormulaPrimitive.h>
#include "Data.hxx"
#include "ClassFormula.hxx"



rb::ClassFormula::ClassFormula(const char* name, const char* expression,
															 const char* branchName, const char* className,
															 void* classAddr):
	fClassname(className), fBranchname(branchName), fAddress(classAddr) {
	/*!
	 * \param name Desired name for this instance.
	 * \param expression The formula to evaluate.
	 * \param branchName Name of the branch associated with the class from which to read data.
	 * \param className Name of the class from which to read data.
	 * \param classAddr Address of the class from which to read data.
	 * \note Empty expressions are set to `"1"`.
	 */
	TString expr(expression);
	if(expr.IsWhitespace()) expr = "1";
	SetNameTitle(name, expr.Data());
	TFormulaConstructor(name, expr.Data());
	if(!GetNdim()) Error("ClassFormula", "Couldn't parse expression: \"%s\"", expression);
}

rb::ClassFormula::ClassFormula(const ClassFormula& other):
	TFormula(other),
	fBranchname(other.fBranchname), fClassname(other.fClassname), fAddress(other.fAddress) {
	/// Make deep copy of fReaders using Clone() method
	for (size_t i=0; i< fReaders.size(); ++i)
		fReaders.push_back(other.fReaders[i]->Clone());
}

rb::ClassFormula::~ClassFormula() {
	for(size_t i=0; i< fReaders.size(); ++i) {
		delete fReaders[i];
	}
}

Double_t rb::ClassFormula::EvalPar(const Double_t*, const Double_t* params) {
	/*!
	 * Copy the evaluations of fReaders into a temporary array, then call fOptimal using
	 * the temp. array as the variables.
	 */
	static Double_t xargs_static[10];
	const Bool_t is_less10 = fReaders.size() < 10;
	Double_t* xargs = is_less10 ? xargs_static : new Double_t[fReaders.size()];

	for(size_t i=0; i< fReaders.size(); ++i) {
		xargs[i] = fReaders[i]->ReadValue();
	}
	Double_t retval = ((*this).*fOptimal)(xargs,params);
	if(!is_less10) delete[] xargs;
	return retval;
}

Int_t rb::ClassFormula::DefinedVariable(TString &name, Int_t &action) {
	/*!
	 * From the TFormula documentation:
	 * \verbatim
	 *-*-*-*-*-*Check if expression is in the list of defined variables*-*-*-*-*
	 *-*        =======================================================
	 *-*
	 *-*   This member function can be overloaded in derived classes
	 *-*
	 *-*   If you overload this member function, you also HAVE TO
	 *-*   never call the constructor:
	 *-*
	 *-*     TFormula::TFormula(const char *name,const char *expression)
	 *-*
	 *-*   and write your own constructor
	 *-*
	 *-*     MyClass::MyClass(const char *name,const char *expression) : TFormula()
	 *-*
	 *-*   which has to call the TFormula default constructor and whose implementation
	 *-*   should be similar to the implementation of the normal TFormula constructor
	 *-*
	 *-*   This is necessary because the normal TFormula constructor call indirectly
	 *-*   the virtual member functions Analyze, DefaultString, DefaultValue
	 *-*   and DefaultVariable.
	 *-*
	 *-*   The expected returns values are
	 *-*     -2 :  the name has been recognized but won't be usable
	 *-*     -1 :  the name has not been recognized
	 *-*    >=0 :  the name has been recognized, return the action parameter.
	 *-*
	 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
	 * \endverbatim
	 */
	rb::data::Mapper mapper(fBranchname, fClassname, reinterpret_cast<Long_t>(fAddress), kFALSE);
	rb::data::MReader* reader = mapper.FindBasicReader(name.Data());

	if(!reader) return -1; // unrecognized string, return error
	action = kVariable; // recognized string
	Int_t retval = fReaders.size();
	fReaders.push_back(reader);
	return retval;
}

void rb::ClassFormula::TFormulaConstructor(const char* name, const char* expression) {
	/*!
	 * With one small change (see note), this is a copy of the TTreeFormula(onst char *name,const char *expression) constructor.
	 * We cannot use that constructor because it calls virtual functions that we have overloaded. See DefinedVariable() for
	 * more information.
	 */
	fNdim   = 0;
	fNpar   = 0;
	fNoper  = 0;
	fNconst = 0;
	fNumber = 0;
	fExpr   = 0;
	///\note removed `fOper = 0;` line: fOper is private and anyway it's done in the default constructor.
	fConst  = 0;
	fParams = 0;
	fNstring= 0;
	fNames  = 0;
	fNval   = 0;
	//
	//MI change
	fNOperOptimized = 0;
	fExprOptimized  = 0;
	fOperOptimized  = 0;
	fOperOffset     = 0;
	fPredefined     = 0;
	fOptimal        = (TFormulaPrimitive::TFuncG)&TFormula::EvalParOld;

	if (!expression || !*expression) {
		Error("TFormula", "expression may not be 0 or have 0 length");
		return;
	}

	//eliminate blanks in expression
	Int_t i,j,nch;
	nch = strlen(expression);
	char *expr = new char[nch+1];
	j = 0;
	for (i=0;i<nch;i++) {
		if (expression[i] == ' ') continue;
		if (i > 0 && (expression[i] == '*') && (expression[i-1] == '*')) {
			expr[j-1] = '^';
			continue;
		}
		expr[j] = expression[i]; j++;
	}
	expr[j] = 0;
	Bool_t gausNorm   = kFALSE;
	Bool_t landauNorm = kFALSE;
	Bool_t linear = kFALSE;

	if (j) {
		TString chaine = expr;
		//special case for functions for linear fitting
		if (chaine.Contains("++"))
			linear = kTRUE;
		// special case for normalized gaus
		if (chaine.Contains("gausn")) {
			gausNorm = kTRUE;
			chaine.ReplaceAll("gausn","gaus");
		}
		// special case for normalized landau
		if (chaine.Contains("landaun")) {
			landauNorm = kTRUE;
			chaine.ReplaceAll("landaun","landau");
		}
		SetTitle(chaine.Data());
	}
	delete [] expr;

	if (linear)    SetBit(kLinear);

	if (Compile()) return;

	if (gausNorm)   SetBit(kNormalized);
	if (landauNorm) SetBit(kNormalized);

//*-*- Store formula in linked list of formula in ROOT

	TFormula *old = (TFormula*)gROOT->GetListOfFunctions()->FindObject(name);
	if (old) {
		gROOT->GetListOfFunctions()->Remove(old);
	}
	if (strcmp(name,"x")==0 || strcmp(name,"y")==0 ||
			strcmp(name,"z")==0 || strcmp(name,"t")==0 )
	{
		Error("TFormula","The name \'%s\' is reserved as a TFormula variable name.\n"
					"\tThis function will not be registered in the list of functions",name);
	} else {
		gROOT->GetListOfFunctions()->Add(this);
	}
}


//
// Testing stuf
#ifdef CLASS_FORMULA_TEST
#include <cassert>
#include "Buffer.hxx"
#include "Main.hxx"

int main_(int argc, char** argv)
{
	int argc2 = argc + 1;
	char** argv2 = (char**)malloc(argc2*sizeof(char*));
	for(int i=0; i< argc; ++i) {
		argv2[i] = (char*)malloc(strlen(argv[i]+1));
		strcpy(argv2[i], argv[i]);
	}
	argv2[argc] = (char*)malloc(4);
	strcpy(argv2[argc], "-ng");
	rb::Rint rbApp("classformulatest", &argc2, argv2, 0, 0, true);
	//
	//

	rb::Event::Instance<DataEvent>()->fData->i = 4;
	rb::Event::Instance<DataEvent>()->fData->f = 5;
	rb::Event::Instance<DataEvent>()->fData->d = 6;
	rb::Event::Instance<DataEvent>()->fData->i2 = 1;
	rb::Event::Instance<DataEvent>()->fData->f2 = 2.;
	rb::Event::Instance<DataEvent>()->fData->d2 = 3.;
	const char* form = argc > 1 ? argv[1] : "i + [0]";
	rb::ClassFormula cf("cfr", form, "data", "Data_t", rb::Event::Instance<DataEvent>()->fData.Get());
	if(cf.GetNdim()) {
		Double_t result = cf();
		std::cout << "Eval: " << result << "\n";
	}
	else {
		std::cout << "Zombie!\n";
	}

	//
	//
	for(int i=0; i< argc2; ++i)
		free(argv2[i]);
	free(argv2);
	return 0;
}


rb::BufferSource* rb::BufferSource::New()
{
	return 0;
}
void rb::Rint::RegisterEvents()
{
	RegisterEvent<DataEvent> (1, "DataEvent");
}
struct TestMain: public rb::Main
{
	int Run(int argc, char** argv) { return main_(argc, argv); }
};
rb::Main* rb::GetMain()
{
	return new TestMain();
}

#endif
