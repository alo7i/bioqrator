/**** Print a collection from an XML file
 ****/

#include <iostream>
#include "BioC.hpp"
#include "BioC_libxml.hpp"

using namespace BioC;

int
main(int argc, char *argv[] ) {
  
  if (argc <= 1) {
    std::cerr << "Usage: " << argv[0] << " docname\n";
    return -1;
  }
  
  char * docname = argv[1];
  Collection collection;
  Connector_libxml xml;
  xml.read(docname, collection);
  
  collection.write();
  
  return 0;
}
