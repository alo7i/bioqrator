/**** Print a collection from an XML file
 ****/

#include <iostream>
#include <string>
#include <vector>

#include "BioC.hpp"
#include "BioC_libxml.hpp"
#include "BioC_util.hpp"

#include "UnicodeIretMap.hpp"

using iret::UnicodeIretMap;

using std::cout;
using std::string;
using std::vector;

using namespace BioC;

class ASCII_Converter : public Node_Converter {
public:
  ASCII_Converter( xmlCharEncodingHandlerPtr in_handler,
                   UnicodeIretMap & in_map ) :
    handler(in_handler), map(in_map)
  {}
  using Node_Converter::convert;

  virtual void convert( const Passage & passage, Passage & asciiPassage) {

    asciiPassage.infons = passage.infons;
    asciiPassage.offset = passage.offset;
    
    int size = passage.text.size();
    
    size_t in_len = size;
    char * buffer = new char[ size+1 ];
    passage.text.copy( buffer, size );
    buffer[size] = 0;
    char ** in_buf = & buffer;
    
    int * unicode = new int[ 2*size+1 ];
    size_t initial_out_len = 2*size * sizeof(int);
    size_t out_len = initial_out_len;
    
    char * out_buf =  (char*)unicode;
    char ** out_buf_adr = & out_buf;
    
    size_t ret = iconv( handler->iconv_out,
                        in_buf, &in_len,
                        out_buf_adr , &out_len );
    
    //  cout << "ret " << ret << '\n';
    
    //  cout << buffer << '\n';
    //  cout << "in_len " << in_len << '\n';
    //  cout << out_buf << '\n';
    //  cout << "out_len " << out_len << '\n';
    
    /*
      for ( int i = 0 ; i < size; ++i ) {
      cout << unicode[i] << ' ';
      }
    */
    
    int result_size = (initial_out_len - out_len ) / sizeof(int);
    
    for ( int i = 0 ; i < result_size; ++i ) {
      const char * ascii_text = map[ unicode[i] ];
      if ( ascii_text )
        asciiPassage.text += ascii_text;
      else
        // map undefined characters to space
        asciiPassage.text += ' ';
    }
    
  }
  
  xmlCharEncodingHandlerPtr handler;
  UnicodeIretMap & map;
};  


int
main(int argc, char **argv) {

  if (argc <= 1) {
    printf("Usage: %s docname\n", argv[0]);
		return(0);
  }

  xmlCharEncodingHandlerPtr handler =
    xmlFindCharEncodingHandler("UCS-4LE");
  if ( ! handler ) {
    std::cerr << "encoding not found\n";
    return 1;
  }

  if ( false ) {
    cout << handler << '\n';
    cout << handler->input << '\n';
    cout << handler->output << '\n';
    cout << handler->iconv_in << '\n';
    cout << handler->iconv_out << '\n';
  }

  string docname = argv[1];
  Collection collection;

  Connector_libxml xml;
  xml.read(docname, collection);

  //        collection.write();

  Collection asciiCollection;

  UnicodeIretMap map;

  ASCII_Converter converter( handler, map );
  converter.convert( collection, asciiCollection );

  //        asciiCollection.write();

  xml.write( "-", asciiCollection );
  
  return 0;
}
