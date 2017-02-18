#include <math.h>

#define Uses_mst_MathWait
#include <adaptive.hpp>


#define mdl mst_MathWait


mdl::mdl(void)
{
 val=0; cnt=1;
}

void mdl::Reset(void)
{
 Mistake_1x1::Reset();
 val=0;
}

float mdl::Value(void)
{
 return val;
}

void mdl::Use(float x,float y,float yt)
{
 if(isOn())
   {
   if(!cnt%65000) cnt=1;
   val-=(val-fabs(y-yt)/2.0)/(float)(cnt++);
   }
 else Wait(); // disable warning

 return;
}

