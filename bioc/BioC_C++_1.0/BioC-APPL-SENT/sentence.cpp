/**** Print a collection from an XML file
 ****/

#include <iostream>
#include <string>
#include <vector>

#include <MPtok.h>

#include "BioC.hpp"
#include "BioC_libxml.hpp"
#include "BioC_util.hpp"

using std::cout;
using std::string;
using std::vector;

using namespace BioC;

class Sentence_Segmenter : public Node_Converter {
public:
  using Node_Converter::convert;

  virtual void convert( const Passage & passage,
                        Passage & passageSent ) {
    passageSent.infons = passage.infons;
    passageSent.offset = passage.offset;
  
    tok.segment( passage.text );

    vector<string> & sentences = tok.sent;
    string::size_type pos = 0;
    for ( int i = 0; i < sentences.size(); ++i ) {
      /* more reliable to use info from MPtok */
      string::size_type loc = passage.text.find( sentences[i], pos );
      if ( loc == string::npos ) {
        cerr << "sentence \"" << sentences[i] << "\" not found "
          "in passage \"" << passage.text << "\"\n";
        exit(-1);
      }
    
      Sentence sentence;
      sentence.offset = passage.offset + loc;
      sentence.text = sentences[i];
      passageSent.sentences.push_back(sentence);

      pos = loc + sentences[i].size();
    }

  }

  MPtok tok;
};


void print_sentences( const Passage & passage ) {

  MPtok tok;
  tok.segment( passage.text );

  vector<string> & sentences = tok.sent;
  for ( int i = 0; i < sentences.size(); ++i ) {
    cout << sentences[i] << '\n';
  }

}

void print_sentences( const Document & document ) {

  for ( int i = 0; i < document.passages.size(); ++i ) {
    print_sentences( document.passages[i] );
  }
}

void print_sentences( const Collection & collection ) {

  for ( int i = 0; i < collection.documents.size(); ++i ) {
    print_sentences( collection.documents[i] );
  }
}


int
main(int argc, char **argv) {

  if (argc <= 1) {
    printf("Usage: %s docname\n", argv[0]);
    return(0);
  }

  char * docname = argv[1];
  Collection collection;

  Connector_libxml xml;
  xml.read(docname, collection);

  //        collection.write();
  //        print_sentences( collection);

  Collection sentenceCollection;

  Sentence_Segmenter segmenter;
  segmenter.convert( collection, sentenceCollection );
  sentenceCollection.key = "sentence.key";
  
  //        sentenceCollection.write();
  
  xml.write( "-", sentenceCollection );
  
  return 0;
}
