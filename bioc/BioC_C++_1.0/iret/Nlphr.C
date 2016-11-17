#include "Nlphr.h"
namespace iret {

Nlphr::Nlphr(const char *nm) : Strset(nm) {
   wordspace=10000;
   xstr=(char**)new long[wordspace];
   buf=new char[max_str];
} 

Nlphr::Nlphr(const char *nm,long wrdspc) : Strset(nm) {
   wordspace=wrdspc;
   xstr=(char**)new long[wordspace];
   buf=new char[max_str];
} 

Nlphr::~Nlphr(void){
   delete [] xstr;
   delete [] buf;
}  
 
void Nlphr::gopen_Nlphr_write(void){
   gopen_write();
   ndoc=nstr=0;
   pfd=get_Ostr("doc",ios::out);
   pfd->write((char*)&num,sizeof(long));
   pft=get_Ostr("typ",ios::out);
} 

void Nlphr::gopen_Nlphr_write(long m){
   gopen_write(m);
   ndoc=nstr=0;
   pfd=get_Ostr(m,"doc",ios::out);
   pfd->write((char*)&num,sizeof(long));
   pft=get_Ostr(m,"typ",ios::out);
}

void Nlphr::add_str(const char *str,const char c){
   Strset::add_str(str);
   pft->put(c);
}

void Nlphr::add_str_rend(const char *str,const char c){
   Strset::add_str_rend(str);
   pft->put(c);
}

void Nlphr::doc_end_mark(void){
   pfd->write((char*)&num,sizeof(long));
   ndoc++;
   nstr=num;
}

void Nlphr::gclose_Nlphr_write(void){
   if(nstr<num){
      pfd->write((char*)&num,sizeof(long));
      ndoc++;
   }
   dst_Ostr(pfd);
   pfd=get_Ostr("ndoc",ios::out);
   *pfd << ndoc << endl;
   dst_Ostr(pfd);
   dst_Ostr(pft);
   gclose_write();
}

void Nlphr::gclose_Nlphr_write(long m){
   if(nstr<num){
      pfd->write((char*)&num,sizeof(long));
      ndoc++;
   }
   dst_Ostr(pfd);
   pfd=get_Ostr(m,"ndoc",ios::out);
   *pfd << ndoc << endl;
   dst_Ostr(pfd);
   dst_Ostr(pft);
   gclose_write(m);
}
   
void Nlphr::gopen_Nlphr_map(void){
   gopen_map();
   cflag=5;
   if(Gcom(MAP_D)){
      ifstream *pfin=get_Istr("ndoc",ios::in);
      *pfin >> ndoc;
      dst_Istr(pfin);
      doc=(long *)get_Mmap("doc");
      typ=get_Mmap("typ");
   }
}

void Nlphr::gopen_Nlphr_map(long m){
   gopen_map(m);
   cflag=5;
   if(Gcom(MAP_D)){
      ifstream *pfin=get_Istr(m,"ndoc",ios::in);
      *pfin >> ndoc;
      dst_Istr(pfin);
      doc=(long *)get_Mmap(m,"doc");
      typ=get_Mmap(m,"typ");
   }
}

void Nlphr::read_map(long n){
   long j,m=doc[n];
   nstr=doc[n+1]-m;
   for(j=0;j<nstr;j++){
      xstr[j]=show_str(m+j);
   }
   xtyp=typ+m;
}

void Nlphr::gclose_Nlphr_map(void){
   gclose_map();
   cflag=5;
   if(Rcom(MAP_D)){
      dst_Mmap("doc",(char*)doc);
      dst_Mmap("typ",typ);
   }
}

void Nlphr::gclose_Nlphr_map(long m){
   gclose_map(m);
   cflag=5;
   if(Rcom(MAP_D)){
      dst_Mmap(m,"doc",(char*)doc);
      dst_Mmap(m,"typ",typ);
   }
}

void Nlphr::gopen_Nlphr_read(void){
   cflag=5;
   if(Gcom(MAP_F|MAP_D)){
      ifstream *pfn=get_Istr("n");
      *pfn >> num;
      dst_Istr(pfn);

      addr=(long*)get_Mmap("a");
      pfst=get_Istr("s",ios::out);

      ifstream *pfin=get_Istr("ndoc",ios::in);
      *pfin >> ndoc;
      dst_Istr(pfin);
      doc=(long *)get_Mmap("doc");
      typ=get_Mmap("typ");
   }
   nstr=0;
}

void Nlphr::gopen_Nlphr_read(long m){
   cflag=5;
   if(Gcom(MAP_F|MAP_D)){
      ifstream *pfn=get_Istr(m,"n");
      *pfn >> num;
      dst_Istr(pfn);

      addr=(long*)get_Mmap(m,"a");
      pfst=get_Istr(m,"s",ios::out);

      ifstream *pfin=get_Istr(m,"ndoc",ios::in);
      *pfin >> ndoc;
      dst_Istr(pfin);
      doc=(long *)get_Mmap(m,"doc");
      typ=get_Mmap(m,"typ");
   }
   nstr=0;
}

void Nlphr::read_disc(long n){
   long j,k,m=doc[n];
   char ch;
   nstr=doc[n+1]-m;
   for(j=0;j<nstr;j++){
      pfst->seekg(addr[m+j],ios::beg);
      k=0;
      while((ch=pfst->get()))buf[k++]=ch;
      buf[k]='\0';
      xstr[j]=new char[k+1];
      strcpy(xstr[j],buf);
   }
   xtyp=typ+m;
}

void Nlphr::clear_read(void){
   long i;
   for(i=0;i<nstr;i++){
      delete [] xstr[i];
   }
}

void Nlphr::gclose_Nlphr_read(void){
   cflag=5;
   if(Rcom(MAP_F|MAP_D)){
      dst_Mmap("a",(char*)addr);
      dst_Istr(pfst);
      dst_Mmap("doc",(char*)doc);
      dst_Mmap("typ",typ);
   }
}

void Nlphr::gclose_Nlphr_read(long m){
   cflag=5;
   if(Rcom(MAP_F|MAP_D)){
      dst_Mmap(m,"a",(char*)addr);
      dst_Istr(pfst);
      dst_Mmap(m,"doc",(char*)doc);
      dst_Mmap(m,"typ",typ);
   }
}

//CNlphr
CNlphr::CNlphr(const char *nm) : FBase("strset",nm){
   cur=-1;
   nst=0;
   pNl=NULL;
   wordspace=10000;
}

CNlphr::CNlphr(const char *nm,long wrdspc) : FBase("strset",nm){
   cur=-1;
   nst=0;
   pNl=NULL;
   wordspace=wrdspc;
}

CNlphr::~CNlphr(void){
   if(!pNl)delete [] pNl;
}

void CNlphr::setup_access(void){
   long i=0,sum;

   while(Exists(i,"n"))i++;
   nst=i;

   pNl=new Nlphr*[nst];
   long *mx=new long[nst];
   for(i=0;i<nst;i++){
      pNl[i]=new Nlphr(name,wordspace);
      map_down((FBase *)pNl[i]);
      pNl[i]->gopen_Nlphr_map(i);
      mx[i]=pNl[i]->ndoc;
      pNl[i]->gclose_Nlphr_map(i);
   }
   pBn=new Bnum(nst+1);
   pBn->mm[0]=0;
   sum=0;
   for(i=0;i<nst;i++){
      sum+=mx[i];
      pBn->mm[i+1]=sum;
   }
   tdoc=sum;
   delete [] mx;
}
   
void CNlphr::read(long n){
   long k,m;
   k=pBn->index(n);
   if(k!=cur){
      if(cur>-1)pNl[cur]->gclose_Nlphr_map(cur);
      cur=k;
      pNl[cur]->gopen_Nlphr_map(cur);
      xstr=pNl[cur]->xstr;
   }
   pNl[cur]->read_map(n-pBn->mm[k]);
   xtyp=pNl[cur]->xtyp;
   nstr=pNl[cur]->nstr;
}

void CNlphr::setup_access_random(void){
   long i=0,sum;

   while(Exists(i,"n"))i++;
   nst=i;

   pNl=new Nlphr*[nst];
   long *mx=new long[nst];
   for(i=0;i<nst;i++){
      pNl[i]=new Nlphr(name,wordspace);
      map_down((FBase *)pNl[i]);
      pNl[i]->gopen_Nlphr_map(i);
      mx[i]=pNl[i]->ndoc;
   }
   pBn=new Bnum(nst+1);
   pBn->mm[0]=0;
   sum=0;
   for(i=0;i<nst;i++){
      sum+=mx[i];
      pBn->mm[i+1]=sum;
   }
   tdoc=sum;
   delete [] mx;
}

void CNlphr::readr(long n){
   long k,m;
   k=pBn->index(n);
   pNl[k]->read_map(n-pBn->mm[k]);
   xstr=pNl[k]->xstr;
   xtyp=pNl[k]->xtyp;
   nstr=pNl[k]->nstr;
}

void CNlphr::show(long n){
   long i;
   read(n);
   for(i=0;i<nstr;i++){
      cout << xtyp[i] << "\t" << xstr[i] << endl;
   }
}

Index *CNlphr::PidField(char c){
   long *arr=new long[tdoc];
   long i,j,k,m;

   j=0;
   for(i=0;i<tdoc;i++){
      read(i);
      m=0;
      while((m<nstr)&&(xtyp[m]!=c))m++;
      if(m<nstr){
         str_long(xstr[0],k);
         arr[j++]=k;
      }
      mark(i,10000,"records");
   }
   Index *pInd=new Index;
   pInd->ix=j;
   pInd->idx=arr;
   return(pInd);
}

Index *CNlphr::NumField(char c){
   long *arr=new long[tdoc];
   long i,j,k,m;

   j=0;
   for(i=0;i<tdoc;i++){
      read(i);
      m=0;
      while((m<nstr)&&(xtyp[m]!=c))m++;
      if(m<nstr){
         str_long(xstr[0],k);
         arr[j++]=i;
      }
      mark(i,10000,"records");
   }
   Index *pInd=new Index;
   pInd->ix=j;
   pInd->idx=arr;
   return(pInd);
}

Index *CNlphr::PidAll(void){
   long *arr=new long[tdoc];
   long i,j,k;

   j=0;
   for(i=0;i<tdoc;i++){
      read(i);
      str_long(xstr[0],k);
      arr[j++]=k;
      mark(i,10000,"records");
   }
   Index *pInd=new Index;
   pInd->ix=j;
   pInd->idx=arr;
   return(pInd);
}

void CNlphr::create_pmid_index(void){
   long i,j,k;

   setup_access();
   Index *pNnd=PidAll();
   bin_Writ("pmid",tdoc*sizeof(long),(char*)pNnd->idx);
   delete pNnd;
}

void CNlphr::extend_access_pmid(void){
   pmid=(long*)get_Mmap("pmid");
}

long CNlphr::read_pmid(long pd){
   long i,j,k,m,n,flag=0;

   n=pmid[tdoc-1];
   if(pd>n)return(0);
   else if(pd==n){
      read(tdoc-1);
      return(tdoc);
   }
   m=pmid[0];
   if(pd<m)return(0);
   else if(pd==m){
      read(0);
      return(1);
   }
   i=0;
   j=tdoc-1;
   while(j-i>1){
      k=(i+j)/2;
      m=pmid[k];
      if(pd<m)j=k;
      else if(pd>m)i=k;
      else {flag=1;break;}
   }
   if(flag){read(k);return(k+1);}
   else return(0);
}

void CNlphr::setup_s_access(void){
   long i=0,sum,sx;

   while(Exists(i,"n"))i++;
   nst=i;

   pNl=new Nlphr*[nst];
   long *mx=new long[nst];
   dx=new long[nst+1];
   dx[0]=0;
   for(i=0;i<nst;i++){
      pNl[i]=new Nlphr(name);
      map_down((FBase *)pNl[i]);
      pNl[i]->gopen_Nlphr_map(i);
      mx[i]=pNl[i]->doc[pNl[i]->ndoc];
      dx[i+1]=pNl[i]->ndoc;
      pNl[i]->gclose_Nlphr_map(i);
   }
   pBn=new Bnum(nst+1);
   pBn->mm[0]=0;
   sum=0;
   for(i=0;i<nst;i++){
      sum+=mx[i];
      dx[i+1]+=dx[i];
      pBn->mm[i+1]=sum;
   }
   tdoc=sum;
   delete [] mx;
}

void CNlphr::read_s(long n){
   long k,m;
   k=pBn->index(n);
   if(k!=cur){
      if(cur>-1)pNl[cur]->gclose_Nlphr_map(cur);
      cur=k;
      pNl[cur]->gopen_Nlphr_map(cur);
   }
   ystr=pNl[cur]->show_str(n-pBn->mm[k]);
   ytyp=pNl[cur]->typ[n-pBn->mm[k]];
}

long CNlphr::doc_n(long n){
   long k,m,*dc,i,j,u;
   k=pBn->index(n);
   if(k!=cur){
      if(cur>-1)pNl[cur]->gclose_Nlphr_map(cur);
      cur=k;
      pNl[cur]->gopen_Nlphr_map(cur);
   }
   m=n-pBn->mm[k];
   dc=pNl[cur]->doc;
   i=0;
   j=pNl[cur]->ndoc;
   while(j-i>1){
      u=(i+j)/2;
      if(dc[u]>m)j=u;
      else i=u;
   }
   if (dc[u]<=m)
	return (u+dx[cur]);
   else
   	return(u-1+dx[cur]);
}

}
