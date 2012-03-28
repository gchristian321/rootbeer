//! \file Manager.hxx
//! \brief Defines a class for managing histogram creation, destruction
//! and storage in a container.
#ifndef HIST_MANAGER_HXX
#define HIST_MANAGER_HXX
#include <typeinfo>
#include "utils/Mutex.hxx"


namespace rb
{
namespace hist
{
// ========= Forward Declarations ========= //
class Base;

// ========= Typedefs ========= //
typedef std::set<rb::hist::Base*> Container_t;

/// \brief Wraps a container of all histograms registered to this event type.
//! \details Also takes care of functions for Adding and Deleting histograms.
class Manager
{
private:
	 //! Container of pointers to histograms registered to this event type.
	 volatile Container_t fSet;

	 //! Mutex to protect access to fSet
public:
	 rb::Mutex fSetMutex;

public:
	 //! Fill all histograms in fSet
	 void FillAll();
	 //! Does nothing
	 Manager();
	 //! Deletes all entries in fSet
	 ~Manager();
	 //! Searches for a histogram by it's fHistogram address
	 Base* FindByTH1(TH1* hist);
	 //! Searches for a histogram by it's name.
	 Base* FindByName(const char* name, TDirectory* owner);
	 //! Create a new 1d histogram and add to fSet
	 template<typename T>
	 rb::hist::Base* Create(const char* name, const char* title, const char* param, const char* gate, Int_t event_code,
													Int_t nbinsx, Double_t xlow, Double_t xhigh) {
		 rb::hist::Base* hist =
				new T(name, title, param, gate, this, event_code, nbinsx, xlow, xhigh);
		 Add(hist); return hist;
	 }
	 //! Create a new 1d histogram and add to fSet, with optional extra specifications
	 template<typename T, typename OPTIONAL>
	 rb::hist::Base* Create(const char* name, const char* title, const char* param, const char* gate, Int_t event_code,
													Int_t nbinsx, Double_t xlow, Double_t xhigh, const OPTIONAL& optional) {
		 rb::hist::Base* hist =
				new T(name, title, param, gate, this, event_code, nbinsx, xlow, xhigh, optional);
		 Add(hist); return hist;
	 }
	 //! Create a new 2d histogram and add to fSet
	 template<typename T>
	 rb::hist::Base* Create(const char* name, const char* title, const char* param, const char* gate, Int_t event_code,
													Int_t nbinsx, Double_t xlow, Double_t xhigh,
													Int_t nbinsy, Double_t ylow, Double_t yhigh) {
		 rb::hist::Base* hist =
				new T(name, title, param, gate, this, event_code, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
		 Add(hist); return hist;
	 }
	 //! Create a new 2d histogram and add to fSet, with optional extra specifications
	 template<typename T, typename OPTIONAL>
	 rb::hist::Base* Create(const char* name, const char* title, const char* param, const char* gate, Int_t event_code,
													Int_t nbinsx, Double_t xlow, Double_t xhigh,
													Int_t nbinsy, Double_t ylow, Double_t yhigh, const OPTIONAL& optional) {
		 rb::hist::Base* hist =
				new T(name, title, param, gate, this, event_code, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh, optional);
		 Add(hist); return hist;
	 }
	 //! Create a new 3d histogram and add to fSet
	 template<typename T>
	 rb::hist::Base* Create(const char* name, const char* title, const char* param, const char* gate, Int_t event_code,
													Int_t nbinsx, Double_t xlow, Double_t xhigh,
													Int_t nbinsy, Double_t ylow, Double_t yhigh,
													Int_t nbinsz, Double_t zlow, Double_t zhigh) {
		 rb::hist::Base* hist =
				new T(name, title, param, gate, this, event_code, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh, nbinsz, zlow, zhigh);
		 Add(hist); return hist;
	 }
	 //! Create a new 3d histogram and add to fSet, with optional extra specifications
	 template<typename T, typename OPTIONAL>
	 rb::hist::Base* Create(const char* name, const char* title, const char* param, const char* gate, Int_t event_code,
													Int_t nbinsx, Double_t xlow, Double_t xhigh,
													Int_t nbinsy, Double_t ylow, Double_t yhigh,
													Int_t nbinsz, Double_t zlow, Double_t zhigh, const OPTIONAL& optional) {
		 rb::hist::Base* hist =
				new T(name, title, param, gate, this, event_code, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh, nbinsz, zlow, zhigh, optional);
		 Add(hist); return hist;
	 }
	 //! Delete all histogram instances
	 void DeleteAll();
private:
	 //! Add a histogram to fSet
	 void Add(rb::hist::Base* hist);
	 //! Remove a histogram from fSet
	 void Remove(rb::hist::Base* hist);
	 //! Allow access to the created histograms
	 friend class rb::hist::Base;
};
}
}


// ========= Inlined Functions ========= //
inline rb::hist::Manager::Manager(): fSetMutex("SetMutex", true) {
}

inline rb::hist::Manager::~Manager() {
  DeleteAll();
}

#endif
