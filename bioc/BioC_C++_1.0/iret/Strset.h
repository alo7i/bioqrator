#ifndef STRSET_H
#define STRSET_H

#include <iostream>
#include <fstream>
#include <runn.h>
#include <Btree.h>
#include <FBase.h>

#define MAP_F 1 //Stands for the Strset files .n, .a, and .s
#define MAP_C 2 //Stands for the LexCnt files .c and .tot

using namespace std;
namespace iret {

class Strset : public FBase{
public:
   Strset(const char *nm);
   Strset(int n,const char *nm); //n gets appended to type if >-1
   ~Strset();
   void gopen_write(void); 
   void gopen_write(long m); //m is file number
   void add_str(const char *pch); //Call to add a string to the set
   void add_str_rend(const char *pch); //Call to add a string to the set
     //appends " ()" on end of each string as an end of string marker
   void add_str_lend(const char *pch); //Call to add a string to the set
     //appends "() " on beginning of each string as an end of string marker
   void add_str_bend(const char *pch); //Call to add a string to the set
     //appends "() " on beginning and " ()" on end of each string as an 
     //end of string marker
   void gclose_write(void);
   void gclose_write(long m);
     //Files are .n, .a, and .s

   void gopen_map(void);
   void gopen_map(long m);
   char *show_str(long n); //Returns pointer at the nth string
   void gclose_map(void);
   void gclose_map(long m);
  
   //Data
   long num; //number of strings in the set (file .n)
   long *addr; //array of offsets to strings (file .a)
   char *str;  //pointer at string map (file .s)
   ofstream *pfa; //Used for file creation
   ofstream *pfs; //Used for file creation
};

class Lexos : public Strset {
public:
   Lexos(const char *nm);
   Lexos(int n,const char *nm); //n gets appended to type if >-1
   ~Lexos();
   void create_Lexos(List &Ls);
   void create_Lexos(List &Ls,long m);
   long find(const char *str); //If finds string
      //returns its number+1 else returns 0.
      //Does binary search.
   int stc_my(const char *,const char *);
      //Function used to compare two strings.
   long lfind(const char *str); //returns number
      //+1 for longest string that matches an
      //initial segment of the string str.
      //Otherwise returns 0.
   long tfind(const char *str); //returns number of
      //string matches found which begin at beginning
      //of str. Held in ht. If ht[i]=n+1>0 then a match
      //of length i occurs at string n [0,num-1]. Calls ifind.
   long ifind(long,long,const char*); //Called by lfind
      //and tfind
   int stc_ly(const char *,long); //Called in ifind
      //Functions for matches to initial segment
      //list members
   long find_low(const char *str); //Finds first string in lexos
      //that str matches initial segment, returns index+1, or 0
   int stc_low(const char *,const char *);
   long find_high(const char *str); //Finds last string in lexos
      //that str matches initial segment, returns index+1, or 0
   int stc_high(const char *,const char *);

   //Data
   int slen; //Length of string str in tfind call.
   int a; //Variable used for comparison of strings.
   int b; //Variable used for comparison of strings.
   char *tx; //Holds string copy for lfind and tfind.
   long *ht; //Holds the matchs at various depths.
      //At i puts the index+1 of a match of length i.
      //If no match of length i, holds 0.
   long *sn; //Holds the beginning of pair
   long *sm; //Holds the end of pair
};

class LexCnt : public Lexos {
public:
   LexCnt(const char *);
   ~LexCnt(void);
   void create_LexCnt(Count &Ct); //The tree Ct needs to
      //be built with the add_count2 function.
   void create_LexCnt(Count &Ct,long m); //m is file #
   void gopen_LexCnt(void); //Calls gopen_map
   void gopen_LexCnt(long m); //Calls gopen_map
   void gclose_LexCnt(void); //Calls gclose_map
   void gclose_LexCnt(long m); //Calls gclose_map
   long count_Exact(const char *str); //Returns count of
      //perfect match to str if there is one, else 0.
   long count_Super(const char *str); //Returns sum of all
      //for all strings with str as an initial segment.

   //Data
   long tot; //Total of all counts in set
   long *cnt; //Holds the counts
};

class LexPrb : public LexCnt {
public:
   LexPrb(int mn,const char *); //mn is minimum length segment to add.
   ~LexPrb(void);
   void set_max(int mx); //Sets max.
   void set_aug(int ax); //Sets aug.
   void set_fst(char fx,long buf_siz=10000); //Sets fst & buf size
   void gopen_add(void); //Sets up Count tree for additions
   void add_segs(const char *str); //Adds all initial segments of str to Count
   void add_segs_aug(const char *str); //Adds all initial segments of str to Count
      //augments the first segment with count aug.
   void add_segs_fst(const char *str); //Adds all initial segments of str to Count
      //Puts fst on beginning of all strings.
   void gclose_add(void); //Creates structures, then deletes Count
   void gclose_add(long m); //Creates structures, then deletes Count
      //m is file number

   //Suffix tree approach
   void create_Substructures(Count &Ct); //Makes the basic string file and a file
      //of counts (.c).
   void create_Substructures(char fx,Count &Ct); //As above but adds fx as first
      //character in each string.
   void create_Final(void); //Makes the final address and count structures
   void create_Final(long aag); //Makes the final address and count
      //sturctures and augments the beginning of strings with aug.

   //Probability calculating functions
   void gopen_LexPrb(void); //Calls gopen_LexCnt
   void gopen_LexPrb(long m); //Calls gopen_LexCnt
   double prob_ext(const char *str); //Returns the prob of str
      //Computes using as long a piece as possible at each step.
   double prob_ext_fst(const char *str); //Returns the prob of str
      //Computes using as long a piece as possible at each step.
      //Uses the special first character from set_fst
   double prob_fxd(const char *str); //Returns the prob of str
      //Computes using pieces as long as possible but limited by max.
   double prob_fxd_fst(const char *str); //Returns the prob of str
      //Computes using pieces as long as possible but limited by max.
      //Uses the special first character from set_fst
   void gclose_LexPrb(void); //Calls gclose_LexCnt
   void gclose_LexPrb(long m); //Calls gclose_LexCnt

   //Data
   int min; //Lower limit on length of segments to add.
            //Used in creation.
   int max; //Maximum lookback in computing prob_fxd.
            //Uses back off to obtain probs as needed.
   int aug; //Number to augment counts for first segment of a string
            //Used with add_segs_aug function
   Count *pCt; //holds data until gclose_add called, then NULL

   //Used to handle extra character at beginning of string
private:
   char *buf;
   char *bfc;
};

//Double precision counting of weights

class LexDCnt : public Lexos {
public:
   LexDCnt(const char *);
   ~LexDCnt(void);
   void create_LexDCnt(DCount &Ct); //The DCount tree Ct needs to
      //be built with the add_count2 function.
   void create_LexDCnt(DCount &Ct,long m); //The DCount tree Ct needs to
   void gopen_LexDCnt(void); //Calls gopen_map
   void gopen_LexDCnt(long m); //Calls gopen_map
   void gclose_LexDCnt(void); //Calls gclose_map
   void gclose_LexDCnt(long m); //Calls gclose_map
   double count_Exact(const char *str); //Returns count of
      //perfect match to str if there is one, else 0.
   double count_Super(const char *str); //Returns sum of all
      //for all strings with str as an initial segment.

   //Data
   double tot; //Total of all counts in set
   double *cnt; //Holds the counts
};

class LexPrb2 : public LexDCnt {
public:
   LexPrb2(int mn,const char *); //mn is minimum length segment to add.
   ~LexPrb2(void);
   void set_max(int mx); //Sets max.
   void set_aug(double ax); //Sets aug.
   void set_fst(char fx,long buf_siz=10000); //Sets fst & buf size
   void gopen_add(void); //Sets up Count tree for additions
   void add_segs(const char *str,double wt); //Adds all initial 
      //segments of str to DCount with a weight of wt
   void add_segs_aug(const char *str,double wt); //Adds all initial 
      //segments of str to DCount with a weight of wt
      //augments the first segment with count aug.
   void add_segs_fst(const char *str,double wt); //Adds all initial 
      //segments of str to DCount with a weight of wt
      //Puts fst on beginning of all strings.
   void gclose_add(void); //Creates structures, then deletes DCount
   void gclose_add(long m); //Creates structures, then deletes DCount

   //Suffix tree approach
   void create_Substructures(DCount &Ct); //Makes the basic string file and a file
      //of counts (.c).
   void create_Substructures(char fx,DCount &Ct); //As above but adds fx as first
      //character in each string.
   void create_Final(void); //Makes the final address and count structures
   void create_Final(double ax); //Makes the final address and count
      //sturctures and augments the beginning of strings with aug.

   //Probability calculating functions
   void gopen_LexPrb2(void); //Calls gopen_LexDCnt
   void gopen_LexPrb2(long m); //Calls gopen_LexDCnt
   double prob_ext(const char *str); //Returns the prob of str
      //Computes using as long a piece as possible at each step.
   double prob_ext_fst(const char *str); //Returns the prob of str
      //Computes using as long a piece as possible at each step.
      //Uses the special first character from set_fst
   double prob_fxd(const char *str); //Returns the prob of str
      //Computes using pieces as long as possible but limited by max.
   double prob_fxd_fst(const char *str); //Returns the prob of str
      //Computes using pieces as long as possible but limited by max.
      //Uses the special first character from set_fst
   void gclose_LexPrb2(void); //Calls gclose_LexDCnt
   void gclose_LexPrb2(long m); //Calls gclose_LexDCnt

   //Data
   int min; //Lower limit on length of segments to add.
            //Used in creation.
   int max; //Maximum lookback in computing prob_fxd.
            //Uses back off to obtain probs as needed.
   double aug; //Number to augment counts for first segment of a string
            //Used with add_segs_aug function
   DCount *pCt; //holds data until gclose_add called, then NULL

   //Used to handle extra character at beginning of string
private:
   char *buf;
   char *bfc;
};

}
#endif

