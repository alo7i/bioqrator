DEBUG=-g

BIOC=../BioC

IRET=../iret

CPPFLAGS = -g `xml2-config --cflags` -I$(BIOC) -I$(IRET)
LDFLAGS = -g `xml2-config --libs` -L$(BIOC) -L$(IRET) -lBioC -liret

PGMS=abbr abbr_html

all: $(PGMS)

test:
	./abbr ../BioC-SAMPLE/ascii.xml | diff -q abbr.xml -
	./abbr_html ../BioC-SAMPLE/ascii.xml abbr.xml | diff -q abbr.html -
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
