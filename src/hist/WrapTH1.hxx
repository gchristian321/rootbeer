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

/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Add">*** TH1 Member Function ***</a>
virtual void Add(TF1* h1, Double_t c1 = 1, Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Add(h1, c1, option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Add">*** TH1 Member Function ***</a>
virtual void Add(const TH1* h1, Double_t c1 = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Add(h1, c1);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Add">*** TH1 Member Function ***</a>
virtual void Add(const TH1* h, const TH1* h2, Double_t c1 = 1, Double_t c2 = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Add(h, h2, c1, c2);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:AddBinContent">*** TH1 Member Function ***</a>
virtual void AddBinContent(Int_t bin)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->AddBinContent(bin);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:AddBinContent">*** TH1 Member Function ***</a>
virtual void AddBinContent(Int_t bin, Double_t w)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->AddBinContent(bin, w);
}
// /// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Browse">*** TH1 Member Function ***</a>
// virtual void Browse(TBrowser* b)
// {
//   rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
//   return AS_TH1->Browse(b);
// }
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Chi2Test">*** TH1 Member Function ***</a>
virtual Double_t Chi2Test(const TH1* h2, Option_t* option = "UU", Double_t* res = 0) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Chi2Test(h2, option, res);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Chi2TestX">*** TH1 Member Function ***</a>
virtual Double_t Chi2TestX(const TH1* h2, Double_t& chi2, Int_t& ndf, Int_t& igood, Option_t* option = "UU", Double_t* res = 0) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Chi2TestX(h2, chi2, ndf, igood, option, res);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:ComputeIntegral">*** TH1 Member Function ***</a>
virtual Double_t ComputeIntegral()
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->ComputeIntegral();
}
// /// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:DirectoryAutoAdd">*** TH1 Member Function ***</a>
// virtual void DirectoryAutoAdd(TDirectory* arg0)
// {
//   rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
//   return AS_TH1->DirectoryAutoAdd(arg0);
// }
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:DistancetoPrimitive">*** TH1 Member Function ***</a>
virtual Int_t DistancetoPrimitive(Int_t px, Int_t py)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->DistancetoPrimitive(px, py);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Divide">*** TH1 Member Function ***</a>
virtual void Divide(TF1* f1, Double_t c1 = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Divide(f1, c1);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Divide">*** TH1 Member Function ***</a>
virtual void Divide(const TH1* h1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Divide(h1);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Divide">*** TH1 Member Function ***</a>
virtual void Divide(const TH1* h1, const TH1* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Divide(h1, h2, c1, c2, option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Draw">*** TH1 Member Function ***</a>
virtual void Draw(Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Draw(option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:DrawCopy">*** TH1 Member Function ***</a>
virtual TH1* DrawCopy(Option_t* option = "") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->DrawCopy(option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:DrawNormalized">*** TH1 Member Function ***</a>
virtual TH1* DrawNormalized(Option_t* option = "", Double_t norm = 1) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->DrawNormalized(option, norm);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:DrawPanel">*** TH1 Member Function ***</a>
virtual void DrawPanel()
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->DrawPanel();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:BufferEmpty">*** TH1 Member Function ***</a>
virtual Int_t BufferEmpty(Int_t action = 0)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->BufferEmpty(action);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Eval">*** TH1 Member Function ***</a>
virtual void Eval(TF1* f1, Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Eval(f1, option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:ExecuteEvent">*** TH1 Member Function ***</a>
virtual void ExecuteEvent(Int_t event, Int_t px, Int_t py)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->ExecuteEvent(event, px, py);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:FFT">*** TH1 Member Function ***</a>
virtual TH1* FFT(TH1* h_output, Option_t* option)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FFT(h_output, option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Fill">*** TH1 Member Function ***</a>
virtual Int_t Fill(Double_t x)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Fill(x);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Fill">*** TH1 Member Function ***</a>
virtual Int_t Fill(Double_t x, Double_t w)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Fill(x, w);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Fill">*** TH1 Member Function ***</a>
virtual Int_t Fill(const char* name, Double_t w)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Fill(name, w);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:FillN">*** TH1 Member Function ***</a>
virtual void FillN(Int_t ntimes, const Double_t* x, const Double_t* w, Int_t stride = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FillN(ntimes, x, w, stride);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:FillN">*** TH1 Member Function ***</a>
virtual void FillN(Int_t arg0, const Double_t* arg1, const Double_t* arg2, const Double_t* arg3, Int_t arg4)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FillN(arg0, arg1, arg2, arg3, arg4);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:FillRandom">*** TH1 Member Function ***</a>
virtual void FillRandom(const char* fname, Int_t ntimes = 5000)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FillRandom(fname, ntimes);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:FillRandom">*** TH1 Member Function ***</a>
virtual void FillRandom(TH1* h, Int_t ntimes = 5000)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FillRandom(h, ntimes);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:FindBin">*** TH1 Member Function ***</a>
virtual Int_t FindBin(Double_t x, Double_t y = 0, Double_t z = 0)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FindBin(x, y, z);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:FindFixBin">*** TH1 Member Function ***</a>
virtual Int_t FindFixBin(Double_t x, Double_t y = 0, Double_t z = 0) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FindFixBin(x, y, z);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:FindFirstBinAbove">*** TH1 Member Function ***</a>
virtual Int_t FindFirstBinAbove(Double_t threshold = 0, Int_t axis = 1) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FindFirstBinAbove(threshold, axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:FindLastBinAbove">*** TH1 Member Function ***</a>
virtual Int_t FindLastBinAbove(Double_t threshold = 0, Int_t axis = 1) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FindLastBinAbove(threshold, axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:FindObject">*** TH1 Member Function ***</a>
virtual TObject* FindObject(const char* name) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FindObject(name);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:FindObject">*** TH1 Member Function ***</a>
virtual TObject* FindObject(const TObject* obj) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FindObject(obj);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Fit">*** TH1 Member Function ***</a>
virtual TFitResultPtr Fit(const char* formula, Option_t* option = "", Option_t* goption = "", Double_t xmin = 0, Double_t xmax = 0)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Fit(formula, option, goption, xmin, xmax);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Fit">*** TH1 Member Function ***</a>
virtual TFitResultPtr Fit(TF1* f1, Option_t* option = "", Option_t* goption = "", Double_t xmin = 0, Double_t xmax = 0)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Fit(f1, option, goption, xmin, xmax);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:FitPanel">*** TH1 Member Function ***</a>
virtual void FitPanel()
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->FitPanel();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetAsymmetry">*** TH1 Member Function ***</a>
TH1* GetAsymmetry(TH1* h2, Double_t c2 = 1, Double_t dc2 = 0)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetAsymmetry(h2, c2, dc2);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetBufferLength">*** TH1 Member Function ***</a>
Int_t GetBufferLength() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBufferLength();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetBufferSize">*** TH1 Member Function ***</a>
Int_t GetBufferSize() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBufferSize();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetBuffer">*** TH1 Member Function ***</a>
const Double_t* GetBuffer() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBuffer();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetIntegral">*** TH1 Member Function ***</a>
virtual Double_t* GetIntegral()
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetIntegral();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetListOfFunctions">*** TH1 Member Function ***</a>
TList* GetListOfFunctions() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetListOfFunctions();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetNdivisions">*** TH1 Member Function ***</a>
virtual Int_t GetNdivisions(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetNdivisions(axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetAxisColor">*** TH1 Member Function ***</a>
virtual Color_t GetAxisColor(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetAxisColor(axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetLabelColor">*** TH1 Member Function ***</a>
virtual Color_t GetLabelColor(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetLabelColor(axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetLabelFont">*** TH1 Member Function ***</a>
virtual Style_t GetLabelFont(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetLabelFont(axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetLabelOffset">*** TH1 Member Function ***</a>
virtual Float_t GetLabelOffset(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetLabelOffset(axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetLabelSize">*** TH1 Member Function ***</a>
virtual Float_t GetLabelSize(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetLabelSize(axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetTitleFont">*** TH1 Member Function ***</a>
virtual Style_t GetTitleFont(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetTitleFont(axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetTitleOffset">*** TH1 Member Function ***</a>
virtual Float_t GetTitleOffset(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetTitleOffset(axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetTitleSize">*** TH1 Member Function ***</a>
virtual Float_t GetTitleSize(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetTitleSize(axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetTickLength">*** TH1 Member Function ***</a>
virtual Float_t GetTickLength(Option_t* axis = "X") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetTickLength(axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetBarOffset">*** TH1 Member Function ***</a>
virtual Float_t GetBarOffset() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBarOffset();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetBarWidth">*** TH1 Member Function ***</a>
virtual Float_t GetBarWidth() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBarWidth();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetContour">*** TH1 Member Function ***</a>
virtual Int_t GetContour(Double_t* levels = 0)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetContour(levels);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetContourLevel">*** TH1 Member Function ***</a>
virtual Double_t GetContourLevel(Int_t level) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetContourLevel(level);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetContourLevelPad">*** TH1 Member Function ***</a>
virtual Double_t GetContourLevelPad(Int_t level) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetContourLevelPad(level);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetBin">*** TH1 Member Function ***</a>
virtual Int_t GetBin(Int_t binx, Int_t biny = 0, Int_t binz = 0) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBin(binx, biny, binz);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetBinXYZ">*** TH1 Member Function ***</a>
virtual void GetBinXYZ(Int_t binglobal, Int_t& binx, Int_t& biny, Int_t& binz) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinXYZ(binglobal, binx, biny, binz);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetBinCenter">*** TH1 Member Function ***</a>
virtual Double_t GetBinCenter(Int_t bin) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinCenter(bin);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetBinContent">*** TH1 Member Function ***</a>
virtual Double_t GetBinContent(Int_t bin) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinContent(bin);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetBinContent">*** TH1 Member Function ***</a>
virtual Double_t GetBinContent(Int_t binx, Int_t biny) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinContent(binx, biny);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetBinContent">*** TH1 Member Function ***</a>
virtual Double_t GetBinContent(Int_t binx, Int_t biny, Int_t binz) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinContent(binx, biny, binz);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetBinError">*** TH1 Member Function ***</a>
virtual Double_t GetBinError(Int_t bin) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinError(bin);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetBinError">*** TH1 Member Function ***</a>
virtual Double_t GetBinError(Int_t binx, Int_t biny) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinError(binx, biny);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetBinError">*** TH1 Member Function ***</a>
virtual Double_t GetBinError(Int_t binx, Int_t biny, Int_t binz) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinError(binx, biny, binz);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetBinLowEdge">*** TH1 Member Function ***</a>
virtual Double_t GetBinLowEdge(Int_t bin) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinLowEdge(bin);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetBinWidth">*** TH1 Member Function ***</a>
virtual Double_t GetBinWidth(Int_t bin) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinWidth(bin);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetBinWithContent">*** TH1 Member Function ***</a>
virtual Double_t GetBinWithContent(Double_t c, Int_t& binx, Int_t firstx = 0, Int_t lastx = 0, Double_t maxdiff = 0) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetBinWithContent(c, binx, firstx, lastx, maxdiff);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetCellContent">*** TH1 Member Function ***</a>
virtual Double_t GetCellContent(Int_t binx, Int_t biny) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetCellContent(binx, biny);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetCellError">*** TH1 Member Function ***</a>
virtual Double_t GetCellError(Int_t binx, Int_t biny) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetCellError(binx, biny);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetCenter">*** TH1 Member Function ***</a>
virtual void GetCenter(Double_t* center) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetCenter(center);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetDirectory">*** TH1 Member Function ***</a>
TDirectory* GetDirectory() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetDirectory();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetEntries">*** TH1 Member Function ***</a>
virtual Double_t GetEntries() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetEntries();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetEffectiveEntries">*** TH1 Member Function ***</a>
virtual Double_t GetEffectiveEntries() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetEffectiveEntries();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetFunction">*** TH1 Member Function ***</a>
virtual TF1* GetFunction(const char* name) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetFunction(name);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetDimension">*** TH1 Member Function ***</a>
virtual Int_t GetDimension() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetDimension();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetKurtosis">*** TH1 Member Function ***</a>
virtual Double_t GetKurtosis(Int_t axis = 1) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetKurtosis(axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetLowEdge">*** TH1 Member Function ***</a>
virtual void GetLowEdge(Double_t* edge) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetLowEdge(edge);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetMaximum">*** TH1 Member Function ***</a>
virtual Double_t GetMaximum(Double_t maxval = 3.4028234663852885981170418348451692544e+38f) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMaximum(maxval);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetMaximumBin">*** TH1 Member Function ***</a>
virtual Int_t GetMaximumBin() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMaximumBin();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetMaximumBin">*** TH1 Member Function ***</a>
virtual Int_t GetMaximumBin(Int_t& locmax, Int_t& locmay, Int_t& locmaz) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMaximumBin(locmax, locmay, locmaz);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetMaximumStored">*** TH1 Member Function ***</a>
virtual Double_t GetMaximumStored() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMaximumStored();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetMinimum">*** TH1 Member Function ***</a>
virtual Double_t GetMinimum(Double_t minval = -3.4028234663852885981170418348451692544e+38f) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMinimum(minval);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetMinimumBin">*** TH1 Member Function ***</a>
virtual Int_t GetMinimumBin() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMinimumBin();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetMinimumBin">*** TH1 Member Function ***</a>
virtual Int_t GetMinimumBin(Int_t& locmix, Int_t& locmiy, Int_t& locmiz) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMinimumBin(locmix, locmiy, locmiz);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetMinimumStored">*** TH1 Member Function ***</a>
virtual Double_t GetMinimumStored() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMinimumStored();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetMean">*** TH1 Member Function ***</a>
virtual Double_t GetMean(Int_t axis = 1) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMean(axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetMeanError">*** TH1 Member Function ***</a>
virtual Double_t GetMeanError(Int_t axis = 1) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetMeanError(axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetNbinsX">*** TH1 Member Function ***</a>
virtual Int_t GetNbinsX() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetNbinsX();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetNbinsY">*** TH1 Member Function ***</a>
virtual Int_t GetNbinsY() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetNbinsY();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetNbinsZ">*** TH1 Member Function ***</a>
virtual Int_t GetNbinsZ() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetNbinsZ();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetNormFactor">*** TH1 Member Function ***</a>
virtual Double_t GetNormFactor() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetNormFactor();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetObjectInfo">*** TH1 Member Function ***</a>
virtual char* GetObjectInfo(Int_t px, Int_t py) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetObjectInfo(px, py);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetOption">*** TH1 Member Function ***</a>
virtual Option_t* GetOption() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetOption();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetPainter">*** TH1 Member Function ***</a>
TVirtualHistPainter* GetPainter(Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetPainter(option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetQuantiles">*** TH1 Member Function ***</a>
virtual Int_t GetQuantiles(Int_t nprobSum, Double_t* q, const Double_t* probSum = 0)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetQuantiles(nprobSum, q, probSum);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetRandom">*** TH1 Member Function ***</a>
virtual Double_t GetRandom() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetRandom();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetStats">*** TH1 Member Function ***</a>
virtual void GetStats(Double_t* stats) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetStats(stats);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetSumOfWeights">*** TH1 Member Function ***</a>
virtual Double_t GetSumOfWeights() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetSumOfWeights();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetSumw2">*** TH1 Member Function ***</a>
virtual TArrayD* GetSumw2()
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetSumw2();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetSumw2">*** TH1 Member Function ***</a>
virtual const TArrayD* GetSumw2() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetSumw2();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetSumw2N">*** TH1 Member Function ***</a>
virtual Int_t GetSumw2N() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetSumw2N();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetRMS">*** TH1 Member Function ***</a>
virtual Double_t GetRMS(Int_t axis = 1) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetRMS(axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetRMSError">*** TH1 Member Function ***</a>
virtual Double_t GetRMSError(Int_t axis = 1) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetRMSError(axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetSkewness">*** TH1 Member Function ***</a>
virtual Double_t GetSkewness(Int_t axis = 1) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetSkewness(axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetXaxis">*** TH1 Member Function ***</a>
TAxis* GetXaxis() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetXaxis();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetYaxis">*** TH1 Member Function ***</a>
TAxis* GetYaxis() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetYaxis();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:GetZaxis">*** TH1 Member Function ***</a>
TAxis* GetZaxis() const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->GetZaxis();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Integral">*** TH1 Member Function ***</a>
virtual Double_t Integral(Option_t* option = "") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Integral(option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Integral">*** TH1 Member Function ***</a>
virtual Double_t Integral(Int_t binx1, Int_t binx2, Option_t* option = "") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Integral(binx1, binx2, option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:IntegralAndError">*** TH1 Member Function ***</a>
virtual Double_t IntegralAndError(Int_t binx1, Int_t binx2, Double_t& err, Option_t* option = "") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->IntegralAndError(binx1, binx2, err, option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Interpolate">*** TH1 Member Function ***</a>
virtual Double_t Interpolate(Double_t x)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Interpolate(x);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Interpolate">*** TH1 Member Function ***</a>
virtual Double_t Interpolate(Double_t x, Double_t y)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Interpolate(x, y);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Interpolate">*** TH1 Member Function ***</a>
virtual Double_t Interpolate(Double_t x, Double_t y, Double_t z)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Interpolate(x, y, z);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:IsBinOverflow">*** TH1 Member Function ***</a>
Bool_t IsBinOverflow(Int_t bin) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->IsBinOverflow(bin);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:IsBinUnderflow">*** TH1 Member Function ***</a>
Bool_t IsBinUnderflow(Int_t bin) const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->IsBinUnderflow(bin);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:KolmogorovTest">*** TH1 Member Function ***</a>
virtual Double_t KolmogorovTest(const TH1* h2, Option_t* option = "") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->KolmogorovTest(h2, option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:LabelsDeflate">*** TH1 Member Function ***</a>
virtual void LabelsDeflate(Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->LabelsDeflate(axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:LabelsInflate">*** TH1 Member Function ***</a>
virtual void LabelsInflate(Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->LabelsInflate(axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:LabelsOption">*** TH1 Member Function ***</a>
virtual void LabelsOption(Option_t* option = "h", Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->LabelsOption(option, axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Merge">*** TH1 Member Function ***</a>
virtual Long64_t Merge(TCollection* list)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Merge(list);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Multiply">*** TH1 Member Function ***</a>
virtual void Multiply(TF1* h1, Double_t c1 = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Multiply(h1, c1);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Multiply">*** TH1 Member Function ***</a>
virtual void Multiply(const TH1* h1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Multiply(h1);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Multiply">*** TH1 Member Function ***</a>
virtual void Multiply(const TH1* h1, const TH1* h2, Double_t c1 = 1, Double_t c2 = 1, Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Multiply(h1, h2, c1, c2, option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Paint">*** TH1 Member Function ***</a>
virtual void Paint(Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Paint(option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Print">*** TH1 Member Function ***</a>
virtual void Print(Option_t* option = "") const
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Print(option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:PutStats">*** TH1 Member Function ***</a>
virtual void PutStats(Double_t* stats)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->PutStats(stats);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Rebin">*** TH1 Member Function ***</a>
virtual TH1* Rebin(Int_t ngroup = 2, const char* newname = "", const Double_t* xbins = 0)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Rebin(ngroup, newname, xbins);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:RebinAxis">*** TH1 Member Function ***</a>
virtual void RebinAxis(Double_t x, TAxis* axis)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->RebinAxis(x, axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Rebuild">*** TH1 Member Function ***</a>
virtual void Rebuild(Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Rebuild(option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:RecursiveRemove">*** TH1 Member Function ***</a>
virtual void RecursiveRemove(TObject* obj)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->RecursiveRemove(obj);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Reset">*** TH1 Member Function ***</a>
virtual void Reset(Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Reset(option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:ResetStats">*** TH1 Member Function ***</a>
virtual void ResetStats()
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->ResetStats();
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SavePrimitive">*** TH1 Member Function ***</a>
virtual void SavePrimitive(ostream& out, Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SavePrimitive(out, option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Scale">*** TH1 Member Function ***</a>
virtual void Scale(Double_t c1 = 1, Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Scale(c1, option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetAxisColor">*** TH1 Member Function ***</a>
virtual void SetAxisColor(Color_t color = 1, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetAxisColor(color, axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetAxisRange">*** TH1 Member Function ***</a>
virtual void SetAxisRange(Double_t xmin, Double_t xmax, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetAxisRange(xmin, xmax, axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetBarOffset">*** TH1 Member Function ***</a>
virtual void SetBarOffset(Float_t offset = 2.5e-1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBarOffset(offset);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetBarWidth">*** TH1 Member Function ***</a>
virtual void SetBarWidth(Float_t width = 5.0e-1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBarWidth(width);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetBinContent">*** TH1 Member Function ***</a>
virtual void SetBinContent(Int_t bin, Double_t content)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBinContent(bin, content);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetBinContent">*** TH1 Member Function ***</a>
virtual void SetBinContent(Int_t binx, Int_t biny, Double_t content)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBinContent(binx, biny, content);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetBinContent">*** TH1 Member Function ***</a>
virtual void SetBinContent(Int_t binx, Int_t biny, Int_t binz, Double_t content)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBinContent(binx, biny, binz, content);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetBinError">*** TH1 Member Function ***</a>
virtual void SetBinError(Int_t bin, Double_t error)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBinError(bin, error);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetBinError">*** TH1 Member Function ***</a>
virtual void SetBinError(Int_t binx, Int_t biny, Double_t error)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBinError(binx, biny, error);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetBinError">*** TH1 Member Function ***</a>
virtual void SetBinError(Int_t binx, Int_t biny, Int_t binz, Double_t error)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBinError(binx, biny, binz, error);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetBins">*** TH1 Member Function ***</a>
virtual void SetBins(Int_t nx, Double_t xmin, Double_t xmax)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBins(nx, xmin, xmax);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetBins">*** TH1 Member Function ***</a>
virtual void SetBins(Int_t nx, const Double_t* xBins)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBins(nx, xBins);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetBins">*** TH1 Member Function ***</a>
virtual void SetBins(Int_t nx, Double_t xmin, Double_t xmax, Int_t ny, Double_t ymin, Double_t ymax)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBins(nx, xmin, xmax, ny, ymin, ymax);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetBins">*** TH1 Member Function ***</a>
virtual void SetBins(Int_t nx, const Double_t* xBins, Int_t ny, const Double_t* yBins)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBins(nx, xBins, ny, yBins);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetBins">*** TH1 Member Function ***</a>
virtual void SetBins(Int_t nx, Double_t xmin, Double_t xmax, Int_t ny, Double_t ymin, Double_t ymax, Int_t nz, Double_t zmin, Double_t zmax)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBins(nx, xmin, xmax, ny, ymin, ymax, nz, zmin, zmax);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetBins">*** TH1 Member Function ***</a>
virtual void SetBins(Int_t nx, const Double_t* xBins, Int_t ny, const Double_t* yBins, Int_t nz, const Double_t* zBins)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBins(nx, xBins, ny, yBins, nz, zBins);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetBinsLength">*** TH1 Member Function ***</a>
virtual void SetBinsLength(Int_t arg0 = -0x00000000000000001)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBinsLength(arg0);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetBuffer">*** TH1 Member Function ***</a>
virtual void SetBuffer(Int_t buffersize, Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetBuffer(buffersize, option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetCellContent">*** TH1 Member Function ***</a>
virtual void SetCellContent(Int_t binx, Int_t biny, Double_t content)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetCellContent(binx, biny, content);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetCellError">*** TH1 Member Function ***</a>
virtual void SetCellError(Int_t binx, Int_t biny, Double_t content)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetCellError(binx, biny, content);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetContent">*** TH1 Member Function ***</a>
virtual void SetContent(const Double_t* content)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetContent(content);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetContour">*** TH1 Member Function ***</a>
virtual void SetContour(Int_t nlevels, const Double_t* levels = 0)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetContour(nlevels, levels);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetContourLevel">*** TH1 Member Function ***</a>
virtual void SetContourLevel(Int_t level, Double_t value)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetContourLevel(level, value);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetDirectory">*** TH1 Member Function ***</a>
virtual void SetDirectory(TDirectory* dir)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetDirectory(dir);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetEntries">*** TH1 Member Function ***</a>
virtual void SetEntries(Double_t n)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetEntries(n);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetError">*** TH1 Member Function ***</a>
virtual void SetError(const Double_t* error)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetError(error);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetLabelColor">*** TH1 Member Function ***</a>
virtual void SetLabelColor(Color_t color = 1, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetLabelColor(color, axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetLabelFont">*** TH1 Member Function ***</a>
virtual void SetLabelFont(Style_t font = 62, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetLabelFont(font, axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetLabelOffset">*** TH1 Member Function ***</a>
virtual void SetLabelOffset(Float_t offset = 5.00000000000000010408340855860842566471546888351440429688e-3, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetLabelOffset(offset, axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetLabelSize">*** TH1 Member Function ***</a>
virtual void SetLabelSize(Float_t size = 2.00000000000000004163336342344337026588618755340576171875e-2, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetLabelSize(size, axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetMaximum">*** TH1 Member Function ***</a>
virtual void SetMaximum(Double_t maximum = -0x00000000000000457)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetMaximum(maximum);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetMinimum">*** TH1 Member Function ***</a>
virtual void SetMinimum(Double_t minimum = -0x00000000000000457)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetMinimum(minimum);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetName">*** TH1 Member Function ***</a>
virtual void SetName(const char* name)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetName(name);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetNameTitle">*** TH1 Member Function ***</a>
virtual void SetNameTitle(const char* name, const char* title)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetNameTitle(name, title);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetNdivisions">*** TH1 Member Function ***</a>
virtual void SetNdivisions(Int_t n = 510, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetNdivisions(n, axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetNormFactor">*** TH1 Member Function ***</a>
virtual void SetNormFactor(Double_t factor = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetNormFactor(factor);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetStats">*** TH1 Member Function ***</a>
virtual void SetStats(Bool_t stats = kTRUE)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetStats(stats);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetOption">*** TH1 Member Function ***</a>
virtual void SetOption(Option_t* option = " ")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetOption(option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetTickLength">*** TH1 Member Function ***</a>
virtual void SetTickLength(Float_t length = 2.00000000000000004163336342344337026588618755340576171875e-2, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetTickLength(length, axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetTitleFont">*** TH1 Member Function ***</a>
virtual void SetTitleFont(Style_t font = 62, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetTitleFont(font, axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetTitleOffset">*** TH1 Member Function ***</a>
virtual void SetTitleOffset(Float_t offset = 1, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetTitleOffset(offset, axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetTitleSize">*** TH1 Member Function ***</a>
virtual void SetTitleSize(Float_t size = 2.00000000000000004163336342344337026588618755340576171875e-2, Option_t* axis = "X")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetTitleSize(size, axis);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetTitle">*** TH1 Member Function ***</a>
virtual void SetTitle(const char* title)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetTitle(title);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetXTitle">*** TH1 Member Function ***</a>
virtual void SetXTitle(const char* title)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetXTitle(title);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetYTitle">*** TH1 Member Function ***</a>
virtual void SetYTitle(const char* title)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetYTitle(title);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:SetZTitle">*** TH1 Member Function ***</a>
virtual void SetZTitle(const char* title)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetZTitle(title);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:ShowBackground">*** TH1 Member Function ***</a>
virtual TH1* ShowBackground(Int_t niter = 20, Option_t* option = "same")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->ShowBackground(niter, option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:ShowPeaks">*** TH1 Member Function ***</a>
virtual Int_t ShowPeaks(Double_t sigma = 2, Option_t* option = "", Double_t threshold = 5.000000000000000277555756156289135105907917022705078125e-2)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->ShowPeaks(sigma, option, threshold);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Smooth">*** TH1 Member Function ***</a>
virtual void Smooth(Int_t ntimes = 1, Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Smooth(ntimes, option);
}
/// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Sumw2">*** TH1 Member Function ***</a>
virtual void Sumw2()
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->Sumw2();
}
// /// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:UseCurrentStyle">*** TH1 Member Function ***</a>
// virtual void UseCurrentStyle()
// {
//   rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
//   return AS_TH1->UseCurrentStyle();
// }
// /// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:IsA">*** TH1 Member Function ***</a>
// virtual TClass* IsA() const
// {
//   rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
//   return AS_TH1->IsA();
// }
// /// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:ShowMembers">*** TH1 Member Function ***</a>
// virtual void ShowMembers(TMemberInspector& insp)
// {
//   rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
//   return AS_TH1->ShowMembers(insp);
// }
// /// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:Streamer">*** TH1 Member Function ***</a>
// virtual void Streamer(TBuffer& b)
// {
//   rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
//   return AS_TH1->Streamer(b);
// }
// /// <a href = "http://root.cern.ch/root/html/TH1.html#TH1:StreamerNVirtual">*** TH1 Member Function ***</a>
// void StreamerNVirtual(TBuffer& b)
// {
//   rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
//   return AS_TH1->StreamerNVirtual(b);
// }

/// <a href = "http://root.cern.ch/root/html/TAttLine.html#TAttLine:ResetAttLine">*** TAttLine Member Function ***</a>
virtual void ResetAttLine(Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->ResetAttLine(option);
}
/// <a href = "http://root.cern.ch/root/html/TAttLine.html#TAttLine:SaveLineAttributes">*** TAttLine Member Function ***</a>
virtual void SaveLineAttributes(ostream& out, const char* name, Int_t coldef = 1, Int_t stydef = 1, Int_t widdef = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SaveLineAttributes(out, name, coldef, stydef, widdef);
}
/// <a href = "http://root.cern.ch/root/html/TAttLine.html#TAttLine:SetLineColor">*** TAttLine Member Function ***</a>
virtual void SetLineColor(Color_t lcolor)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetLineColor(lcolor);
} 
/// <a href = "http://root.cern.ch/root/html/TAttLine.html#TAttLine:SetLineStyle">*** TAttLine Member Function ***</a>
virtual void SetLineStyle(Style_t lstyle)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetLineStyle(lstyle);
}
/// <a href = "http://root.cern.ch/root/html/TAttLine.html#TAttLine:SetLineWidth">*** TAttLine Member Function ***</a>
virtual void SetLineWidth(Width_t lwidth)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetLineWidth(lwidth);
}
/// <a href = "http://root.cern.ch/root/html/TAttFill.html#TAttFill:ResetAttFill">*** TAttFill Member Function ***</a>
virtual void ResetAttFill(Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->ResetAttFill(option);
}
// /// <a href = "http://root.cern.ch/root/html/TAttFill.html#TAttFill:SaveFillAttributes">*** TAttFill Member Function ***</a>
// virtual void SaveFillAttributes(ostream& out, const char* name, Int_t coldef = 1, Int_t stydef = 1001)
// {
//   rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
//   return AS_TH1->SaveFillAttributes(out, name, coldef, stydef, stydef);
// }
/// <a href = "http://root.cern.ch/root/html/TAttFill.html#TAttFill:SetFillColor">*** TAttFill Member Function ***</a>
virtual void SetFillColor(Color_t fcolor)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetFillColor(fcolor);
}
/// <a href = "http://root.cern.ch/root/html/TAttFill.html#TAttFill:SetFillStyle">*** TAttFill Member Function ***</a>
virtual void SetFillStyle(Style_t fstyle)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetFillStyle(fstyle);
}
/// <a href = "http://root.cern.ch/root/html/TAttMarker.html#TAttMarker:ResetAttMarker">*** TAttMarker Member Function ***</a>
virtual void ResetAttMarker(Option_t* toption = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->ResetAttMarker(toption);
}
/// <a href = "http://root.cern.ch/root/html/TAttMarker.html#TAttMarker:SaveMarkerAttributes">*** TAttMarker Member Function ***</a>
virtual void SaveMarkerAttributes(ostream& out, const char* name, Int_t coldef = 1, Int_t stydef = 1, Int_t sizdef = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SaveMarkerAttributes(out, name, coldef, stydef, sizdef);
}
/// <a href = "http://root.cern.ch/root/html/TAttMarker.html#TAttMarker:SetMarkerColor">*** TAttMarker Member Function ***</a>
virtual void SetMarkerColor(Color_t tcolor = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetMarkerColor(tcolor);
}
/// <a href = "http://root.cern.ch/root/html/TAttMarker.html#TAttMarker:SetMarkerSize">*** TAttMarker Member Function ***</a>
virtual void SetMarkerSize(Size_t msize = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetMarkerSize(msize);
}
/// <a href = "http://root.cern.ch/root/html/TAttMarker.html#TAttMarker:SetMarkerStyle">*** TAttMarker Member Function ***</a>
virtual void SetMarkerStyle(Style_t mstyle = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetMarkerStyle(mstyle);
}

#undef AS_TH1
