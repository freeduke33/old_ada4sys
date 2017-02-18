#include <math.h>

#define Uses_Buffer
#define Uses_gen_StaticBuf
#define Uses_Defence
#include <adaptive.hpp>


#define mdl gen_StaticBuf

mdl::mdl(long nr,float *buf)
{
 len=nr;
 Buf=buf;
 Reset();
 return;
}

float _HUGE mdl::Calc(float v)
{
 float yt=0;

 if(Tpl) yt=Tpl->getNext();

 return Verify(Buf[cnt=((cnt+1)%len)],v,yt);
}

