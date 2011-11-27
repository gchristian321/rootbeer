#!/bin/bash

# where the script runs from
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"


# print standard stuff
echo \
"/** \file Linkdef.h */
/*! \brief Tells CINT to generate dictionaries for our classes. */
 
 
#ifdef __CINT__ 
#pragma link off all globals; 
#pragma link off all classes; 
#pragma link off all functions; 
#pragma link C++ nestedclasses; 
 
#pragma link C++ namespace rb; 
#pragma link C++ namespace rb::canvas; 
 
#pragma link C++ class rb::Hist+; 
#pragma link C++ class rb::H1D+; 
#pragma link C++ class rb::H2D+; 
#pragma link C++ class rb::H3D+; 
 
#pragma link C++ class rb::MData+; 

// User templates" > $DIR/Linkdef.h 

## try to figure out the user classes and template #pragmas using awk
awk -F ", " '
     /ADD_CLASS_INSTANCE/ {
          if(index($1, "#define") == 0 && index($1, "#undef") == 0 && index($1, "//") == 0) {
                printf("#pragma link C++ class %s+;\n", $2);
                printf("#pragma link C++ class rb::Data<%s>+;\n", $2);
          }
}' $DIR/../src/Skeleton.hh  >>$DIR/Linkdef.h

# the end
echo "

#endif // #ifdef __CINT__"  >> $DIR/Linkdef.h


