#include <conio.h>

#define Uses_dfnFull

#include "adaptive.hpp"



float dfnFull::Verify(float x,float y,float yt)
{
 float val;

 y=y; yt=yt; // disable warning

 if(cnt)
   {
   val=x-2*old[0]+old[1]; val*= val<0 ? -1:1;
   if(val/(delt*delt)>acc)
     {
     if(x>old[0]) x=2.0*old[0]-old[1]+acc*delt*delt;
     else         x=2.0*old[0]-old[1]-acc*delt*delt;
     }
   else
     {
     if(x>old[0]) { if((x-old[0])/delt>speed) x=old[0]+speed*delt; }
     else         { if((old[0]-x)/delt>speed) x=old[0]-speed*delt; }
     }
   }
 else { cnt++; old[0]=x; }

 old[1]=old[0];

 return old[0]=x;
}
