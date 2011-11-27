SRC=$(PWD)/src
SOURCES=$(SRC)/Rootbeer.cxx $(SRC)/Unpack.cxx $(SRC)/Hist.cxx $(SRC)/Canvas.cxx $(SRC)/Data.cxx $(SRC)/WriteConfig.cxx
HEADERS=$(SRC)/Rootbeer.hxx $(SRC)/Hist.hxx $(SRC)/Data.hxx \
	$(SRC)/sData.hxx
LINKDEF=cint/Linkdef.h
ROOTFLAGS=-dynamiclib -single_module -undefined dynamic_lookup `root-config --cflags --libs`
INCFLAGS=-I$(SRC) -I$(PWD)/cint
CXXFLAGS=-ggdb -O0

all: rootbeer

rootbeer: libRootbeer $(SRC)/main.cc
	g++ $(SRC)/main.cc -D__MAIN__ -o rootbeer `root-config --cflags --libs` $(INCFLAGS) -Llib -lRootbeer

libRootbeer: Dictionary $(SOURCES) $(HEADERS) $(LINKDEF)
	g++ $(DEFINITIONS) -o $(PWD)/lib/libRootbeer.so $(CXXFLAGS) -Icint -p cint/Dictionary.cxx \
	$(SOURCES) $(ROOTFLAGS) -lTreePlayer -lThread $(BOOSTLIBS)

Dictionary: $(HEADERS) $(LINKDEF)
	$(PWD)/cint/auto_linkdef.sh ; rootcint -f cint/Dictionary.cxx -c $(CXXFLAGS) $(DEFINITIONS) -p $^

doc:
	cd $(PWD)/doxygen ; doxygen Doxyfile ; cd latex; make; cd $(PWD)

doccopy:
	cd $(PWD)/doxygen ; doxygen Doxyfile ; cd latex; make; cd $(PWD) ; \
	$(PWD)/doxygen/copydoc.sh

clean:
	rm -f lib/libRootbeer.so rootbeer cint/Dictionary.*
