// АР-СС
// с учетом тренда 0-го порядка
// с тек. сглаживанием
// сдвиг q


#define DEBUG
// #define USE_TEMPLATE
#define OPTIMIZE_P

#define USE_FAST_BETTA
#define USE_FAST_GAMMA

#define Uses_Buffer
#define Uses_arss_qTS270392

#include <adaptive.hpp>
#include <log.hpp>

#define DMAX DMAX_270392
#define PMAX PMAX_270392
#define QMAX QMAX_270392
#define model arss_qTS270392
#define NU 0 // 1e-30
#define MARGIN (PS+10)

#define C Betta
#define Lam C

//#define P(i) (((i)+PMAX)%PMAX)
#define M(i) (((i)+DMAX+DMAX)%DMAX)

#define max(a,b) ( (a)>(b) ? (a) : (b) )
#define min(a,b) ( (a)<(b) ? (a) : (b) )


model::model(unsigned int _p,unsigned int _Q,unsigned int _q,
             unsigned int T)
{
 int i,j;

 p=min(PMAX,_p); Q=min(QMAX,_Q); q=_q; type=T%3;

 _X[0]=_X[1]=oldX=A=0;

 for(i=0;i<DMAX;i++)
   X[i]=Y[i]=G_xx0[i]=G_yy0[i]=G_xy0[i]=G_0xy[i]=B_x[i]=B_y[i]=0;
 for(i=0;i<PMAX;i++) Lam[i]=C[i]=0;

 for(i=0;i<PMAX;i++) for(j=0;j<PMAX;j++)
    A_xx[i][j]=A_yy[i][j]=A_xy[i][j]=0;

 PS=2*p+1+Q;
 cnt=l=0;

 okBld=0;
 return;
}

void _HUGE model::params1(void)
{
 int i,v;
 double su;

//
// ------------------------- (26)
//


// for(i=1;i<=p;i++) for(v=1;v<=i;v++)
 for(i=p;i>0;i--) for(v=p;v>0;v--)
   {
   A_xy[i][v]=A_xy[i-1][v-1];
//   if(v>i) continue;

   A_xx[i][v]=A_xx[i-1][v-1];
   A_yy[i][v]=A_yy[i-1][v-1];
   }


 for(i=0;i<=p;i++)
   {
#ifdef USE_FAST_GAMMA
   G_xx0[i]+=X[M(l-i-q)]*X[M(l-q)]-X[M(l-Q-i-q)]*X[M(l-Q-q)];
   G_yy0[i]+=Y[M(l-i)  ]*Y[l     ]-Y[M(l-Q-i)  ]*Y[M(l-Q)  ];
   G_0xy[i]+=X[M(l-q)  ]*Y[M(l-i)]-X[M(l-Q-q)  ]*Y[M(l-Q-i)];
#else
   for(su=0,v=l+1-Q;v<=l;v++) su+=X[M(v-i-q)]*X[M(v-q)];  G_xx0[i]=su;
   for(su=0,v=l+1-Q;v<=l;v++) su+=Y[M(v-i)  ]*Y[M(v)  ];  G_yy0[i]=su;
   for(su=0,v=l+1-Q;v<=l;v++) su+=X[M(v-q)  ]*Y[M(v-i)];  G_0xy[i]=su;
#endif
   A_xx[i][0]=G_xx0[i]-(B_x[i]*B_x[0])/Q;
   A_yy[i][0]=G_yy0[i]-(B_y[i]*B_y[0])/Q;
   A_xy[0][i]=G_0xy[i]-(B_x[0]*B_y[i])/Q;

   if(i==0) continue;

#ifdef USE_FAST_GAMMA
   G_xy0[i]+=X[M(l-i-q)]*Y[l]-X[M(l-Q-i-q)]*Y[M(l-Q)];
#else
   for(su=0,v=l+1-Q;v<=l;v++) su+=X[M(v-q-i)]*Y[M(v)]; G_xy0[i]=su;
#endif
   A_xy[i][0]=G_xy0[i]-(B_x[i]*B_y[0])/Q;
   }

#ifdef DEBUG
 if(LOG) { LOG->put('X',"\nA_xx0[]="); for(i=0;i<=p;i++) { LOG->put('X',"\n"); for(v=0;v<=p;v++) LOG->put('X',"%8.5f ",A_xx[i][v]); } }
 if(LOG) { LOG->put('X',"\nA_yy0[]="); for(i=0;i<=p;i++) { LOG->put('X',"\n"); for(v=0;v<=p;v++) LOG->put('X',"%8.5f ",A_yy[i][v]); } }
 if(LOG) { LOG->put('X',"\nA_xy[]= "); for(i=0;i<=p;i++) { LOG->put('X',"\n"); for(v=0;v<=p;v++) LOG->put('X',"%8.5f ",A_xy[i][v]); } }
#endif

 return;
}


void _HUGE model::params(void)
{
 int v;

 for(v=p;v>0;v--)
   {
   B_x[v]=B_x[v-1];
   B_y[v]=B_y[v-1];
   }


#ifndef USE_FAST_BETTA
//
// -------------------------- (21)
//
 double su;
 int j;

 for(su=0,j=l+1-Q;j<=l;j++) su+=X[M(j-q)];  B_x[0]=su;
 for(su=0,j=l+1-Q;j<=l;j++) su+=Y[M(j)];    B_y[0]=su;

#else
//
// -------------------------- (22)
//

 B_x[0]+=X[M(l-q)]-X[M(l-Q-q)];
 B_y[0]+=Y[M(l)]-Y[M(l-Q)];

#endif


#ifdef DEBUG
 if(LOG)  LOG->put('D'," B_x[]=%8.5f ",B_x[0]);
 if(LOG)  LOG->put('D'," B_y[]=%8.5f ",B_y[0]);
#endif


 params1();
 return;
}




void _HUGE model::xbuild(void)
{
 double su,su1;
 int v,j,q,i,P=p;

 if(cnt<MARGIN) return;

//
// ---------------------------- (5)
//

#ifdef DEBUG
 if(LOG) LOG->put('X',"\n");
#endif

 for(v=0;v<=P;v++)  for(q=0;q<=v;q++)
     {
     for(j=su=0;j<q;j++)  su+=cc[q][j]*fi[v][j];
     fi[v][q]=A_yy[v][q]-su;
#ifdef DEBUG
 if(LOG)  LOG->put('X',"\n%g fi[%d][%d]= A_yy[%d][%d]- ...",fi[v][q],v,q,v,q);
#endif

     if(v<1 || q>v) continue;
     cc[v][q]=fi[v][q]/(fi[q][q]+NU);
#ifdef DEBUG
 if(LOG)  LOG->put('X',"\n%g cc[%d][%d]= ... /fi[%d][%d]",cc[v][q],v,q,q,q);
#endif
     }

// ----------------------------------------------------------
 for(v=0;v<=P;v++) for(q=0;q<=P;q++)
     {
     for(j=su=0;j<q;j++) su+=cc[q][j]*dd[j][v];
     dd[q][v]=A_xy[v][q]-su;
#ifdef DEBUG
 if(LOG)  LOG->put('X',"\n%g dd[%d][%d]=A_xy[%d][%d]~%g - %g",dd[q][v],q,v,v,q,A_xy[v][q],su);
#endif
     }

// ----------------------------------------------------------
 for(q=1;q<=P;q++) for(v=1;v<=q;v++)
     {
     for(i=su=0;i<=P;i++)   su+=dd[i][q]*dd[i][v]/(fi[i][i]+NU);
     for(i=1,su1=0;i<v;i++) su1+=cc[v][i]*psi[q][i];
     psi[q][v]=A_xx[q][v]-su-su1;

#ifdef DEBUG
 if(LOG)  LOG->put('X',"\n %g psi[%d][%d]=A_xx[%d][%d] - %g - %g",psi[q][v],q,v,q,v,su,su1);
#endif

#ifdef OPTIMIZE_P
     if(v==q && psi[q][v]==0) { P=v-1; goto E; }
#endif

     if((q<2) || v>q) continue;
     cc[q][v]=psi[q][v]/(psi[v][v]+NU);
#ifdef DEBUG
 if(LOG)  LOG->put('X',"\n %g cc[%d][%d]=psi[%d][%d]/psi[%d][%d]",cc[q][v],q,v,q,v,v,v);
#endif
     }

E:

// ----------------------------------------------------------
  for(q=1;q<=P;q++)
     {
     for(i=su=0;i<=P;i++)   su+=dd[i][q]*dd[i][0]/(fi[i][i]+NU);
     for(i=1,su1=0;i<q;i++)  su1+=psi[q][i]*d[i];
     d[q]=(A_xx[q][0]-su-su1)/(psi[q][q]+NU);
#ifdef DEBUG
 if(LOG) LOG->put('X',"\n %g d[%d] = A_xx[%d][0] - %g - %g / psi[%d][%d]",d[q],q,q,su,su1,q,q);
#endif
     }

  for(v=P;v>=1;v--)
     {
     for(i=v+1,su=0;i<=P;i++) su+=cc[i][v]*Lam[i];
     Lam[v]=d[v]-su;
#ifdef DEBUG
 if(LOG) LOG->put('X',"\n %g Lam[%d] = d[%d]- %g",Lam[v],v,v,su);
#endif
     }

  for(v=P;v>=0;v--)
     {
     for(j=1,su=0;j<=P;j++)    su+=Lam[j]*dd[v][j];
     for(j=v+1,su1=0;j<=P;j++) su1+=fi[j][v]*C[j];
     C[v]=(dd[v][0]-su-su1)/(fi[v][v]+NU);
#ifdef DEBUG
 if(LOG) LOG->put('X',"\n %g C[%d] = dd[%d][0]- ... /fi[%d][%d]",C[v],v,v,v,v);
#endif
     }

//
// -------------------------------- (32)
//

  if(ExtTrend) A=ExtTrend->Calc(l);
  else
   {
   for(v=1,su=0;v<=P;v++) su+=Lam[v]*B_x[v];
   for(v=su1=0;v<=P;v++)  su1+=C[v]*B_y[v];

   A=(B_x[0]-su-su1)/Q;
   }

#ifdef DEBUG
 if(LOG)   LOG->put('?'," A=%8.6f\t",A);

 if(LOG)   LOG->put('?'," Lam[]= ");
  for(v=1;v<=p;v++)  if(LOG) LOG->put('?',"%8.6f\t",Lam[v]);

 if(LOG)   LOG->put('?'," C[]= ");
  for(v=0;v<=p;v++)  if(LOG) LOG->put('?',"%8.6f\t",C[v]);
#endif

 okBld=1;
 return;
}


float _HUGE model::Result(float val)
{
 switch(type)
   {
   case 1: val+=_X[0]; break;
   case 2: val+=2.0*_X[0]-_X[1]; break;
   }
 return val;
}

float _HUGE model::calc(void)
{
 int v;
 float su,su1,val;

//
// -------------------------------- (38)
//

 for(v=1,su=0;v<=p;v++) su+=Lam[v]*X[M(l+1-v)];
 for(v=su1=0;v<=q;v++) su1+=C[v]*Tpl->get(1+q-v);
 for(v=q+1;v<=p;v++)   su1+=C[v]* Y[M(l+1+q-v)];

 return Result(A+su+su1);
}

float _HUGE model::Calc(float y)
{
 float val,yt;

 Y[l]=y;
 params();
 xbuild();
 val=calc();
 if(type) _X[1]=_X[0];

 yt=Tpl->getNext(); l=(l+1)%DMAX;

#ifdef USE_TEMPLATE
 return toType(_X[0]=Verify(yt,y,yt));
#else
 return toType(_X[0]=Verify(val,y,yt));
#endif
}

void _HUGE model::Append(float x,float y)
{
 if(l) Y[l]=y;
 params();

 l=(l+1)%DMAX;

 if(type) { _X[1]=_X[0]; }
 toType(_X[0]=x);

 if(cnt<MARGIN) cnt++;
 return;
}

void model::Build(void)
{
 if(!okBld && cnt>=MARGIN) okBld=1;
 return;
}

int model::Update(void)  { return okBld; }


float _HUGE model::toType(float x)
{
 switch(type)
   {
   case 0:
     X[l]=x;
   case 1: //------------------------- (42)
     X[l]=_X[0]-_X[1];
     break;
   case 2: //------------------------- (85a)
     X[l]=-oldX;
     oldX=_X[0]-_X[1];
     X[l]+=oldX;
     break;
   }
 return x;
}
