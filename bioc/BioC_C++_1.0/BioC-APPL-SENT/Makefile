DEBUG=-g

BIOC=../BioC

IRET=../iret

CPPFLAGS = -g `xml2-config --cflags` -I$(BIOC) -I$(IRET)
LDFLAGS = -g `xml2-config --libs` -L$(BIOC) -L$(IRET) -lBioC -liret

PGMS=sentence sentence_annotation sentence_serial

all: $(PGMS)

test:
	./sentence ../BioC-SAMPLE/ascii.xml | diff -q sentence.xml -
	./sentence_serial ../BioC-SAMPLE/ascii.xml | diff -q sentence.xml -
	./sentence_annotation ../BioC-SAMPLE/ascii.xml | diff -q sentence_annotation.xml -
	xmllint --noout --valid *.xml

%: %.cpp
	$(CXX) -c $(CPPFLAGS) $^
	$(CXX) -o $@ $@.o $(LDFLAGS)

%.o:	%.cpp
	$(CXX) -c $(CPPFLAGS) $^

%:	%.o
	$(CXX) -o $@ $^ $(LDFLAGS)

clean: 
	rm $(PGMS) *.o
