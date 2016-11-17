#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include "Strset.h"

using namespace std;
namespace iret {

Strset::Strset(const char *nam) : FBase("strset",nam){
}

Strset::Strset(int n,const char *nam) : FBase("strset",n,nam){
}

Strset::~Strset(void){
}

void Strset::gopen_write(void){
   num=0;
   pfa=get_Ostr("a");
   pfs=get_Ostr("s");
}

void Strset::gopen_write(long m){
   num=0;
   pfa=get_Ostr(m,"a");
   pfs=get_Ostr(m,"s");
}

void Strset::add_str(const char *pch){
   long i=pfs->tellp();   
   pfa->write((const char*)&i,sizeof(long));
   *pfs << pch << ends;
   num++;
}

void Strset::add_str_rend(const char *pch){
   long i=pfs->tellp();   
   pfa->write((const char*)&i,sizeof(long));
   *pfs << pch << " ()" << ends;
   num++;
}

void Strset::add_str_lend(const char *pch){
   long i=pfs->tellp();   
   pfa->write((const char*)&i,sizeof(long));
   *pfs << "() " << pch << ends;
   num++;
}

void Strset::add_str_bend(const char *pch){
   long i=pfs->tellp();   
   pfa->write((const char*)&i,sizeof(long));
   *pfs << "() " << pch << " ()" << ends;
   num++;
}

void Strset::gclose_write(void){
   ofstream *pfn=get_Ostr("n");
   *pfn << num << endl;
   dst_Ostr(pfn);
   dst_Ostr(pfa);
   dst_Ostr(pfs);
}

void Strset::gclose_write(long m){
   ofstream *pfn=get_Ostr(m,"n");
   *pfn << num << endl;
   dst_Ostr(pfn);
   dst_Ostr(pfa);
   dst_Ostr(pfs);
}

void Strset::gopen_map(void){
   cflag=1;
   if(Gcom(MAP_F)){
      ifstream *pfn=get_Istr("n");
      *pfn >> num;
      dst_Istr(pfn);

      addr=(long*)get_Mmap("a");
      str=get_Mmap("s");
   }
}

void Strset::gopen_map(long m){
   cflag=1;
   if(Gcom(MAP_F)){
      ifstream *pfn=get_Istr(m,"n");
      *pfn >> num;
      dst_Istr(pfn);

      addr=(long*)get_Mmap(m,"a");
      str=get_Mmap(m,"s");
   }
}

char *Strset::show_str(long n){
   if(n<0)return(NULL);
   if(n>=num)return(NULL);
   return(str+addr[n]);
}

void Strset::gclose_map(void){
   cflag=1;
   if(Rcom(MAP_F)){
      dst_Mmap("a",(char*)addr);
      dst_Mmap("s",str);
   }
}

void Strset::gclose_map(long m){
   cflag=1;
   if(Rcom(MAP_F)){
      dst_Mmap(m,"a",(char*)addr);
      dst_Mmap(m,"s",str);
   }
}

//Lexos is built on Strset

Lexos::Lexos(const char *nam) : Strset(nam){
   this->change_type("lexos");
   tx=new char[max_str];
   ht=new long[max_str];
   sn=new long[max_str];
   sm=new long[max_str];
}

Lexos::Lexos(int n,const char *nam) : Strset(n,nam){
   this->change_type("lexos");
   tx=new char[max_str];
   ht=new long[max_str];
   sn=new long[max_str];
   sm=new long[max_str];
}

Lexos::~Lexos(void){
   delete [] tx;
   delete [] ht;
   delete [] sn;
   delete [] sm;
}

void Lexos::create_Lexos(List &Ls){
   long i=0;
   this->gopen_write();
   Ls.node_first();
   while(Ls.node_next()){
      this->add_str(Ls.show_str());
      mark(++i,10000,"strings out");
   }
   this->gclose_write();
}

void Lexos::create_Lexos(List &Ls,long m){
   long i=0;
   this->gopen_write(m);
   Ls.node_first();
   while(Ls.node_next()){
      this->add_str(Ls.show_str());
      mark(++i,10000,"strings out");
   }
   this->gclose_write(m);
}

long Lexos::find(const char *ssr){
   int j;
   a=b=0;
   if((j=stc_my(ssr,str+addr[0]))<0)return(0);
   else if(j==0)return(1);

   if((j=stc_my(ssr,str+addr[num-1]))>0)return(0);
   else if(j==0)return(num);

   long i,x=0,y=num-1;
   while(y-x>1){
      i=(y+x)/2;
      if((j=stc_my(ssr,str+addr[i]))==0)return(i+1);
      else if(j<0)y=i;
      else x=i;
   }
   return(0);
}

int Lexos::stc_my(const char *ssr,const char *ptr)
   {register int i=(a<b) ? a : b;
   register const char *p1=ssr+i;
   register const char *p2=ptr+i;
   register int j=0;
   while((*p1==*p2)&&(*p1!='\0')){
      j++;
      p1++;
      p2++;
   }
   if(*p1==*p2)return(0);
   else if(*p1<*p2){
      b=i+j;
      return(-1);
   }
   else {
      a=i+j;
      return(1);
   }
}

long Lexos::lfind(const char *ssr){
   int i,j,k;
   char *p1;
   const char *p2;

   a=b=0;
   slen=strlen(ssr);
   strcpy(tx,ssr);
   for(i=0;i<slen;i++){
      ht[i]=0;
      sn[i]=0;
      sm[i]=0;
   }

   //Process first string
   p1=tx;
   p2=str+addr[0];
   j=0;
   while((*p1==*p2)&&(*p1!='\0')){
      j++;
      p1++;
      p2++;
   }
   if(*p1==*p2)return(1);
   else if(*p1<*p2)return(0);
   else {
      if(*p2=='\0'){
         ht[j]=1;
      }
      a=j;
   }
   //Process last string
   p1=tx;
   p2=str+addr[num-1];
   j=0;
   while((*p1==*p2)&&(*p1!='\0')){
      j++;
      p1++;
      p2++;
   }
   if(*p1==*p2)return(num);
   else if(*p1<*p2){
      b=j;
   }
   else {
      if(*p2=='\0'){
         return(num);
      }
      *p1='\0';
      b=j;
   }

   if(k=ifind(0,num,tx))return(k);
   i=slen-1;
   while(i>0){
      if(ht[i]>0)return(ht[i]);
      else if(ht[i]<0){
         tx[i]='\0';
         if(k=ifind(sn[i],sm[i],tx))return(k);
      }
      i--;
   }
   return(0);
}

long Lexos::ifind(long n,long m,const char *ssr){
   int j;
   a=b=0;

   long i,x=n,y=m;
   while(y-x>1){
      i=(y+x)/2;
      if((j=stc_ly(ssr,i))==0){
         if(a&&!ht[a]){
            ht[a]=-1;
            sn[a]=x;
            sm[a]=i;
         }
         return(i+1);
      }
      else if(j<0)y=i;
      else {
         if((j>1)&&(!ht[j-1])){
            ht[j-1]=-1;
            sn[j-1]=x;
            sm[j-1]=i;
         }
         x=i;
      }
   }
   return(0);
}

int Lexos::stc_ly(const char *ssr,long m)
   {register int i=(a<b) ? a : b;
   register const char *p1=ssr+i;
   register const char *p2=str+addr[m]+i;
   register int j=0;
   while((*p1==*p2)&&(*p1!='\0')){
      j++;
      p1++;
      p2++;
   }
   if(*p1==*p2){ht[i+j]=m+1;a=i+j-1;return(0);}
   else if(*p1<*p2){
      b=i+j;
      return(-1);
   }
   else {
      a=i+j;
      if(*p2=='\0'){
         ht[i+j]=m+1;
         return(i+j);
      }
      return(i+j+1);
   }
}

long Lexos::tfind(const char *ssr){
   int i,j,k;
   char *p1;
   const char *p2;

   a=b=0;
   slen=strlen(ssr);
   strcpy(tx,ssr);
   for(i=0;i<=slen;i++){
      ht[i]=0;
      sn[i]=0;
      sm[i]=0;
   }

   //Process first string
   p1=tx;
   p2=str+addr[0];
   j=0;
   while((*p1==*p2)&&(*p1!='\0')){
      j++;
      p1++;
      p2++;
   }
   if(*p1==*p2){ht[j]=1;return(1);}
   else if(*p1<*p2)return(0);
   else {
      if(*p2=='\0'){
         ht[j]=1;
      }
   }
   //Process last string
   p1=tx;
   p2=str+addr[num-1];
   j=0;
   while((*p1==*p2)&&(*p1!='\0')){
      j++;
      p1++;
      p2++;
   }
   if(*p1==*p2){
      ht[j]=num;
      if(j>1)*(--p1)='\0';
   }
   else if(*p2<*p1){
      if(*p2=='\0'){
         ht[j]=num;
         if(j>1)*(--p1)='\0';
      }
      else *p1='\0';
   }

   ifind(0,num-1,tx);
   i=slen;
   k=0;
   while(i>0){
      if(ht[i]>0)k++;
      else if(ht[i]<0){
         tx[i]='\0';
         if(ifind(sn[i],sm[i],tx))k++;
      }
      i--;
   }
   return(k);
}

long Lexos::find_low(const char *ssr){
   long j,k=0;
   a=b=0;
   if((j=stc_low(ssr,str+addr[0]))<0){
      if(j==-2)return(1);
      else return(0);
   }

   if((j=stc_low(ssr,str+addr[num-1]))>0)return(0);
   else if(j==-2)k=j;

   long i,x=0,y=num-1;
   while(y-x>1){
      i=(y+x)/2;
      if((j=stc_low(ssr,str+addr[i]))<0){y=i;k=j;}
      else x=i;
   }
   if(k==-2)return(y+1);
   else return(0);
}

int Lexos::stc_low(const char *ssr,const char *ptr)
   {register int i=(a<b) ? a : b;
   register const char *p1=ssr+i;
   register const char *p2=ptr+i;
   register int j=0;
   while((*p1==*p2)&&(*p1!='\0')){
      j++;
      p1++;
      p2++;
   }
   if(*p1<*p2){
      b=i+j;
      if(!*p1)return(-2);
      else return(-1);
   }
   else {
      if(!*p1){b=i+j;return(-2);}
      else {a=i+j;return(1);}
   }
}

long Lexos::find_high(const char *ssr){
   long j,k=0;
   a=b=0;
   if((j=stc_high(ssr,str+addr[0]))<0){
      if(j==-1)return(0);
      else if(j==-2)k=j;
   }

   if((j=stc_high(ssr,str+addr[num-1]))>0)return(0);
   else if(j==-2)return(num);

   long i,x=0,y=num-1;
   while(y-x>1){
      i=(y+x)/2;
      if((j=stc_high(ssr,str+addr[i]))<0){
         if(j==-1)y=i;
         if(j==-2){x=i;k=j;}
      }
      else {x=i;k=j;}
   }
   if(k==-2)return(x+1);
   else return(0);
}

int Lexos::stc_high(const char *ssr,const char *ptr)
   {register int i=(a<b) ? a : b;
   register const char *p1=ssr+i;
   register const char *p2=ptr+i;
   register int j=0;
   while((*p1==*p2)&&(*p1!='\0')){
      j++;
      p1++;
      p2++;
   }
   if(*p1<=*p2){
      if(!*p1){a=i+j;return(-2);}
      else {b=i+j;return(-1);}
   }
   else {
      a=i+j;
      return(1);
   }
}

//LexCnt

LexCnt::LexCnt(const char *nam) : Lexos(nam){
   this->change_type("lexcnt");
}

LexCnt::~LexCnt(void){
}
void LexCnt::create_LexCnt(Count &Ct){
   long k=0,i=0;
   this->gopen_write();
   ofstream *pfout=get_Ostr("c");
   Ct.node_first();
   pfout->write((char*)&k,sizeof(long));
   while(Ct.node_next()){
      this->add_str(Ct.show_str());
      k+=Ct.count();
      pfout->write((char*)&k,sizeof(long));
      mark(++i,10000,"strings out");
   }
   this->gclose_write();
   dst_Ostr(pfout);
   pfout=get_Ostr("tot");
   *pfout << Ct.total << endl;
   dst_Ostr(pfout);
}

void LexCnt::create_LexCnt(Count &Ct,long m){
   long k=0,i=0;
   this->gopen_write(m);
   ofstream *pfout=get_Ostr(m,"c");
   Ct.node_first();
   pfout->write((char*)&k,sizeof(long));
   while(Ct.node_next()){
      this->add_str(Ct.show_str());
      k+=Ct.count();
      pfout->write((char*)&k,sizeof(long));
      mark(++i,10000,"strings out");
   }
   this->gclose_write(m);
   dst_Ostr(pfout);
   pfout=get_Ostr(m,"tot");
   *pfout << Ct.total << endl;
   dst_Ostr(pfout);
}

void LexCnt::gopen_LexCnt(void){
   gopen_map();
   cflag=3;
   if(Gcom(MAP_C)){
      cnt=(long*)get_Mmap("c");
      ifstream *pfn=get_Istr("tot");
      *pfn >> tot;
      dst_Istr(pfn);
   }
}

void LexCnt::gopen_LexCnt(long m){
   gopen_map(m);
   cflag=3;
   if(Gcom(MAP_C)){
      cnt=(long*)get_Mmap(m,"c");
      ifstream *pfn=get_Istr(m,"tot");
      *pfn >> tot;
      dst_Istr(pfn);
   }
}

void LexCnt::gclose_LexCnt(void){
   gclose_map();
   cflag=3;
   if(Rcom(MAP_C)){
      dst_Mmap("c",(char*)cnt);
   }
}

void LexCnt::gclose_LexCnt(long m){
   gclose_map(m);
   cflag=3;
   if(Rcom(MAP_C)){
      dst_Mmap(m,"c",(char*)cnt);
   }
}

long LexCnt::count_Exact(const char *stt){
   long i;
   if(i=find(stt))return(cnt[i]-cnt[i-1]);
   else return(0);
}

long LexCnt::count_Super(const char *stt){
   long i,j,isum=0;

   if(i=find_low(stt)){
      j=find_high(stt);
      isum=cnt[j]-cnt[i-1];
   }
   return(isum);
}

//LexPrb

LexPrb::LexPrb(int mn,const char *nam) : LexCnt(nam){
   this->change_type("lexprb");
   min=mn;
}

LexPrb::~LexPrb(void){
}

void LexPrb::set_max(int mx){
   max=mx;
}

void LexPrb::set_aug(int ax){
   aug=ax;
}

void LexPrb::set_fst(char fx,long buf_size){
   buf=new char[buf_size];
   *buf=fx;
   bfc=&(buf[1]);
}

void LexPrb::gopen_add(void){
   pCt=new Count;
}

void LexPrb::add_segs(const char *stt){
   long i,j;

   i=strlen(stt);
   for(j=0;j<=i-min;j++){
      pCt->add_count2(stt+j,1);
   }
}

void LexPrb::add_segs_aug(const char *stt){
   long i,j;

   i=strlen(stt);
   if(min<=i)pCt->add_count2(stt,aug);
   for(j=1;j<=i-min;j++){
      pCt->add_count2(stt+j,1);
   }
}

void LexPrb::add_segs_fst(const char *stt){
   long i,j;

   strcpy(bfc,stt);
   i=strlen(buf);
   for(j=0;j<=i-min;j++){
      pCt->add_count2(buf+j,1);
   }
}

void LexPrb::gclose_add(void){
   create_LexCnt(*pCt);
   delete pCt;
   pCt=NULL;
}

void LexPrb::gclose_add(long m){
   create_LexCnt(*pCt,m);
   delete pCt;
   pCt=NULL;
}

//Suffix Tree Approach

void LexPrb::create_Substructures(Count &Ct){
   long k=0,i=0;
   this->gopen_write();
   ofstream *pfout=get_Ostr("c");
   Ct.node_first();
   while(Ct.node_next()){
      this->add_str(Ct.show_str());
      k=Ct.count();
      pfout->write((char*)&k,sizeof(long));
      mark(++i,10000,"strings out");
   }
   this->gclose_write();
   dst_Ostr(pfout);
}

void LexPrb::create_Substructures(char fx,Count &Ct){
   long k=0,i=0;
   char cnam[max_str],*pch;
   cnam[0]=fx;
   pch=cnam+1;
   this->gopen_write();
   ofstream *pfout=get_Ostr("c");
   Ct.node_first();
   while(Ct.node_next()){
      strcpy(pch,Ct.show_str());
      this->add_str(cnam);
      k=Ct.count();
      pfout->write((char*)&k,sizeof(long));
      mark(++i,10000,"strings out");
   }
   this->gclose_write();
   dst_Ostr(pfout);
}

void LexPrb::create_Final(void){
   long i,j,k,len;
   char *pch;

   cnt=(long *)get_Read("c");
   gopen_map();
   Count *pCp=new Count;
   for(i=0;i<num;i++){
      pch=show_str(i);
      k=cnt[i];
      len=strlen(pch);
      for(j=0;j<=len-min;j++){
         pCp->addp_count2(pch+j,k);
      }
   }
   i=(long)str;
   gclose_map();
   delete [] cnt;
   
   ofstream *pfad=get_Ostr("a");
   ofstream *pfct=get_Ostr("c");
   k=0;
   pfct->write((char*)&k,sizeof(long));
   pCp->node_first();
   while(pCp->node_next()){
      j=(long)pCp->show_str()-i;
      k+=pCp->count();
      pfct->write((char*)&k,sizeof(long));
      pfad->write((char*)&j,sizeof(long));
   }
   dst_Ostr(pfct);
   dst_Ostr(pfad);
   ofstream *pfout=get_Ostr("tot");
   *pfout << pCp->total << endl;
   dst_Ostr(pfout);
   ofstream *pfn=get_Ostr("n");
   *pfn << pCp->cnt_key << endl;
   dst_Ostr(pfn);
}

void LexPrb::create_Final(long aag){
   long i,j,k,len;
   char *pch;

   cnt=(long *)get_Read("c");
   gopen_map();
   Count *pCp=new Count;
   for(i=0;i<num;i++){
      pch=show_str(i);
      k=cnt[i];
      len=strlen(pch);
      pCp->addp_count2(pch,k+aag);
      for(j=1;j<=len-min;j++){
         pCp->addp_count2(pch+j,k);
      }
   }
   i=(long)str;
   gclose_map();
   delete [] cnt;

   ofstream *pfad=get_Ostr("a");
   ofstream *pfct=get_Ostr("c");
   k=0;
   pfct->write((char*)&k,sizeof(long));
   pCp->node_first();
   while(pCp->node_next()){
      j=(long)pCp->show_str()-i;
      k+=pCp->count();
      pfct->write((char*)&k,sizeof(long));
      pfad->write((char*)&j,sizeof(long));
   }
   dst_Ostr(pfct);
   dst_Ostr(pfad);
   ofstream *pfout=get_Ostr("tot");
   *pfout << pCp->total << endl;
   dst_Ostr(pfout);
   ofstream *pfn=get_Ostr("n");
   *pfn << pCp->cnt_key << endl;
   dst_Ostr(pfn);
}

//Prob functions

void LexPrb::gopen_LexPrb(void){
   gopen_LexCnt();
}

void LexPrb::gopen_LexPrb(long m){
   gopen_LexCnt(m);
}

double LexPrb::prob_ext(const char *stt){
   long i,j,k,m,ok,is;
   char *pch;
   double xx,zz;

   i=strlen(stt);
   pch=new char[i+1];
   strcpy(pch,stt);

   j=1;
   k=1;
   ok=0;
   while(k&&(j<=i)){
      pch[j]='\0';
      k=find_low(pch);
      if(k==0){
         if(j==1){
            delete [] pch;
            return(0);
         }
         else {
            pch[j]=stt[j];
            j--;
            pch[j]='\0';
            m=find_high(pch);
            zz=(cnt[m]-cnt[ok-1])/((double)tot);
            pch[j]=stt[j];
         }
      }
      else {
         if(j<i){
            ok=k;
            pch[j]=stt[j];
            j++;
         }
         else {
            m=find_high(pch);
            zz=(cnt[m]-cnt[k-1])/((double)tot);
            delete [] pch;
            return(zz);
         }
      }
   }
   is=0;
   while(j+is<i){
      is++;
      pch[j+is]='\0';
      while(!(k=find_low(pch+is))){is++;j--;}
      if(j<1){
         delete [] pch;
         return(0);
      }
      pch[j+is]=stt[j+is];
      pch[j-1+is]='\0';
      xx=(double)count_Super(pch+is);
      pch[j-1+is]=stt[j-1+is];

      if(j+is<i){
         ok=k;
         j++;
         while(k&&(j+is<=i)){
            pch[j+is]='\0';
            k=find_low(pch+is);
            if(k==0){
               pch[j+is]=stt[j+is];
               j--;
               pch[j+is]='\0';
               m=find_high(pch+is);
               zz*=(cnt[m]-cnt[ok-1])/xx;
               pch[j+is]=stt[j+is];
            }
            else {
               if(j+is<i){
                  ok=k;
                  pch[j+is]=stt[j+is];
                  j++;
               }
               else {
                  m=find_high(pch+is);
                  zz*=(cnt[m]-cnt[k-1])/xx;
                  delete [] pch;
                  return(zz);
               }
            }
         }
      }
      else {
         m=find_high(pch+is);
         zz*=(cnt[m]-cnt[k-1])/xx;
         delete [] pch;
         return(zz);
      }
   }
   delete [] pch;
   return(zz);
}

double LexPrb::prob_ext_fst(const char *stt){
   strcpy(bfc,stt);
   return(prob_ext(buf));
}

double LexPrb::prob_fxd(const char *stt){
   long i,j,k,m,ok,is,iz;
   char *pch;
   double xx,zz;

   i=strlen(stt);
   pch=new char[i+1];
   strcpy(pch,stt);

   iz=(i<max)?i:max;
   j=1;
   k=1;
   ok=0;
   while(k&&(j<=iz)){
      pch[j]='\0';
      k=find_low(pch);
      if(k==0){
         if(j==1){
            delete [] pch;
            return(0);
         }
         else {
            pch[j]=stt[j];
            j--;
            pch[j]='\0';
            m=find_high(pch);
            zz=(cnt[m]-cnt[ok-1])/((double)tot);
            pch[j]=stt[j];
         }
      }
      else {
         if(j<iz){
            ok=k;
            pch[j]=stt[j];
            j++;
         }
         else {
            m=find_high(pch);
            zz=(cnt[m]-cnt[k-1])/((double)tot);
            if(iz==i){
               delete [] pch;
               return(zz);
            }
            else {
               pch[j]=stt[j];
               k=0;
            }
         }
      }
   }
   is=0;
   while(j+is<i){
      is++;
      pch[j+is]='\0';
      while(!(k=find_low(pch+is))){is++;j--;}
      if(j<1){
         delete [] pch;
         return(0);
      }
      pch[j+is]=stt[j+is];
      pch[j-1+is]='\0';
      xx=(double)count_Super(pch+is);
      pch[j-1+is]=stt[j-1+is];

      iz=is+((max<i-is)?max:(i-is));
      if(j+is<iz){
         ok=k;
         j++;
         while(k&&(j+is<=iz)){
            pch[j+is]='\0';
            k=find_low(pch+is);
            if(k==0){
               pch[j+is]=stt[j+is];
               j--;
               pch[j+is]='\0';
               m=find_high(pch+is);
               zz*=(cnt[m]-cnt[ok-1])/xx;
               pch[j+is]=stt[j+is];
            }
            else {
               if(j+is<iz){
                  ok=k;
                  pch[j+is]=stt[j+is];
                  j++;
               }
               else {
                  m=find_high(pch+is);
                  zz*=(cnt[m]-cnt[k-1])/xx;
                  if(iz==i){
                     delete [] pch;
                     return(zz);
                  }
                  else {
                     pch[j+is]=stt[j+is];
                     k=0;
                  }
               }
            }
         }
      }
      else {
         pch[j+is]='\0';
         m=find_high(pch+is);
         zz*=(cnt[m]-cnt[k-1])/xx;
         if(iz==i){
            delete [] pch;
            return(zz);
         }
         else pch[j+is]=stt[j+is];
      }
   }
   delete [] pch;
   return(zz);
}

double LexPrb::prob_fxd_fst(const char *stt){
   strcpy(bfc,stt);
   return(prob_fxd(buf));
}

void LexPrb::gclose_LexPrb(void){
   gclose_LexCnt();
}

//Double precisions counting
//LexDCnt

LexDCnt::LexDCnt(const char *nam) : Lexos(nam){
   this->change_type("lexcnt");
}

LexDCnt::~LexDCnt(void){
}
void LexDCnt::create_LexDCnt(DCount &Ct){
   long i=0;
   double z=0;
   this->gopen_write();
   ofstream *pfout=get_Ostr("c");
   Ct.node_first();
   pfout->write((char*)&z,sizeof(double));
   while(Ct.node_next()){
      this->add_str(Ct.show_str());
      z+=Ct.count();
      pfout->write((char*)&z,sizeof(double));
      mark(++i,10000,"strings out");
   }
   this->gclose_write();
   dst_Ostr(pfout);
   pfout=get_Ostr("tot");
   *pfout << Ct.total << endl;
   dst_Ostr(pfout);
}

void LexDCnt::create_LexDCnt(DCount &Ct,long m){
   long i=0;
   double z=0;
   this->gopen_write(m);
   ofstream *pfout=get_Ostr(m,"c");
   Ct.node_first();
   pfout->write((char*)&z,sizeof(double));
   while(Ct.node_next()){
      this->add_str(Ct.show_str());
      z+=Ct.count();
      pfout->write((char*)&z,sizeof(double));
      mark(++i,10000,"strings out");
   }
   this->gclose_write(m);
   dst_Ostr(pfout);
   pfout=get_Ostr(m,"tot");
   *pfout << Ct.total << endl;
   dst_Ostr(pfout);
}

void LexDCnt::gopen_LexDCnt(void){
   gopen_map();
   cflag=3;
   if(Gcom(MAP_C)){
      cnt=(double*)get_Mmap("c");
      ifstream *pfn=get_Istr("tot");
      *pfn >> tot;
      dst_Istr(pfn);
   }
}

void LexDCnt::gopen_LexDCnt(long m){
   gopen_map(m);
   cflag=3;
   if(Gcom(MAP_C)){
      cnt=(double*)get_Mmap(m,"c");
      ifstream *pfn=get_Istr(m,"tot");
      *pfn >> tot;
      dst_Istr(pfn);
   }
}

void LexDCnt::gclose_LexDCnt(void){
   gclose_map();
   cflag=3;
   if(Rcom(MAP_C)){
      dst_Mmap("c",(char*)cnt);
   }
}

void LexDCnt::gclose_LexDCnt(long m){
   gclose_map(m);
   cflag=3;
   if(Rcom(MAP_C)){
      dst_Mmap(m,"c",(char*)cnt);
   }
}

double LexDCnt::count_Exact(const char *stt){
   long i;
   if(i=find(stt))return(cnt[i]-cnt[i-1]);
   else return(0);
}

double LexDCnt::count_Super(const char *stt){
   long i,j;
   double zsum=0;

   if(i=find_low(stt)){
      j=find_high(stt);
      zsum=cnt[j]-cnt[i-1];
   }
   return(zsum);
}

//LexPrb2

LexPrb2::LexPrb2(int mn,const char *nam) : LexDCnt(nam){
   this->change_type("lexprb");
   min=mn;
}

LexPrb2::~LexPrb2(void){
}

void LexPrb2::set_max(int mx){
   max=mx;
}

void LexPrb2::set_aug(double ax){
   aug=ax;
}

void LexPrb2::set_fst(char fx,long buf_size){
   buf=new char[buf_size];
   *buf=fx;
   bfc=&(buf[1]);
}

void LexPrb2::gopen_add(void){
   pCt=new DCount;
}

void LexPrb2::add_segs(const char *stt,double wt){
   long i,j;

   i=strlen(stt);
   for(j=0;j<=i-min;j++){
      pCt->add_count2(stt+j,wt);
   }
}

void LexPrb2::add_segs_aug(const char *stt,double wt){
   long i,j;

   i=strlen(stt);
   if(min<=i)pCt->add_count2(stt,aug);
   for(j=1;j<=i-min;j++){
      pCt->add_count2(stt+j,wt);
   }
}

void LexPrb2::add_segs_fst(const char *stt,double wt){
   long i,j;

   strcpy(bfc,stt);
   i=strlen(buf);
   for(j=0;j<=i-min;j++){
      pCt->add_count2(buf+j,wt);
   }
}

void LexPrb2::gclose_add(void){
   create_LexDCnt(*pCt);
   delete pCt;
   pCt=NULL;
}

void LexPrb2::gclose_add(long m){
   create_LexDCnt(*pCt,m);
   delete pCt;
   pCt=NULL;
}

//Suffix Tree Approach

void LexPrb2::create_Substructures(DCount &Ct){
   long i=0;
   double xx;
   this->gopen_write();
   ofstream *pfout=get_Ostr("c");
   Ct.node_first();
   while(Ct.node_next()){
      this->add_str(Ct.show_str());
      xx=Ct.count();
      pfout->write((char*)&xx,sizeof(double));
      mark(++i,10000,"strings out");
   }
   this->gclose_write();
   dst_Ostr(pfout);
}

void LexPrb2::create_Substructures(char fx,DCount &Ct){
   long i=0;
   double xx;
   char cnam[max_str],*pch;
   cnam[0]=fx;
   pch=cnam+1;
   this->gopen_write();
   ofstream *pfout=get_Ostr("c");
   Ct.node_first();
   while(Ct.node_next()){
      strcpy(pch,Ct.show_str());
      this->add_str(cnam);
      xx=Ct.count();
      pfout->write((char*)&xx,sizeof(double));
      mark(++i,10000,"strings out");
   }
   this->gclose_write();
   dst_Ostr(pfout);
}

void LexPrb2::create_Final(void){
   long i,j,len;
   char *pch;
   double xx;

   cnt=(double *)get_Read("c");
   gopen_map();
   DCount *pCp=new DCount;
   for(i=0;i<num;i++){
      pch=show_str(i);
      xx=cnt[i];
      len=strlen(pch);
      for(j=0;j<=len-min;j++){
         pCp->addp_count2(pch+j,xx);
      }
   }
   i=(long)str;
   gclose_map();
   delete [] cnt;

   ofstream *pfad=get_Ostr("a");
   ofstream *pfct=get_Ostr("c");
   xx=0;
   pfct->write((char*)&xx,sizeof(double));
   pCp->node_first();
   while(pCp->node_next()){
      j=(long)pCp->show_str()-i;
      xx+=pCp->count();
      pfct->write((char*)&xx,sizeof(double));
      pfad->write((char*)&j,sizeof(long));
   }
   dst_Ostr(pfct);
   dst_Ostr(pfad);
   ofstream *pfout=get_Ostr("tot");
   *pfout << pCp->total << endl;
   dst_Ostr(pfout);
   ofstream *pfn=get_Ostr("n");
   *pfn << pCp->cnt_key << endl;
   dst_Ostr(pfn);
   pCp->iclean=1;
   pCp->~DCount();
}

void LexPrb2::create_Final(double aax){
   long i,j,len;
   char *pch;
   double xx;

   cnt=(double *)get_Read("c");
   gopen_map();
   DCount *pCp=new DCount;
   for(i=0;i<num;i++){
      pch=show_str(i);
      xx=cnt[i];
      len=strlen(pch);
      pCp->addp_count2(pch,xx+aax);
      for(j=1;j<=len-min;j++){
         pCp->addp_count2(pch+j,xx);
      }
   }
   i=(long)str;
   gclose_map();
   delete [] cnt;

   ofstream *pfad=get_Ostr("a");
   ofstream *pfct=get_Ostr("c");
   xx=0;
   pfct->write((char*)&xx,sizeof(double));
   pCp->node_first();
   while(pCp->node_next()){
      j=(long)pCp->show_str()-i;
      xx+=pCp->count();
      pfct->write((char*)&xx,sizeof(double));
      pfad->write((char*)&j,sizeof(long));
   }
   dst_Ostr(pfct);
   dst_Ostr(pfad);
   ofstream *pfout=get_Ostr("tot");
   *pfout << pCp->total << endl;
   dst_Ostr(pfout);
   ofstream *pfn=get_Ostr("n");
   *pfn << pCp->cnt_key << endl;
   dst_Ostr(pfn);
   pCp->iclean=1;
   pCp->~DCount();
}

//Probability functions

void LexPrb2::gopen_LexPrb2(void){
   gopen_LexDCnt();
}

void LexPrb2::gopen_LexPrb2(long m){
   gopen_LexDCnt(m);
}

double LexPrb2::prob_ext(const char *stt){
   long i,j,k,m,ok,is;
   char *pch;
   double xx,zz;

   i=strlen(stt);
   pch=new char[i+1];
   strcpy(pch,stt);

   j=1;
   k=1;
   ok=0;
   while(k&&(j<=i)){
      pch[j]='\0';
      k=find_low(pch);
      if(k==0){
         if(j==1){
            delete [] pch;
            return(0);
         }
         else {
            pch[j]=stt[j];
            j--;
            pch[j]='\0';
            m=find_high(pch);
            zz=(cnt[m]-cnt[ok-1])/tot;
            pch[j]=stt[j];
         }
      }
      else {
         if(j<i){
            ok=k;
            pch[j]=stt[j];
            j++;
         }
         else {
            m=find_high(pch);
            zz=(cnt[m]-cnt[k-1])/tot;
            delete [] pch;
            return(zz);
         }
      }
   }
   is=0;
   while(j+is<i){
      is++;
      pch[j+is]='\0';
      while(!(k=find_low(pch+is))){is++;j--;}
      if(j<1){
         delete [] pch;
         return(0);
      }
      pch[j+is]=stt[j+is];
      pch[j-1+is]='\0';
      xx=count_Super(pch+is);
      pch[j-1+is]=stt[j-1+is];

      if(j+is<i){
         ok=k;
         j++;
         while(k&&(j+is<=i)){
            pch[j+is]='\0';
            k=find_low(pch+is);
            if(k==0){
               pch[j+is]=stt[j+is];
               j--;
               pch[j+is]='\0';
               m=find_high(pch+is);
               zz*=(cnt[m]-cnt[ok-1])/xx;
               pch[j+is]=stt[j+is];
            }
            else {
               if(j+is<i){
                  ok=k;
                  pch[j+is]=stt[j+is];
                  j++;
               }
               else {
                  m=find_high(pch+is);
                  zz*=(cnt[m]-cnt[k-1])/xx;
                  delete [] pch;
                  return(zz);
               }
            }
         }
      }
      else {
         m=find_high(pch+is);
         zz*=(cnt[m]-cnt[k-1])/xx;
         delete [] pch;
         return(zz);
      }
   }
   delete [] pch;
   return(zz);
}

double LexPrb2::prob_ext_fst(const char *stt){
   strcpy(bfc,stt);
   return(prob_ext(buf));
}

double LexPrb2::prob_fxd(const char *stt){
   long i,j,k,m,ok,is,iz;
   char *pch;
   double xx,zz;

   i=strlen(stt);
   pch=new char[i+1];
   strcpy(pch,stt);

   iz=(i<max)?i:max;
   j=1;
   k=1;
   ok=0;
   while(k&&(j<=iz)){
      pch[j]='\0';
      k=find_low(pch);
      if(k==0){
         if(j==1){
            delete [] pch;
            return(0);
         }
         else {
            pch[j]=stt[j];
            j--;
            pch[j]='\0';
            m=find_high(pch);
            zz=(cnt[m]-cnt[ok-1])/tot;
            pch[j]=stt[j];
         }
      }
      else {
         if(j<iz){
            ok=k;
            pch[j]=stt[j];
            j++;
         }
         else {
            m=find_high(pch);
            zz=(cnt[m]-cnt[k-1])/tot;
            if(iz==i){
               delete [] pch;
               return(zz);
            }
            else {
               pch[j]=stt[j];
               k=0;
            }
         }
      }
   }
   is=0;
   while(j+is<i){
      is++;
      pch[j+is]='\0';
      while(!(k=find_low(pch+is))){is++;j--;}
      if(j<1){
         delete [] pch;
         return(0);
      }
      pch[j+is]=stt[j+is];
      pch[j-1+is]='\0';
      xx=count_Super(pch+is);
      pch[j-1+is]=stt[j-1+is];

      iz=is+((max<i-is)?max:(i-is));
      if(j+is<iz){
         ok=k;
         j++;
         while(k&&(j+is<=iz)){
            pch[j+is]='\0';
            k=find_low(pch+is);
            if(k==0){
               pch[j+is]=stt[j+is];
               j--;
               pch[j+is]='\0';
               m=find_high(pch+is);
               zz*=(cnt[m]-cnt[ok-1])/xx;
               pch[j+is]=stt[j+is];
            }
            else {
               if(j+is<iz){
                  ok=k;
                  pch[j+is]=stt[j+is];
                  j++;
               }
               else {
                  m=find_high(pch+is);
                  zz*=(cnt[m]-cnt[k-1])/xx;
                  if(iz==i){
                     delete [] pch;
                     return(zz);
                  }
                  else {
                     pch[j+is]=stt[j+is];
                     k=0;
                  }
               }
            }
         }
      }
      else {
         pch[j+is]='\0';
         m=find_high(pch+is);
         zz*=(cnt[m]-cnt[k-1])/xx;
         if(iz==i){
            delete [] pch;
            return(zz);
         }
         else pch[j+is]=stt[j+is];
      }
   }
   delete [] pch;
   return(zz);
}

double LexPrb2::prob_fxd_fst(const char *stt){
   strcpy(bfc,stt);
   return(prob_fxd(buf));
}

void LexPrb2::gclose_LexPrb2(void){
   gclose_LexDCnt();
}

void LexPrb2::gclose_LexPrb2(long m){
   gclose_LexDCnt(m);
}

}
