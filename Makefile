SRC=$(PWD)/src
SOURCES=$(SRC)/Rootbeer.cxx $(SRC)/Unpacker.cxx $(SRC)/Hist.cxx $(SRC)/CanvasUpdate.cxx
HEADERS=$(SRC)/Unpacker.hxx $(SRC)/Rootbeer.hxx $(SRC)/Hist.hxx $(SRC)/Mutex.hxx
LINKDEF=cint/Linkdef.h
ROOTFLAGS=-dynamiclib -single_module -undefined dynamic_lookup `root-config --cflags --libs`
INCFLAGS=-I$(SRC)

all: rootbeer

rootbeer: libRootbeer $(SRC)/main.cc
	g++ $(SRC)/main.cc -D__MAIN__ -o rootbeer `root-config --cflags --libs` $(INCFLAGS) -Llib -lRootbeer

libRootbeer: Dictionary $(SOURCES) $(HEADERS) $(SRC)/HistMembers.hxx $(LINKDEF)
	g++ $(DEFINITIONS) -o $(PWD)/lib/libRootbeer.so $(CXXFLAGS) -Icint -p cint/Dictionary.cxx \
	$(SOURCES) $(ROOTFLAGS) -lTreePlayer -lThread $(BOOSTLIBS)

Dictionary: $(HEADERS) $(LINKDEF)
	rootcint -f cint/Dictionary.cxx -c $(CXXFLAGS) $(DEFINITIONS) -p $^

doc:
	cd $(PWD)/doxygen ; doxygen Doxyfile ; cd latex; make; cd $(PWD)

doccopy:
	cd $(PWD)/doxygen ; doxygen Doxyfile ; cd latex; make; cd $(PWD) ; \
	$(PWD)/doxygen/copydoc.sh

clean:
	rm -f lib/libRootbeer.so rootbeer cint/Dictionary.*
