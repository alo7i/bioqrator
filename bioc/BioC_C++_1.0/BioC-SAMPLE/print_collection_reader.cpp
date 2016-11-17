/**** Print a collection from an XML file
      Using the XML TextReader interface
 ****/

#include <iostream>
#include "BioC.hpp"
#include "BioC_libxml.hpp"

using std::cerr;
using std::string;

using namespace BioC;

int
main(int argc, char *argv[] ) {
  
  if (argc <= 1) {
    std::cerr << "Usage: " << argv[0] << " docname\n";
    return -1;
  }

  char * docname = argv[1];

  Connector_libxml libxml;
  Collection collection;
  libxml.start_read( docname, collection );
  collection.write();

  Document document;
  while ( libxml.read_next(document) ) {
    document.write();
  }

  return 0;
}
