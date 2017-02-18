
//
// 13-14 декабря 1996 "Управление с использованием тренда"
// построение тренда в виде ортогональных полиномов
//


#define Uses_etr_PL141296
#define Uses_TemplateARSS_1x1

#include <adaptive.hpp>

#define DMAX DMAX_ARSS
#define mdl etr_PL141296
#define M(i) (((i)+DMAX+DMAX)%DMAX)

mdl::mdl(int _T,int _Q,int _q)
{
 int i;
 double A0,A1,A2;

 T=_T>2 ?2:_T; Q=_Q; q=_q;

 for(i=0;i<DMAX;i++) fi[i]=0;
 k=0;

 // ------------- (11)
 switch(T)
   {
   case 0: A0=1.0/Q; A1=0;A2=0;
           break;
   case 1: A0=(6.0*q+4.0*Q+4.0)/(Q*(Q+1.0));
           A1=6.0*(2.0*q+Q+1.0)/((Q-1.0)*Q*(Q+1.0));
           A2=0;
           break;
   case 2: A0=(30.0*q*q+6.0*q*(6.0*q+7.0)+9.0*(Q+1.0)*(Q+2.0))/(Q*(Q+1.0)*(Q+2.0));
           A1=6.0*(30.0*q*q*(Q-1.0)+2.0*q*(16.0*Q*Q-19.0)+(Q+1.0)*(Q+2.0)*(6.0*Q-7.0))/((Q-2.0)*(Q-1.0)*Q*(Q+1.0)*(Q+2.0));
           A2=30.0*(6.0*q*q+6.0*q*(Q+1.0)+(Q+1.0)*(Q+2.0))/((Q-2.0)*(Q-1.0)*Q*(Q+1.0)*(Q+2.0));
           break;
   }
 for(i=0;i<Q;i++) psi[i]=A0-A1*i+A2*i*i;

 return;
}

float mdl::Calc(int l)
{
 float val,su0,su1;
 int v,i;

 k=(k+1)%DMAX;

#ifndef DETECTIVE
 // ------------- (15)
 for(v=su0=0;v<=Model->p;v++) su0+=Model->C[v]    *Model->X[M(l-v-q)];
 for(v=su1=0;v<=Model->p;v++) su1+=Model->Betta[v]*Model->Y[M(l-v)];

 fi[k]=su0-su1;
#endif

 // ------------- (16)
 for(i=val=0;i<Q;i++) val+=fi[M(k-1-i)]*psi[i];

 return val;
}

#ifdef DETECTIVE

#include <stdio.h>

class tst: public mdl
{
 public:

   tst(int _T,int _Q,int _q):mdl(_T,_Q,_q) {;}

   void doit(void);
};

void tst::doit(void)
{
 int i;
 float v;

 fi[0]=1;
 for(i=1;i<Q+T+10;i++) fi[i]=(T+i)*fi[i-1]/i;
 for(i=0;i<Q+T+10;i++)
   {
   v=Calc(0);
   printf("%f\t == %f\n",fi[i],v);
   }
}

int main()
{
 static tst t(2,10,0);

 t.doit();
 return 0;
}

#endif
