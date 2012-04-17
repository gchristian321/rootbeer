//! \file ANSort.hxx
//! \brief Defines a functor class to do Alpha-numeric sorting of strings.

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
