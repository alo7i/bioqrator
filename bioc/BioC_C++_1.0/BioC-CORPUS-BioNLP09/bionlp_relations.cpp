/****  Create BioNLP annotations from .a1 files
 ****/

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

#include "BioC.hpp"
#include "BioC_libxml.hpp"
#include "BioC_util.hpp"

using namespace BioC;

using std::ifstream;
using std::map;
using std::string;


class Note_parse {
public:

  Note_parse( ifstream & in_file, Document & relation_document,
              map<string,int> & in_psg_num ) :
    file(in_file), document(relation_document), psg_num(in_psg_num)
  {}

  void equiv_note( const string & id, int & star_count ) {
    Relation relation;
    std::ostringstream id_sstr;
    id_sstr << '*' << star_count++;
    relation.id = id_sstr.str();

    string type;
    file >> type;
    relation.infons["type"] = type;

    string rest_of_line;
    getline(file,rest_of_line);
    std::istringstream line(rest_of_line);
    
    string ref_id;
    while ( line >> ref_id ) {
      relation.nodes.push_back( Node( ref_id, type) );
    }

    int i_psg = psg_num[ relation.nodes[0].refid ];
    document.passages[i_psg].relations.push_back( relation );
    psg_num[relation.id] = i_psg;
      
  }

  void entity_note( const string & id ) {
    // annotation
    Annotation annotation;
    annotation.id = id;

    string type;
    getline( file, type, ' ' );
    annotation.infons["type"] = type;
    int offset;
    file >> offset;
    int end;
    file >> end;
    int length = end - offset;
    annotation.add_location( offset, length );
    char tab;
    file.get(tab);
    getline( file, annotation.text );

    int i_psg = 0;
    if ( annotation.offset() >= document.passages[1].offset )
      i_psg = 1;
    document.passages[i_psg].annotations.push_back( annotation );
    psg_num[annotation.id] = i_psg;

  }

  void relation_note( const string & id ) {
    Relation relation;
    relation.id = id;

    string rest_of_line;
    getline(file,rest_of_line);
    std::istringstream line(rest_of_line);

    string label_id;
    while ( line >> label_id ) {
      int colon = label_id.find(':');
      string label = label_id.substr(0,colon);
      if ( relation.infons["type"].empty() )
        relation.infons["type"] = label;
      relation.add_node( label_id.substr(colon+1), label );
    }

    int i_psg = psg_num[ relation.refid(0) ];
    document.passages[i_psg].relations.push_back( relation );
    psg_num[relation.id] = i_psg;

  }

  void modify_note( const string & id ) {
    Relation relation;
    relation.id = id;

    string type;
    file >> type;
    relation.infons["type"] = type;
    
    string ref_id;
    file >> ref_id;
    relation.add_node( ref_id, type );

    getline(file, ref_id );     // final newline

    int i_psg = psg_num[ ref_id ];
    document.passages[i_psg].relations.push_back( relation );
    psg_num[relation.id] = i_psg;

  }

  std::ifstream & file;
  Document & document;
  map<string,int> & psg_num;
  
};


void annotate_document( Document & document,
                        Document & relation_document ) {

  string filename = document.id + ".a2";
  std::ifstream file( filename.c_str() );
  if ( ! file ) {
    std::cerr << "failed to open " << filename << '\n';
    exit(-1);
  }

  // how to know which passage in a document receives which relation?
  // placed in the passage with the first reference id

  std::map<string,int> psg_num;
  for ( int i_psg = 0; i_psg < document.passages.size(); ++i_psg ) {
    for ( int i_note = 0;
          i_note < document.passages[i_psg].annotations.size();
          ++i_note ) {
      psg_num[ document.passages[i_psg].annotations[i_note].id ] = i_psg;
    }
  }
  
  relation_document.id = document.id;

  Passage psg_title;
  psg_title.infons["type"] = document.passages[0].infons["type"];
  psg_title.offset = document.passages[0].offset;

  Passage psg_abs;
  psg_abs.infons["type"] = document.passages[1].infons["type"];
  psg_abs.offset = document.passages[1].offset;

  relation_document.passages.push_back(psg_title);
  relation_document.passages.push_back(psg_abs);

  int star_count = 0;
  Note_parse parse( file, relation_document, psg_num );

  string id;
  while ( getline( file, id, '\t' ) ) {

    switch ( id[0] ) {
    case '*': parse.equiv_note( id, star_count );
      break;

    case 'T': parse.entity_note( id );
      break;
      
    case 'E': parse.relation_note( id );
      break;

    case 'M': parse.modify_note( id );
      break;

    default:
      std::cerr << "Unexpected annotation id: " << id << '\n';
      exit(-1);
    }

  }

}


int main( int argc, char * argv[] ) {

  Collection bionlp;

  Connector_libxml bionlp_xml;
  bionlp_xml.start_read( "bionlp_annotate.xml", bionlp );

  Collection relation_collection;
  Node_Converter converter;
  converter.convert( bionlp, relation_collection );

  Connector_libxml relation_xml;
  relation_xml.start_write( "-", relation_collection );
  
  Document document;
  while ( bionlp_xml.read_next(document) ) {
    Document relation_doc;
    annotate_document( document, relation_doc );
    relation_xml.write_next( relation_doc );
  }

  relation_xml.end_write();

  return 0;
}
