//
// ----------------[ Колебательное звено по УСКОРЕНИЮ ]-------------
//  АНАЛОГОВОЕ

#include <math.h>
#include <Oscill.hpp>
#include <link.hpp>

LINK_ZVENO(DSpeedOscillA,"Колеб.(ускорение)")


float _HUGE DSpeedOscillA::Calc(float val)
{
 float wy1;

  X[0]=val;
  switch(tik)
    {
    case 0:  wy1=0; tik++; break;
    case 1:  wy1=C[0]*wy1; tik++; break;
    default: wy1=Lam[0]*Y[0]-Lam[1]*Y[1]+C[0]*(X[0]-2.0*X[1]+X[2]); break;
    }
  Y[1]=Y[0]; Y[0]=wy1;
  X[2]=X[1]; X[1]=X[0];
 return wy1;
}

int _HUGE DSpeedOscillA::Build(void)
{
 float D,D1;

//  D=2.0*M_PI*Freq*delt;
  D=Freq*delt;
  D1=D*sqrt(1.0-(Betta*Betta));
  D*=Betta;
  C[0]=exp(-2.0*D);
  Lam[0]=2.0*exp(-D)*cos(D1);
  Lam[1]=exp(-2.0*D);
 return 1;
}
