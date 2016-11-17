/****  Dump BioNLP corpus to .txt files
 ****/

#include <iostream>
#include <fstream>

#include "BioC.hpp"
#include "BioC_libxml.hpp"

using std::cerr;

using namespace BioC;

void dump_document( Document & doc ) {

  string filename = doc.id + ".txt";
  std::ofstream file( filename.c_str() );
  if ( ! file ) {
    cerr << "failed to open " << filename << '\n';
    exit(-1);
  }

  for ( int i = 0; i < doc.passages.size(); ++i ) {
    if ( i != 0 )
      file << '\n';
    file << doc.passages[i].text;
  }

}


int main( int argc, char * argv[] ) {

  if ( argc != 2 ) {
    cerr << "usage: " << argv[0] << " file\n";
    exit(-1);
  }

  Collection bionlp;
  Connector_libxml libxml;
  libxml.start_read( argv[1], bionlp );

  Document doc;
  while ( libxml.read_next( doc ) ) {
    dump_document( doc );
  }

  return 0;
}
