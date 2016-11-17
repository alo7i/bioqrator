DEBUG=-g

BIOC=../BioC

IRET=../iret

CPPFLAGS = -g `xml2-config --cflags` -I$(BIOC) -I$(IRET)
LDFLAGS = -g `xml2-config --libs` -L$(BIOC) -L$(IRET) -lBioC -liret

PGMS=bionlp_corpus bionlp_corpus_dump bionlp_relations bionlp_relations_dump bionlp_annotate bionlp_annotate_dump


all: $(PGMS)             

test: 
	./bionlp_test.sh

%: %.cpp
	$(CXX) -c $(CPPFLAGS) $^
	$(CXX) -o $@ $@.o $(LDFLAGS)

%.o:	%.cpp
	$(CXX) -c $(CPPFLAGS) $^

%:	%.o
	$(CXX) -o $@ $^ $(LDFLAGS)

clean: 
	rm $(PGMS) *.o
