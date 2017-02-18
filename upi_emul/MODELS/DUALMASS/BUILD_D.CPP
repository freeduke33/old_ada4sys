#include <math.h>

#include <dualmass.hpp>


#define Betta1 Betta[0]
#define Betta2 Betta[1]
#define Freq1  Freq[0]
#define Freq2  Freq[1]

#if !defined(M_PI)
#define M_PI 3.1415
#endif


int _HUGE DualMass::Build(void)
{
 double w1,w2,A,A1,A2,A3,CC,J[4],D0,D1,Gam1,Gam2;
 int j;


  w1=2.0*M_PI*Freq1; w2=2.0*M_PI*Freq2;
  A=4.0*w1*w1*w2*w2*(Betta1*Betta1+Betta2*Betta2)-
    4.0*Betta1*Betta2*w1*w2*(w1*w1+w2*w2)+
    (w1*w1-w2*w2)*(w1*w1-w2*w2);
  A1=(w1*w1)*(4.0*Betta1*Betta2*w1*w2-4*Betta1*Betta1*w2*w2+
    w2*w2-w1*w1)/A;
  A2=2.0*w1*(4.0*Betta1*Betta2*Betta2*w1*w1*w2-
    4.0*Betta1*Betta1*Betta2*w1*w2*w2+Betta1*w2*w2*w2-
    Betta2*w1*w1*w1)/A;
  CC=2.0*Betta1-A2*w2/w1;

  if(Betta1>1)
    {
    J[2]=sqrt(Betta1*Betta1-1.0);
    J[0]=exp(-(delt*w1)/(Betta1+J[2]));
    J[1]=exp(-delt*w1*(Betta1+J[2]));

    Lam[0]=J[0]+J[1];
    Gam1=(J[0]-J[1])/(2.0*J[2]);
    }
  else
    {
    J[0]=sqrt(1-Betta1*Betta1);
    J[2]=delt*w1*J[0];
    J[3]=exp(-delt*Betta1*w1);

    Lam[0]=2.0*J[3]*cos(J[2]);
    Gam1=J[3]*sin(J[2])/J[0];
    }

  D0=(CC+A1*Betta1)*Gam1;
  Lam[1]=exp(-2.0*delt*Betta1*w1);


  if(Betta2>1)
    {
    J[2]=Betta2+sqrt(Betta2*Betta2-1.0);
    J[0]=exp(-(delt*w2)/J[2]);
    J[1]=exp(-delt*w2*J[2]);

    Et[0]=J[0]+J[1];
    Gam2=J[0]-J[1];
    }
  else
    {
    J[0]=sqrt(1-Betta2*Betta2);
    J[2]=delt*w2*J[0];
    J[3]=exp(-delt*Betta2*w2);

    Et[0]=2.0*J[3]*cos(J[2]);
    Gam2=delt*J[3]*sin(J[2])/J[0];
    }
 D1=(A2-A1*Betta2)*Gam2;
 Et[1]=exp(-2.0*delt*Betta2*w2);

 E[0]=D0+D1+A1*(Lam[0]-Et[0])/2.0;
 E[1]=D0*Et[0]+D1*Lam[0]+
            A1*(Lam[1]-Et[1]);
 E[2]=D0*Et[1]+D1*Lam[1]+
            A1*(Et[0]*Lam[1]-Et[1]*Lam[0])/2.0;

 for(j=0;j<3;j++) Psi[j]=0;
 return 0;
};