VPATH = $(PWD)/cint:$(PWD)/src:$(PWD)/lib:$(PWD)/rbgui

### Include the user-defined portion of the makefile
include $(PWD)/user/Makefile.user

### Variable definitions
SRC=$(PWD)/src
OBJ=$(PWD)/obj
CINT=$(PWD)/cint
USER=$(PWD)/user
RBLIB=$(PWD)/lib

ROOTGLIBS = $(shell root-config --glibs) -lXMLParser -lThread -lTreePlayer
RPATH    += -Wl,-rpath,$(ROOTSYS)/lib -Wl,-rpath,$(PWD)/lib
DYLIB=-shared
FPIC=-fPIC
INCFLAGS=-I$(SRC) -I$(CINT) -I$(USER) $(USER_INCLUDES)
DEBUG=-ggdb -O0
CXXFLAGS=$(INCFLAGS) -L$(PWD)/lib $(STOCK_BUFFERS) -DBUFFER_TYPE=$(USER_BUFFER_TYPE) $(DEBUG)


ifdef ROOTSYS
ROOTGLIBS = -L$(ROOTSYS)/lib $(shell $(ROOTSYS)/bin/root-config --glibs) -lXMLParser -lThread -lTreePlayer
CXXFLAGS += -L$(ROOTSYS)/lib $(shell $(ROOTSYS)/bin/root-config --cflags) -I$(ROOTSYS)/include
endif

# optional MIDAS libraries
ifdef MIDASSYS
MIDASLIBS = $(MIDASSYS)/linux/lib/libmidas.a -lutil -lrt
CXXFLAGS += -DMIDAS_ONLINE -DOS_LINUX -Dextname -I$(MIDASSYS)/include
MIDASONLINE=$(OBJ)/midas/TMidasOnline.o
endif

UNAME=$(shell uname)
ifeq ($(UNAME),Darwin)
CXXFLAGS += -DOS_LINUX -DOS_DARWIN
ifdef MIDASSYS
MIDASLIBS = $(MIDASSYS)/darwin/lib/libmidas.a
endif
DYLIB=-dynamiclib -single_module -undefined dynamic_lookup 
FPIC=
RPATH=
endif

COMPILE= g++ $(CXXFLAGS) $(ROOTGLIBS) $(RPATH)



#### MAIN PROGRAM ####
all: rootbeer

rootbeer: $(RBLIB)/libRBHist.so $(RBLIB)/libRootbeer.so $(SRC)/main.cc 
	g++ $(CXXFLAGS) $(INCFLAGS) -lRootbeer $(SRC)/main.cc -o rootbeer $(MIDASLIBS) $(ROOTGLIBS) -lm -lz -lpthread $(RPATH) -I$(ROOTSYS)/include


#### ROOTBEER LIBRARY ####
OBJECTS=$(OBJ)/midas/TMidasEvent.o $(OBJ)/midas/TMidasFile.o $(MIDASONLINE) \
        $(OBJ)/Data.o $(OBJ)/Buffer.o $(OBJ)/User.o $(OBJ)/Canvas.o $(OBJ)/WriteConfig.o $(OBJ)/Rootbeer.o

HEADERS=$(SRC)/Rootbeer.hxx $(SRC)/Data.hxx $(SRC)/Buffer.hxx $(SRC)/midas/*.h $(SRC)/utils/*.h* $(USER_HEADERS)


$(RBLIB)/libRootbeer.so: $(RBLIB)/libRBHist.so cint/RBDictionary.cxx $(USER_SOURCES) $(OBJECTS)
	$(COMPILE) $(DYLIB) $(FPIC) -o $@ -lRBHist $(MIDASLIBS) $(OBJECTS) $(MOBJ) \
-p cint/RBDictionary.cxx $(USER_SOURCES) \

$(OBJ)/Rootbeer.o: $(RBLIB)/libRBHist.so cint/RBDictionary.cxx $(SRC)/Rootbeer.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/Rootbeer.cxx \

$(OBJ)/WriteConfig.o: $(RBLIB)/libRBHist.so cint/RBDictionary.cxx $(SRC)/WriteConfig.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/WriteConfig.cxx \

$(OBJ)/Canvas.o: $(RBLIB)/libRBHist.so cint/RBDictionary.cxx $(SRC)/Canvas.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/Canvas.cxx \

$(OBJ)/User.o: $(RBLIB)/libRBHist.so cint/RBDictionary.cxx $(USER)/Skeleton.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(USER)/Skeleton.cxx \

$(OBJ)/Buffer.o: $(RBLIB)/libRBHist.so cint/RBDictionary.cxx $(SRC)/Buffer.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/Buffer.cxx \

$(OBJ)/Data.o: $(RBLIB)/libRBHist.so cint/RBDictionary.cxx $(SRC)/Data.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/Data.cxx \

$(OBJ)/midas/TMidasFile.o: $(RBLIB)/libRBHist.so cint/RBDictionary.cxx $(SRC)/midas/TMidasFile.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/midas/TMidasFile.cxx \

$(OBJ)/midas/TMidasEvent.o: $(RBLIB)/libRBHist.so cint/RBDictionary.cxx $(SRC)/midas/TMidasEvent.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/midas/TMidasEvent.cxx \

$(OBJ)/midas/TMidasOnline.o: $(RBLIB)/libRBHist.so cint/RBDictionary.cxx $(SRC)/midas/TMidasOnline.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/midas/TMidasOnline.cxx \


cint/RBDictionary.cxx: $(HEADERS) $(USER)/UserLinkdef.h $(CINT)/Linkdef.h
	rootcint -f $@ -c $(CXXFLAGS)  -p $(HEADERS) $(CINT)/Linkdef.h \



#### COMPILE HISTOGRAM LIBRARY ####

$(RBLIB)/libRBHist.so: Hist.cxx cint/HistDictionary.cxx
	$(COMPILE) $(DYLIB) $(FPIC) -lTreePlayer \
-o $@ $(CXXFLAGS) -p $^ \


cint/HistDictionary.cxx: $(SRC)/Hist.hxx $(SRC)/utils/LockingPointer.hxx $(CINT)/HistLinkdef.h
	rootcint -f $@ -c $(CXXFLAGS) -p $^ \



#### REMOVE EVERYTHING GENERATED BY MAKE ####

clean:
	rm -f lib/*.so rootbeer cint/RBDictionary.* cint/HistDictionary.* rbgui/HistDict.* obj/*.o obj/*/*.o



#### FOR DOXYGEN ####

doc:
	cd $(PWD)/doxygen ; doxygen Doxyfile ; cd latex; make; cd $(PWD)

doccopy:
	cd $(PWD)/doxygen ; doxygen Doxyfile ; cd latex; make; cd $(PWD) ; \
	$(PWD)/doxygen/copydoc.sh




#### GUI STUFF ####


GUI=$(PWD)/rbgui
GUIHEADERS=$(GUI)/HistViewer.h $(GUI)/HistMaker.h $(GUI)/TH2D_SF.h $(GUI)/TH1D_SF.h $(GUI)/TH3D_SF.h
GUISOURCES=$(GUI)/HistViewer.cc $(GUI)/HistMaker.cc $(GUI)/TH2D_SF.cc $(GUI)/TH1D_SF.cc $(GUI)/TH3D_SF.cc

GUIROOTFLAGS=-dynamiclib -single_module -undefined dynamic_lookup `root-config --cflags --libs` -lTreePlayer

GUICXXFLAGS=$(FPIC)

gui: librbgui.so

librbgui.so: $(RBLIB)/libRBHist.so HistDict.cxx $(GUISOURCES)
	g++ -L$(PWD)/lib -lRBHist -shared -o $(PWD)/lib/$@ $(GUICXXFLAGS) -I/opt/local/include/root $(GUI)/HistDict.cxx $(GUISOURCES) $(GUIROOTFLAGS) -I$(PWD)/src

HistDict.cxx: $(GUIHEADERS) rbgui/Linkdef.h
	rootcint -f rbgui/$@ -c -Isrc $(GUICXXFLAGS) -p $^

#HistViewer.o:
#	g++ -c $(ROOTFLAGS) $(CXXFLAGS) HistViewer.cc

guiclean:
	rm -f lib/librbgui.so rbgui/HistDict.*
