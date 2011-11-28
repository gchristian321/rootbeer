#include "TH2D_SF.h"
Double_t
TH2D_SF::FillFromBranch()
{
    Double_t datax = dataxp->GetValue(ix,0,kFALSE);
    Double_t datay = datayp->GetValue(iy,0,kFALSE);
    if (*gate && (datax != -1) && (datay != -1)) // see that gate is satisfied and value of data is good
    {
        hist->Fill(datax,datay);
    }
    return datax;
}

