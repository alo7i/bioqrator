DEBUG=-g

libiret.a: Ab3P.o AbbrStra.o AbbrvE.o Bnum.o Btree.o DataObj.o FBase.o Hash.o MPtok.o Nlphr.o Strset.o runn.o UnicodeIretMap.o
	ar rus $@ $?

%.o: %.C
	$(CXX) -c $(DEBUG) $< -o $@ -I.

%.o: %.cpp
	$(CXX) -c $(DEBUG) $< -o $@ -I.

clean: 
	rm libiret.a *.o
