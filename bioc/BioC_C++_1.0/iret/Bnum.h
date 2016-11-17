#ifndef BNUM_H
#define BNUM_H
#include <fstream>
#include <iostream>
#include <runn.h>
using namespace std;
namespace iret {

class Bnum {
   public:
      Bnum(long nm); 
      Bnum(Bnum &Anm); 
      Bnum(long nm,long *mz); //nm is number of values in mz
         //values in mz are converted to cumulative quantities and put
         //in array mm. Used to randomly sample a term based on its count
         //which was entered in mz.
     ~Bnum();
      long index(long qn); //Finds by binary search that i, 0<=i<num,
         //with qn-mm[i]>=0 and minimal.
      long num;
      long tz; //Used for the total (Only used for sampling)
      long *mm;
};

class DBnum {
   public:
      DBnum(long nm);
      DBnum(DBnum &Anm);
      DBnum(long nm,double *mz); //nm is number of values in mz
         //values in mz are converted to cumulative quantities and put
         //in array mm and normalized to a total of 1. 
         //Used to randomly sample a term based on its count
         //which was entered in mz.
     ~DBnum();
      long index(double qn); //Finds by binary search that i, 0<=i<num,
         //with qn-mm[i]>=0 and minimal. Here qn is expected to be a
         //number between 0<=qn<1.
      long num;
      double tz; //Used for the total (Only used for sampling)
      double *mm;
};

}
#endif
