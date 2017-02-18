#include <math.h>

#define Uses_Buffer
#define Uses_gen_Template
#define Uses_Defence
#include <adaptive.hpp>


#define mdl gen_Template

mdl::mdl(float _k)
{
 k=_k;
 Reset();
 return;
}

void mdl::Reset(void)
{
 if(Tpl) Tpl->reset();
 return;
}

float _HUGE mdl::Calc(float v)
{
 float val,yt=0.5;

 if(Tpl) yt=Tpl->getNext();

 val=k*yt;

 return Verify(val,v,yt);
}

