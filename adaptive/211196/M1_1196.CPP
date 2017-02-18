//
// АР-СС модель от 21.11.96
//

#define DEBUG


#include <stdio.h>
#include <stdlib.h>

#define Uses_Buffer
#define Uses_mdl_arss211196

#include <adaptive.hpp>

#define mdl mdl_arss211196
#define DMAX DMAX_211196
#define PMAX PMAX_211196
#define M(a) (((a)+DMAX)%DMAX)
#define MARGIN (PS)

mdl::mdl(int _t,unsigned _p,unsigned _Q,unsigned _q)
{
 int i,j;

 Typ=_t; p=_p; Q=_Q<DMAX ? _Q : DMAX; q=_q;
 okBld=okUpd=0;
 l=0;
 cnt=0;
 PS=Q+p+q+1;

 for(i=0;i<DMAX;i++) X[i]=Y[i]=0;
 for(i=0;i<PMAX;i++)
   {
   C[i]=Betta[i]=0;
//   for(j=0;j<PMAX;j++) psi_x[i][j]=psi_y[i][j]=psi_xy[i][j]=0;
   }
 C[0]=1;

 return;
}

void mdl::Build(void)
{
 if(cnt>MARGIN) okBld=1;
 else cnt++;
 return;
}
int mdl::Update(void) { return okUpd=okBld; }

void mdl::params(void)
{
 int m,k,v;
 double su;

//
//  --------------- (3)
//
 for(k=0;k<=p;k++) for(v=0;v<=k;v++)
   {
   for(m=l-Q,su=0;m<l;m++) su+=X[M(m-q-k)]*X[M(m-q-v)]; psi_x[k][v]=su;
   for(m=l-Q,su=0;m<l;m++) su+=Y[M(m-k)]*Y[M(m-v)];     psi_y[k][v]=su;
   }
 for(k=0;k<=p;k++) for(v=0;v<=p;v++)
   {
   for(m=l-Q,su=0;m<l;m++) su+=Y[M(m-k)]*X[M(m-q-v)];
   psi_xy[k][v]=su;
   }
#ifdef DEBUG
 if(LOG)
  {
  LOG->put('D',"\n psi_x[][]="); for(k=0;k<=p;k++) { LOG->put('D',"\n"); for(v=0;v<=k;v++) LOG->put('D',"%f\t",psi_x[k][v]); }
  LOG->put('D',"\n psi_y[][]="); for(k=0;k<=p;k++) { LOG->put('D',"\n"); for(v=0;v<=k;v++) LOG->put('D',"%f\t",psi_y[k][v]); }
  LOG->put('D',"\n psi_x[][]="); for(k=0;k<=p;k++) { LOG->put('D',"\n"); for(v=0;v<=p;v++) LOG->put('D',"%f\t",psi_xy[k][v]); }
  }
#endif
 return;
}

void mdl::xbuild(void)
{
 int k,m,v,j;
 double su,su1;
 double fi[PMAX][PMAX],D[PMAX][PMAX],E[PMAX][PMAX],DD[PMAX],
        psi[PMAX][PMAX];

 if(cnt<MARGIN) return;
//
// ---------------- (4)
//
 for(k=0;k<=p;k++) for(v=0;v<=k;v++)
   {
   for(j=0,su=0;j<v;j++)
      {
      su+=psi[v][j]*psi[k][j]/psi[j][j];
      }
   psi[k][v]=psi_y[k][v]-su;
   }
 if(LOG) { LOG->put('D',"\n psi[][]="); for(k=0;k<=p;k++) { LOG->put('D',"\n"); for(v=0;v<=k;v++) LOG->put('D',"%f\t",psi[k][v]); } }

 for(v=0;v<=p;v++) for(k=0;k<=p;k++)
   {
   for(m=0,su=0;m<k;m++) su+=psi[k][m]*D[m][v]/psi[m][m];
   D[k][v]=psi_xy[k][v]-su;
   }
 if(LOG) { LOG->put('D',"\n D[][]="); for(k=0;k<=p;k++) { LOG->put('D',"\n"); for(v=0;v<=p;v++) LOG->put('D',"%f\t",D[k][v]); } }

 for(k=1;k<=p;k++) for(v=0;v<=k;v++)
   {
   for(m=0,su=0;m<=p;m++) su+=D[m][k]*D[m][v]/psi[m][m];
   fi[k][v]=psi_x[k][v]-su;
   }
 if(LOG) { LOG->put('D',"\n fi[][]="); for(k=0;k<=p;k++) { LOG->put('D',"\n"); for(v=0;v<=k;v++) LOG->put('D',"%f\t",fi[k][v]); } }

 for(k=1;k<=p;k++) for(v=1;v<=k;v++)
   {
   for(j=1,su=0;j<v;j++) su+=E[v][j]*E[k][j]/E[j][j];
   E[k][v]=fi[k][v]-su;
   }
 if(LOG) { LOG->put('D',"\n E[][]="); for(k=0;k<=p;k++) { LOG->put('D',"\n"); for(v=0;v<=k;v++) LOG->put('D',"%f\t",E[k][v]); } }

 for(k=1;k<=p;k++)
   {
   for(v=1,su=0;v<k;v++) su+=E[k][v]*DD[v]/E[v][v];
   DD[k]=-fi[k][0]-su;
   }
 if(LOG) { LOG->put('D',"\n DD[]="); for(v=0;v<=p;v++) LOG->put('D',"%f\t",DD[v]); }

 for(k=p;k>=1;k--)
   {
   for(v=k+1,su=0;v<=p;v++) su+=C[v]*E[v][k];
   C[k]=(DD[k]-su)/E[k][k];
   }
 if(LOG) { LOG->put('?'," C[]="); for(v=0;v<=p;v++) LOG->put('?',"%f\t",C[v]); }

 for(v=p;v>=0;v--)
   {
   for(k=0,su=0;k<=p;k++) su+=C[k]*D[v][k];
   for(k=v+1,su1=0;k<=p;k++) su1+=Betta[k]*psi[k][v];
   Betta[v]=(su-su1)/psi[v][v];
   }
 if(LOG) { LOG->put('?',"\tBetta[]="); for(v=0;v<=p;v++) LOG->put('?',"%f\t",Betta[v]); }

 return;
}

inline float mdl::_y_(long d)
{
 return d>=l ? Tpl->get(d-l) : Y[M(d)];
// return Tpl->get(d-l);
}

float _HUGE mdl::Calc(float y)
{
 float val,yt;
 double su,su1;
 int v;

 Y[l]=y;  params();
 l=(l+1)%DMAX; yt=Tpl->getNext();

 xbuild();
 for(v=1,su=0;v<=p;v++) su+=C[v]*X[M(l-v)];
 for(v=0,su1=0;v<=p;v++) su1+=Betta[v]*_y_(l+q-v);
 val=-su+su1;

 if(ExtTrend) val+=ExtTrend->Calc(l-1);

 return X[l]=Verify(val,y,yt);
}

void _HUGE mdl::Append(float _x,float _y)
{
 Y[l]=_y; params();
 l=(l+1)%DMAX;

// xbuild();
 X[l]=_x;

 if(cnt<MARGIN) cnt++;
 return;
}



