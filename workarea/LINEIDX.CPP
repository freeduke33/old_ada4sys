// #define DEBUG

#ifdef DEBUG
#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#endif

#include "workarea.hpp"
#include "tabidx.hpp"

lineIdx::lineIdx(unsigned long blkSz,unsigned long blkAdd):tabIndex(blkSz)
{
 blkSizeAdd= blkAdd ? blkAdd : blkSz;
 Last=NULL;
 Cur=NULL;
 append(blkSz);
}

lineIdx::~lineIdx()
{
 lineBlk *P;

 while(Last)
   {
   P=Last->prevPtr;
   free(Last);
   Last=P;
   }
 return;
}

int lineIdx::Expand(unsigned long idx)
{
 int ret=1;
 unsigned long i;

 LOG(">  lineIdx::Expand(%lu)\n",idx);

 if(!Last) return 0;
 for(i=Last->Idx;i<=idx && ret;i+=blkSizeAdd) ret=append(blkSizeAdd);

 LOG("<  %d=lineIdx::Expand(%lu)\n",ret,idx);
 return ret;
}

int lineIdx::append(unsigned long sz)
{
 lineBlk *P;
 unsigned long s=sizeof(lineBlk)+sizeof(unsigned long)*(sz-1);
#ifdef DEBUG
 static unsigned long ss=0;
#endif

 P=(lineBlk *)calloc(1,s);

#ifdef DEBUG
 ss+=s;
 printf("alloc(%lu) \n",ss);
#endif

 if(P)
   {
   P->nextPtr=NULL;
   P->Idx=sz;

   if(Last) { P->prevPtr=Last; Last->nextPtr=P; P->Idx+=Last->Idx; }
   else P->prevPtr=NULL;
   Last=P;
   LOG("==  lineIdx::append(?): Last=%p[%lu-%lu]\n",Last,Last->Idx-sz,Last->Idx);
   if(!Cur) Cur=P;
   }
#ifdef DEBUG
 else
   {
   printf("Null pointer."); exit(0);
   }
#endif

 return (P!=NULL);
}

int lineIdx::Locate(unsigned long idx)
{
 int ret;
 LOG(">  lineIdx::Locate(%lu)\n",idx);

 if(!Expand(idx)) return 0;

 if(abs(Last->Idx-idx)<abs(Cur->Idx-idx)) ret=locate(Last,idx);
 else ret=locate(Cur,idx);

 LOG("<  %d=lineIdx::Locate(%lu)\n",ret,idx);
 return ret;
}


int lineIdx::locate(lineBlk *P,unsigned long idx)
{
 lineBlk *L;
 int ret=1;

 LOG(">  lineIdx::locate(%p[%lu-%lu],%lu)\n",P,P->Idx-blkSizeAdd,P->Idx,idx);

 if(P->Idx>idx)
   {
   L=P->prevPtr;
   while(L && L->Idx>idx) { P=L; L=P->prevPtr; }
   }
 else
   {
   while(P && P->Idx<=idx) P=P->nextPtr;
   if(!P) ret=0;
   }
 Cur=P;

 LOG("<  %d=lineIdx::locate(%p[%lu-%lu],%lu)\n",ret,P,P->Idx-blkSizeAdd,P->Idx,idx);
 return ret;
}

int lineIdx::get(unsigned long idx,unsigned long *offs)
{
 int ret;
 LOG("> int lineIdx::get(%lu,?)\n",idx);

 if(ret=Locate(idx)) *offs=Cur->Item[Cur->Idx-idx-1];

 if(!(*offs)-- ) ret=0;

 LOG("< %d=lineIdx::get(%lu,%lu)\n",ret,idx,*offs);
 return ret;
}

int lineIdx::getlast(unsigned long *idx,unsigned long *offs)
{
 int ret=0;
 unsigned i;
 lineBlk *P;

 LOG("> int lineIdx::getlast(?,?)\n");

 if(Last)
   {
   for(i=0;i<blkSizeAdd;i++) if(Last->Item[i]>0) { ret=1; break; }
   if(ret)
     {
     *idx=Last->Idx-i-1;
     *offs=Last->Item[i];
     if(!(*offs)--) ret=0;
     }
   else
     {
     P=Last->prevPtr;
     if(ret=(P!=NULL))
       {
       *idx=P->Idx-1;
       *offs=P->Item[0];
       }
     }
   }

 LOG("< %d=lineIdx::getlast(%lu,%lu)\n",ret,*idx,*offs);
 return ret;
}

int lineIdx::put(unsigned long idx,unsigned long PosSeek)
{
 int ret;

 LOG("> int lineIdx::put(%lu,%lu)\n",idx,PosSeek);

 if((ret=Locate(idx))!=0) Cur->Item[Cur->Idx-idx-1]=PosSeek+1;

 LOG("< %d=lineIdx::put(%lu,%lu)\n",ret,idx,PosSeek);
 return ret;
}



#ifdef DEBUG

main()
{
 lineIdx A(10000,5000);
 unsigned long l,i=0,k;

while(!kbhit())
  {
  i+=10;
//  i=rand(); i*=10;
//  if(i>1000000L) i=0;
  k=rand();
  A.put(i,k);
  l=A[i];
  printf("\r %lu\t--> [%lu]\t--> %lu\t",k,i,l);
  if(k!=l) { printf("wait ..."); getch(); }
  }

}

#endif
