#include "TH1D_SF.h"
Double_t
TH1D_SF::FillFromBranch()
{
    Double_t datax = dataxp->GetValue(ix,0,kFALSE);
    if (*gate && (datax != -1)) // see that gate is satisfied and value of data is good
    {
        hist->Fill(datax);
    }
    return datax;
}
