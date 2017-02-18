#include <string.h>
#include <math.h>
// #include <stdio.h>

#define Uses_Buffer
#define Uses_gen_vBuffer
#define Uses_Defence
#include "adaptive.hpp"

#define mdl gen_vBuffer

#define cvt(a) ((((float)(a)*2)/MaxVol)-1.0)
#define min(a,b) ((a)>(b) ? (b) : (a))

mdl::mdl(Word mv)
{
 need=EOB=0; act=0;
 bufs=0; next=0;
 MaxVol=mv;
 Tik=0;


 buf[0]=buf[1]=NULL;


 return;
}

mdl::~mdl()
{
 int i;

 for(i=0;i<bufs;i++) if(buf[i]) delete buf[i];
}


DWord mdl::NeedMemory()
{
  return (need=calcBufSize())*sizeof(Word);
}

DWord mdl::Allocated() { return 0; }

int mdl::CreateBuffer(DWord ms)
{
 int a,ret=0;

 if(bufs>0) return 0;

 if(ms)
   {
   EOB=(ms/(2*sizeof(Word)*16))*16;
   bufs=2;
   }
 else
   {
   EOB=need;
   bufs=1;
   }
 for(a=0;a<bufs;a++)
   {
   buf[a]=new Word[EOB];
   if(buf[a]) fillBuf(a);
   else ret=-1;
   }

 return ret;
}

int mdl::isSuccessful(char *mbuf)
{
  int val=1,i;

  for(i=0;i<bufs;i++) if(!buf[i]) { val=0; /*strcpy(mbuf,"no free memory");*/ }
  return val;
}

float _HUGE mdl::Calc(float v)
{
 float val,yt=0;

  if(Tpl) yt=Tpl->getNext();

  val=Verify(cvt(buf[act][Tik=(++Tik)%EOB]),v,yt);
  if(!Tik) act=(++act)%bufs;

  return val;
}

void mdl::Build()
{
 if(bufs>1 && next!=act)
   {
   fillBuf(next);
   next=(++next)%bufs;
   }

 return;
}

