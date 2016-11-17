/**** HTML file with PubMed references and abbreviations highlighted
 ****/

#include "BioC.hpp"
#include "BioC_libxml.hpp"
#include <iostream>
#include <map>

using std::cerr;
using std::cout;
using std::string;

using namespace BioC;

void html_head(void) {
  cout << "<html>\n"
       << "<head>\n"
       << "<link rel=\"stylesheet\" type=\"text/css\" href=\"abbr.css\" />\n"
       << "<title>PubMed Docs with Abbr</title>\n"
       << "</head>\n"
       << "<body>\n"
       << "<h1>PubMed Docs with Abbr</h1>\n";
}

void html_text( const string & text ) {

  const string special = "<>&\"\'";

  for ( int i = 0; i < text.size(); ++i ) {
    char c = text[i];
    if ( special.find(c) != string::npos )
      cout << "&#" << int(c) << ';';
    else
      cout << c;
  }
    
}

void html_abbr( const Passage & passage, const Passage & abbr_passage ) {

  // lookup table for span classes

  std::map<string,string> class_map;
  for ( int i_relation = 0; i_relation < abbr_passage.relations.size();
        ++i_relation ) {
    const Relation & relation = abbr_passage.relations[i_relation];
    string class_name = "abbr" + relation.id;
    for ( int i_node = 0; i_node <  relation.nodes.size(); ++i_node )
      class_map[ relation.nodes[i_node].refid ] = class_name;
  }

  // assumes the annotations are in loc order and do not overlap

  const string & text = passage.text;
  const int offset = passage.offset;
  int loc = offset;

  for ( int i = 0; i < abbr_passage.annotations.size(); ++i ) {
    const Annotation & note = abbr_passage.annotations[i];
    if ( loc < note.offset() )
      html_text( text.substr(loc-offset, note.offset()-loc ));
    cout << "<span class=\"" << class_map[note.id] << "\">";
    html_text( text.substr( note.offset() - offset, note.length() ));
    cout << "</span>";
    loc = note.offset() + note.length();
  }

  if ( loc-offset < text.size() )
    html_text( text.substr( loc-offset ));
}
               
void html( Document & document, const Document & abbr_document ) {

  cout << "PMID: " << document.id << "<br>\n";

  if ( document.passages.size() != abbr_document.passages.size() )
    cerr << "doc size diff " << document.passages.size()
         << ' ' << abbr_document.passages.size() << '\n';

  for ( int i = 0; i < document.passages.size(); ++i ) {
    cout << "<div class=\"" << document.passages[i].infons["type"] << "\" >";
    html_abbr( document.passages[i], abbr_document.passages[i] );
    cout << "</div>\n";
  }

}

void html_end(void) {
  cout << "</body>\n"
       << "</html>\n";
}

int main( int argc, char *argv[] ) {
  if ( argc != 3 ) {
    std::cerr << "usage: " << argv[0] << " collection.xml abbr.xml\n";
    return -1;
  }

  char * collection_name = argv[1];
  char * abbr_name = argv[2];
  
  Collection collection;
  Connector_libxml xml;
  xml.start_read(collection_name, collection);

  Collection abbr_collection;
  Connector_libxml abbr_xml;
  abbr_xml.start_read(abbr_name, abbr_collection);
  
  html_head();
  
  Document document;
  while ( xml.read_next(document) ) {
    Document abbr_document;
    abbr_xml.read_next( abbr_document );
    html(document, abbr_document);
  }

  html_end();

  return 0;
}
