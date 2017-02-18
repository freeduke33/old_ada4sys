#include <stdio.h>
#include <conio.h>

#define Uses_mdl_TR270395
#define Uses_Buffer

#include <adaptive.hpp>

#define PMAX PMAX_270395
#define QMAX QMAX_270395
#define DMAX DMAX_270395

#define model mdl_TR270395

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#define M(a) (((a)+DMAX)%DMAX)
#define CC(N,v) (s[(v)%2]*(C=( ((N)-(v)+1)*C/(v) ) ))
#define declare_CC const int s[]={1,-1}

model::model(unsigned int _t,unsigned int _p,unsigned int _q,
             unsigned int _Q,unsigned int _N):
                mdl_270395(_t,_p,_q,_Q)
{
 N=_N%QMAX;
 PS+=N;

 return;
}


void model::params(void)
{
 double su;
 int v,n,C;
 declare_CC;

 su=(C=1)*Y[l];
    for(v=1;v<=N+1;v++) su+=CC(N+1,v)*Y[M(l-v)];
 dy[l]=su;

 su=(C=1)*X[l]; n=N+Typ;
    for(v=1;v<=n+1;v++) su+=CC(n+1,v)*X[M(l-v)];
 dx[l]=su;


 return;
}

inline float model::_y_(int ll)
{
 return ll>=l ? Tpl->get(ll-l) : Y[M(ll)];
}

inline double model::calc_su(int v)
{
 double val;
 int k,C;
 declare_CC;

 val=(C=1)*_y_(v);
 for(k=1;k<=N+1;k++) val+=CC(N+1,k)*_y_(v-k);

 return val;
}

inline double model::calc_ddy(int m)
{
 double val;
 int v;

 for(v=0,val=0;v<=p;v++)
    val+=Betta[v]*calc_su(m+q-v);
 return val;
}

inline double model::calc_ddx(int m,double *ddx)
{
 int C,v;
 double su;
 declare_CC;

 for(v=C=1,su=0;v<=N+1;v++) su+=CC(N+1,v)*ddx[M(m-v)];

 return -su+calc_ddy(m);
}

float model::calc(void)
{
 double val,su,dd[DMAX];
 int v,C;
 declare_CC;


   switch(Typ)
     {
     case 2:
       for(v=Q-3+N+1;v>Q-3;v--) dd[M(l-v)]=0; // ?????????????
       for(;v>=0;v--)           dd[M(l-v)]=calc_ddx(l-v,dd);
       for(v=0,su=0;v<=Q-3;v++)
          su+=(v+1)*(Q-1-v)*(Q-2-v)*dd[M(l-v)]/(Q*(Q+1));
       val=su;
       break;
     case 1:
       for(v=Q-2+N+1;v>Q-2;v--) dd[M(l-v)]=0; // ?????????????
       for(;v>=0;v--)           dd[M(l-v)]=calc_ddx(l-v,dd);
       for(v=0,su=0;v<=Q-2;v++) su+=(Q-1-v)*dd[M(l-v)]/Q;
       val=su;
       break;
     case 0:
       for(v=C=1,su=0;v<=N+1;v++) su+=CC(N+1,v)*X[M(l-v)];
       val=-su+calc_ddy(l);
       break;
     }
 return val;
}


