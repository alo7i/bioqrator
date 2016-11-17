/**** Identify tokens in a sentence xml file
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

class Token_Converter : public Node_Converter {
public:
  using Node_Converter::convert;
  
  virtual void convert( const Sentence & sentence,
                        Sentence & tokenSentence ) {
    tokenSentence.offset = sentence.offset;
    
    tok.set_segment(0);           // do not split sentences
    tok.tokenize( sentence.text );

    vector<string> & tokens = tok.word;
    string::size_type pos = 0;
    for ( int i = 0; i < tokens.size(); ++i ) {
      /* more reliable to use info from MPtok */
      string::size_type loc = sentence.text.find( tokens[i], pos );
      if ( loc == string::npos ) {
        cerr << "token \"" << tokens[i] << "\" not found "
          "in sentence \"" << sentence.text << "\"\n";
        exit(-1);
      }
    
      Annotation annotation;
      //      annotation.infons["type"] = "token";
      annotation.add_location( sentence.offset + loc, tokens[i].size() );
      annotation.text = tokens[i];
      tokenSentence.annotations.push_back(annotation);
      
      pos = loc + tokens[i].size();
    }
  }

  MPtok tok;

};


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

  Collection tokenCollection;

  Token_Converter converter;
  converter.convert( collection, tokenCollection );
  tokenCollection.key = "tokens.key";

  //        sentenceCollection.write();

  xml.write( "-", tokenCollection );
        
  return 0;
}
