/**** Annotate abbreviation long forms and short forms in each passage.
      A relation shows which long and short forms are related.
****/

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include <Ab3P.h>

#include "BioC.hpp"
#include "BioC_libxml.hpp"
#include "BioC_util.hpp"

using std::cout;
using std::string;
using std::vector;

using BioC::Annotation;
using BioC::Collection;
using BioC::Connector_libxml;
using BioC::Document;
using BioC::Passage;
using BioC::Relation;

class Abbr_Converter : public BioC::Node_Converter {
public:
  using BioC::Node_Converter::convert;

  virtual void convert( const Passage & passage,
                        Passage & abbrPassage ) {
    abbrPassage.infons = passage.infons;
    abbrPassage.offset = passage.offset;

    vector<AbbrOut> abbrs;
    ab3p.get_abbrs( passage.text, abbrs );

    int annote_count = 0;
    int relate_count = 0;

    for ( int i = 0; i < abbrs.size(); ++i ) {

      if ( passage.text.substr( abbrs[i].lf_offset, abbrs[i].lf.size() ) !=
           abbrs[i].lf ) {
        cerr << "lf miss-match: "
             << passage.text.substr( abbrs[i].lf_offset, abbrs[i].lf.size() )
             << ' ' << abbrs[i].lf << '\n';
      }
      if ( passage.text.substr( abbrs[i].sf_offset, abbrs[i].sf.size() ) !=
           abbrs[i].sf ) {
        cerr << "sf miss-match: "
             << passage.text.substr( abbrs[i].sf_offset, abbrs[i].sf.size() )
             << ' ' << abbrs[i].sf << '\n';
      }

      Annotation lf_annote;
      int lf_id = annote_count++;
      ostringstream sstr;
      sstr << lf_id;
      lf_annote.id = sstr.str();
      lf_annote.infons["type"] = "LF";
      lf_annote.add_location( passage.offset + abbrs[i].lf_offset,
                              abbrs[i].lf.size() );
      lf_annote.text = abbrs[i].lf;

      Annotation sf_annote;
      int sf_id = annote_count++;
      sstr.str("");
      sstr << sf_id;
      sf_annote.id = sstr.str();
      sf_annote.infons["type"] = "SF";
      sf_annote.add_location( passage.offset + abbrs[i].sf_offset,
                              abbrs[i].sf.size() );
      sf_annote.text = abbrs[i].sf;

      Relation relation;
      sstr.str("");
      sstr << "R" << relate_count++;
      relation.id = sstr.str();
      relation.nodes.push_back( BioC::Node( lf_annote.id,"lf") );
      relation.nodes.push_back( BioC::Node( sf_annote.id,"sf") );

      abbrPassage.annotations.push_back(lf_annote);
      abbrPassage.annotations.push_back(sf_annote);
      abbrPassage.relations.push_back(relation);
    }

  }

  Ab3P ab3p;
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

  Abbr_Converter converter;
  Collection abbrCollection;
  converter.convert( collection, abbrCollection );
  abbrCollection.key = "abbreviation.key";

  Connector_libxml xml_writer;
  xml_writer.start_write( "-", abbrCollection );
  
  Document document;
  while ( xml.read_next(document) ) {
    Document abbrDocument;
    converter.convert( document, abbrDocument );
    xml_writer.write_next( abbrDocument );
  }
  
  xml_writer.end_write();
  
  return 0;
}
