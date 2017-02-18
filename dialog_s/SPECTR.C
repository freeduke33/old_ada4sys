#include <math.h>
#include "spectr.h"

int CalcSpectr(float fn,float fw,int nq,double *S,double *Axis,
               float freq,float lam,long nr,float *X)
{
 double s,f,delt=1/freq,Y[3],psi;
 long i;
 int k;

 for(i=0,s=0;i<nr;i++) s+=X[i]; s/=nr;
 for(k=0;k<nq;k++)
   {
   f=(fn+(fw-fn)*k)/(nq-1); if(Axis) Axis[k]=f;
   psi=2*M_PI*delt*f;
   Y[0]=Y[1]=0;
   for(i=0,S[k]=0;i<nr;i++)
     {
     Y[2]=Y[1]; Y[1]=Y[0];
     Y[0]=2.0*lam*Y[1]*cos(psi)-lam*lam*Y[2]+(X[i]-s)-lam*lam*(i>1 ? (X[i-2]-s):0);
     S[k]+=(X[i]-s)*Y[0];
     }
   S[k]=(S[k]*delt)/(2*M_PI*nr);
   }
  return 0;
}
