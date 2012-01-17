/*! \file ExampleData.hh
 *  \brief An example of a user data class that can be plugged
 *  into the ROOTBEER framework.
 */
#ifndef __EXAMPLE_DATA__
#define __EXAMPLE_DATA__

/// Code to tell us that there was no data in a given event.
static const Int_t NO_DATA = -1;


/// Example of a variable class we can nest
struct InnerVariables {
  /// Example float
  Long64_t inner1;

  /// Example int array
  Int_t innerArray[27];

  /// Constructor, initialize to unity
  InnerVariables() {
    inner1 = 1.;
    for(Int_t i=0; i< 27;++i) innerArray[i] = 1.;
  }
};

/// Example of a class to hold experiment variables.
struct ExampleVariables {

  /// Variable 1
  Double_t var1;

  /// Variable 2
  Double_t var2;

  /// An array of variables
  Double_t varr[14];

  /// A nested class
  InnerVariables inner;

  /// Constructor
  /*! Initialize everthing to unity. */
  ExampleVariables() {
    var1=1.;
    var2=1.;
    for(Int_t i=0; i< 14; ++i) varr[i] = 1.;
  }
  /// Non-default constructor
  //! Initialize everything to initVal
  ExampleVariables(Int_t initVal) {
    var1=initVal;
    var2=initVal;
    for(Int_t i=0; i< 14; ++i) varr[i] = initVal;
  }
};


/// Example of a class to hold experiment parameters.
class ExampleData {

public:
  /// Some raw parameter
  Short_t a;

  /// Some other raw parameter
  Short_t b;

  /// Some other other raw parameter
  Short_t c;


  /// Some calibrated parameter
  Double_t cal1;

  /// Some other calibrated parameter
  Double_t cal2;

  /// calibrated array
  Double_t calArray[10];

  /// Reset everything to the NO_DATA value
  void reset() {
    a = NO_DATA;
    b = NO_DATA;
    c = NO_DATA;

    cal1 = NO_DATA;
    cal2 = NO_DATA;
  }

  /// Some function to get calibrated params from raw params and variables.
  void calibrate(ExampleVariables& v) {
    if(a > 0 && b > 0 && v.var1 != 0) {
      cal1 = (a*b) / v.var1;
    }
    if(c > 0) {
      cal2 = c * v.var2;
    }
    if(cal1 != -1) {
      for(Int_t i=0; i< 10; ++i)
	calArray[i] = cal1 + i*1.;
    }
  }

  /// Function which processes a raw event from a data file.
  Int_t process_event(UShort_t* pEvt) {
    reset();
    UShort_t* p0 = pEvt, * p = pEvt;
    Int_t size = *p++;
    while(p< p0 + size + 1) {
      UShort_t code = *p++;
      switch(code) {
      case 1: a = *p++; break;
      case 2: b = *p++; break;
      case 3: c = *p++; break;
      default: p++; break;
      }
    }
    return p - p0 - 2;
  }

  /// Constructor, initialize all to NO_DATA.
  ExampleData() {
    reset();
  }

  /// Copy constructor
  ExampleData(ExampleData& other) :
    a(other.a), b(other.b), c(other.c),
    cal1(other.cal1), cal2(other.cal2)			
  { };

  /// Destructor
  ~ExampleData() { }
};

struct TimeStamp {
  Int_t time;
};

#endif
