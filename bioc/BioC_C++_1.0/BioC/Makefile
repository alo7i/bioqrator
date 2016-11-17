DEBUG=-g

CPPFLAGS = $(DEBUG) `xml2-config --cflags`

libBioC.a: BioC.o BioC_libxml.o BioC_util.o
	ar rus $@ $?

%.o: %.cpp
	$(CXX) -c $(CPPFLAGS) $< -o $@ -I.

clean: 
	rm libBioC.a *.o
