#include <math.h>
#include <function.hpp>
#include <link.hpp>

LINK_ZVENO(DSpeedFunction,"Ускорение")



float _HUGE DSpeedFunction::Calc(float val)
{
 X[2]=X[1]; X[1]=X[0];
 X[0]=val;
 return k*(X[0]+2*X[1]-X[2])/delt;
}
