#ifndef __TH1DSF_HH
#define __TH1DSF_HH
// ROOT library includes
#include <TBranchElement.h>
#include <TH1D.h>
class TH1D_SF // wrapper class for TH1D to help it fill itself
{
    public:
        // public data members
        // TODO: add necessary members to fill from aliases or simply addresses
        TBranchElement* dataxp; // pointer to where the data is coming from to fill

        Int_t ix;               // index for use in GetValue function (mainly
                                // relevant for arrays)

        Double_t* gate;         // pointer to TTreeFormula result (result of a gate
                                // since I'm applying them as TTreeFormulas)

        TH1D* hist;           // histogram to be filled. I chose to have a data
                                // member be the histogram instead of directly
                                // inheriting from TH1D because I believe this 
                                // approach gives the user more freedom to integrate
                                // with other codes that perhaps generate histograms
    public:
        // public functions
        // TODO: add necessary functions to fill from aliases or simply addresses
        Double_t FillFromBranch();    // fill from the datax branch reference
};
typedef std::map<std::string,TH1D_SF*> TH1DMap;
#endif
