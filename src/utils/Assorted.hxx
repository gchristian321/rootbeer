//! \file Assorted.hxx
//! \brief Contains random utility functions that don't fir anywhere else.
#ifndef RB_UTILS_ASSORTED_HXX
#define RB_UTILS_ASSORTED_HXX
#ifndef __MAKECINT__
#include <string>
#include <TError.h>
#include <TString.h>
#include "Error.hxx"



class TErrorIgnore {
private:
	Int_t fIgnore;
public:
	TErrorIgnore(Int_t level):
		fIgnore(gErrorIgnoreLevel)
		{ gErrorIgnoreLevel = level; }
	~TErrorIgnore()
		{ gErrorIgnoreLevel = fIgnore; }
};


namespace {

#ifdef RB_DEFAULT_FILE_DIRECTORY
const TString kFileStaticDefault = RB_DEFAULT_FILE_DIRECTORY;
#else
const TString kFileStaticDefault = ".";
#endif

#ifdef RB_DEFAULT_CONFIG_DIRECTORY
const TString kConfigStaticDefault = RB_DEFAULT_CONFIG_DIRECTORY;
#else
const TString kConfigStaticDefault = ".";
#endif

#ifdef RB_DEFAULT_SAVE_DIRECTORY
const TString kSaveStaticDefault = RB_DEFAULT_SAVE_DIRECTORY;
#else
const TString kSaveStaticDefault = ".";
#endif

inline TString expand_path(const TString& static_, const TString& dynamic_) {
	TString out = dynamic_;
	Bool_t expandResult;
	{
		TErrorIgnore nomsg(3001);
		expandResult = gSystem->ExpandPathName(out);
	}
	if(expandResult == kTRUE) { // Not in environment
		out = static_;
	}
	FileStat_t dummy;
	if(gSystem->GetPathInfo(out, dummy)) { // Directory doesn't exist
		out = static_;
	}
	return out;
}
inline std::string expand_path_std(const TString& static_, const char* dynamic_) {
	return std::string(expand_path(static_, dynamic_).Data());
}

//\\\\\\\\ ALPHA-NUMERIC SORTING \\\\\\\\\//

namespace {
void to_lower(std::string& str) {
  std::string l = "abcdefghijklmnopqrstuvwxyz";
  std::string u = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  for(unsigned i=0; i< str.size(); ++i) {
    unsigned long upos = u.find(str[i]);
    if(upos< u.size())
       str[i] = l[upos];
  }
}
std::string get_number(const std::string& str, unsigned posn) {
  std::string num = "0123456789", out = "";
  while(posn < str.size() && num.find(str[posn]) < num.size())
     out += std::string(1, str[posn++]);
  return out;
}
}

struct ANSort { // alpha-numeric sorting
   bool operator() (const std::string& lhs, const std::string& rhs) {
     std::string l = "_abcdefghijklmnopqrstuvwxyz";
     std::string left(lhs), right(rhs);
     to_lower(left); to_lower(right);

     for(unsigned int i=0; i< left.size(); ++i) {
       if(!get_number(left, i).empty()) {
         if(!get_number(right, i).empty()) {
           int ileft = atoi(get_number(left, i).c_str());
           int iright = atoi(get_number(right, i).c_str());
           if(ileft != iright) return ileft < iright;
           else i += get_number(left, i).size()-1;
         }
       }
       else if(l.find(left[i]) != l.find(right[i]))
          return l.find(left[i]) < l.find(right[i]);
     }
     return false;
   }
};

}


#endif
#endif
