#ifndef __SDATA__
#define __SDATA__

/// Example of a really simple user class.
struct sVariables {
  /// Variables
  Double_t var1;
  Double_t var2;
  sVariables() { var1=1;var2=1; }
};

class sData {
public:
  /// Raw data
  Short_t a;
  Short_t b;
  Short_t c;


  /// Calibrated data
  Double_t cal1;
  Double_t cal2;


public:
  /// Variables
  sVariables v; //!

  void reset() { a=-1; b=-1; c=-1; cal1 = -1; cal2 = -1; }

  void calibrate() {
    if(a>0 && b>0 && v.var1!= 0) {
      cal1 = (a*b) / v.var1;
    }
    if(c>0) {
      cal2 = c*v.var2;
    }
  }

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

  Int_t unpack_buffer(Short_t* pBuf) {
    Short_t* p = pBuf;
    Int_t nEvts = *p++;
    for(Int_t i=0; i< nEvts; ++i) {
      p += process_event(p);
    }
    return 0;
  }

  sData() { reset(); }
  sData(sData& other) : a(other.a), b(other.b), c(other.c) { };
  ~sData() { }

};


#endif
