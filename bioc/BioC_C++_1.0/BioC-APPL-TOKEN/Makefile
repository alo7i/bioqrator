DEBUG=-g

BIOC=../BioC

IRET=../iret

CPPFLAGS = -g `xml2-config --cflags` -I$(BIOC) -I$(IRET)
LDFLAGS = -g `xml2-config --libs` -L$(BIOC) -L$(IRET) -lBioC -liret

PGMS=tokens

all: $(PGMS)

test:
	./tokens ../BioC-APPL-SENT/sentence.xml | diff -q tokens.xml -
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
