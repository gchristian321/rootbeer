//! \file Visitor.hxx
//! \brief Uses the boost::variant class and visitor pattern to...
#ifndef __MAKECINT__
#ifndef VISITOR_HXX
#define VISITOR_HXX
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TTreeFormula.h>
#include "utils/Mutex.hxx"
#include "utils/Error.hxx"
#include "boost/scoped_ptr.hpp"
#include "boost/function.hpp"
#include "boost/variant.hpp"
#include "boost/mem_fn.hpp"
#include "boost/bind.hpp"

namespace rb
{
  /// Histogram (1d, 2d, 3d) variant typedef
  typedef boost::variant<TH1D, TH2D, TH3D> HistVariant;

  /// Encloses visitor classes
  namespace visit
  {
    /// boost::static_visitor derivative that locks/unlocks the global TThread mutex upon creation/destruction
    template <class T>
    struct Locked: public boost::static_visitor<T>
    {
      /// Lock TThread global mutex
      Locked() { rb::TThreadMutex::Instance()->Lock(); }
      /// Unlock TThread global mutex
      ~Locked() { rb::TThreadMutex::Instance()->UnLock(); }
    };
    /// encloses "visitor" functions to be preformed on the histograms
    namespace hist
    {
      /// Clears (zeroes) the histogram bins.
      struct Clear : public rb::visit::Locked<void>
      {
	template <class T> void operator() (T& t) const {
	  for(Int_t p = 0; p < t.fN; ++p) t.fArray[p] = 0.;
	}
	static void Do(HistVariant& hist) {
	  boost::apply_visitor(Clear(), hist);
	}
      };

      /// Clones the histogram into a TH1* pointer.
      struct Clone : public rb::visit::Locked<void>
      {
	template <class T> void operator() (T& t) const {
	  fResultHist.reset(static_cast<TH1*>(t.Clone()));
	}
	static void Do(HistVariant& hist, boost::scoped_ptr<TH1>& result_hist) {
	  boost::apply_visitor(Clone(result_hist), hist);
	}
	Clone(boost::scoped_ptr<TH1>& result_hist):
	  fResultHist(result_hist) {}
      private:
	boost::scoped_ptr<TH1>& fResultHist;
      };

      /// Returns a cast to const TH1*
      /// \warning Does not perform any mutex locking
      struct ConstCast : public boost::static_visitor<const TH1*>
      {
	template <class T> const TH1* operator() (T& t) const {
	  return static_cast<const TH1*> (&t);
	}
      };
      /// Returns a cast to TH1*
      /// \warning Does not perform any mutex locking
      struct Cast : public boost::static_visitor<TH1*>
      {
	template <class T> TH1* operator() (T& t) const {
	  return static_cast<TH1*> (&t);
	}
	static TH1* Do(HistVariant& hist) {
	  return boost::apply_visitor(Cast(), hist);
	}
	static const TH1* Do(const HistVariant& hist) {
	  return boost::apply_visitor(ConstCast(), hist);
	}
      };

      /// Create visitors for any TH1 member function
      //! \note Example
      //! \code
      //! visit::hist::DoConstMember(fHistVariant, &TH1::GetName); //< Calls const member function TH1::GetName()
      //! visit::hist::DoMember(fHistVariant, &TH1::SetNameTitle, "name", "title"); //< Calls non-const member function TH1::SetName("name", "title")
      //! \endcode
#define MAX_MEMBER_FN_ARGUMENTS 9
#define BOOST_VARIANT_VISITATION
#include "hist/Member.hxx"
#undef  BOOST_VARIANT_VISITATION
#undef  MAX_MEMBER_FN_ARGUMENTS
      /*
#include "boost/preprocessor.hpp"
#define REPEAT_ARGS(Z, N, text) \
      BOOST_PP_COMMA_IF(N) BOOST_PP_CAT(text, BOOST_PP_INC(N))
#define REPEAT_ARGS2(Z, N, text) \
      BOOST_PP_COMMA_IF(N) BOOST_PP_CAT(A, BOOST_PP_INC(N)) BOOST_PP_CAT(a, BOOST_PP_INC(N))
#define REPEAT_ARGS3(Z, N, text) \
      BOOST_PP_COMMA_IF(N) BOOST_PP_CAT(fA, BOOST_PP_INC(N)) (BOOST_PP_CAT(a, BOOST_PP_INC(N)))
#define REPEAT_ARGS4(Z, N, text) \
      BOOST_PP_CAT(A, BOOST_PP_INC(N)) BOOST_PP_CAT(fA, BOOST_PP_INC(N)) ;

#define REP(arg) BOOST_PP_REPEAT(N, REPEAT_ARGS, arg)
#define REP2 BOOST_PP_REPEAT(N, REPEAT_ARGS2, )

#define COMMA_0 BOOST_PP_COMMA_IF(N)
#define BOOST_PP_LOCAL_MACRO(N)						\
      template < class R, class TT, class T  COMMA_0  REP(class A) >	\
      struct DoMember : public rb::visit::Locked<R> {			\
      template <class TT> R operator() (TT& t) {			\
	return boost::bind(fFun, &t COMMA_0 REP(_)) (REP(fA));		\
      }									\
	DoMember(R (T::* fun)(REP(A)) COMMA_0 REP2) :			\
	  fFun(fun), BOOST_PP_REPEAT(N, REPEAT_ARGS3, ) {}		\
    private:								\
    BOOST_PP_REPEAT(N, REPEAT_ARGS4, )					\
    R (T::* fFun)(REP(A))						\
      };

#define BOOST_PP_LOCAL_LIMITS (0, MAX_MEMBER_FN_ARGUMENTS)
#include BOOST_PP_LOCAL_ITERATE()
      */

      /// Performs the Fill() function
      struct Fill : public rb::visit::Locked<Int_t>
      {
      public:
	Int_t operator() (TH1D& hst) const { return hst.Fill(x_); }
	Int_t operator() (TH2D& hst) const { return hst.Fill(x_,y_); }
	Int_t operator() (TH3D& hst) const { return hst.Fill(x_,y_,z_); }
	static Int_t Do(HistVariant& hist, Double_t x, Double_t y=0, Double_t z=0) {
	  return boost::apply_visitor(Fill(x,y,z), hist);
	}
	Fill(Double_t x, Double_t y, Double_t z): x_(x), y_(y), z_(z) {}
      private:
	Double_t x_, y_, z_;
      };

    } // namespace hist
  } // namespace visit
} // namespace rb
#endif

#else // Forward declarations for rootcint
namespace boost {
  template <class T> class scoped_ptr<T>;
  template <class T1, class T2, class T3> class variant<T1,T2,T3>;
}
namespace rb { typedef boost::variant<TH1D, TH2D, TH3D> HistVariant; }

#endif
