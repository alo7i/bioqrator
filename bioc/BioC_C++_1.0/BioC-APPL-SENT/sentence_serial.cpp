/**** Produce sentence collection from a document collection
      Read serial, written all from memory
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

  for ( int i = 0; i< collection.documents.size(); ++i ) {
    print_sentences( collection.documents[i] );
  }
}


int
main(int argc, char **argv) {

  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " docname\n";
    return -1;
  }

  char * docname = argv[1];
  
  Collection collection;
  Connector_libxml xml;
  xml.start_read(docname, collection);

  Sentence_Segmenter segmenter;
  Collection sentenceCollection;
  segmenter.convert( collection, sentenceCollection );
  sentenceCollection.key = "sentence.key";

  Connector_libxml xml_writer;
  xml_writer.start_write( "-", sentenceCollection );
  
  Document document;
  while ( xml.read_next(document) ) {
    Document sentenceDocument;
    segmenter.convert( document, sentenceDocument );
    xml_writer.write_next( sentenceDocument );
  }
  
  xml_writer.end_write();
  
  return 0;
}
