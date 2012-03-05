#ifndef AM_ITERATING_MEMBER
#define AM_ITERATING_MEMBER
#include "boost/preprocessor/iteration/iterate.hpp"
#include "boost/preprocessor/iteration/local.hpp"

#ifndef MAX_MEMBER_FN_ARGUMENTS
#define MAX_MEMBER_FN_ARGUMENTS 10
#define UNDEF__MAX_MEMBER_FN_ARGUMENTS
#endif

#define BOOST_PP_ITERATION_LIMITS (0,MAX_MEMBER_FN_ARGUMENTS)
#define BOOST_PP_FILENAME_1 "hist/Member.hxx"
#include BOOST_PP_ITERATE()

#define DOCONST
#define BOOST_PP_ITERATION_LIMITS (0,MAX_MEMBER_FN_ARGUMENTS)
#define BOOST_PP_FILENAME_1 "hist/Member.hxx"
#include BOOST_PP_ITERATE()
#undef  DOCONST

#undef AM_ITERATING_MEMBER

#else

#ifdef  DOCONST
#define CONST const
#define NAME  BOOST_PP_CAT(ConstMember, BOOST_PP_ITERATION())
#define FNAME DoConstMember
#else
#define CONST
#define NAME BOOST_PP_CAT(Member, BOOST_PP_ITERATION())
#define FNAME DoMember
#endif
#define NNN BOOST_PP_ITERATION()

#if NNN == 0
template <class R, class T>
struct NAME : public rb::visit::Locked<R>
{
  template <class TT> R operator() (TT& t) CONST {
    return boost::bind(fFun, &t)();
  }
  NAME(R (T::* fun)() CONST) :
    fFun(fun) {}
private:
  R (T::* fFun)() CONST;
};
template <class R, class TT, class T>
inline R FNAME(TT& object, R(T::* function)() CONST) {
  NAME<R, T> member(function);
#ifdef BOOST_VARIANT_VISITATION
  return boost::apply_visitor(member, object);
#else
  member(object);
#endif
}

#elif NNN == 1

template <class R, class T, class A1>
struct NAME : public rb::visit::Locked<R>
{
  template <class TT> R operator() (TT& t) CONST {
    return boost::bind(fFun, &t, _1)(fA1);
  }
  NAME(R (T::* fun)(A1) CONST, A1 a1) :
    fFun(fun), fA1(a1) {}
private:
  A1 fA1;
  R (T::* fFun)(A1) CONST;
};
template <class R, class TT, class T, class A1>
inline R FNAME(TT& object, R(T::* function)(A1) CONST, A1 a1) {
  NAME<R, T, A1> member(function, a1);
#ifdef BOOST_VARIANT_VISITATION
  boost::apply_visitor(member, object);
#else
  member(object);
#endif
}

#else

/// TEMPLATE ARGUMENTS ///
template <class R, class T,
#define BOOST_PP_LOCAL_MACRO(n) class BOOST_PP_CAT(A, n),
#define BOOST_PP_LOCAL_LIMITS (1, BOOST_PP_ITERATION()-1)
#include                    BOOST_PP_LOCAL_ITERATE()
	  class BOOST_PP_CAT(A, NNN)  >
struct NAME : public rb::visit::Locked<R> {

  /// OPERATOR () FUNCTION ///
  template <class TT> R operator() (TT& t) CONST {
    return boost::bind(fFun, &t, 

#define BOOST_PP_LOCAL_MACRO(n) BOOST_PP_CAT(_, n) ,
#define BOOST_PP_LOCAL_LIMITS (1, BOOST_PP_ITERATION()-1)
#include               BOOST_PP_LOCAL_ITERATE()
		       BOOST_PP_CAT(_, BOOST_PP_ITERATION())) (

#define BOOST_PP_LOCAL_MACRO(n) BOOST_PP_CAT(fArg, n) ,
#define BOOST_PP_LOCAL_LIMITS (1, BOOST_PP_ITERATION()-1)
#include                                                       BOOST_PP_LOCAL_ITERATE()
							       BOOST_PP_CAT(fArg, BOOST_PP_ITERATION())
							       );
  }

  /// CONSTRUCTOR ///
  NAME    (R (T::* fun) (
#define BOOST_PP_LOCAL_MACRO(n) BOOST_PP_CAT(A, n) ,
#define BOOST_PP_LOCAL_LIMITS (1, BOOST_PP_ITERATION()-1)
#include                                  BOOST_PP_LOCAL_ITERATE()
			 BOOST_PP_CAT(A, BOOST_PP_ITERATION())
			 ) CONST ,
       
#define BOOST_PP_LOCAL_MACRO(n) BOOST_PP_CAT(A, n) BOOST_PP_CAT(arg, n) ,
#define BOOST_PP_LOCAL_LIMITS (1, BOOST_PP_ITERATION()-1)
#include   BOOST_PP_LOCAL_ITERATE()
	   BOOST_PP_CAT(A, BOOST_PP_ITERATION()) BOOST_PP_CAT(arg, BOOST_PP_ITERATION())) :
    fFun(fun),

#define BOOST_PP_LOCAL_MACRO(n) BOOST_PP_CAT(fArg, n) (BOOST_PP_CAT(arg, n)) ,
#define BOOST_PP_LOCAL_LIMITS (1, BOOST_PP_ITERATION()-1)
#include   BOOST_PP_LOCAL_ITERATE()
    BOOST_PP_CAT(fArg, BOOST_PP_ITERATION()) (BOOST_PP_CAT(arg, BOOST_PP_ITERATION())) {}

private:
#define BOOST_PP_LOCAL_MACRO(n) BOOST_PP_CAT(A, n) BOOST_PP_CAT(fArg, n) ;
#define BOOST_PP_LOCAL_LIMITS (1, BOOST_PP_ITERATION())
#include   BOOST_PP_LOCAL_ITERATE()

  R (T::* fFun) (
#define BOOST_PP_LOCAL_MACRO(n) BOOST_PP_CAT(A, n) ,
#define BOOST_PP_LOCAL_LIMITS (1, BOOST_PP_ITERATION()-1)
#include        BOOST_PP_LOCAL_ITERATE()
		 BOOST_PP_CAT(A, BOOST_PP_ITERATION())
		 ) CONST ;
};

template <class R, class TT, class T,
#define BOOST_PP_LOCAL_MACRO(n) class BOOST_PP_CAT(A, n),
#define BOOST_PP_LOCAL_LIMITS (1, BOOST_PP_ITERATION()-1)
#include                    BOOST_PP_LOCAL_ITERATE()
	  class BOOST_PP_CAT(A, BOOST_PP_ITERATION()) >
inline R FNAME(TT& object, R(T::* function) (
#define BOOST_PP_LOCAL_MACRO(n) BOOST_PP_CAT(A, n) ,
#define BOOST_PP_LOCAL_LIMITS (1, BOOST_PP_ITERATION()-1)
#include                                                   BOOST_PP_LOCAL_ITERATE()
					     BOOST_PP_CAT(A, BOOST_PP_ITERATION())
					     ) CONST ,
       
#define BOOST_PP_LOCAL_MACRO(n) BOOST_PP_CAT(A, n) BOOST_PP_CAT(arg, n) ,
#define BOOST_PP_LOCAL_LIMITS (1, BOOST_PP_ITERATION()-1)
#include   BOOST_PP_LOCAL_ITERATE()
	       BOOST_PP_CAT(A, BOOST_PP_ITERATION()) BOOST_PP_CAT(arg, BOOST_PP_ITERATION())) {
  NAME <R, T, 
#define BOOST_PP_LOCAL_MACRO(n) BOOST_PP_CAT(A, n),
#define BOOST_PP_LOCAL_LIMITS (1, BOOST_PP_ITERATION()-1)
#include                      BOOST_PP_LOCAL_ITERATE()
    BOOST_PP_CAT(A, BOOST_PP_ITERATION()) >
    member (function,
#define BOOST_PP_LOCAL_MACRO(n) BOOST_PP_CAT(arg, n) ,
#define BOOST_PP_LOCAL_LIMITS (1, BOOST_PP_ITERATION()-1)
#include                      BOOST_PP_LOCAL_ITERATE()
	    BOOST_PP_CAT(arg, BOOST_PP_ITERATION()) );

#ifdef BOOST_VARIANT_VISITATION
  return boost::apply_visitor(member, object);
#else
  member(object);
#endif
}

#endif

#ifdef UNDEF__MAX_MEMBER_FN_ARGUMENTS
#undef UNDEF__MAX_MEMBER_FN_ARGUMENTS
#undef MAX_MEMBER_FN_ARGUMENTS
#endif

#undef NNN
#undef NAME
#undef FNAME
#endif

