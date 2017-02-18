#define Uses_MemBufPSinus
#define Uses_gen_PSinus

#include <adaptive.hpp>


MemBufPSinus::MemBufPSinus(unsigned long _nr,float fdisk,float *a,float *f,float *fi,float ad)

{
 nr=_nr;

 _nr = _nr>100 ? 100 : _nr;
 gen=new gen_PSinus(a,f,fi,1/fdisk,ad);

 MemBuf::setSize(_nr);
 reset();
 return;
}

MemBufPSinus::~MemBufPSinus(void)
{
 if(gen) delete gen;
 return;
}


int MemBufPSinus::setSize(unsigned long p)
{
 return p==p;
}


void _HUGE MemBufPSinus::putNext(float val)
{
 val=val;
 return;
}

float _HUGE MemBufPSinus::getNext(void)
{
 cnt = (cnt+1)%len;
 buf[cnt]=gen->Calc(0);
 return MemBuf::getNext();
}


unsigned long MemBufPSinus::getSize(void)
{
 return nr;
}


void MemBufPSinus::reset(void)
{
 MemBuf::reset();
 if(gen)
   {
   gen->Reset();
   for(cnt=0;cnt<50;cnt++) buf[cnt]=gen->Calc(0);
   }
 return;
}