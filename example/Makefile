ROOTLIBS:= $(shell root-config --glibs) -lXMLParser -lThread -lTreePlayer
ROOTFLAGS:= $(shell root-config --cflags)
CXXFLAGS=-I$(PWD) -I$(PWD)/../src

ifeq ($(UNAME),Darwin)
SHARED=-dynamiclib -single_module -undefined dynamic_lookup 
FPIC=
else
SHARED+=-shared
FPIC=-fPIC
endif


all: rbexample

%.o: %.cxx %.hxx
	$(CXX) -c $< -o $@

AnalyzerDict.cxx: Analyzer.hxx
	rootcint -f $@ -c $(CXXFLAGS) -p Analyzer.hxx Linkdef.h

libAnalyzer.so: Analyzer.o AnalyzerDict.cxx
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(ROOTLIBS) $(SHARED) $(FPIC) $^ -o $@

rbexample: rbexample.cxx rbexample.hxx libAnalyzer.so
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(ROOTLIBS) -L$(PWD) -L$(PWD)/../lib -lAnalyzer -lRootbeer $< -o $@

clean:
	rm -f *.o AnalyzerDict.* *.so rbexample