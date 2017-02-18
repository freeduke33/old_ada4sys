#include <math.h>
#include <function.hpp>
#include <link.hpp>

LINK_ZVENO(SpeedFunction,"Скорость")

SpeedFunction::SpeedFunction(void)
{
 X[0]=X[1]=X[2]=0;
 return;
}

float _HUGE SpeedFunction::Calc(float val)
{
 X[1]=X[0];
 X[0]=val;
 return k*((X[0]-X[1])/delt);
}
