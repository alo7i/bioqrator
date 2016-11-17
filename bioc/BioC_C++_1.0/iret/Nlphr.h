#ifndef NLPHR_H
#define NLPHR_H
#include <fstream>
#include <iostream>
#include <runn.h>
#include <Strset.h>
#include <Bnum.h>
#include <DataObj.h>

#define MAP_D 4 //Stands for the files .doc & .ndoc

using namespace std;
namespace iret {

class Nlphr : public Strset{
   public:
      Nlphr(const char *nm); 
      Nlphr(const char *nm,long wrdspc); 
     ~Nlphr(void);
      void gopen_Nlphr_write(void);
      void gopen_Nlphr_write(long m);
      void add_str(const char *str,const char typ); 
         //typ stands for a code character marking
         //nature of string
      void add_str_rend(const char *str,const char typ); 
      void doc_end_mark(void);
      void gclose_Nlphr_write(void);
      void gclose_Nlphr_write(long m);

      //Use as mapped data
      void gopen_Nlphr_map(void);
      void gopen_Nlphr_map(long m);
      void read_map(long n);
      void gclose_Nlphr_map(void);
      void gclose_Nlphr_map(long m);
      //Use as disk access
      void gopen_Nlphr_read(void);
      void gopen_Nlphr_read(long m);
      void read_disc(long n);
      void clear_read(void);
      void gclose_Nlphr_read(void);
      void gclose_Nlphr_read(long m);
      
      long ndoc; //Number of docs in data base
      //Data for map/read access
      long *doc; //cumulative count of strings to
         //beginning of this document
      char *typ; //type information for each string
         //File pointer for read access
      ifstream *pfst; //To read the .s file
         //space for document
      long wordspace; //Number of phrases allowed
         //in a document, default 10,000
      long nstr; //Number of phrases in this doc
      char **xstr; //Pointer at set of phrases for
         //current document.
      char *xtyp; //Pointer at type information.
      ofstream *pfd; //To use in writing ".doc" file.
      ofstream *pft; //To use in writing ".typ" file.
      char *buf; //Work space
};

class CNlphr : public FBase {
   public:
      CNlphr(const char *nm);
      CNlphr(const char *nm,long wrdspc);
      ~CNlphr(void);
      //Access by documet number
      void setup_access(void);
      void read(long n);
      //Access random
      void setup_access_random(void);
      void readr(long n);
      void show(long n); //Writes the doc to stdout
      Index *NumField(char c); //Returns pointer at Index object
         //containing list of all doc nums with at least one string of 
         //type c
      Index *PidField(char c); //Returns pointer at Index object
         //containing list of all doc pids with at least one string of 
         //type c
      Index *PidAll(void); //Returns pointer at Index object
         //containing list of all pmids in the set
      //Access by PMID
      void create_pmid_index(void); //Creates the file to access by pmid
      void extend_access_pmid(void); //Maps file to allow access by pmid
      long read_pmid(long pd); //pd is a pmid and reads the doc
      //Access by sentence number
      void setup_s_access(void);
      void read_s(long n);
      long doc_n(long n); //returns the doc num for sent n.
      
      //Single document data
      long wordspace; //Number of phrases allowed in a document
      long nstr; //Number of phrases in this doc
      char **xstr; //Pointer at set of phrases for
         //current document.
      char *xtyp; //Pointer at type information.
      //Single sentence data
      char *ystr; //Pointer at current sentence
      char ytyp;  //Type character
      //String set management
      long nst; //Number of sets
      long cur; //Current set that is open for reading
      Nlphr **pNl; //Array of Nlphr objects
      long *dx; //Array of cumulative sums of docs
          //used by doc_n function
      Bnum *pBn; //Access by number to piece
      long tdoc; //Total number of docs in system
          //used to memory map the array of pmids
      long *pmid;
};

}
#endif

