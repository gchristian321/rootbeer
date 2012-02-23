//! \file ImportData.h
//! \brief Adds global instances of users' data classes to rotbeer.
//! \details Users can add instances of their data classes to rootbeer by 
//! invoking the <a href="rb__import__data_8h.html#a7623fcbdf2c95113416d11bc0fd5286e"><b>RB_IMPORT_DATA</b></a> macro.
//!
//! Example:
//! \code
//! // [MyClasses.hxx] //
//! struct MyVariableClass {
//!    double v1;
//!    MyVariableClass(double v1_);
//!    // ... //
//! };
//!
//! class MyParamClass {
//!    double par1;
//!    MyParamClass();
//!    void calcPar(const MyVariableClass& v);
//!    // ... //
//! };
//!
//! // Here //
//! #include "MyClasses.hxx"
//! RB_IMPORT_DATA(MyVariableClass, gVariable, "variables", true, "27.2"); //< Create global instance of MyVariableClass
//! RB_IMPORT_DATA(MyParamClass, gParam, "params", false, ""); //< Create global instance of MyParamClass
//!
//! // In CINT //
//! rootbeer [0] rb::Hist::New("hst","",100,0,100,"params.par1","params.par1 != -1");  //< Histogram data from gParam
//!
//! rootbeer [1] rb::MData::GetValue("variables.var1") //< Check the value of gVariable::v1
//! (double)2.71999999999999993e+01 //< return value
//!
//! rootbeer [2] rb::MData::SetValue("variables.var1", 13.2) //< Set gVariable::v1 to 13.2
//! \endcode
//!
#include "vme/vme.hxx"
RB_IMPORT_DATA(Bgo, fBgo, "bgo", true, "")
// RB_IMPORT_DATA(Bgo, fBgo2, "bgo2", true, "")
