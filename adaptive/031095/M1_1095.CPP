//
//
//
//

#define DEBUG
// #define FAST_ALFA
// #define USE_ORIGINAL

#define Uses_mdl_0TR031095
#define Uses_Buffer
#include <adaptive.hpp>

#define model mdl_0TR031095
#define DMAX DMAX_031095
#define QMAX QMAX_031095
#define PMAX PMAX_031095
#define NU 0 //  1e-30
#define MARGIN (PS+5)


#define M(a) ((DMAX+(a))%DMAX)
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))

model::model(unsigned int _t,unsigned int _p,unsigned int _Q,unsigned int _q)
{
 int i;

 p=min(_p,PMAX); Q=min(max(_Q,1),QMAX); Typ=_t%3; q=_q;

 cnt=okBld=okUpd=l=0;

 X[0]=0; PS=Q+p+1;

 for(i=0;i<PMAX;i++) Betta[i]=0;
 for(i=0;i<DMAX;i++) Ax[i]=Ay[i]=X[i]=Y[i]=0;
 for(i=0;i<3;i++) x[i]=0;
 return;
}


void model::params(void)
{
 double su;
 int k;

 switch(Typ)
   {
   case 0: X[l]=x[0]; break;
   case 1: if(cnt>0) X[l]=x[0]-x[1]; x[1]=x[0]; break;
   case 2: if(cnt>1) X[l]=x[0]-2*x[1]+x[2]; x[2]=x[1]; x[1]=x[0]; break;
   }

 if(cnt<=Q) return;

#ifndef FAST_ALFA
// -----------  (3)
 for(k=su=0;k<Q;k++) su+=X[M(l-k)]; Ax[l]=su/Q;
 for(k=su=0;k<Q;k++) su+=Y[M(l-k)]; Ay[l]=su/Q;

#else
// -----------  (4)
 Ax[l]=Ax[M(l-1)]+(X[l]-X[M(l-Q)])/Q;
 Ay[l]=Ay[M(l-1)]+(Y[l]-Y[M(l-Q)])/Q;
#endif

 return;
}


void _HUGE model::Append(float _x,float _y)
{
 Y[l]=_y; params();
 l=(l+1)%DMAX;

 x[0]=_x;

 if(cnt<MARGIN) cnt++;
 return;
}

float model::_y_(int k)
{
 return (k>=l) ? Tpl->get(k-l) : Y[M(k)];
}

float model::calc_su(int m)
{
 float val;
 int v;

 for(v=val=0;v<=p;v++) val+=Betta[v]*(_y_(m+1-v)-Ay[M(m-1-v)]);

 return Ax[M(m-2)]+val;
}


float _HUGE model::Calc(float y)
{
 float val,yt;
 int v;

 Y[l]=y;  params();
 l=(l+1)%DMAX; yt=Tpl->getNext();



 switch(Typ)
   {
   case 0:
     // --------- (8)
     val=calc_su(l);
     break;
   case 1:
     // --------- (9)
     for(v=val=0;v<=Q-2;v++) val+=(Q-1-v)*calc_su(l-v)/Q;
     break;
   case 2:
     // --------- (10)
     for(v=val=0;v<=Q-3;v++) val+=(v+1)*(Q-1-v)*(Q-2-v)*calc_su(l-v)/(Q*(Q+1));
     break;
   }

 return x[0]=Verify(val,y,yt);
}


void model::Build(void)
{
 double su1,su2;
 double su;
 double fi[PMAX][PMAX],psi[PMAX],H[PMAX][PMAX],D[PMAX];
 int ll,k,v,j,i,m;


 if(cnt<MARGIN) return;


 ll=(DMAX+l-Q)%DMAX;

 switch(p)
   {
   case 0:  // ========================================================
     for(i=0,su=0;i<Q;i++)
       su+=Ax[M(ll+i)]*Ay[M(ll+i+1)];
     for(i=0,su1=0;i<Q;i++) { su2=Ax[M(ll+i)]; su1+=su2*su2; }
     beta[0]=su1/(su+NU);
     break;
   default: // ========================================================
     // ----------- (11)
     for(k=0;k<=p;k++)
        {
        for(v=su=0;v<=k;v++)
           {
           for(m=ll;m<ll+Q;m++)
#ifdef USE_ORIGINAL
              su+=(Y[M(m+1-k)]-Ay[M(m-1-k)])*(Y[M(m+1-v)]-Ay[M(m-1-v)]);
#else
              su+=(Y[M(m-k)]*Y[M(m-v)]);
#endif
           fi[k][v]=su;
           }
        for(m=ll;m<ll+Q;m++)
#ifdef USE_ORIGINAL
           su+=(X[M(m)]-Ax[M(m-2)])*(Y[M(m+1-k)]-Ay[M(m-1-k)]);
#else
           su+=(Y[M(m-k)]*X[M(m-q)]);
#endif
        psi[k]=su;
        }
     for(k=0;k<=p;k++) for(v=0;v<=k;v++)
        {
        for(j=su=0;j<=v-1;j++)
            su+=H[k][j]*H[v][j]/(H[j][j]+NU);
        H[k][v]=fi[k][v]-su;
        }
     for(k=0;k<=p;k++)
        {
        for(v=su=0;v<=k-1;v++)
            su+=H[k][v]*D[v]/(H[v][v]+NU);
        D[k]=psi[k]-su;
        }
     for(k=p;k>=0;k--)
        {
        for(v=k+1,su=0;v<=p;v++)
            su+=beta[v]*H[v][k];
        beta[k]=(D[k]-su)/(H[k][k]+NU);
        }
     break;
   }

#ifdef DEBUG
 if(LOG)  LOG->put('?',"Betta[]=");
 for(i=0;i<=p;i++)  if(LOG) LOG->put('?',"%g\t",beta[i]);
#endif


 okBld=1;
 return;
}

int model::Update(void)
{
 int i;

 if(okBld)
   {
   // обновить параметры
   for(i=0;i<=p;i++) Betta[i]=beta[i];
   okUpd=1;
//   cprintf("U");
   }
 return okUpd;
}
