#define DEBUG

#include <conio.h>
#include <stdio.h>

#define Uses_mdl_270395
#define Uses_Buffer



#include <adaptive.hpp>

#define PMAX PMAX_270395
#define QMAX QMAX_270395
#define DMAX DMAX_270395

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#define M(a) (((a)+DMAX)%DMAX)
#define NU 0 // 1e-30
#define MARGIN (PS*10)

mdl_270395::mdl_270395(unsigned int _t,unsigned int _p,unsigned int _q,unsigned int _Q)
{
 int i;

 p=min(_p,PMAX); q=min(_q,10); Q=min(max(_Q,1),QMAX); Typ=_t%3;

 cnt=okBld=okUpd=l=0;

 X[0]=0; PS=Q+q+p;

 for(i=0;i<DMAX;i++) dx[i]=dy[i]=X[i]=Y[i]=0;
 for(i=0;i<PMAX;i++) Betta[i]=0;
 return;
}



void mdl_270395::Build()
{

 double psi[PMAX],fi[PMAX][PMAX],Psi,H[PMAX][PMAX],Lam[PMAX];
 double C,su,su1,su2,D[3];
 int i,k,v,ll;

// ------------------- (35)

#define precalc_par()                                            \
   {                                                             \
   for(k=0;k<=p;k++)                                             \
     {                                                           \
     for(i=ll,su=0;i<ll+Q;i++) su+=dx[M(i)]*dy[M(i+q-k)];        \
     psi[k]=su;                                                  \
                                                                 \
     if(!k) continue;                                            \
     for(v=0;v<=k;v++)                                           \
       {                                                         \
       for(i=ll,su=0;i<ll+Q;i++) su+=dy[M(i+q-k)]*dy[M(i+q-v)];  \
       fi[k][v]=su;                                              \
       }                                                         \
     }                                                           \
                                                                 \
   for(i=ll,Psi=0;i<ll+Q;i++)                                    \
         Psi+=dx[M(i)]*dx[M(i)];                                 \
   }

 if(cnt<MARGIN) return;

 ll=M(l-Q-q-1);

 switch(p)
 {
 case 0: // ========================================================

   // ------------ (35) p=0

   for(i=ll,su=0;i<ll+Q;i++)
          su+=dx[M(i)]*dy[M(i+q)];
   for(i=ll,su1=0;i<ll+Q;i++)
          su1+=dx[M(i)]*dx[M(i)];

   // ------------ (42)

   beta[0]=su1/(su+NU);
   break;
 case 1: // ========================================================

   precalc_par();

   // ------------ (42a)
   su=psi[0]*fi[1][1]-psi[1]*fi[1][0]+NU;
   beta[0]=(fi[1][1]*Psi-psi[1]*psi[1])/su;
   beta[1]=(psi[1]*psi[0]-Psi*fi[1][0])/su;
   break;
 case 2: // ========================================================
   precalc_par();

   // ------------ (43)

   su=fi[1][1]*fi[2][2]-fi[2][1]*fi[2][1];
   su1=fi[2][0]*fi[2][1]-fi[1][0]*fi[2][2];
   su2=fi[1][0]*fi[2][1]-fi[2][0]*fi[1][0];
   D[0]=psi[2]*fi[1][1]-psi[1]*fi[2][1];
   D[1]=psi[1]*fi[2][2]-psi[2]*fi[2][1];
   D[2]=psi[1]*fi[2][0]-psi[2]*fi[1][0];
   C=psi[0]*su+psi[1]*su1+psi[2]*su2+NU;
   beta[0]=(Psi*su-psi[2]*D[0]-psi[1]*D[1])/C;
   beta[1]=(Psi*su1-psi[2]*D[2]+psi[0]*D[1])/C;
   beta[2]=(Psi*su2+psi[0]*D[0]+psi[1]*D[2])/C;
   break;
 default:// ========================================================
   precalc_par();

// cprintf("?");
   for(k=0;k<=p;k++) for(v=0;v<=k && v<p;v++)
     {
     for(i=0,su=0;i<v;i++)
        su+=(H[k][i]*H[v][i])/(H[i][i]+NU);
//     cprintf("%lf %lf %lf %lf \n\r",fi[p-v][p-k],psi[p-v],psi[p-k],Psi);
     H[k][v]=fi[p-v][p-k]-(psi[p-v]*psi[p-k])/(Psi+NU)-su;
     }
// cprintf("==> 4\n\r");

   for(i=p-1;i>=0;i--)
     {
     for(v=i+1,su=0;v<p;v++)
            su+=Lam[p-v]*H[v][i];
     Lam[p-i]=(H[p][i]-su)/(H[i][i]+NU);
     }
// cprintf("5");

   for(v=1,su=0;v<=p;v++) su+=Lam[v]*psi[v];
   C=Psi/(psi[0]-su+NU);

// cprintf("6");
   beta[0]=C; for(i=1;i<=p;i++) beta[i]=-Lam[i]*C;
  break;
 } // end switch(p)

// cprintf("!");
#ifdef DEBUG
 if(LOG)  LOG->put('?',"Lam[]= ");
 for(i=0;i<=p;i++)  if(LOG) LOG->put('?',"\t%g",Lam[i]);
 if(LOG)  LOG->put('D',"Betta[]= ");
 for(i=0;i<=p;i++)  if(LOG) LOG->put('D',"\t%g",beta[i]);
#endif

 // модель построена
 okBld=1;
 return;
}

// обновление параметров модели
int mdl_270395::Update(void)
{
 int i;

 if(okBld)
   {
   for(i=0;i<=p;i++) Betta[i]=beta[i];
   okUpd=1;
//   cprintf("U");
   }
 return okUpd;
}

void _HUGE mdl_270395::Append(float x,float y)
{
 Y[l]=y; params();

 l=(l+1)%DMAX; // l1=(l1+1)%DMAX;
 X[l]=x;

 if(cnt<MARGIN) cnt++;

 return;
}


inline float  mdl_270395::_y_(int ll)
{
 return ll>=l ? Tpl->get(ll-l) : Y[M(ll)];
}

inline double mdl_270395::calc_su(int l)
{
 int v;
 double val;

// cprintf("%f\n\r",Betta[0]);
 for(v=0,val=0;v<=p;v++)  val+=Betta[v]*_y_(l+q-v);
 return val;
}

float mdl_270395::calc(void)
{
 float su,val;
 int  m;

   switch(Typ)
     {
     case 0:
       val=calc_su(l);
       break;
     case 1:
       for(m=0,su=0;m<=Q-2;m++)
          su+=((Q-1-m)*calc_su(l-m))/Q;
       val=su;
       break;
     case 2:
       for(m=0,su=0;m<=Q-3;m++)
          su+=(m+1)*(Q-1-m)*(Q-2-m)*calc_su(l-m)/(Q*(Q+1));
       val=su;
       break;
     }
 return val;
}

float _HUGE mdl_270395::Calc(float y)
{
 float yt,val;

 Y[l]=y; params();
 l=(l+1)%DMAX; yt=Tpl->getNext();

// cprintf("<");

 val=calc();
 if(ExtTrend) val+=ExtTrend->Calc(l-1);

// cprintf(">");

 return X[l]=Verify(val,y,yt);
}


void mdl_270395::params(void)
{
 dy[l]=Y[l];

 switch(Typ)
   {
   case 0: dx[l]=X[l];  break;
   case 1: if(cnt>0)  dx[l]=X[l]-X[M(l-1)];  break;
   case 2: if(cnt>1)  dx[l]=X[l]-2.0*X[M(l-1)]+X[M(l-2)];  break;
   }
// printf("---->%lu %f \t %f\n",l,dx[l],dy[l]);
 return;
}
