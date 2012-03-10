//! \file WrapTH1.hxx
//! \brief Wraps TH1 member functions.
//! \details This file provides simple wrappers around the majority of TH1 member functions, 
//! for use in rb::hist::Base derived classes. Each wrapper simply delegates the appropriate 
//! member function to fHistVariant, casted to TH1* using the Cast or ConstCast visitors in
//! Visitor.hxx
//!
//! The file was generated using wrap.py, operating on the XML file TH1.xml, which was
//! produced by running the program gccxml on the root v5.32/01 version of TH1.h
//! Subsequently, member functions that we did not want transferred to rb::hist::Base
//! (or which would not compile) were commented out by hand.
#define AS_TH1 visit::hist::Cast::Do(fHistVariant)

void Add(TF1* h1, Double_t c1 = 1, Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Add(h1, c1, option);
}
void Add(const TH1* h1, Double_t c1 = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Add(h1, c1);
}
void Add(const TH1* h, const TH1* h2, Double_t c1 = 1, Double_t c2 = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Add(h, h2, c1, c2);
}
void AddBinContent(Int_t bin)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->AddBinContent(bin);
}
void AddBinContent(Int_t bin, Double_t w)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->AddBinContent(bin, w);
}
// void Browse(TBrowser* b)
// {
//   rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
//   return AS_TH1->Browse(b);
// }
Double_t Chi2Test(const TH1* h2, Option_t* option = "UU", Double_t* res = 0) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Chi2Test(h2, option, res);
}
Double_t Chi2TestX(const TH1* h2, Double_t& chi2, Int_t& ndf, Int_t& igood, Option_t* option = "UU", Double_t* res = 0) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Chi2TestX(h2, chi2, ndf, igood, option, res);
}
Double_t ComputeIntegral()
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->ComputeIntegral();
}
// void DirectoryAutoAdd(TDirectory* arg0)
// {
//   rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
//   return AS_TH1->DirectoryAutoAdd(arg0);
// }
Int_t DistancetoPrimitive(Int_t px, Int_t py)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->DistancetoPrimitive(px, py);
}
void Divide(TF1* f1, Double_t c1 = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Divide(f1, c1);
}
void Divide(const TH1* h1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Divide(h1);
}
void Divide(const TH1* h1, const TH1* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Divide(h1, h2, c1, c2, option);
}
void Draw(Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Draw(option);
}
TH1* DrawCopy(Option_t* option = "") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->DrawCopy(option);
}
TH1* DrawNormalized(Option_t* option = "", Double_t norm = 1) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->DrawNormalized(option, norm);
}
void DrawPanel()
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->DrawPanel();
}
Int_t BufferEmpty(Int_t action = 0)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->BufferEmpty(action);
}
void Eval(TF1* f1, Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Eval(f1, option);
}
void ExecuteEvent(Int_t event, Int_t px, Int_t py)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->ExecuteEvent(event, px, py);
}
TH1* FFT(TH1* h_output, Option_t* option)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FFT(h_output, option);
}
Int_t Fill(Double_t x)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Fill(x);
}
Int_t Fill(Double_t x, Double_t w)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Fill(x, w);
}
Int_t Fill(const char* name, Double_t w)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Fill(name, w);
}
void FillN(Int_t ntimes, const Double_t* x, const Double_t* w, Int_t stride = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FillN(ntimes, x, w, stride);
}
void FillN(Int_t arg0, const Double_t* arg1, const Double_t* arg2, const Double_t* arg3, Int_t arg4)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FillN(arg0, arg1, arg2, arg3, arg4);
}
void FillRandom(const char* fname, Int_t ntimes = 5000)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FillRandom(fname, ntimes);
}
void FillRandom(TH1* h, Int_t ntimes = 5000)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FillRandom(h, ntimes);
}
Int_t FindBin(Double_t x, Double_t y = 0, Double_t z = 0)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FindBin(x, y, z);
}
Int_t FindFixBin(Double_t x, Double_t y = 0, Double_t z = 0) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FindFixBin(x, y, z);
}
Int_t FindFirstBinAbove(Double_t threshold = 0, Int_t axis = 1) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FindFirstBinAbove(threshold, axis);
}
Int_t FindLastBinAbove(Double_t threshold = 0, Int_t axis = 1) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FindLastBinAbove(threshold, axis);
}
TObject* FindObject(const char* name) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FindObject(name);
}
TObject* FindObject(const TObject* obj) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FindObject(obj);
}
TFitResultPtr Fit(const char* formula, Option_t* option = "", Option_t* goption = "", Double_t xmin = 0, Double_t xmax = 0)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Fit(formula, option, goption, xmin, xmax);
}
TFitResultPtr Fit(TF1* f1, Option_t* option = "", Option_t* goption = "", Double_t xmin = 0, Double_t xmax = 0)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Fit(f1, option, goption, xmin, xmax);
}
void FitPanel()
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FitPanel();
}
TH1* GetAsymmetry(TH1* h2, Double_t c2 = 1, Double_t dc2 = 0)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetAsymmetry(h2, c2, dc2);
}
Int_t GetBufferLength() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBufferLength();
}
Int_t GetBufferSize() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBufferSize();
}
const Double_t* GetBuffer() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBuffer();
}
Double_t* GetIntegral()
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetIntegral();
}
TList* GetListOfFunctions() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetListOfFunctions();
}
Int_t GetNdivisions(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetNdivisions(axis);
}
Color_t GetAxisColor(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetAxisColor(axis);
}
Color_t GetLabelColor(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetLabelColor(axis);
}
Style_t GetLabelFont(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetLabelFont(axis);
}
Float_t GetLabelOffset(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetLabelOffset(axis);
}
Float_t GetLabelSize(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetLabelSize(axis);
}
Style_t GetTitleFont(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetTitleFont(axis);
}
Float_t GetTitleOffset(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetTitleOffset(axis);
}
Float_t GetTitleSize(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetTitleSize(axis);
}
Float_t GetTickLength(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetTickLength(axis);
}
Float_t GetBarOffset() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBarOffset();
}
Float_t GetBarWidth() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBarWidth();
}
Int_t GetContour(Double_t* levels = 0)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetContour(levels);
}
Double_t GetContourLevel(Int_t level) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetContourLevel(level);
}
Double_t GetContourLevelPad(Int_t level) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetContourLevelPad(level);
}
Int_t GetBin(Int_t binx, Int_t biny = 0, Int_t binz = 0) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBin(binx, biny, binz);
}
void GetBinXYZ(Int_t binglobal, Int_t& binx, Int_t& biny, Int_t& binz) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinXYZ(binglobal, binx, biny, binz);
}
Double_t GetBinCenter(Int_t bin) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinCenter(bin);
}
Double_t GetBinContent(Int_t bin) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinContent(bin);
}
Double_t GetBinContent(Int_t binx, Int_t biny) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinContent(binx, biny);
}
Double_t GetBinContent(Int_t binx, Int_t biny, Int_t binz) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinContent(binx, biny, binz);
}
Double_t GetBinError(Int_t bin) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinError(bin);
}
Double_t GetBinError(Int_t binx, Int_t biny) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinError(binx, biny);
}
Double_t GetBinError(Int_t binx, Int_t biny, Int_t binz) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinError(binx, biny, binz);
}
Double_t GetBinLowEdge(Int_t bin) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinLowEdge(bin);
}
Double_t GetBinWidth(Int_t bin) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinWidth(bin);
}
Double_t GetBinWithContent(Double_t c, Int_t& binx, Int_t firstx = 0, Int_t lastx = 0, Double_t maxdiff = 0) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinWithContent(c, binx, firstx, lastx, maxdiff);
}
Double_t GetCellContent(Int_t binx, Int_t biny) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetCellContent(binx, biny);
}
Double_t GetCellError(Int_t binx, Int_t biny) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetCellError(binx, biny);
}
void GetCenter(Double_t* center) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetCenter(center);
}
TDirectory* GetDirectory() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetDirectory();
}
Double_t GetEntries() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetEntries();
}
Double_t GetEffectiveEntries() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetEffectiveEntries();
}
TF1* GetFunction(const char* name) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetFunction(name);
}
Int_t GetDimension() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetDimension();
}
Double_t GetKurtosis(Int_t axis = 1) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetKurtosis(axis);
}
void GetLowEdge(Double_t* edge) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetLowEdge(edge);
}
Double_t GetMaximum(Double_t maxval = 3.4028234663852885981170418348451692544e+38f) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMaximum(maxval);
}
Int_t GetMaximumBin() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMaximumBin();
}
Int_t GetMaximumBin(Int_t& locmax, Int_t& locmay, Int_t& locmaz) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMaximumBin(locmax, locmay, locmaz);
}
Double_t GetMaximumStored() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMaximumStored();
}
Double_t GetMinimum(Double_t minval = -3.4028234663852885981170418348451692544e+38f) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMinimum(minval);
}
Int_t GetMinimumBin() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMinimumBin();
}
Int_t GetMinimumBin(Int_t& locmix, Int_t& locmiy, Int_t& locmiz) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMinimumBin(locmix, locmiy, locmiz);
}
Double_t GetMinimumStored() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMinimumStored();
}
Double_t GetMean(Int_t axis = 1) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMean(axis);
}
Double_t GetMeanError(Int_t axis = 1) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMeanError(axis);
}
Int_t GetNbinsX() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetNbinsX();
}
Int_t GetNbinsY() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetNbinsY();
}
Int_t GetNbinsZ() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetNbinsZ();
}
Double_t GetNormFactor() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetNormFactor();
}
char* GetObjectInfo(Int_t px, Int_t py) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetObjectInfo(px, py);
}
Option_t* GetOption() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetOption();
}
TVirtualHistPainter* GetPainter(Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetPainter(option);
}
Int_t GetQuantiles(Int_t nprobSum, Double_t* q, const Double_t* probSum = 0)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetQuantiles(nprobSum, q, probSum);
}
Double_t GetRandom() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetRandom();
}
void GetStats(Double_t* stats) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetStats(stats);
}
Double_t GetSumOfWeights() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetSumOfWeights();
}
TArrayD* GetSumw2()
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetSumw2();
}
const TArrayD* GetSumw2() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetSumw2();
}
Int_t GetSumw2N() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetSumw2N();
}
Double_t GetRMS(Int_t axis = 1) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetRMS(axis);
}
Double_t GetRMSError(Int_t axis = 1) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetRMSError(axis);
}
Double_t GetSkewness(Int_t axis = 1) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetSkewness(axis);
}
TAxis* GetXaxis() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetXaxis();
}
TAxis* GetYaxis() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetYaxis();
}
TAxis* GetZaxis() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetZaxis();
}
Double_t Integral(Option_t* option = "") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Integral(option);
}
Double_t Integral(Int_t binx1, Int_t binx2, Option_t* option = "") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Integral(binx1, binx2, option);
}
Double_t IntegralAndError(Int_t binx1, Int_t binx2, Double_t& err, Option_t* option = "") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->IntegralAndError(binx1, binx2, err, option);
}
Double_t Interpolate(Double_t x)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Interpolate(x);
}
Double_t Interpolate(Double_t x, Double_t y)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Interpolate(x, y);
}
Double_t Interpolate(Double_t x, Double_t y, Double_t z)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Interpolate(x, y, z);
}
Bool_t IsBinOverflow(Int_t bin) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->IsBinOverflow(bin);
}
Bool_t IsBinUnderflow(Int_t bin) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->IsBinUnderflow(bin);
}
Double_t KolmogorovTest(const TH1* h2, Option_t* option = "") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->KolmogorovTest(h2, option);
}
void LabelsDeflate(Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->LabelsDeflate(axis);
}
void LabelsInflate(Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->LabelsInflate(axis);
}
void LabelsOption(Option_t* option = "h", Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->LabelsOption(option, axis);
}
Long64_t Merge(TCollection* list)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Merge(list);
}
void Multiply(TF1* h1, Double_t c1 = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Multiply(h1, c1);
}
void Multiply(const TH1* h1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Multiply(h1);
}
void Multiply(const TH1* h1, const TH1* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Multiply(h1, h2, c1, c2, option);
}
void Paint(Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Paint(option);
}
void Print(Option_t* option = "") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Print(option);
}
void PutStats(Double_t* stats)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->PutStats(stats);
}
TH1* Rebin(Int_t ngroup = 2, const char* newname = "", const Double_t* xbins = 0)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Rebin(ngroup, newname, xbins);
}
void RebinAxis(Double_t x, TAxis* axis)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->RebinAxis(x, axis);
}
void Rebuild(Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Rebuild(option);
}
void RecursiveRemove(TObject* obj)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->RecursiveRemove(obj);
}
void Reset(Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Reset(option);
}
void ResetStats()
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->ResetStats();
}
void SavePrimitive(ostream& out, Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SavePrimitive(out, option);
}
void Scale(Double_t c1 = 1, Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Scale(c1, option);
}
void SetAxisColor(Color_t color = 1, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetAxisColor(color, axis);
}
void SetAxisRange(Double_t xmin, Double_t xmax, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetAxisRange(xmin, xmax, axis);
}
void SetBarOffset(Float_t offset = 2.5e-1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBarOffset(offset);
}
void SetBarWidth(Float_t width = 5.0e-1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBarWidth(width);
}
void SetBinContent(Int_t bin, Double_t content)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBinContent(bin, content);
}
void SetBinContent(Int_t binx, Int_t biny, Double_t content)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBinContent(binx, biny, content);
}
void SetBinContent(Int_t binx, Int_t biny, Int_t binz, Double_t content)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBinContent(binx, biny, binz, content);
}
void SetBinError(Int_t bin, Double_t error)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBinError(bin, error);
}
void SetBinError(Int_t binx, Int_t biny, Double_t error)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBinError(binx, biny, error);
}
void SetBinError(Int_t binx, Int_t biny, Int_t binz, Double_t error)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBinError(binx, biny, binz, error);
}
void SetBins(Int_t nx, Double_t xmin, Double_t xmax)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBins(nx, xmin, xmax);
}
void SetBins(Int_t nx, const Double_t* xBins)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBins(nx, xBins);
}
void SetBins(Int_t nx, Double_t xmin, Double_t xmax, Int_t ny, Double_t ymin, Double_t ymax)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBins(nx, xmin, xmax, ny, ymin, ymax);
}
void SetBins(Int_t nx, const Double_t* xBins, Int_t ny, const Double_t* yBins)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBins(nx, xBins, ny, yBins);
}
void SetBins(Int_t nx, Double_t xmin, Double_t xmax, Int_t ny, Double_t ymin, Double_t ymax, Int_t nz, Double_t zmin, Double_t zmax)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBins(nx, xmin, xmax, ny, ymin, ymax, nz, zmin, zmax);
}
void SetBins(Int_t nx, const Double_t* xBins, Int_t ny, const Double_t* yBins, Int_t nz, const Double_t* zBins)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBins(nx, xBins, ny, yBins, nz, zBins);
}
void SetBinsLength(Int_t arg0 = -0x00000000000000001)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBinsLength(arg0);
}
void SetBuffer(Int_t buffersize, Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBuffer(buffersize, option);
}
void SetCellContent(Int_t binx, Int_t biny, Double_t content)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetCellContent(binx, biny, content);
}
void SetCellError(Int_t binx, Int_t biny, Double_t content)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetCellError(binx, biny, content);
}
void SetContent(const Double_t* content)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetContent(content);
}
void SetContour(Int_t nlevels, const Double_t* levels = 0)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetContour(nlevels, levels);
}
void SetContourLevel(Int_t level, Double_t value)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetContourLevel(level, value);
}
void SetDirectory(TDirectory* dir)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetDirectory(dir);
}
void SetEntries(Double_t n)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetEntries(n);
}
void SetError(const Double_t* error)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetError(error);
}
void SetLabelColor(Color_t color = 1, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetLabelColor(color, axis);
}
void SetLabelFont(Style_t font = 62, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetLabelFont(font, axis);
}
void SetLabelOffset(Float_t offset = 5.00000000000000010408340855860842566471546888351440429688e-3, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetLabelOffset(offset, axis);
}
void SetLabelSize(Float_t size = 2.00000000000000004163336342344337026588618755340576171875e-2, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetLabelSize(size, axis);
}
void SetMaximum(Double_t maximum = -0x00000000000000457)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetMaximum(maximum);
}
void SetMinimum(Double_t minimum = -0x00000000000000457)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetMinimum(minimum);
}
void SetName(const char* name)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetName(name);
}
void SetNameTitle(const char* name, const char* title)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetNameTitle(name, title);
}
void SetNdivisions(Int_t n = 510, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetNdivisions(n, axis);
}
void SetNormFactor(Double_t factor = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetNormFactor(factor);
}
void SetStats(Bool_t stats = kTRUE)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetStats(stats);
}
void SetOption(Option_t* option = " ")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetOption(option);
}
void SetTickLength(Float_t length = 2.00000000000000004163336342344337026588618755340576171875e-2, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetTickLength(length, axis);
}
void SetTitleFont(Style_t font = 62, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetTitleFont(font, axis);
}
void SetTitleOffset(Float_t offset = 1, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetTitleOffset(offset, axis);
}
void SetTitleSize(Float_t size = 2.00000000000000004163336342344337026588618755340576171875e-2, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetTitleSize(size, axis);
}
void SetTitle(const char* title)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetTitle(title);
}
void SetXTitle(const char* title)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetXTitle(title);
}
void SetYTitle(const char* title)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetYTitle(title);
}
void SetZTitle(const char* title)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetZTitle(title);
}
TH1* ShowBackground(Int_t niter = 20, Option_t* option = "same")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->ShowBackground(niter, option);
}
Int_t ShowPeaks(Double_t sigma = 2, Option_t* option = "", Double_t threshold = 5.000000000000000277555756156289135105907917022705078125e-2)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->ShowPeaks(sigma, option, threshold);
}
void Smooth(Int_t ntimes = 1, Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Smooth(ntimes, option);
}
void Sumw2()
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Sumw2();
}
void UseCurrentStyle()
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->UseCurrentStyle();
}
// TClass* IsA() const
// {
//   rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
//   return AS_TH1->IsA();
// }
// void ShowMembers(TMemberInspector& insp)
// {
//   rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
//   return AS_TH1->ShowMembers(insp);
// }
// void Streamer(TBuffer& b)
// {
//   rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
//   return AS_TH1->Streamer(b);
// }
// void StreamerNVirtual(TBuffer& b)
// {
//   rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
//   return AS_TH1->StreamerNVirtual(b);
// }

#undef AS_TH1
