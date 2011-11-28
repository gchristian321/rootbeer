HEADERS=HistViewer.h HistMaker.h TH2D_SF.h TH1D_SF.h TH3D_SF.h Hist.hxx
SOURCES=HistViewer.cc HistMaker.cc TH2D_SF.cc TH1D_SF.cc TH3D_SF.cc Hist.cxx
###ROOTFLAGS=`root-config --libs --cflags`
ROOTFLAGS=-dynamiclib -single_module -undefined dynamic_lookup `root-config --cflags --libs` -lTreePlayer

CXXFLAGS=-fPIC

rbgui.so: HistDict.cxx $(SOURCES)
	g++ -shared -o $@ $(CXXFLAGS) -I/opt/local/include/root $^ $(ROOTFLAGS)

HistDict.cxx: $(HEADERS) Linkdef.h
	rootcint -f $@ -c $(CXXFLAGS) -p $^

#HistViewer.o:
#	g++ -c $(ROOTFLAGS) $(CXXFLAGS) HistViewer.cc


clean:
	rm -f rbgui.so HistDict.*