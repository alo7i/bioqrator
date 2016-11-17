#include "Bnum.h"
namespace iret {

Bnum::Bnum(long nm){
   num=nm;
   mm=new long[num];
} 

Bnum::Bnum(Bnum &Anm){
   long i;
   num=Anm.num;
   mm=new long[num];
   for(i=0;i<num;i++)mm[i]=Anm.mm[i];
}

Bnum::Bnum(long nm,long *mz){
   long i;
   num=nm;
   mm=new long[num];
   tz=0;
   for(i=0;i<nm;i++){
      tz+=mz[i];
      mm[i]=tz;
   }
}
   
Bnum::~Bnum(){
    if(mm!=NULL)delete [] mm;
}  
 
long Bnum::index(long qn){
   long i,j,k,x,y;

   if(qn<mm[0]){cout << qn << " Below range!" << endl;return(0);}
   if(qn>=mm[num-1]){cout << qn << " Above range!" << endl;return(num-1);}
   x=0;
   y=num-1;
   while(y-x>1){
      i=(y+x)/2;
      if(qn>=mm[i])x=i;
      else y=i;
   }
   return(x);
}

//Double Precision Version
   
DBnum::DBnum(long nm){
   num=nm;
   mm=new double[num];
}

DBnum::DBnum(DBnum &Anm){
   long i;
   num=Anm.num;
   mm=new double[num];
   for(i=0;i<num;i++)mm[i]=Anm.mm[i];
}

DBnum::DBnum(long nm,double *mz){
   long i;
   num=nm;
   mm=new double[num];
   tz=0;
   for(i=0;i<nm;i++){
      tz+=mz[i];
      mm[i]=tz;
   }
   for(i=0;i<nm;i++){
      mm[i]=mm[i]/tz;
   }
}

DBnum::~DBnum(){
    if(mm!=NULL)delete [] mm;
}

long DBnum::index(double qn){
   long i,j,k,x,y;

   if(qn<mm[0]){cout << qn << " Below range!" << endl;return(0);}
   if(qn>=mm[num-1]){cout << qn << " Above range!" << endl;return(num-1);}
   x=0;
   y=num-1;
   while(y-x>1){
      i=(y+x)/2;
      if(qn>=mm[i])x=i;
      else y=i;
   }
   return(x);
}

} 
