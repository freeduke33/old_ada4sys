#include <math.h>

#define Uses_Buffer
#define Uses_gen_PSinus
#define Uses_Defence
#include <adaptive.hpp>


#define mdl gen_PSinus

mdl::mdl(float a[],float f[],float z[],float d,float ad)
{
 int i;

 for(i=0;i<3;i++)  { freq[i]=f[i]; ampl[i]=a[i]; faza[i]=z[i]; }
 delt=d;
 add=ad;

 cnt=0;
 return;
}

float _HUGE mdl::Calc(float v)
{
 float val,yt=0;

 if(Tpl) yt=Tpl->getNext();

 val=add+ampl[0]*sin(2.0*M_PI*freq[0]*delt*cnt+faza[0])+
     ampl[1]*sin(2.0*M_PI*freq[1]*delt*cnt+faza[1])+
     ampl[2]*sin(2.0*M_PI*freq[2]*delt*cnt+faza[2]);

 cnt++;

 return Verify(val,v,yt);
}

