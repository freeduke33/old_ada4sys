//
// 13-14 декабря 1996 "Управление с использованием тренда"
// построение тренда по критерию совпадения
//

#define Uses_etr_EQ141296
#define Uses_TemplateARSS_1x1

#include <adaptive.hpp>

#define DMAX DMAX_ARSS
#define mdl etr_EQ141296
#define M(i) (((i)+DMAX+DMAX)%DMAX)

mdl::mdl(int _T,int _Q,int _q)
{
 int i;

 T=_T; Q=_Q; q=_q;

 for(i=0;i<DMAX;i++) fi[i]=0;
 k=0;

 return;
}

float mdl::Calc(int l)
{
 float val,su0,su1;
 int v,i;

 k=(k+1)%DMAX;

#ifndef DETECTIVE

 // ------------- (29)
 for(v=su0=0;v<=Model->p;v++) su0+=Model->C[v]    *Model->X[M(l-v-q)];
 for(v=su1=0;v<=Model->p;v++) su1+=Model->Betta[v]*Model->Y[M(l-v)];
 fi[k]=su0-su1;

#endif

 // ------------- (27)
 val=0;
 switch(T)
   {
   case 2: val =(q+2.0)*(q+1)*(fi[M(k-1)]-2.0*fi[M(k-2)]+fi[M(k-3)])/2.0;
   case 1: val+=(q+1.0)*(fi[M(k-1)]-fi[M(k-2)]);
   case 0: val+=fi[M(k-1)];
   }


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

