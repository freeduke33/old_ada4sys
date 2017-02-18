//
// ----------------[ Колебательное звено по ПЕРЕМЕЩЕНИЮ ]-------------
//

#include <math.h>
#include <Oscill.hpp>
#include <Link.hpp>

#if !defined(M_PI)
#define M_PI 3.1415
#endif

LINK_ZVENO(TransferOscill,"Колебательное")

float _HUGE TransferOscill::Calc(float val )
{
 float wy1=val;

  X[0]=val;
  switch(tik)
   {
   case 0:  wy1=0; tik++; break;
   case 1:  wy1=C[0]*X[0]; tik++; break;
   default: wy1=Lam[0]*Y[0]-Lam[1]*Y[1]+C[0]*X[0]-C[1]*X[1]; break;
   }
  Y[1]=Y[0]; Y[0]=wy1;
  X[2]=X[1]; X[1]=X[0];
 return wy1;
}

int _HUGE TransferOscill::Build(void)
{
 int Ret=1;
 float D,Psi[3];

 if(Betta!=1)
   {
   if(Betta>1)
     {
     D=delt*2.0*M_PI*Freq*sqrt(Betta*Betta-1);
     D=delt*2.0*M_PI*Freq*sqrt(Betta*Betta-1);
     Psi[0]=(exp(D)-exp(-D))/(2.0*sqrt(Betta*Betta-1));
     Psi[1]=exp(D)+exp(-D);
     }
   else
     {
     D=delt*2.0*M_PI*Freq*sqrt(1-Betta*Betta);
     Psi[0]=sin(D)/sqrt(1-Betta*Betta);
     Psi[1]=2.0*cos(D);
     }
   D=exp(-delt*2.0*M_PI*Freq*Betta);

   Lam[0]=D*Psi[1]; C[0]=(2.0-Lam[0])/2.0-Betta*D*Psi[0];
   Lam[1]=D*D;      C[1]=(Lam[0]-2.0*Lam[1])/2.0-Betta*D*Psi[0];
   }
 else Ret=0; /* Ошибка !!! Betta==1 */
 return Ret;
}
