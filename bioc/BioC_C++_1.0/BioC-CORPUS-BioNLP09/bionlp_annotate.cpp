/****  Create BioNLP annotations from .a1 files
 ****/

#include <iostream>
#include <fstream>

#include "BioC.hpp"
#include "BioC_libxml.hpp"
#include "BioC_util.hpp"

using namespace BioC;

void annotate_document( Document & document,
                        Document & annotated_document ) {

  string filename = document.id + ".a1";
  std::ifstream file( filename.c_str() );
  if ( ! file ) {
    std::cerr << "failed to open " << filename << '\n';
    exit(-1);
  }

  annotated_document.id = document.id;

  Passage psg_title;
  psg_title.infons["type"] = document.passages[0].infons["type"];
  psg_title.offset = document.passages[0].offset;

  Passage psg_abs;
  psg_abs.infons["type"] = document.passages[1].infons["type"];
  psg_abs.offset = document.passages[1].offset;

  string id;
  while ( getline( file, id, '\t' ) ) {

    Annotation annotation;
    annotation.id = id;
    
    string type;
    getline( file, type, ' ' );
    annotation.infons["type"] = type;
    int offset;
    file >> offset;
    int end;
    file >> end;
    annotation.add_location( offset, end - offset );
    char tab;
    file.get(tab);
    getline( file, annotation.text );

    if ( annotation.offset() >= psg_abs.offset )
      psg_abs.annotations.push_back( annotation );
    else
      psg_title.annotations.push_back( annotation );

  }

  annotated_document.passages.push_back(psg_title);
  annotated_document.passages.push_back(psg_abs);

}


int main( int argc, char * argv[] ) {

  Collection bionlp;

  Connector_libxml bionlp_xml;
  bionlp_xml.start_read( "bionlp_corpus.xml", bionlp );

  Collection annotation_collection;
  Node_Converter converter;
  converter.convert( bionlp, annotation_collection );

  Connector_libxml annotate_xml;
  annotate_xml.start_write( "-", annotation_collection );

  Document document;
  while ( bionlp_xml.read_next(document) ) {
    Document annotated_doc;
    annotate_document( document, annotated_doc );
    annotate_xml.write_next( annotated_doc );
  }

  annotate_xml.end_write();

  return 0;
}
