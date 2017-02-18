#include <trace.hpp>
#include <link.hpp>

LINK_ZVENO(TestBuildModel,"Тест")

#define M(a) (((a)+NMAX)%NMAX)


float _HUGE TestBuildModel::Calc(float val)
{
 int v;
 double su,su1;

 k=M(k+1);

 x[k]=val;
                       //       vvv---------------Lam[0..N-1] <-- 1..N
 for(su=0,v=1;v<=p;v++) su+=Lam[v-1]*x[M(k-v)];
 for(su1=0,v=1;v<=p;v++) su1+=C[v]*y[M(k-v)];

 return y[k]=(x[k]-E-su-su1)/C[0];
}

