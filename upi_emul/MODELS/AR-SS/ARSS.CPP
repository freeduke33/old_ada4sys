#include <ArSS.hpp>

#define M(a) (((a)+100)%100)

ArSS::ArSS(void)
{
 int i;

 for(i=0;i<100;i++) X[i]=Y[i]=0;
 m=0;
 return;
}

float _HUGE ArSS::Calc(float wy1)
{
 int v;
 float su,su1;

 X[m]=wy1;
 for(v=0,su=0 ;v<=Def.p;v++) su+=Def.Lam[v]*X[M(m-Def.q-v)];
 for(v=1,su1=0;v<=Def.p;v++) su1+=Def.C[v]*Y[M(m-v)];
 return Y[m++]=(su+su1)/Def.C[0];
}
