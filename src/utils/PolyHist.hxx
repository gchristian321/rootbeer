//! \file PolyHist.hxx
//! \brief Uses the boost::variant class and visitor pattern to allow
//! treatment of 1d, 2d, or 3d histograms with just one object.
#ifndef __MAKECINT__
#ifndef POLYHIST_HXX
#define POLYHIST_HXX
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include "boost/variant.hpp"
#include "boost/scoped_ptr.hpp"

typedef boost::variant<TH1D, TH2D, TH3D> HistVariant_t;

/// encloses "visitor" functions to be preformed on the histograms
namespace visit
{
  /// Returns a downcast to TH1, allowing transparent use of virtual functions.
  class Cast : public boost::static_visitor<TH1&>
  {
  public:
    template <class T>
    TH1& operator() (T& hst) const {
      return static_cast<TH1&>(hst);
    }
    static TH1& Do(HistVariant_t& hist) {
      return boost::apply_visitor(Cast(), hist);
    }
  };
  /// Clears (zeroes) the histogram bins.
  class Clear : public boost::static_visitor<>
  {
  public:
    template <class T>
    void operator() (T& hst) const {
      for(Int_t p = 0; p < hst.fN; ++p) hst.fArray[p] = 0.;
    }
    static void Do(HistVariant_t& hist) {
      boost::apply_visitor(Clear(), hist);
    }
  };
  /// Performs the Fill() function
  class Fill : public boost::static_visitor<Int_t>
  {
  private:
    Double_t x_, y_, z_;
  public:
    Fill(Double_t x, Double_t y, Double_t z) :
      x_(x), y_(y), z_(z) {}
    Int_t operator() (TH1D& hst) const {
      return hst.Fill(x_);
    }
    Int_t operator() (TH2D& hst) const {
      return hst.Fill(x_,y_);
    }
    Int_t operator() (TH3D& hst) const {
      return hst.Fill(x_,y_,z_);
    }
    static Int_t Do(HistVariant_t& hist, Double_t x, Double_t y=0, Double_t z=0) {
      return boost::apply_visitor(Fill(x,y,z), hist);
    }
  };
} // namespace visit


/// Wrapper for a dynamically alocated HistVariant and it's visitors
class PolyHist
{
private:
  boost::scoped_ptr<HistVariant_t> fHist;
public:
  /// Initialize fHist as TH1D
  PolyHist(const char* name, const char* title,
	   Int_t bx, Double_t xl, Double_t xh) :
    fHist(new HistVariant_t(TH1D(name, title, bx, xl, xh))){}
  /// Initialize fHist as TH2D
  PolyHist(const char* name, const char* title,
	   Int_t bx, Double_t xl, Double_t xh,
	   Int_t by, Double_t yl, Double_t yh) :
    fHist(new HistVariant_t(TH2D(name, title, bx, xl, xh, by, yl, yh))){}
  /// Initialize fHist as TH3D
  PolyHist(const char* name, const char* title,
	   Int_t bx, Double_t xl, Double_t xh,
	   Int_t by, Double_t yl, Double_t yh,
	   Int_t bz, Double_t zl, Double_t zh) :
    fHist(new HistVariant_t(TH3D(name, title, bx, xl, xh, by, yl, yh, bz, zl, zh))){}
  /// Set fHist to NULL
  PolyHist(): fHist(0){}
  /// Empty
  ~PolyHist(){}
  /// Fill function
  /// \note Unweighted only, weighted would need to be implemented with a different visitor.
  Int_t Fill(Double_t x, Double_t y, Double_t z) {
    return visit::Fill::Do(*fHist,x,y,z);
  }
  /// Clear (zero) function
  void Clear() {
    visit::Clear::Do(*fHist);
  }
  /// Return a pointer to fHist
  HistVariant_t* Get() {
    return fHist.get();
  }
  /// Return a downcast to TH1
  TH1& AsTH1() {
    return visit::Cast::Do(*fHist);
  }
  TH1& operator() () {
    return visit::Cast::Do(*fHist);
  }
  /// Downcast to TH1*, gives pointer-like semantics.
  TH1* operator-> () {
    return &visit::Cast::Do(*fHist);
  }
};
#endif

#else // Forward declarations for rootcint
namespace boost { template <class T> class scoped_ptr<T>; }
class PolyHist;
#endif
