#include <conio.h>

#define Uses_mst_Square
#include <adaptive.hpp>

const double RMAX=100000;
#define NU 1e-30

mst_Square::mst_Square(void)
{
 Reset();
}

void mst_Square::Reset(void)
{
 Mistake_1x1::Reset();
 val=val1=0;
 return;
}


float mst_Square::Value(void)
{
 return val1>NU ? val/val1 : val;
}

void mst_Square::Use(float x,float y,float yt)
{
 if(isOn())
   {
   val+=(y-yt)*(y-yt);
   val1+=yt*yt;
   if(val>RMAX || val1>RMAX) { val/=1000; val1/=1000; }
   }
 else Wait(); // disable warning

 return;
}

