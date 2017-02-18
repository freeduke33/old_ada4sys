#include <conio.h>

#define Uses_dfnAcceleration

#include "adaptive.hpp"

float dfnAcceleration::Verify(float x,float y,float yt)
{
 float val;

 y=y; yt=yt; // disable warning


 if(cnt)
   {
   val=x-2.0*old[0]+old[1]; val*= val<0 ? -1:1;
   if(val/(delt*delt)>acc)
     {
     if(x>old[0]) x=2.0*old[0]-old[1]+acc*delt*delt;
     else         x=2.0*old[0]-old[1]-acc*delt*delt;
     }
   }
 else { cnt++; old[0]=x; }

 old[1]=old[0];

 return old[0]=x;
}
