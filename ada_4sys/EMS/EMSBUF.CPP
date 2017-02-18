#include <dos.h>
#include "EMS\\emsbuf.hpp"

#define EMSPAGESIZE (16*1024)

EMSMemBuf *EMSMemBuf::Top;

EMSMemBuf::EMSMemBuf(unsigned long size,int pg,int _ndx,int _ndx_count)
{
 isEMS=EMSstat();

 List=Top;
 Top=this;

 ndx=_ndx;
 ndx_count=_ndx_count;
 HdwPage=pg;
 Hdl=0;
 Hdl=SearchEMSHandle();

 if(isEMS)
   {
   MapedBuf=(float *)MK_FP(MemEseg,pg*EMSPAGESIZE);
   setSize(size);
   }
 return;
}

_HdlT EMSMemBuf::SearchEMSHandle(void)
{
 EMSMemBuf *p=Top;

// while(p)
   {
   ;
   }
 return 0;
}

EMSMemBuf::~EMSMemBuf(void)
{
 if(isEMS && Hdl>0) EMSfree(Hdl);
}

void EMSMemBuf::reset(void)
{
 int e;

 cur=0;
 curm=begm;
 LclPage=0;
 if(Hdl) EMSmap(Hdl,HdwPage,LclPage);
 e=MemEerr;
 return;
}


unsigned long EMSMemBuf::getSize(void)
{
 if(Hdl) return len;
 else return 0;
}

int EMSMemBuf::setSize(unsigned long size)
{
 int e;
 unsigned all=EMSquery();

  if(isEMS)
   {
   if(Hdl>0 && isFirst) { EMSfree(Hdl); Hdl=0; }
   Pages=size*sizeof(float)/EMSPAGESIZE;
   if(Pages*EMSPAGESIZE<size*sizeof(float)) Pages++;
   Pages= all>Pages ? Pages : all;

   if(!Hdl) Hdl=EMSalloc(Pages);
   e=MemEerr;
   if(Hdl) len=EMSPAGESIZE/sizeof(float);
   else    len=0;
   len= len>size ? size : len;
   reset();
   }
 return 0;
}



void huge EMSMemBuf::putNext(float val)
{
 if(len)
   {
   MapedBuf[cur]=val;
   CheckNextPage();
   }
 return;
}

float huge EMSMemBuf::getNext(void)
{

 if(len)
   {
   if(curm) curm--;
   else
     {
     CheckNextPage();
     return MapedBuf[cur];
     }
   }
 return 0;
}

float huge EMSMemBuf::get(signed long idx)
{
 return operator[](LclPage*EMSPAGESIZE+cur+idx);
}

float huge EMSMemBuf::operator[](unsigned long idx)
{
 unsigned long p=idx/EMSPAGESIZE,i=idx%EMSPAGESIZE;
 float val=0;
 int e;

 if(len)
   {
   if(p!=LclPage) EMSmap(Hdl,HdwPage,p);
   e=MemEerr;
   val=MapedBuf[i];
   if(p!=LclPage) EMSmap(Hdl,HdwPage,LclPage);
   e=MemEerr;
   }
 return val;
}

void EMSMemBuf::CheckNextPage(void)
{
 int e;

 cur++;
 if(cur==EMSPAGESIZE/sizeof(float) && Hdl)
     {
     LclPage=(LclPage+1)%Pages;
     EMSmap(Hdl,HdwPage,LclPage);
     e=MemEerr;
     cur=0;
     }
}
