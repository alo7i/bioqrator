/****  Create BioNLP corpus from .txt files
 ****/

#include <iostream>
#include <fstream>

#include "BioC.hpp"
#include "BioC_libxml.hpp"

using namespace BioC;

void read_document( Document & doc, const string & filename ) {

  int dot = filename.find('.');
  doc.id = filename.substr(0,dot);

  std::ifstream file( filename.c_str() );
  if ( ! file ) {
    std::cerr << "failed to open " << filename << '\n';
    exit(-1);
  }

  string str_title;
  getline(file,str_title);
  Passage psg_title;
  psg_title.infons["type"] = "title";
  psg_title.offset = 0;
  psg_title.text = str_title;
  doc.passages.push_back(psg_title);

  string str_abs;
  getline(file,str_abs);
  Passage psg_abs;
  psg_abs.infons["type"] = "abstract";
  psg_abs.offset = str_title.size() + 1;
  psg_abs.text = str_abs;
  doc.passages.push_back(psg_abs);

}


int main( int argc, char * argv[] ) {

  Collection bionlp;
  bionlp.source = "bionlp09_shared_task_sample_data_rev3";
  bionlp.date = "20120921";
  bionlp.key = "bionlp.key";

  Connector_libxml libxml;
  libxml.start_write( "-", bionlp );

  for ( int i = 1; i < argc; ++i ) {
    Document doc;
    read_document( doc, argv[i] );
    libxml.write_next( doc );
  }

  libxml.end_write();

  return 0;
}

