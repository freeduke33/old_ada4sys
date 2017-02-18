// АР-СС
// с учетом тренда 1-го порядка
// с тек. сглаживанием
// сдвиг q


#define Uses_arss_q1TS270392
#define Uses_Buffer
#include <adaptive.hpp>

#define USE_FAST

#define A_x B_x
#define A_y B_y
#define C Betta
#define Lam C

#define model arss_q1TS270392
#define DMAX DMAX_270392
#define M(a) (((a)+DMAX)%DMAX)

model::model(unsigned int _p,unsigned int _Q,unsigned int _q,
             unsigned int T): arss_qTS270392(_p,_Q,_q,T)
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

 // ---------------- (107)
 arss_qTS270392::params1();

 for(i=0;i<=p;i++)
   {
   A_xx[i][0]-=(3.0*BB_x[i]*BB_x[0])/(Q*(Q*Q-1));
   A_yy[i][0]-=(3.0*BB_y[i]*BB_y[0])/(Q*(Q*Q-1));
   A_xy[0][i]-=(3.0*BB_x[0]*BB_y[i])/(Q*(Q*Q-1));

   if(i==0) continue;

   A_xy[i][0]-=(3*BB_x[i]*BB_y[0])/(Q*(Q*Q-1));
   }
 return;
}

float _HUGE model::_y_(int m)
{
 return m>l ? Tpl->get(m-l) : Y[M(m)];
}

float _HUGE model::calc(void)
{
 float su,su1,su2,su3,su4,su5,A;
 int v;

 // ---------------- (108a)
   for(su4=0,v=1;v<=p;v++) su4+=Lam[v]*X[M(l+1-v)];
   for(su5=v=0;v<=p;v++)   su5+=C[v]*_y_(l+q+1-v);

 if(ExtTrend) A=ExtTrend->Calc(l);
 else
   {
 // ---------------- (108b)
   for(su=su2=0,v=1;v<=p;v++)
     {
     su +=Lam[v]*A_x[v];
     su2+=Lam[v]*BB_x[v];
     }
   for(su1=su3=v=0;v<=p;v++)
     {
     su1+=C[v]*A_y[v];
     su3+=C[v]*BB_y[v];
     }
   A=(A_x[0]-su-su1)/Q+(3.0*(Q+1+2*q)*(BB_x[0]-su2-su3))/(Q*(Q*Q-1));
   }

 return Result(A+su4+su5);
}

