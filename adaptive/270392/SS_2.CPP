// СС
// с учетом тренда 1-го порядка
// с тек. сглаживанием
// сдвиг q

#define Uses_ss_q1TS270392
#define Uses_Buffer
#define Uses_Defence
#define Uses_CtrlError
#include <adaptive.hpp>


// #define USE_FAST

#define model ss_q1TS270392
#define DMAX DMAX_270392
#define M(a) (((a)+DMAX)%DMAX)

#define A_x B_x
#define A_y B_y
#define C Betta
#define Lam C

model::model(unsigned int _p,unsigned int _Q,unsigned int _q,
             unsigned int T): ss_qTS270392(_p,_Q,_q,T)
{
 int i;
 for(i=0;i<DMAX;i++) BB_x[i]=BB_y[i]=0;

 return;
}

void _HUGE model::params1(void)
{
 int i;

 for(i=p;i>0;i--)
   {
   BB_x[i]=BB_x[i-1];
   BB_y[i]=BB_y[i-1];
   }
#ifndef USE_FAST
 int j;
 double su;

 // ---------------- (100)
  for(su=0,j=0;j<Q;j++) su+=(Q-1-2*j)*X[M(l-j-q)]; BB_x[0]=su;
  for(su=0,j=0;j<Q;j++) su+=(Q-1-2*j)*Y[M(l-j  )]; BB_y[0]=su;
#else
 // ---------------- (105)
 BB_x[0]=BB_x[1]-A_x[0]-A_x[1]+Q*(X[M(l-q)]+X[M(l-q-Q)]);
 // ---------------- (106)                                   ????????? стр.34
 BB_y[0]=BB_y[1]-A_y[0]-A_y[1]+Q*(Y[l]+Y[M(l-Q)]);
#endif

 // ---------------- (116)
 ss_qTS270392::params1();

 for(i=0;i<=p;i++)
   {
   A_yy[i][0]-=(3.0*BB_y[i]*BB_y[0])/(Q*(Q*Q-1));
   A_xy[0][i]-=(3.0*BB_x[0]*BB_y[i])/(Q*(Q*Q-1));
   }

 return;
}

float _HUGE model::_y_(int m)
{
 return m>l ? Tpl->get(m-l) : Y[M(m)];
}

float _HUGE model::calc(void)
{
 float su1,su2,su3,A;
 int v;

 // ---------------- (118a)
 for(su3=v=0;v<=p;v++) su3+=C[v]*_y_(l+1+q-v);

 if(ExtTrend) A=ExtTrend->Calc(l);
 else
  {
 // ---------------- (118b)
  for(su1=su2=v=0;v<=p;v++)
     {
     su1+=C[v]*A_y[v];
     su2+=C[v]*BB_y[v];
     }
  A=(A_x[0]-su1)/Q+(3.0*(Q+1+2*q)*(BB_x[0]-su2))/(Q*(Q*Q-1));
  }

 return Result(A+su3);
}


