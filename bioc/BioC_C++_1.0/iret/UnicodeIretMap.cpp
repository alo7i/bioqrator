#include "UnicodeIretMap.hpp"
#include <fstream>
#include <Nlphr.h>
#include "runn.h"

using namespace iret;

static const string RCSid = "$Id: UnicodeIretMap.cpp,v 1.3 2010/08/23 21:13:45 comeau Exp $";

void
UnicodeIretMap::init_map(void) {

  Nt = new CNlphr("uni");
  Nt->setup_access();
  Nt->extend_access_pmid();

}

char *
UnicodeIretMap::map( int chr ) const {
  int i;

  i = Nt->read_pmid(chr);
  if (i>0) {
    return Nt->xstr[1];
  }
  else
    return NULL;
}


