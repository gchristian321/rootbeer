VPATH = $(PWD)/cint:$(PWD)/src:$(PWD)/lib:$(PWD)/rbgui
SRC=$(PWD)/src
CINT=$(PWD)/cint
ROOTFLAGS=`root-config --cflags --libs`
DYLIB=-dynamiclib -single_module -undefined dynamic_lookup 


### In case your ROOT headers aren't searched for automatically, add
# -I/where/your/root/headers/are to the next line.
INCFLAGS=-I$(SRC) -I$(CINT)
CXXFLAGS=$(INCFLAGS) -L$(PWD)/lib
####CXXFLAGS=-ggdb -O0

USER_HEADERS=$(SRC)/ExampleData.hh
USER_SOURCES=

#### MAIN PROGRAM ####
all: rootbeer


rootbeer: libHist.so libRootbeer.so $(SRC)/main.cc 
	g++ $(SRC)/main.cc -o rootbeer $(ROOTFLAGS) $(CXXFLAGS) -lHist -lRootbeer



#### ROOTBEER LIBRARY ####
SOURCES=$(SRC)/Rootbeer.cxx $(SRC)/Data.cxx $(SRC)/Unpack.cxx $(SRC)/Canvas.cxx $(SRC)/WriteConfig.cxx $(USER_SOURCES)
HEADERS=$(SRC)/Rootbeer.hxx $(SRC)/Data.hxx $(USER_HEADERS)

libRootbeer.so: libHist.so cint/RBDictionary.cxx $(SOURCES) Skeleton.hh
	g++ -lHist -o lib/$@ $(CXXFLAGS) $(ROOTFLAGS) -lThread $(DYLIB) -p cint/RBDictionary.cxx $(SOURCES)


cint/RBDictionary.cxx: $(HEADERS) Linkdef.h
	$(PWD)/cint/auto_linkdef.sh ; rootcint -f $@ -c $(CXXFLAGS)  -p $^



#### COMPILE HISTOGRAM LIBRARY ####

libHist.so: Hist.cxx cint/HistDictionary.cxx
	g++ -o lib/$@ $(CXXFLAGS) $(ROOTFLAGS) -lTreePlayer $(DYLIB) -p $^


cint/HistDictionary.cxx: Hist.hxx HistLinkdef.h
	rootcint -f $@ -c $(CXXFLAGS) -p $^



#### REMOVE EVERYTHING GENERATED BY MAKE ####

clean:
	rm -f lib/*.so rootbeer cint/RBDictionary.* cint/HistDictionary.* rbgui/HistDict.*




#### FOR DOXYGEN ####

doc:
	cd $(PWD)/doxygen ; doxygen Doxyfile ; cd latex; make; cd $(PWD)

doccopy:
	cd $(PWD)/doxygen ; doxygen Doxyfile ; cd latex; make; cd $(PWD) ; \
	$(PWD)/doxygen/copydoc.sh




#### GUI STUFF ####


GUI=$(PWD)/rbgui
GUIHEADERS=$(GUI)/HistViewer.h $(GUI)/HistMaker.h $(GUI)/TH2D_SF.h $(GUI)/TH1D_SF.h $(GUI)/TH3D_SF.h $(SRC)/Hist.hxx
GUISOURCES=$(GUI)/HistViewer.cc $(GUI)/HistMaker.cc $(GUI)/TH2D_SF.cc $(GUI)/TH1D_SF.cc $(GUI)/TH3D_SF.cc $(SRC)/Hist.cxx

GUIROOTFLAGS=-dynamiclib -single_module -undefined dynamic_lookup `root-config --cflags --libs` -lTreePlayer

GUICXXFLAGS=-fPIC

gui: librbgui.so

librbgui.so: libHist.so HistDict.cxx $(GUISOURCES)
	g++ -shared -o lib/$@ $(GUICXXFLAGS) -I/opt/local/include/root $(GUI)/HistDict.cxx $(GUISOURCES) $(GUIROOTFLAGS)

HistDict.cxx: $(GUIHEADERS) rbgui/Linkdef.h
	rootcint -f rbgui/$@ -c $(GUICXXFLAGS) -p $^

#HistViewer.o:
#	g++ -c $(ROOTFLAGS) $(CXXFLAGS) HistViewer.cc

