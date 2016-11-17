#!/bin/sh

## Describe BioNLP'09 Shared Task files with BioC XML
## Then verify that the information has been preserved

cd bionlp_sample
# create XML files
../bionlp_corpus [0-9]*.txt > bionlp_corpus.xml
../bionlp_annotate > bionlp_annotate.xml
../bionlp_relations > bionlp_relations.xml

cd ../bionlp_copy
# recreate shared task files
../bionlp_corpus_dump ../bionlp_sample/bionlp_corpus.xml
../bionlp_annotate_dump ../bionlp_sample/bionlp_annotate.xml
../bionlp_relations_dump ../bionlp_sample/bionlp_relations.xml

# .txt and .a1 files should be identical
for i in [0-9]*.txt *.a1 ; do
  diff -q $i ../bionlp_sample
done 

# order not preserved for .a2 files
# some files in the other sets vary number of spaces between fields
for i in *.a2 ; do
  # echo $i
  sort $i > z2
  sort ../bionlp_sample/$i > z1
  diff -q -w z1 z2
done
