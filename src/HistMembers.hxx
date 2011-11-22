/*! \file HistMembers.hxx
 * \brief Contains histogram member functions whose format
 *  is shared between the various \c rb::HnF classes.
 *  \note The \c HCLASS and \c HBASE macros should be \c #defined
 *  before <tt>#including</tt>.
 */

// Destructor
rb::HCLASS::~HCLASS() {
  HistMutex::Lock();
  gHistograms.Remove(this);
  gHistograms.Compress();
  HistMutex::Unlock();
}

// Draw function
void rb::HCLASS::Draw(Option_t* option) {
  HistMutex::Lock();
  HBASE::Draw(option);
  HistMutex::Unlock();
}

//Regate function
Int_t rb::Hist::Regate(const char* newgate) {
  TTreeFormula tempFormula("temp", fGate.GetExpFormula(), fGate.GetTree());
  if(!tempFormula.GetTree()) return -1;
  HistMutex::Lock();
  fGate.Compile(Hist::CheckGate(newgate).c_str());
  HistMutex::Unlock();
}


