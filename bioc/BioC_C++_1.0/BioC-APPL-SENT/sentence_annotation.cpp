/**** Produce sentence annotations from a document collection
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

      Annotation annotation;
      annotation.add_location( passage.offset + loc, sentences[i].size() );
      annotation.text = sentences[i];
      passageSent.annotations.push_back(annotation);

      pos = loc + sentences[i].size();
    }

  }

  MPtok tok;
};


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
  sentenceCollection.key = "sentence_annotation.key";

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
