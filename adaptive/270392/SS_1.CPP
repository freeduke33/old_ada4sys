// СС
// с учетом тренда 0-го порядка
// с тек. сглаживанием
// сдвиг q

#define DEBUG
//#define OPTIMIZE_P

#define Uses_Buffer
#define Uses_ss_qTS270392

#include <adaptive.hpp>

#define DMAX DMAX_270392
#define PMAX PMAX_270392
#define QMAX QMAX_270392
#define model ss_qTS270392
#define NU 0 // 1e-30
#define MARGIN (PS+10)

#define M(i) (((i)+DMAX)%DMAX)

#define C Betta
#define Lam C

model::model(unsigned int p,unsigned int Q,unsigned int q,
             unsigned int t) : arss_qTS270392(p,Q,q,t)
{
 return;
}

void _HUGE model::params1(void)
{
 int i,v;

//
// ------------------------- (28)
//
// for(i=1;i<=p;i++) for(v=1;v<=i;v++)
 for(i=p;i>0;i--) for(v=p;v>0;v--)
   {
   A_yy[i][v]=A_yy[i-1][v-1];
   A_xy[i][v]=A_xy[i-1][v-1];
   }


 for(i=0;i<=p;i++)
   {
   G_yy0[i]+=Y[M(l-i)]*Y[l]-Y[M(l-Q-i)]*Y[M(l-Q)];
   A_yy[i][0]=G_yy0[i]-B_y[i]*B_y[0]/Q;

   G_0xy[i]+=X[M(l-q)]*Y[M(l-i)]-X[M(l-Q-q)]*Y[M(l-Q-i)];
   A_xy[0][i]=G_0xy[i]-B_x[0]*B_y[i]/Q;
   }

 return;
}

void _HUGE model::xbuild(void)
{
 float su;
 int v,q,j,P=p;


 if(cnt<MARGIN) return;

//
// ---------------------- (14)
//

 for(v=0;v<=P;v++) for(q=0;q<=v;q++)
    {
    for(j=su=0;j<q;j++) su+=cc[q][j]*fi[v][j];
    fi[v][q]=A_yy[v][q]-su;
#ifdef DEBUG
 if(LOG)  LOG->put('X',"\n%g fi[%d][%d]= A_yy[%d][%d]- ...",fi[v][q],v,q,v,q);
#endif

    if(v<1 || q>v) continue;
    cc[v][q]=fi[v][q]/(fi[q][q]+NU);

#ifdef OPTIMIZE_P
    if(v==q && cc[v][q]==1) P=v-1;
#endif

#ifdef DEBUG
 if(LOG)  LOG->put('X',"\n%g cc[%d][%d]= ... /fi[%d][%d]",cc[v][q],v,q,q,q);
#endif

    }

 for(v=0;v<=P;v++)
    {
    for(j=su=0;j<v;j++) su+=fi[v][j]*d[j];
    d[j]=(A_xy[0][v]-su)/(fi[v][v]+NU);
    }

 for(v=P;v>=0;v--)
    {
    for(j=v+1,su=0;j<=P;j++) su+=cc[j][v]*C[j];
    C[v]=d[v]-su;
    }

 if(ExtTrend) A=ExtTrend->Calc(l);
 else
   {
//
// --------------------- (52)
//
   for(v=su=0;v<=P;v++) su+=C[v]*B_y[v];
   A=(B_x[0]-su)/Q;
   }

#ifdef DEBUG
 if(LOG)   LOG->put('?'," A=%8.5f ",A);

 if(LOG)   LOG->put('?'," C[]= ");
  for(v=0;v<=p;v++)  if(LOG) LOG->put('?',"%8.5f ",C[v]);
#endif


 okBld=1;
 return;
}

float _HUGE model::calc(void)
{
 float su,val;
 int v;

//
// ------------------------- (58)
//

 for(v=su=0;v<=q;v++) su+=C[v]*Tpl->get(1+q-v);
 for(v=q+1;v<=p;v++)  su+=C[v]*Y[M(l+1+q-v)];

 return Result(A+su);
}
