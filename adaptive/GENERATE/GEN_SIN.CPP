#include <math.h>

#define Uses_Buffer
#define Uses_gen_Sinus
#define Uses_Defence
#include <adaptive.hpp>


#define mdl gen_Sinus

mdl::mdl(float a,float f,float z,float d,float ad)
{
 freq=f; ampl=a; faza=z; delt=d; add=ad;
 cnt=0;
 return;
}

float _HUGE mdl::Calc(float v)
{
 float val,yt=0;

 if(Tpl) yt=Tpl->getNext();

 val=add+ampl*sin(2.0*M_PI*freq*delt*(cnt++)+faza);

 return Verify(val,v,yt);
}

