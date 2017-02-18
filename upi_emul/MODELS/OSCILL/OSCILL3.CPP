//
// ----------------[ Колебательное звено по СКОРОСТИ ]-------------
//  АНАЛОГОВОЕ

#include <math.h>
#include <Oscill.hpp>
#include <link.hpp>

LINK_ZVENO(SpeedOscillA,"Колеб.(скорость)")


int _HUGE SpeedOscillA::Build(void)
{
 float D,D1;

//   D=2.0*M_PI*Freq*delt;
   D=Freq*delt;
   D1=sqrt(1.0-(Betta*Betta));
   C[0]=exp(-D*Betta)*sin(D*D1)/D1;
   C[1]=C[0];
   Lam[0]=2.0*exp(-Betta*D)*cos(D*D1);
   Lam[1]=exp(-2.0*D*Betta);
 return 1;
}

