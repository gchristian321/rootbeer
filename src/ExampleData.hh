/*! \file ExampleData.hh
 *  \brief An example of a user data class that can be plugged
 *  into the ROOTBEER framework.
 */
#ifndef __EXAMPLE_DATA__
#define __EXAMPLE_DATA__

/// Code to tell us that there was no data in a given event.
static const Int_t NO_DATA = -1;

/// Example of a class to hold experiment variables.
struct ExampleVariables {

  /// Variable 1
  Double_t var1;

  /// Variabl 2
  Double_t var2;

  /// Constructor
  /*! Initialize everthing to unity. */
  ExampleVariables() {
    var1=1;
    var2=1;
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
  }

  /// Function which processes a raw event from a data file.
  Int_t process_event(Short_t* pEvt) {
    reset();
    Short_t* p0 = pEvt, * p = pEvt;
    Int_t size = *p++;
    while(p< p0 + size + 1) {
      Short_t code = *p++;
      switch(code) {
      case 1: a = *p++; break;
      case 2: b = *p++; break;
      case 3: c = *p++; break;
      default: p++; break;
      }
    }
    return p - p0 - 2;
  }

  // Int_t unpack_buffer(Short_t* pBuf) {
  //   Short_t* p = pBuf;
  //   Int_t nEvts = *p++;
  //   for(Int_t i=0; i< nEvts; ++i) {
  //     p += process_event(p);
  //   }
  //   return 0;
  // }

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


#endif
