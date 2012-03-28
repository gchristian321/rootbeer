### Include the user-defined portion of the makefile
include $(PWD)/user/Makefile.user

### Variable definitions
SRC=$(PWD)/src
GUI=$(SRC)/gui
OBJ=$(PWD)/obj
CINT=$(PWD)/cint
USER=$(PWD)/user
RBLIB=$(PWD)/lib

ROOTGLIBS = $(shell root-config --glibs) -lXMLParser -lThread -lTreePlayer
RPATH    += -Wl,-rpath,$(ROOTSYS)/lib -Wl,-rpath,$(PWD)/lib
DYLIB=-shared
FPIC=-fPIC
INCFLAGS=-I$(SRC) -I$(CINT) -I$(USER) $(USER_INCLUDES)
DEBUG=-ggdb -O0 -DDEBUG
#-DRB_LOGGING
#-DDEBUG
CXXFLAGS=$(DEBUG) $(INCFLAGS) -L$(PWD)/lib $(STOCK_BUFFERS) -DBUFFER_TYPE=$(USER_BUFFER_TYPE)


ifdef ROOTSYS
ROOTGLIBS = -L$(ROOTSYS)/lib $(shell $(ROOTSYS)/bin/root-config --glibs) -lXMLParser -lThread -lTreePlayer
CXXFLAGS += -L$(ROOTSYS)/lib $(shell $(ROOTSYS)/bin/root-config --cflags) -I$(ROOTSYS)/include
else
ROOTGLIBS = $(shell root-config --glibs --cflags) -lXMLParser -lThread -lTreePlayer
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

COMPILER=g++ -Wall
#COMPILER=clang++ -I/opt/local/include/ -I/opt/local/include/root

COMPILE=$(COMPILER) $(CXXFLAGS) $(RPATH) $(DEF_EXT) $(DEF_DIR)
LINK=$(COMPILER) $(CXXFLAGS) $(ROOTGLIBS) $(RPATH) $(DEF_DIR)


#### MAIN PROGRAM ####
all: rootbeer

rootbeer: $(RBLIB)/libRootbeer.so $(SRC)/main.cc 
	$(LINK) -lRootbeer -lRBGui $(SRC)/main.cc -o rootbeer


#### ROOTBEER LIBRARY ####
OBJECTS=$(OBJ)/hist/Hist.o $(OBJ)/hist/Manager.o \
$(OBJ)/Formula.o $(OBJ)/midas/TMidasEvent.o $(OBJ)/midas/TMidasFile.o $(MIDASONLINE) \
$(OBJ)/Data.o $(OBJ)/Event.o $(OBJ)/Buffer.o $(OBJ)/user/User.o $(OBJ)/Canvas.o $(OBJ)/WriteConfig.o \
$(OBJ)/Rint.o $(OBJ)/Signals.o $(OBJ)/Rootbeer.o $(OBJ)/Gui.o $(OBJ)/HistGui.o \
$(OBJ)/TGSelectDialog.o $(OBJ)/TGDivideSelect.o

HEADERS=$(SRC)/Rootbeer.hxx $(SRC)/Rint.hxx $(SRC)/Data.hxx $(SRC)/Buffer.hxx $(SRC)/Event.hxx $(SRC)/user/User.hxx \
$(SRC)/Signals.hxx $(SRC)/Formula.hxx $(SRC)/utils/LockingPointer.hxx $(SRC)/utils/Mutex.hxx \
$(SRC)/hist/Hist.hxx $(SRC)/hist/Visitor.hxx $(SRC)/hist/Manager.hxx $(SRC)/TGSelectDialog.h $(SRC)/TGDivideSelect.h \
$(SRC)/utils/Save.hxx $(SRC)/midas/*.h $(SRC)/utils/*.h* $(USER_HEADERS)


RBlib: $(RBLIB)/libRootbeer.so
$(RBLIB)/libRootbeer.so: $(CINT)/RBDictionary.cxx $(USER_SOURCES) $(OBJECTS) $(RBLIB)/libRBGui.so
	$(LINK) $(DYLIB) $(FPIC) -o $@ -lRBGui $(MIDASLIBS) $(OBJECTS) \
-p $(CINT)/RBDictionary.cxx $(USER_SOURCES) \

Rootbeer: $(OBJ)/Rootbeer.o
$(OBJ)/Rootbeer.o: $(CINT)/RBDictionary.cxx $(SRC)/Rootbeer.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/Rootbeer.cxx \

Rint: $(OBJ)/Rint.o
$(OBJ)/Rint.o: $(CINT)/RBDictionary.cxx $(SRC)/Rint.cxx $(OBJ)/Gui.o $(OBJ)/HistGui.o
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/Rint.cxx \

Signals: $(OBJ)/Signals.o
$(OBJ)/Signals.o: $(CINT)/RBDictionary.cxx $(SRC)/Signals.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/Signals.cxx \

Gui: $(OBJ)/Gui.o
$(OBJ)/Gui.o: $(CINT)/RBDictionary.cxx $(SRC)/GuiLayout.cxx $(SRC)/MakeConnections.hxx
	python gui_edit.py src/GuiLayout.cxx 
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/Gui.cxx \

HistGui: $(OBJ)/HistGui.o
$(OBJ)/HistGui.o: $(CINT)/RBDictionary.cxx $(SRC)/HistGuiLayout.cxx $(SRC)/MakeHistConnections.hxx
	python gui_edit.py src/HistGuiLayout.cxx 
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/HistGui.cxx \

TGSelectDialog: $(OBJ)/TGSelectDialog.o
$(OBJ)/TGSelectDialog.o: $(CINT)/RBDictionary.cxx $(SRC)/TGSelectDialog.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/TGSelectDialog.cxx \

TGDivideSelect: $(OBJ)/TGDivideSelect.o
$(OBJ)/TGDivideSelect.o: $(CINT)/RBDictionary.cxx $(SRC)/TGDivideSelect.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/TGDivideSelect.cxx \

WriteConfig: $(OBJ)/WriteConfig.o
$(OBJ)/WriteConfig.o: $(CINT)/RBDictionary.cxx $(SRC)/WriteConfig.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/WriteConfig.cxx \

Canvas: $(OBJ)/Canvas.o
$(OBJ)/Canvas.o: $(CINT)/RBDictionary.cxx $(SRC)/Canvas.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/Canvas.cxx \

User: $(OBJ)/user/User.o
$(OBJ)/user/User.o: $(CINT)/RBDictionary.cxx $(SRC)/user/User.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/user/User.cxx \

Buffer: $(OBJ)/Buffer.o
$(OBJ)/Buffer.o: $(CINT)/RBDictionary.cxx $(SRC)/Buffer.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/Buffer.cxx \

Event: $(OBJ)/Event.o
$(OBJ)/Event.o: $(CINT)/RBDictionary.cxx $(SRC)/Event.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/Event.cxx \

Data: $(OBJ)/Data.o
$(OBJ)/Data.o: $(CINT)/RBDictionary.cxx $(SRC)/Data.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/Data.cxx \

TMidasFile: $(OBJ)/midas/TMidasFile.o
$(OBJ)/midas/TMidasFile.o: $(CINT)/RBDictionary.cxx $(SRC)/midas/TMidasFile.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/midas/TMidasFile.cxx \

TMidasEvent: $(OBJ)/midas/TMidasEvent.o
$(OBJ)/midas/TMidasEvent.o: $(CINT)/RBDictionary.cxx $(SRC)/midas/TMidasEvent.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/midas/TMidasEvent.cxx \

TMidasOnline: $(OBJ)/midas/TMidasOnline.o
$(OBJ)/midas/TMidasOnline.o: $(CINT)/RBDictionary.cxx $(SRC)/midas/TMidasOnline.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/midas/TMidasOnline.cxx \

Hist: $(OBJ)/hist/Hist.o $(SRC)/hist/WrapTH1.hxx 
$(OBJ)/hist/Hist.o: $(CINT)/RBDictionary.cxx $(SRC)/hist/Hist.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/hist/Hist.cxx \

Manager: $(OBJ)/hist/Manager.o
$(OBJ)/hist/Manager.o: $(CINT)/RBDictionary.cxx $(SRC)/hist/Manager.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/hist/Manager.cxx \

Formula: $(OBJ)/Formula.o
$(OBJ)/Formula.o: $(CINT)/RBDictionary.cxx $(SRC)/Formula.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(SRC)/Formula.cxx \

RBdict: $(CINT)/RBDictionary.cxx
$(CINT)/RBDictionary.cxx:  $(HEADERS) $(USER)/UserLinkdef.h $(CINT)/Linkdef.h \
$(SRC)/utils/Mutex.hxx $(SRC)/utils/LockingPointer.hxx $(SRC)/utils/ANSort.hxx
	rootcint -f $@ -c $(CXXFLAGS)  -p $(HEADERS) $(CINT)/Linkdef.h \


### GUI ###
GUI_HEADERS=$(GUI)/Widget.hxx $(GUI)/MainFrame.hxx $(GUI)/CanvasGui.hxx
GUI_OBJECTS=$(OBJ)/gui/CanvasGui.o

RBGuilib: $(RBLIB)/libRBGui.so
$(RBLIB)/libRBGui.so: $(CINT)/RBGuiDictionary.cxx $(GUI_OBJECTS)
	$(LINK) $(DYLIB) $(FPIC) -o $@ $(GUI_OBJECTS) \
-p $(CINT)/RBGuiDictionary.cxx \

CanvasGui: $(OBJ)/gui/CanvasGui.o
$(OBJ)/gui/CanvasGui.o: $(CINT)/RBGuiDictionary.cxx $(GUI)/CanvasGui.cxx
	$(COMPILE) $(FPIC) -c \
-o $@  -p $(GUI)/CanvasGui.cxx \

Guidict: $(CINT)/RBGuiDictionary.cxx
$(CINT)/RBGuiDictionary.cxx:  $(GUI_HEADERS) $(CINT)/GuiLinkdef.h
	rootcint  -f $@ -c $(CXXFLAGS)  -p $(GUI_HEADERS) $(CINT)/GuiLinkdef.h \



#### REMOVE EVERYTHING GENERATED BY MAKE ####

clean:
	rm -f $(RBLIB)/*.so rootbeer $(CINT)/RBDictionary.* $(CINT)/RBGuiDictionary.* rbgui/HistDict.* $(OBJ)/*.o $(OBJ)/*/*.o



#### FOR DOXYGEN ####

doc:
	cd $(PWD)/doxygen ; doxygen Doxyfile ; cd latex; make; cd $(PWD)



#### GUI STUFF ####


# GUI=$(PWD)/rbgui
# GUIHEADERS=$(GUI)/HistViewer.h $(GUI)/HistMaker.h $(GUI)/TH2D_SF.h $(GUI)/TH1D_SF.h $(GUI)/TH3D_SF.h
# GUISOURCES=$(GUI)/HistViewer.cc $(GUI)/HistMaker.cc $(GUI)/TH2D_SF.cc $(GUI)/TH1D_SF.cc $(GUI)/TH3D_SF.cc

# GUIROOTFLAGS=-dynamiclib -single_module -undefined dynamic_lookup `root-config --cflags --libs` -lTreePlayer

# GUICXXFLAGS=$(FPIC)

# gui: librbgui.so

# librbgui.so: $(RBLIB)/libRBHist.so HistDict.cxx $(GUISOURCES)
# 	g++ -L$(PWD)/lib -lRBHist -shared -o $(PWD)/lib/$@ $(GUICXXFLAGS) -I/opt/local/include/root $(GUI)/HistDict.cxx $(GUISOURCES) $(GUIROOTFLAGS) -I$(PWD)/src

# HistDict.cxx: $(GUIHEADERS) rbgui/Linkdef.h
# 	rootcint -f rbgui/$@ -c -Isrc $(GUICXXFLAGS) -p $^

# #HistViewer.o:
# #	g++ -c $(ROOTFLAGS) $(CXXFLAGS) HistViewer.cc

# guiclean:
# 	rm -f lib/librbgui.so rbgui/HistDict.*
