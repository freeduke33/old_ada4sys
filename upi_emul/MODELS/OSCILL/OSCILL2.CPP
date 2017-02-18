//
// ----------------[ Колебательное звено по ПЕРЕМЕЩЕНИЮ ]-------------
//  АНАЛОГОВОЕ

#include <math.h>
#include <Oscill.hpp>
#include <link.hpp>

LINK_ZVENO(TransferOscillA,"Колеб.(перемещение)")

int _HUGE TransferOscillA::Build(void)
{
 float D,D1;

//   D=2.0*M_PI*Freq*delt;
   D=Freq*delt;
   D1=sqrt(1.0-(Betta*Betta));
   C[0]=1.0-exp(-D*Betta)*cos(D*D1)-((Betta*exp(-Betta*D)*sin(D*D1))/D1);
   C[1]=exp(-D*Betta)*cos(D*D1)-exp(-2.0*D*Betta)-((Betta*exp(-D*Betta)*sin(D*D1))/D1);
   Lam[0]=2.0*exp(-Betta*D)*cos(D*D1);
   Lam[1]=exp(-2.0*Betta*D);
 return 1;
}

