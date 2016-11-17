/****  Dump BioNLP annotations to .a1 files
 ****/

#include <iostream>
#include <fstream>

#include "BioC.hpp"
#include "BioC_libxml.hpp"

using std::cerr;

using namespace BioC;

void dump_document( Document & doc ) {

  string filename = doc.id + ".a1";
  std::ofstream file( filename.c_str() );
  if ( ! file ) {
    cerr << "failed to open " << filename << '\n';
    exit(-1);
  }

  for ( int i_psg = 0; i_psg < doc.passages.size(); ++i_psg ) {
    Passage & psg = doc.passages[i_psg];

    for ( int i_note = 0; i_note < psg.annotations.size(); ++i_note ) {
      Annotation & note = psg.annotations[i_note];
      file << note.id << '\t';
      file << note.infons["type"] << ' ';
      file << note.offset() << ' ';
      file << note.offset() + note.length() << '\t';
      file << note.text << '\n';
    }
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
