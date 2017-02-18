#include <conio.h>

#define Uses_dfnSpeed

#include "adaptive.hpp"


float dfnSpeed::Verify(float x,float y,float yt)
{
 y=y; yt=yt; // disable warning

 if(cnt)
   {
   if(x>old)
     {
     if((x-old)/delt>speed)
       {
//       cprintf("%f %f -->",x,old);
       x=old+speed*delt;
//       cprintf("%f\n\r",x);
       }
     }
   else
     {
     if((old-x)/delt>speed)
       {
//       cprintf("%f %f ==>",x,old);
       x=old-speed*delt;
//       cprintf("%f\n\r",x);
       }
     }
   }
 else cnt++;

 return old=x;
}
