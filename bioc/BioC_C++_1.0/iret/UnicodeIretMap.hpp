#ifndef UNICODE_IRET_MAP
#define UNICODE_IRET_MAP

#include <map>
#include <string>
#include "Nlphr.h"

using std::string;
using namespace std;
namespace iret {

/** Map unicode characters to ASCII strings.
    Could handle other mappings depending on data files.

    Undefined mappings return NULL.
**/

class UnicodeIretMap {
public:
  UnicodeIretMap(void) { init_map(); }
  ~UnicodeIretMap(void) {  }
  char * operator[](int chr) { return map(chr); }


char * map( int chr ) const;
  /** Map the characters in the array. */
  
protected:
  void init_map(void);

 private:
  CNlphr *Nt;
};

}
#endif // UNICODE_IRET_MAP
