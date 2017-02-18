#include <malloc.h>

#define Uses_MemBuf
#define Uses_StaticMemBuf
#include <adaptive.hpp>

MemBuf::MemBuf(void)
{
 buf=NULL;
 begm=0;
 lock=0;
 len=0;

 return;
}

MemBuf::~MemBuf(void)
{
 if(buf) delete buf;
 return;
}

int MemBuf::setSize(unsigned long s)
{
 unsigned long i;

 if(!lock)
   {
   if(buf) delete buf; len=cur=cnt=0;
   buf=NULL;

   if(s*sizeof(float)>65000L) s=65000L/sizeof(float);
   buf=new float[s];
   if( !buf ) return RC_NOMEM;
   len=s;
   for(i=0;i<s;buf[i++]=0);
   }
 reset();

 return RC_DONE;
}

int MemBuf::lockSize() { lock=1; return RC_DONE; }
int MemBuf::unlockSize() { lock=0; return RC_DONE; }


unsigned long MemBuf::getSize(void) { return len; }


void MemBuf::reset(void)
{
 cur=0; curm=begm;
 return;
}

void _HUGE MemBuf::putNext(float val)
{
 if(len)
   {
   if(curm) curm--;
   else buf[cur=(cur+1)%len]=val;
   }
 return;
}

float _HUGE MemBuf::getNext(void)
{
 float val=0;

 if(len)
   {
   if(curm) curm--;
   else  return buf[cur=(cur+1)%len];
   }
 return val;
}

float _HUGE MemBuf::get(signed long idx)
{
  if(len) return buf[(cur+((len+idx)%len))%len];
  else return 0;
}

int MemBuf::setMargin(unsigned int m)
{
 if(m>len) return RC_VAL;

 begm=m;
 reset();
 return RC_DONE;
}

float _HUGE MemBuf::operator[](unsigned long idx)
{
 return idx>len ? 0 : buf[idx];
}


// ==============================================================
#define bufMem StaticMemBuf

bufMem::bufMem(unsigned long s,float *mem)
{
 buf=mem;
 siz=s;
 setSize(s);
 return;
}

bufMem::~bufMem(void)
{
 buf=NULL;
 return;
}

int bufMem::setSize(unsigned long s)
{
 if(s>siz) s=siz;

 len=s;
 reset();

 return RC_DONE;
}




