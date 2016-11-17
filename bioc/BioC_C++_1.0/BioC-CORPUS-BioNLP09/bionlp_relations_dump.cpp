/****  Dump BioNLP annotations to .a1 files
 ****/

#include <iostream>
#include <fstream>

#include "BioC.hpp"
#include "BioC_libxml.hpp"

using std::cerr;
using std::ofstream;

using namespace BioC;

void dump_equiv( ofstream & file, Relation & relation ) {

  file << relation.id[0] << '\t';
  file << relation.infons["type"];
  for ( int i = 0; i < relation.num_nodes(); ++i )
    file << ' ' << relation.refid(i);
  file << '\n';
}


void dump_event( ofstream & file, Relation & relation ) {

  file << relation.id << '\t';
  for ( int i = 0; i < relation.num_nodes(); ++i ) {
    if ( i > 0 )
      file << ' ';
    file << relation.role(i) << ':' << relation.refid(i);
  }
  file << '\n';
}


void dump_modify( ofstream & file, Relation & relation ) {
  file << relation.id << '\t';
  file << relation.infons["type"] << ' ' << relation.refid(0) << '\n';
}


void dump_document( Document & doc ) {

  string filename = doc.id + ".a2";
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

    for ( int i_relate = 0; i_relate < psg.relations.size(); ++i_relate ) {
      Relation & relation = psg.relations[i_relate];
      switch ( relation.id[0] ) {
      case '*': dump_equiv( file, relation );
        break;
      case 'E': dump_event( file, relation );
        break;
      case 'M': dump_modify( file, relation );
        break;
      default:
        std::cerr << "Unexpected relation id: " << relation.id << '\n';
        exit(-1);
      }
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
