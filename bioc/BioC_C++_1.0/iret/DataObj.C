#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <runn.h>
#include "DataObj.h"

using namespace std;
namespace iret {

//Index set object

Index::Index(void){
   idx=NULL;
}

Index::Index(long num){
   ix=num;
   idx=new long[ix];
}

Index::Index(long begin,long end_plus_one){
   long i;
   ix=end_plus_one-begin;
   idx=new long[ix];
   for(i=begin;i<end_plus_one;i++)*(idx+i-begin)=i;
}

Index::Index(long num,long *imd,long oflag){
   long i;

   ix=num;
   idx=new long[ix];
   for(i=0;i<num;i++)*(idx+i)=*(imd+i);
   if(oflag)sSort();
   if(oflag>1)unique();
}

Index::Index(Index *ind){
   long i;

   ix=ind->ix;
   idx=new long[ix];
   for(i=0;i<ix;i++)idx[i]=ind->idx[i];
}

Index::Index(Indes *ind){
   unsigned int i;

   ix=(long)ind->ix;
   idx=new long[ix];
   for(i=0;i<ix;i++)idx[i]=(long)ind->idx[i];
}

Index::~Index(void){
   if(idx!=NULL)delete [] idx;
}

void Index::sSort(void){
  long k, j, ir, i;
  long rra;

  if(ix<2)return;

  k=(ix>>1);
  ir=ix-1;
  for(;;) {
    if(k>0) {
      rra=idx[--k];
    }
    else {
      rra=idx[ir];
      idx[ir] = idx[0];
      if(--ir ==0) {
        idx[0]=rra;
        return;
      }
    }
    i=k;
    j=((k+1)<<1)-1;
    while(j<=ir) {
      if(j<ir && (idx[j]<idx[j+1])) ++j;
      if(rra<idx[j]) {
        idx[i]=idx[j];
        j +=(i=j)+1;
      }
      else j=ir+1;
    }
    idx[i]=rra;
  }
}

long Index::unique(void){
   long i,j,k;

   if(ix<2)return(ix);
   k=1;
   j=*idx;
   for(i=1;i<ix;i++){
      if(j<*(idx+i))*(idx+(k++))=j=*(idx+i);
   }
   if(k<ix){
      long *jdx=new long[k];
      for(i=0;i<k;i++)*(jdx+i)=*(idx+i);
      delete [] idx;
      idx=jdx;
      ix=k;
   }
   return(k);
}

Index *Index::cbool_And(Index *jnd){
   long i,j,k,su,w,p,m,bu;
   long *pdx,*bdx,*sdx,bx,sx;

   if(jnd==NULL){
      return(NULL);
   }
   if(ix>jnd->ix){
      bdx=idx;
      bx=ix;
      sdx=jnd->idx;
      sx=jnd->ix;
   }
   else {
      bdx=jnd->idx;
      bx=jnd->ix;
      sdx=idx;
      sx=ix;
   }
   pdx=new long[sx];
   i=j=k=0;
   while((i<sx)&&(j<bx)){
      bu=*(bdx+j);
      while((i<sx)&&(*(sdx+i)<bu))i++;
      if(i<sx){
         su=*(sdx+i);
         if(su==bu){
            *(pdx+k)=bu;
            k++;j++;i++;
         }
         else {
            if(bx-j>sx-i)w=(bx-j)/(sx-i);
            else w=1;
            while((j+w<bx)&&(su>*(bdx+j+w)))j+=w;
            if(j+w>=bx){
               w=bx-j-1;
               if(su>*(bdx+j+w))i=sx;
            }
            if(i<sx){
               if(su==*(bdx+j+w)){
                  *(pdx+k)=su;
                  k++;i++;j+=w+1;
               }
               else {
                  p=j+w;
                  while(p-j>1){
                     m=(j+p)/2;
                     if(su<*(bdx+m))p=m;
                     else j=m;
                  }
                  if(su==*(bdx+j)){
                     *(pdx+k)=su;
                     k++;
                  }
                  i++;j++;
               }
            }
         }
      }
   }
   if(k==0){
      delete [] pdx;
      return(NULL);
   }
   else {
      Index *pnd=new Index(k);
      for(i=0;i<k;i++)*(pnd->idx+i)=*(pdx+i);
      delete [] pdx;
      return(pnd);
   }
}

Index *Index::cbool_Or(Index *jnd){
   long i,j,k,*pdx,bx,ii,jj;
   long *iix,*jjx,iu,ju;

   if(jnd==NULL){
      Index *pnd=new Index(ix,idx,0);
      return(pnd);
   }

   ii=ix;
   iix=idx;
   jj=jnd->ix;
   jjx=jnd->idx;
   bx=ii+jj;
   pdx=new long[bx];
   i=j=k=0;
   while((i<ii)&&(j<jj)){
      ju=*(jjx+j);
      while((i<ii)&&((iu=*(iix+i))<ju)){
         *(pdx+k)=iu;
         k++;i++;
      }
      if(i<ii){
         if(iu==ju){
            *(pdx+k)=iu;
            k++;i++;j++;
         }
         else {
            while((j<jj)&&(iu>(ju=*(jjx+j)))){
               *(pdx+k)=ju;
               k++;j++;
            }
            if(j<jj){
               if(iu==ju){
                  *(pdx+k)=iu;
                  k++;i++;j++;
               }
            }
         }
      }
   }
   while(i<ii){
      *(pdx+k)=*(iix+i);
      k++;i++;
   }
   while(j<jj){
      *(pdx+k)=*(jjx+j);
      k++;j++;
   }

   if(k==0){
      delete [] pdx;
      return(NULL);
   }
   else {
      Index *pnd=new Index(k);
      for(i=0;i<k;i++)*(pnd->idx+i)=*(pdx+i);
      delete [] pdx;
      return(pnd);
   }
}

Index *Index::cbool_Butnot(Index *jnd){
   long i,j,k,su,w,p,m,bu,flab;
   long *pdx,*bdx,*sdx,bx,sx;

   if(jnd==NULL){
      Index *pnd=new Index(ix,idx,0);
      return(pnd);
   }

   if(ix>jnd->ix){
      bdx=idx;
      bx=ix;
      sdx=jnd->idx;
      sx=jnd->ix;
      flab=1;
   }
   else {
      bdx=jnd->idx;
      bx=jnd->ix;
      sdx=idx;
      sx=ix;
      flab=0;
   }
   pdx=new long[ix];
   for(i=0;i<ix;i++)*(pdx+i)=1; //Initialize as marker.
   if(flab){ //Case ind is big.
   i=j=k=0;
   while((i<sx)&&(j<bx)){
      bu=*(bdx+j);
      while((i<sx)&&(*(sdx+i)<bu))i++;
      if(i<sx){
         su=*(sdx+i);
         if(su==bu){
            *(pdx+j)=0;
            k++;j++;i++;
         }
         else {
            if(bx-j>sx-i)w=(bx-j)/(sx-i);
            else w=1;
            while((j+w<bx)&&(su>*(bdx+j+w)))j+=w;
            if(j+w>=bx){
               w=bx-j-1;
               if(su>*(bdx+j+w))i=sx;
            }
            if(i<sx){
               if(su==*(bdx+j+w)){
                  *(pdx+j+w)=0;
                  k++;i++;j+=w+1;
               }
               else {
                  p=j+w;
                  while(p-j>1){
                     m=(j+p)/2;
                     if(su<*(bdx+m))p=m;
                     else j=m;
                  }
                  if(su==*(bdx+j)){
                     *(pdx+j)=0;
                     k++;
                  }
                  i++;j++;
               }
            }
         }
      }
   }
   } //End of case ind is big.
   else { //Case ind is small.
   i=j=k=0;
   while((i<sx)&&(j<bx)){
      bu=*(bdx+j);
      while((i<sx)&&(*(sdx+i)<bu))i++;
      if(i<sx){
         su=*(sdx+i);
         if(su==bu){
            *(pdx+i)=0;
            k++;j++;i++;
         }
         else {
            if(bx-j>sx-i)w=(bx-j)/(sx-i);
            else w=1;
            while((j+w<bx)&&(su>*(bdx+j+w)))j+=w;
            if(j+w>=bx){
               w=bx-j-1;
               if(su>*(bdx+j+w))i=sx;
            }
            if(i<sx){
               if(su==*(bdx+j+w)){
                  *(pdx+i)=0;
                  k++;i++;j+=w+1;
               }
               else {
                  p=j+w;
                  while(p-j>1){
                     m=(j+p)/2;
                     if(su<*(bdx+m))p=m;
                     else j=m;
                  }
                  if(su==*(bdx+j)){
                     *(pdx+i)=0;
                     k++;
                  }
                  i++;j++;
               }
            }
         }
      }
   }
   } //End of case ind is small.

   j=ix-k;
   if(k==0){
      delete [] pdx;
      Index *pnd=new Index(ix,idx,0);
      return(pnd);
   }
   else if(j==0){
      delete [] pdx;
      return(NULL);
   }
   else {
      Index *pnd=new Index(j);
      j=0;
      for(i=0;i<ix;i++){
         if(*(pdx+i)){
            *(pnd->idx+j)=*(idx+i);
            j++;
         }
      }
      delete [] pdx;
      return(pnd);
   }
}

long Index::Subvalue(long j){
   long x,y,i,k;
   
   if(j<=(k=idx[0])){
      if(j!=k)return(0);
      else return(1);
   }
   if(j>=(k=idx[ix-1])){
      if(j!=k)return(0);
      else return(ix);
   }
   x=0;
   y=ix-1;
   if(y==1)return(0);

   while(y-x>1){
      i=(y+x)/2;
      if(j>(k=idx[i]))x=i;
      else if(j<k)y=i;
      else return(i+1);
   }
   return(0);
}

Index *Index::Subvalue(Index *jnd){
   if(jnd==NULL)return(NULL);
   else if(jnd->ix==0)return(NULL);

   Index *knd=new Index(jnd->ix);

   long i=0,j=0,k;
   while(j<jnd->ix){
      k=jnd->idx[j];
      while(idx[i]<k)i++;
      knd->idx[(j++)]=i;
   }
 
   return(knd);
}

Index *Index::Subinterval(long n,long m){
   if(m<=n)return(NULL);
   if(n<0)return(NULL);
   if(ix<m)return(NULL);
   Index *pind=new Index(m-n);
   long i,*ptr;
   ptr=pind->idx;
   for(i=n;i<m;i++)*(ptr++)=*(idx+i);
   return(pind);
}

Index *Index::Subsample(long n,long seed){
   long i,j,*ptr;

   if(n>=ix){
      Index *pnd=new Index(ix,idx,0);
      return(pnd);
   }

   srandom((unsigned int)seed);

   long *udx=new long[ix];
   ptr=udx;
   for(i=0;i<n;i++)*(ptr++)=1;
   for(i=n;i<ix;i++)*(ptr++)=0;
   shuffle(ix,udx);
   Index *rind=new Index(n);
   j=0;
   ptr=udx;
   for(i=0;i<ix;i++){
      if(*(ptr++))*(rind->idx+(j++))=*(idx+i);
   }
   delete [] udx;
   return(rind);
}

Index *Index::Subsample(long n){
   long i,j,*ptr;

   if(n>=ix){
      Index *pnd=new Index(ix,idx,0);
      return(pnd);
   }

   long *udx=new long[ix];
   ptr=udx;
   for(i=0;i<n;i++)*(ptr++)=1;
   for(i=n;i<ix;i++)*(ptr++)=0;
   shuffle(ix,udx);
   Index *rind=new Index(n);
   j=0;
   ptr=udx;
   for(i=0;i<ix;i++){
      if(*(ptr++))*(rind->idx+(j++))=*(idx+i);
   }
   delete [] udx;
   return(rind);
}

long Index::Lbound(long n){
   if(idx[0]>=n)return(0);
   if(idx[ix-1]<n)return(ix);
   long i=0,j=ix-1;
   long k;
   
   while(j-i>1){
      k=(i+j)/2;
      if(idx[k]<n)i=k;
      else j=k;
   }
   return(j);
}

long Index::Ubound(long n){
   if(idx[ix-1]<n)return(ix-1);
   if(idx[0]>=n)return(-1);
   long i=0,j=ix-1;
   long k;

   while(j-i>1){
      k=(i+j)/2;
      if(idx[k]<n)i=k;
      else j=k;
   }
   return(i);
}

void Index::Compose(Index *pKnd){
   long i,j,k;

   for(i=0;i<ix;i++)idx[i]=pKnd->idx[idx[i]];
}

Index *Index::Greater(float *sxx,float thresh){
   long i,k,ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+i)>thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+i)>thresh)ind->idx[ct++]=idx[i];
   }
   return(ind);
}

Index *Index::Greateq(float *sxx,float thresh){
   long i,k,ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+i)>=thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+i)>=thresh)ind->idx[ct++]=idx[i];
   }
   return(ind);
}

Index *Index::Greater(double *sxx,double thresh){
   long i,k,ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+i)>thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+i)>thresh)ind->idx[ct++]=idx[i];
   }
   return(ind);
}

Index *Index::Greateq(double *sxx,double thresh){
   long i,k,ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+i)>=thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+i)>=thresh)ind->idx[ct++]=idx[i];
   }
   return(ind);
}

Index *Index::Lesser(float *sxx,float thresh){
   long i,k,ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+i)<thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+i)<thresh)ind->idx[ct++]=idx[i];
   }
   return(ind);
}

Index *Index::Lesseq(float *sxx,float thresh){
   long i,k,ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+i)<=thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+i)<=thresh)ind->idx[ct++]=idx[i];
   }
   return(ind);
}

Index *Index::Lesser(double *sxx,double thresh){
   long i,k,ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+i)<thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+i)<thresh)ind->idx[ct++]=idx[i];
   }
   return(ind);
}

Index *Index::Lesseq(double *sxx,double thresh){
   long i,k,ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+i)<=thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+i)<=thresh)ind->idx[ct++]=idx[i];
   }
   return(ind);
}

//sxx[idx[]]
Index *Index::Greater(float thresh,float *sxx){
   long i,k,ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+idx[i])>thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+(k=idx[i]))>thresh)ind->idx[ct++]=k;
   }
   return(ind);
}

Index *Index::Greateq(float thresh,float *sxx){
   long i,k,ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+idx[i])>=thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+(k=idx[i]))>=thresh)ind->idx[ct++]=k;
   }
   return(ind);
}

Index *Index::Greater(double thresh,double *sxx){
   long i,k,ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+idx[i])>thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+(k=idx[i]))>thresh)ind->idx[ct++]=k;
   }
   return(ind);
}

Index *Index::Greateq(double thresh,double *sxx){
   long i,k,ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+idx[i])>=thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+(k=idx[i]))>=thresh)ind->idx[ct++]=k;
   }
   return(ind);
}

Index *Index::Lesser(float thresh,float *sxx){
   long i,k,ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+idx[i])<thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+(k=idx[i]))<thresh)ind->idx[ct++]=k;
   }
   return(ind);
}

Index *Index::Lesseq(float thresh,float *sxx){
   long i,k,ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+idx[i])<=thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+(k=idx[i]))<=thresh)ind->idx[ct++]=k;
   }
   return(ind);
}

Index *Index::Lesser(double thresh,double *sxx){
   long i,k,ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+idx[i])<thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+(k=idx[i]))<thresh)ind->idx[ct++]=k;
   }
   return(ind);
}

Index *Index::Lesseq(double thresh,double *sxx){
   long i,k,ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+idx[i])<=thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   ct=0;
   for(i=0;i<ix;i++){
      if(*(sxx+(k=idx[i]))<=thresh)ind->idx[ct++]=k;
   }
   return(ind);
}

Order *Index::oGreater(float thresh,float *sxx){
   long i,k,ct=0;
   float ss;

   for(i=0;i<ix;i++){
      if(*(sxx+idx[i])>thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   long *ord=new long[ct];
   float *scx=new float[ct];
   ct=0;
   for(i=0;i<ix;i++){
      if((ss=*(sxx+(k=idx[i])))>thresh){
         scx[ct]=ss;
         ord[ct]=ct;
         ind->idx[ct++]=k;
      }
   }
   if(ct>1)hRort(ct,scx,ord);
   Order *jOrd=new Order;
   jOrd->pInd=ind;
   jOrd->order=ord;
   jOrd->score=scx;
   return(jOrd);
}

Order *Index::oGreateq(float thresh,float *sxx){
   long i,k,ct=0;
   float ss;

   for(i=0;i<ix;i++){
      if(*(sxx+idx[i])>=thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   long *ord=new long[ct];
   float *scx=new float[ct];
   ct=0;
   for(i=0;i<ix;i++){
      if((ss=*(sxx+(k=idx[i])))>=thresh){
         scx[ct]=ss;
         ord[ct]=ct;
         ind->idx[ct++]=k;
      }
   }
   if(ct>1)hRort(ct,scx,ord);
   Order *jOrd=new Order;
   jOrd->pInd=ind;
   jOrd->order=ord;
   jOrd->score=scx;
   return(jOrd);
}

Order *Index::oGreater(double thresh,double *sxx){
   long i,k,ct=0;
   double ss;

   for(i=0;i<ix;i++){
      if(*(sxx+idx[i])>thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   long *ord=new long[ct];
   float *scx=new float[ct];
   ct=0;
   for(i=0;i<ix;i++){
      if((ss=*(sxx+(k=idx[i])))>thresh){
         scx[ct]=(float)ss;
         ord[ct]=ct;
         ind->idx[ct++]=k;
      }
   }
   if(ct>1)hRort(ct,scx,ord);
   Order *jOrd=new Order;
   jOrd->pInd=ind;
   jOrd->order=ord;
   jOrd->score=scx;
   return(jOrd);
}

Order *Index::oGreateq(double thresh,double *sxx){
   long i,k,ct=0;
   double ss;

   for(i=0;i<ix;i++){
      if(*(sxx+idx[i])>=thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   long *ord=new long[ct];
   float *scx=new float[ct];
   ct=0;
   for(i=0;i<ix;i++){
      if((ss=*(sxx+(k=idx[i])))>=thresh){
         scx[ct]=(float)ss;
         ord[ct]=ct;
         ind->idx[ct++]=k;
      }
   }
   if(ct>1)hRort(ct,scx,ord);
   Order *jOrd=new Order;
   jOrd->pInd=ind;
   jOrd->order=ord;
   jOrd->score=scx;
   return(jOrd);
}

Order *Index::oLesser(float thresh,float *sxx){
   long i,k,ct=0;
   float ss;

   for(i=0;i<ix;i++){
      if(*(sxx+idx[i])<thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   long *ord=new long[ct];
   float *scx=new float[ct];
   ct=0;
   for(i=0;i<ix;i++){
      if((ss=*(sxx+(k=idx[i])))<thresh){
         scx[ct]=ss;
         ord[ct]=ct;
         ind->idx[ct++]=k;
      }
   }
   if(ct>1)hRort(ct,scx,ord);
   Order *jOrd=new Order;
   jOrd->pInd=ind;
   jOrd->order=ord;
   jOrd->score=scx;
   return(jOrd);
}

Order *Index::oLesseq(float thresh,float *sxx){
   long i,k,ct=0;
   float ss;

   for(i=0;i<ix;i++){
      if(*(sxx+idx[i])<=thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   long *ord=new long[ct];
   float *scx=new float[ct];
   ct=0;
   for(i=0;i<ix;i++){
      if((ss=*(sxx+(k=idx[i])))<=thresh){
         scx[ct]=ss;
         ord[ct]=ct;
         ind->idx[ct++]=k;
      }
   }
   if(ct>1)hRort(ct,scx,ord);
   Order *jOrd=new Order;
   jOrd->pInd=ind;
   jOrd->order=ord;
   jOrd->score=scx;
   return(jOrd);
}

Order *Index::oLesser(double thresh,double *sxx){
   long i,k,ct=0;
   double ss;

   for(i=0;i<ix;i++){
      if(*(sxx+idx[i])<thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   long *ord=new long[ct];
   float *scx=new float[ct];
   ct=0;
   for(i=0;i<ix;i++){
      if((ss=*(sxx+(k=idx[i])))<thresh){
         scx[ct]=(float)ss;
         ord[ct]=ct;
         ind->idx[ct++]=k;
      }
   }
   if(ct>1)hRort(ct,scx,ord);
   Order *jOrd=new Order;
   jOrd->pInd=ind;
   jOrd->order=ord;
   jOrd->score=scx;
   return(jOrd);
}

Order *Index::oLesseq(double thresh,double *sxx){
   long i,k,ct=0;
   double ss;

   for(i=0;i<ix;i++){
      if(*(sxx+idx[i])<=thresh)ct++;
   }
   if(!ct)return(NULL);
   Index *ind=new Index(ct);
   long *ord=new long[ct];
   float *scx=new float[ct];
   ct=0;
   for(i=0;i<ix;i++){
      if((ss=*(sxx+(k=idx[i])))<=thresh){
         scx[ct]=(float)ss;
         ord[ct]=ct;
         ind->idx[ct++]=k;
      }
   }
   if(ct>1)hRort(ct,scx,ord);
   Order *jOrd=new Order;
   jOrd->pInd=ind;
   jOrd->order=ord;
   jOrd->score=scx;
   return(jOrd);
}

  /* no longer used -- left just in case

void Index::Convert_to_pmid(Regist *pReg){
   long i;
   pReg->gopen_map(READ_U);

   for(i=0;i<ix;i++)idx[i]=pReg->pmid(idx[i]);
}

void Index::Convert_to_pmid(Regist_pth *pReg){
   long i;
   pReg->gopen_map(READ_U);

   for(i=0;i<ix;i++)idx[i]=pReg->pmid(idx[i]);
}

void Index::Convert_to_index(Regist *pReg){
   long i,j=0,k;
   pReg->gopen_map(READ_U);

   for(i=0;i<ix;i++){
      if(k=pReg->index(idx[i]))idx[j++]=k-1;
   }
   if(j<ix){
      cout << ix-j << " pmids not found" << endl;      
      ix=j;
   }
}

void Index::Convert_to_index(Regist_pth *pReg){
   long i,j=0,k;
   pReg->gopen_map(READ_U);

   for(i=0;i<ix;i++){
      if(k=pReg->index(idx[i]))idx[j++]=k-1;
   }
   if(j<ix){      
      cout << ix-j << " pmids not found" << endl;
      ix=j;
   }
}

   */


void Index::Save(const char *nam){
   FBase Fb("index",nam);
   ofstream *pfout=Fb.get_Ostr("x",ios::out);
   write(*pfout);
   Fb.dst_Ostr(pfout);
}

void Index::Load(const char *nam){
   FBase Fb("index",nam);
   ifstream *pfin=Fb.get_Istr("x",ios::in);
   read(*pfin);
   Fb.dst_Istr(pfin);
}

void Index::write(ofstream &fout){
   long i;

   fout << ix << endl;
   for(i=0;i<ix;i++){
      fout << " " << *(idx+i) << endl;
   }
}

void Index::read(ifstream &fin){
   long i;

   if(idx!=NULL)delete [] idx;

   fin >> ix;
   idx=new long[ix];
   for(i=0;i<ix;i++){
      fin >> idx[i];
   }
}

void Index::debug(void){
   long i;
  
   for(i=0;i<ix;i++)cout << i << " " << idx[i] << endl;
}

//Indes set object

Indes::Indes(void){
   idx=NULL;
}

Indes::Indes(unsigned int num){
   ix=num;
   idx=new unsigned int[ix];
}

Indes::Indes(unsigned int begin,unsigned int end_plus_one){
   unsigned int i;
   ix=end_plus_one-begin;
   idx=new unsigned int[ix];
   for(i=begin;i<end_plus_one;i++)*(idx+i-begin)=i;
}

Indes::Indes(Indes *ind){
   unsigned int i;

   ix=ind->ix;
   idx=new unsigned int[ix];
   for(i=0;i<ix;i++)idx[i]=ind->idx[i];
}

Indes::Indes(Index *ind){
   unsigned int i;

   ix=(unsigned int)ind->ix;
   idx=new unsigned int[ix];
   for(i=0;i<ix;i++)idx[i]=(unsigned int)ind->idx[i];
}

Indes::~Indes(void){
   if(idx!=NULL)delete [] idx;
}

//Cross validation

CValid::CValid(Index *gdd, long n){
   ind=new Index(0,n);
   gnd=new Index(gdd);
   bnd=ind->cbool_Butnot(gnd);


}

CValid::CValid(Index *gdd, Index *bdd){
   gnd=new Index(gdd);
   bnd=new Index(bdd);
   ind=gnd->cbool_Or(bnd);

}


void CValid::cross_valid(long m, long seed){

   long i,j,k,size,blk,rem;
   setn=m;

   pBTS=new Index* [setn];
   pGTS=new Index* [setn];
   pWTS=new Index* [setn];
   pBTR=new Index* [setn];
   pGTR=new Index* [setn];
   pWTR=new Index* [setn];

   size=gnd->ix;
   if(size<setn) {
      cout <<"Size of Relevant set is smaller than the the number of cross validation"<<endl;
      exit(0);
   }

   long *sizg, *sizb;

   sizg=new long[setn];
   blk=size/setn;
   rem=size%setn;

   for(i=0;i<setn;i++){
      if(i<rem){sizg[i]=blk+1;}
      else {sizg[i]=blk;}
   }

   sizb=new long[setn];
   size=bnd->ix;

   if(size<setn) {
      cout <<"Size of Non-Relevant set is smaller than the number of cross validations"<<endl;
      exit(0);
   }


   blk=size/setn;
   rem=size%setn;
   k=0;
   for(i=0;i<setn;i++){
      if(i<rem){sizb[i]=blk+1;}
      else {sizb[i]=blk;}
   }



   Index *tmp1=new Index(gnd);
   Index *tmp2=new Index(bnd);
   Index *tmp11,*tmp22;
   for(i=0;i<setn;i++){

      pGTS[i]=tmp1->Subsample(sizg[i],seed);
      pBTS[i]=tmp2->Subsample(sizb[i],seed);
      pWTS[i]=pGTS[i]->cbool_Or(pBTS[i]);

      pGTR[i]=gnd->cbool_Butnot(pGTS[i]);
      pBTR[i]=bnd->cbool_Butnot(pBTS[i]);
      pWTR[i]=pGTR[i]->cbool_Or(pBTR[i]);

      tmp11=tmp1->cbool_Butnot(pGTS[i]);
      delete tmp1;
      tmp1=NULL;
      if(tmp11) {
         tmp1=new Index(tmp11);
         delete tmp11;
         tmp11=NULL;
      }
      tmp22=tmp2->cbool_Butnot(pBTS[i]);
      delete tmp2;
      tmp2=NULL;
      if(tmp22){
         tmp2=new Index(tmp22);
         delete tmp22;
         tmp22=NULL;
      }


   }

   delete tmp1;
   delete tmp2;
   if(tmp11) delete tmp11;
   if(tmp22) delete tmp22;
   delete [] sizg;
   delete [] sizb;

}

CValid::~CValid(){
   for(long i=0;i<setn;i++){
      delete pGTS[i];
      delete pBTS[i];
      delete pWTS[i];
      delete pGTR[i];
      delete pBTR[i];
      delete pWTR[i];
   }
   delete [] pGTS;
   delete [] pBTS;
   delete [] pWTS;
   delete [] pGTR;
   delete [] pBTR;
   delete [] pWTR;  

   if (ind) delete ind;
   if (gnd) delete gnd;
   if (bnd) delete bnd;
}

//Scoring order object.

Order::Order(void){
   pInd=NULL;
   order=NULL;
   score=NULL;
}

Order::Order(long n,long m,float *sco){
   long i,j,k,*pt,ir,ii;
   long u,*idx,*ord,*inv;
   float *scx,ss,xx,*bt,*dt;
   n=(m<n)?m:n;

   if(n<2){
      if(n<1){
         pInd=NULL;
         order=NULL;
         score=NULL;
         return;
      }
      ss=*sco;
      i=1;
      j=0;
      while(i<m){
         xx=sco[i];
         if(ss<xx){
            j=i;
            ss=xx;
         }
         i++;
      }
      pInd=new Index(1);
      *(pInd->idx)=j;
      order=new long[1];
      score=new float[1];
      *order=0;
      *score=ss;
      return;
   }

   pInd=new Index(n);
   scx=new float[n];

   pt=idx=pInd->idx;
   bt=scx;
   dt=sco;
   for(i=0;i<n;i++){
      *(pt++)=i;
      *(bt++)=*(dt++);
   }

   //Build the initial heap
   k=(n>>1);
   ir=n-1;
   while(k){
      ss=scx[(--k)];
      ii=idx[k];

      i=k;
      j=((k+1)<<1)-1;
      while(j<=ir){
         if(j<ir && scx[j]>scx[j+1])++j;
         if(ss>scx[j]){
            scx[i]=scx[j];
            idx[i]=idx[j];
            j+=(i=j)+1;
         }
         else j=ir+1;
      }
      scx[i]=ss;
      idx[i]=ii;
   }
   //Filter the remaining points into heap
   xx=*scx;
   for(u=n;u<m;u++){
      if((ss=*(dt++))>xx){
         ii=u;
         i=0;
         j=1;
         while(j<=ir){
            if(j<ir && scx[j]>scx[j+1])++j;
            if(ss>scx[j]){
               scx[i]=scx[j];
               idx[i]=idx[j];
               j+=(i=j)+1;
            }
            else j=ir+1;
         }
         scx[i]=ss;
         idx[i]=ii;
         xx=*scx;
      }
   }
   //Order the heap by promotion & filtering
   for(;;){
      ss=scx[ir];
      ii=idx[ir];
      scx[ir]=scx[0];
      idx[ir]=idx[0];
      if((--ir)==0){
         scx[0]=ss;
         idx[0]=ii;
         break;
      }
      i=0;
      j=1;
      while(j<=ir){
         if(j<ir && scx[j]>scx[j+1])++j;
         if(ss>scx[j]){
            scx[i]=scx[j];
            idx[i]=idx[j];
            j+=(i=j)+1;
         }
         else j=ir+1;
      }
      scx[i]=ss;
      idx[i]=ii;
   }

   ord=new long[n];
   inv=new long[n];
   pt=inv;
   for(i=0;i<n;i++)*(pt++)=i;
   hSort(n,idx,inv);
   pt=inv;
   for(i=0;i<n;i++)*(ord+*(pt++))=i;
   delete [] inv;

   order=ord;
   score=scx;
}

Order::Order(long n,long m,double *sco){
   long i,j,k,*pt,ir,ii;
   long u,*idx,*ord,*inv;
   float *scx,ss,xx,*bt;
   double *dt;
   n=(m<n)?m:n;

   if(n<2){
      if(n<1){
         pInd=NULL;
         order=NULL;
         score=NULL;
         return;
      }
      ss=(float)*sco;
      i=1;
      j=0;
      while(i<m){
         xx=(float)sco[i];
         if(ss<xx){
            j=i;
            ss=xx;
         }
         i++;
      }
      pInd=new Index(1);
      *(pInd->idx)=j;
      order=new long[1];
      score=new float[1];
      *order=0;
      *score=ss;
      return;
   }

   pInd=new Index(n);
   scx=new float[n];

   pt=idx=pInd->idx;
   bt=scx;
   dt=sco;
   for(i=0;i<n;i++){
      *(pt++)=i;
      *(bt++)=(float)*(dt++);
   }

   //Build the initial heap
   k=(n>>1);
   ir=n-1;
   while(k){
      ss=scx[(--k)];
      ii=idx[k];

      i=k;
      j=((k+1)<<1)-1;
      while(j<=ir){
         if(j<ir && scx[j]>scx[j+1])++j;
         if(ss>scx[j]){
            scx[i]=scx[j];
            idx[i]=idx[j];
            j+=(i=j)+1;
         }
         else j=ir+1;
      }
      scx[i]=ss;
      idx[i]=ii;
   }
   //Filter the remaining points into heap
   xx=*scx;
   for(u=n;u<m;u++){
      if((ss=*(dt++))>xx){
         ii=u;
         i=0;
         j=1;
         while(j<=ir){
            if(j<ir && scx[j]>scx[j+1])++j;
            if(ss>scx[j]){
               scx[i]=scx[j];
               idx[i]=idx[j];
               j+=(i=j)+1;
            }
            else j=ir+1;
         }
         scx[i]=ss;
         idx[i]=ii;
         xx=*scx;
      }
   }
   //Order the heap by promotion & filtering
   for(;;){
      ss=scx[ir];
      ii=idx[ir];
      scx[ir]=scx[0];
      idx[ir]=idx[0];
      if((--ir)==0){
         scx[0]=ss;
         idx[0]=ii;
         break;
      }
      i=0;
      j=1;
      while(j<=ir){
         if(j<ir && scx[j]>scx[j+1])++j;
         if(ss>scx[j]){
            scx[i]=scx[j];
            idx[i]=idx[j];
            j+=(i=j)+1;
         }
         else j=ir+1;
      }
      scx[i]=ss;
      idx[i]=ii;
   }

   ord=new long[n];
   inv=new long[n];
   pt=inv;
   for(i=0;i<n;i++)*(pt++)=i;
   hSort(n,idx,inv);
   pt=inv;
   for(i=0;i<n;i++)*(ord+*(pt++))=i;
   delete [] inv;

   order=ord;
   score=scx;
}

Order::Order(long n,Index *ind,float *sco){
   long i,j,k,*pt,ir,ii;
   long u,*idx,*ord,*inv;
   float *scx,ss,xx,*bt,*dt;
   long m=ind->ix,*udx=ind->idx;
   n=(m<n)?m:n;

   if(n<2){
      if(n<1){
         pInd=NULL;
         order=NULL;
         score=NULL;
         return;
      }
      ss=sco[udx[0]];
      i=1;
      j=udx[0];
      while(i<m){
         xx=sco[udx[i]];
         if(ss<xx){
            j=udx[i];
            ss=xx;
         }
         i++;
      }
      pInd=new Index(1);
      *(pInd->idx)=j;
      order=new long[1];
      score=new float[1];
      *order=0;
      *score=ss;
      return;
   }

   pInd=new Index(n);
   scx=new float[n];

   pt=idx=pInd->idx;
   bt=scx;
   for(i=0;i<n;i++){
      *(pt++)=udx[i];
      *(bt++)=sco[udx[i]];
   }

   //Build the initial heap
   k=(n>>1);
   ir=n-1;
   while(k){
      ss=scx[(--k)];
      ii=idx[k];

      i=k;
      j=((k+1)<<1)-1;
      while(j<=ir){
         if(j<ir && scx[j]>scx[j+1])++j;
         if(ss>scx[j]){
            scx[i]=scx[j];
            idx[i]=idx[j];
            j+=(i=j)+1;
         }
         else j=ir+1;
      }
      scx[i]=ss;
      idx[i]=ii;
   }
   //Filter the remaining points into heap
   xx=*scx;
   for(u=n;u<m;u++){
      if((ss=sco[udx[u]])>xx){
         ii=udx[u];
         i=0;
         j=1;
         while(j<=ir){
            if(j<ir && scx[j]>scx[j+1])++j;
            if(ss>scx[j]){
               scx[i]=scx[j];
               idx[i]=idx[j];
               j+=(i=j)+1;
            }
            else j=ir+1;
         }
         scx[i]=ss;
         idx[i]=ii;
         xx=*scx;
      }
   }
   //Order the heap by promotion & filtering
   for(;;){
      ss=scx[ir];
      ii=idx[ir];
      scx[ir]=scx[0];
      idx[ir]=idx[0];
      if((--ir)==0){
         scx[0]=ss;
         idx[0]=ii;
         break;
      }
      i=0;
      j=1;
      while(j<=ir){
         if(j<ir && scx[j]>scx[j+1])++j;
         if(ss>scx[j]){
            scx[i]=scx[j];
            idx[i]=idx[j];
            j+=(i=j)+1;
         }
         else j=ir+1;
      }
      scx[i]=ss;
      idx[i]=ii;
   }

   ord=new long[n];
   inv=new long[n];
   pt=inv;
   for(i=0;i<n;i++)*(pt++)=i;
   hSort(n,idx,inv);
   pt=inv;
   for(i=0;i<n;i++)*(ord+*(pt++))=i;
   delete [] inv;

   order=ord;
   score=scx;
}

Order::Order(long n,Index *ind,double *sco){
   long i,j,k,*pt,ir,ii;
   long u,*idx,*ord,*inv;
   float *scx,ss,xx,*bt;
   double *dt;
   long m=ind->ix,*udx=ind->idx;
   n=(m<n)?m:n;

   if(n<2){
      if(n<1){
         pInd=NULL;
         order=NULL;
         score=NULL;
         return;
      }
      ss=(float)sco[udx[0]];
      i=1;
      j=udx[0];
      while(i<m){
         xx=(float)sco[udx[i]];
         if(ss<xx){
            j=udx[i];
            ss=xx;
         }
         i++;
      }
      pInd=new Index(1);
      *(pInd->idx)=j;
      order=new long[1];
      score=new float[1];
      *order=0;
      *score=ss;
      return;
   }

   pInd=new Index(n);
   scx=new float[n];

   pt=idx=pInd->idx;
   bt=scx;
   for(i=0;i<n;i++){
      *(pt++)=udx[i];
      *(bt++)=(float)sco[udx[i]];
   }

   //Build the initial heap
   k=(n>>1);
   ir=n-1;
   while(k){
      ss=scx[(--k)];
      ii=idx[k];

      i=k;
      j=((k+1)<<1)-1;
      while(j<=ir){
         if(j<ir && scx[j]>scx[j+1])++j;
         if(ss>scx[j]){
            scx[i]=scx[j];
            idx[i]=idx[j];
            j+=(i=j)+1;
         }
         else j=ir+1;
      }
      scx[i]=ss;
      idx[i]=ii;
   }
   //Filter the remaining points into heap
   xx=*scx;
   for(u=n;u<m;u++){
      if((ss=(float)sco[udx[u]])>xx){
         ii=udx[u];
         i=0;
         j=1;
         while(j<=ir){
            if(j<ir && scx[j]>scx[j+1])++j;
            if(ss>scx[j]){
               scx[i]=scx[j];
               idx[i]=idx[j];
               j+=(i=j)+1;
            }
            else j=ir+1;
         }
         scx[i]=ss;
         idx[i]=ii;
         xx=*scx;
      }
   }
   //Order the heap by promotion & filtering
   for(;;){
      ss=scx[ir];
      ii=idx[ir];
      scx[ir]=scx[0];
      idx[ir]=idx[0];
      if((--ir)==0){
         scx[0]=ss;
         idx[0]=ii;
         break;
      }
      i=0;
      j=1;
      while(j<=ir){
         if(j<ir && scx[j]>scx[j+1])++j;
         if(ss>scx[j]){
            scx[i]=scx[j];
            idx[i]=idx[j];
            j+=(i=j)+1;
         }
         else j=ir+1;
      }
      scx[i]=ss;
      idx[i]=ii;
   }

   ord=new long[n];
   inv=new long[n];
   pt=inv;
   for(i=0;i<n;i++)*(pt++)=i;
   hSort(n,idx,inv);
   pt=inv;
   for(i=0;i<n;i++)*(ord+*(pt++))=i;
   delete [] inv;

   order=ord;
   score=scx;
}

Order::Order(long n,float *sco,Index *ind){
   long i,j,k,*pt,*qt,ir,ii;
   long u,*idx,*ord,*inv;
   float *scx,ss,xx,*bt,*dt;
   n=((ind->ix)<n)?(ind->ix):n;

   if(n<2){
      if(n<1){
         pInd=NULL;
         order=NULL;
         score=NULL;
         return;
      }
      ss=*sco;
      i=1;
      j=0;
      while(i<ind->ix){
         xx=sco[i];
         if(ss<xx){
            j=i;
            ss=xx;
         }
         i++;
      }
      pInd=new Index(1);
      *(pInd->idx)=ind->idx[j];
      order=new long[1];
      score=new float[1];
      *order=0;
      *score=ss;
      return;
   }

   pInd=new Index(n);
   scx=new float[n];

   pt=idx=pInd->idx;
   qt=ind->idx;
   bt=scx;
   dt=sco;
   for(i=0;i<n;i++){
      *(pt++)=*(qt++);
      *(bt++)=*(dt++);
   }

   //Build the initial heap
   k=(n>>1);
   ir=n-1;
   while(k){
      ss=scx[(--k)];
      ii=idx[k];

      i=k;
      j=((k+1)<<1)-1;
      while(j<=ir){
         if(j<ir && scx[j]>scx[j+1])++j;
         if(ss>scx[j]){
            scx[i]=scx[j];
            idx[i]=idx[j];
            j+=(i=j)+1;
         }
         else j=ir+1;
      }
      scx[i]=ss;
      idx[i]=ii;
   }
   //Filter the remaining points into heap
   xx=*scx;
   for(u=n;u<ind->ix;u++){
      if((ss=*(dt++))>xx){
         ii=ind->idx[u];
         i=0;
         j=1;
         while(j<=ir){
            if(j<ir && scx[j]>scx[j+1])++j;
            if(ss>scx[j]){
               scx[i]=scx[j];
               idx[i]=idx[j];
               j+=(i=j)+1;
            }
            else j=ir+1;
         }
         scx[i]=ss;
         idx[i]=ii;
         xx=*scx;
      }
   }
   //Order the heap by promotion & filtering
   for(;;){
      ss=scx[ir];
      ii=idx[ir];
      scx[ir]=scx[0];
      idx[ir]=idx[0];
      if((--ir)==0){
         scx[0]=ss;
         idx[0]=ii;
         break;
      }
      i=0;
      j=1;
      while(j<=ir){
         if(j<ir && scx[j]>scx[j+1])++j;
         if(ss>scx[j]){
            scx[i]=scx[j];
            idx[i]=idx[j];
            j+=(i=j)+1;
         }
         else j=ir+1;
      }
      scx[i]=ss;
      idx[i]=ii;
   }

   ord=new long[n];
   inv=new long[n];
   pt=inv;
   for(i=0;i<n;i++)*(pt++)=i;
   hSort(n,idx,inv);
   pt=inv;
   for(i=0;i<n;i++)*(ord+*(pt++))=i;
   delete [] inv;

   order=ord;
   score=scx;
}

Order::Order(long n,double *sco,Index *ind){
   long i,j,k,*pt,*qt,ir,ii;
   long u,*idx,*ord,*inv;
   float *scx,ss,xx,*bt;
   double *dt;
   n=((ind->ix)<n)?(ind->ix):n;

   if(n<2){
      if(n<1){
         pInd=NULL;
         order=NULL;
         score=NULL;
         return;
      }
      ss=(float)*sco;
      i=1;
      j=0;
      while(i<ind->ix){
         xx=(float)sco[i];
         if(ss<xx){
            j=i;
            ss=xx;
         }
         i++;
      }
      pInd=new Index(1);
      *(pInd->idx)=ind->idx[j];
      order=new long[1];
      score=new float[1];
      *order=0;
      *score=ss;
      return;
   }

   pInd=new Index(n);
   scx=new float[n];

   pt=idx=pInd->idx;
   qt=ind->idx;
   bt=scx;
   dt=sco;
   for(i=0;i<n;i++){
      *(pt++)=*(qt++);
      *(bt++)=(float)*(dt++);
   }

   //Build the initial heap
   k=(n>>1);
   ir=n-1;
   while(k){
      ss=scx[(--k)];
      ii=idx[k];

      i=k;
      j=((k+1)<<1)-1;
      while(j<=ir){
         if(j<ir && scx[j]>scx[j+1])++j;
         if(ss>scx[j]){
            scx[i]=scx[j];
            idx[i]=idx[j];
            j+=(i=j)+1;
         }
         else j=ir+1;
      }
      scx[i]=ss;
      idx[i]=ii;
   }
   //Filter the remaining points into heap
   xx=*scx;
   for(u=n;u<ind->ix;u++){
      if((ss=(float)*(dt++))>xx){
         ii=ind->idx[u];
         i=0;
         j=1;
         while(j<=ir){
            if(j<ir && scx[j]>scx[j+1])++j;
            if(ss>scx[j]){
               scx[i]=scx[j];
               idx[i]=idx[j];
               j+=(i=j)+1;
            }
            else j=ir+1;
         }
         scx[i]=ss;
         idx[i]=ii;
         xx=*scx;
      }
   }
   //Order the heap by promotion & filtering
   for(;;){
      ss=scx[ir];
      ii=idx[ir];
      scx[ir]=scx[0];
      idx[ir]=idx[0];
      if((--ir)==0){
         scx[0]=ss;
         idx[0]=ii;
         break;
      }
      i=0;
      j=1;
      while(j<=ir){
         if(j<ir && scx[j]>scx[j+1])++j;
         if(ss>scx[j]){
            scx[i]=scx[j];
            idx[i]=idx[j];
            j+=(i=j)+1;
         }
         else j=ir+1;
      }
      scx[i]=ss;
      idx[i]=ii;
   }

   ord=new long[n];
   inv=new long[n];
   pt=inv;
   for(i=0;i<n;i++)*(pt++)=i;
   hSort(n,idx,inv);
   pt=inv;
   for(i=0;i<n;i++)*(ord+*(pt++))=i;
   delete [] inv;

   order=ord;
   score=scx;
}

Order::Order(Order *pOrd){
   long i;

   pInd=new Index(pOrd->pInd);
   order=new long[pInd->ix];
   score=new float[pInd->ix];
   for(i=0;i<pInd->ix;i++){
      order[i]=pOrd->order[i];
      score[i]=pOrd->score[i];
   }
}

Order::~Order(void){
   if(pInd!=NULL)delete pInd;
   if(order!=NULL)delete [] order;
   if(score!=NULL)delete [] score;
}

long Order::num(void){
   return(pInd->ix);
}

long Order::ind(long i,float &sco){
   sco=*(score+i);
   return(*(pInd->idx+*(order+i)));
}

long Order::inv(long i){
   long j=0;
   while((j<pInd->ix)&&(i!=order[j]))j++;
   return(j);
}

long *Order::seq(void){
   long *ssq=new long[pInd->ix];
   long i;
   for(i=0;i<pInd->ix;i++)ssq[i]=*(pInd->idx+*(order+i));
   return(ssq);
}

Order *Order::cbool_And(Index *jnd){
   long i,j,k,*sub;

   Index *pind=pInd->cbool_And(jnd);
   if(pind==NULL)return(NULL);
   if(pind->ix<1)return(NULL);

   sub=new long[pInd->ix];
   for(i=0;i<pInd->ix;i++)sub[i]=0;
   long *pi=pInd->idx;
   long *pj=pind->idx;
   i=j=0;
   while(j<pind->ix){
      while(pi[i]<pj[j])i++;
      sub[i]=j+1;
      j++;
   }
   Order *psub=new Order;
   psub->pInd=pind;
   psub->order=new long[pind->ix];
   psub->score=new float[pind->ix];
   j=0;
   for(i=0;i<pInd->ix;i++){
      if((k=sub[order[i]])>0){
         psub->order[j]=k-1;
         psub->score[j]=score[i];
         j++;
      }
   }
   delete [] sub;
   return(psub);
}

Order *Order::cbool_Butnot(Index *jnd){
   long i,j,k,*sub;

   Index *pind=pInd->cbool_Butnot(jnd);
   if(pind==NULL)return(NULL);
   if(pind->ix<1)return(NULL);

   sub=new long[pInd->ix];
   for(i=0;i<pInd->ix;i++)sub[i]=0;
   long *pi=pInd->idx;
   long *pj=pind->idx;
   i=j=0;
   while(j<pind->ix){
      while(pi[i]<pj[j])i++;
      sub[i]=j+1;
      j++;
   }
   Order *psub=new Order;
   psub->pInd=pind;
   psub->order=new long[pind->ix];
   psub->score=new float[pind->ix];
   j=0;
   for(i=0;i<pInd->ix;i++){
      if((k=sub[order[i]])>0){
         psub->order[j]=k-1;
         psub->score[j]=score[i];
         j++;
      }
   }
   delete [] sub;
   return(psub);
}

float Order::Precision(long n,Index *ind){
   if(n>pInd->ix)return(0);
   else if(!n)return(0);
   Index *jnd=pInd->Subvalue(ind);
   float cx=0.0,sx,ss,vx;
   long i,j,k=0;
   long *bz=new long[pInd->ix];
   for(i=0;i<pInd->ix;i++)bz[i]=0;
   for(i=0;i<jnd->ix;i++)bz[jnd->idx[i]]=1;
   sx=score[0];
   while(k<n){
      i=0;
      vx=0.0;
      while((k+i<pInd->ix)&&(score[k+i]==sx)){
         vx+=bz[order[k+i]];
         i++;
      }
      if(k+i<n){
         cx+=vx;
         k+=i;
         sx=score[k];
      }
      else {
         cx+=vx*(n-k)/((float)i);
         k+=i;
      }
   }
   delete jnd;
   delete [] bz;
   return(cx/((float)n));
}

long Order::CtGreateq(float thresh){
   float sss,si,sj;
   long i,j,k;

   i=0;
   ind(i,sss);
   if(sss<thresh)return(0);
   j=num()-1;
   ind(j,sss);
   if(sss>=thresh)return(j+1);
   while(j-i>1){
      k=(i+j)/2;
      ind(k,sss);
      if(sss>=thresh)i=k;
      else j=k;
   }
   return(i+1);
}

void Order::debug(void){
   long i,j,k;
   float ss;
   k=num();
   cout << k << endl;
   for(i=0;i<k;i++){
      j=ind(i,ss);
      cout << i << " " << j << " " << ss << endl;
   }
}

void Order::debugW(void){
   long i;

   cout << pInd->ix << endl;
   for(i=0;i<pInd->ix;i++){
      cout << " " << *(pInd->idx+i) << " " << *(order+i) << " " \
         << *(score+i) << endl;
   }
}

void Order::write(ofstream &fout){
   long i;

   fout << pInd->ix << endl;
   for(i=0;i<pInd->ix;i++){
      fout << " " << *(pInd->idx+i) << " " << *(order+i) << " " \
         << *(score+i) << endl;
   }
}

void Order::read(ifstream &fin){
   long i;

   if(pInd!=NULL)delete pInd;
   if(order!=NULL)delete [] order;
   if(score!=NULL)delete [] score;

   fin >> i;
   pInd=new Index(i);
   order=new long[i];
   score=new float[i];
   for(i=0;i<pInd->ix;i++){
      fin >> pInd->idx[i] >> order[i] >> score[i];
   }
}

Index *Order::Topn(long n){
   if(n>=pInd->ix){
      Index *pnd=new Index(pInd);
      return(pnd);
   }
   else {
      long i,j;
      float xx;
      Index *pnd=new Index(n);
      long *md=new long[pInd->ix];
      for(j=0;j<pInd->ix;j++)md[j]=0;
      for(i=0;i<n;i++){
         j=order[i];
         md[j]=1;
      }
      i=0;
      for(j=0;j<pInd->ix;j++){
         if(md[j]){
            pnd->idx[i++]=pInd->idx[j];
         }
      }
      delete [] md;    
      return(pnd);
   }
}

double Order::SVar(Order *qOrd){
   long i,j,k;
   double sum1=0,sum2=0;
   float xx,yy;
   if(!qOrd)return(0);
   Index *qInd=qOrd->pInd;
   k=pInd->ix;
   for(i=0;i<k;i++){
      j=ind(i,xx);
      if(qInd->Subvalue(j))sum1+=xx*xx;
   }
   k=qInd->ix;
   for(i=0;i<k;i++){
      sum2+=(qOrd->score[i])*(qOrd->score[i]);
   }
   if(sum2>0)return(sum1/sum2);
   else return(0);
}

double Order::SVar(Order *qOrd,long m){
   long i,j,k;
   double sum1=0,sum2=0;
   float xx,yy;
   if(!qOrd)return(0);
   Index *sInd=new Index(m);
   for(i=0;i<m;i++){
      j=qOrd->ind(i,xx);
      sInd->idx[i]=j;
      sum2+=xx*xx;
   }
   sInd->sSort();
   k=pInd->ix;
   for(i=0;i<k;i++){
      j=ind(i,xx);
      if(sInd->Subvalue(j)){
         sum1+=xx*xx;
      }
   }
   delete sInd;
   if(sum2>0)return(sum1/sum2);
   else return(0);
}

}
