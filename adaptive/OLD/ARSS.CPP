#define DEBUG

#include <math.h>
#include <stdio.h>

#define Uses_mdl_ARSS
#define Uses_Buffer

#include <adaptive.hpp>
#include <log.hpp>

#define ModelARSS mdl_ARSS
#define PMAX      PMAX_ARSS
#define EMAX      EMAX_ARSS
#define QQMAX     QQMAX_ARSS
#define PSMAX     PSMAX_ARSS
#define NU        0 //1e-30

#define M(i) ((PSMAX+(i))%PSMAX)
#define C Betta
#define Lam C
#define m l

ModelARSS::ModelARSS(int _p,int _qq,int _shift,int _ep)
{
 int i,j,k,_k1,q;
 p=_p; qq=_qq<2 ? 2 : _qq>PSMAX ? _qq : PSMAX;
 shift=_shift; ep=_ep>EMAX ? EMAX : _ep;

 PS=qq*2+shift+2*p;

 q=qq;

 ok=0; m=0;


for(k=0;k<=PMAX;k++)
  {
  for(i=0;i<=PMAX;i++) ps[k][i]=dxy[k][i]=ffi[k][i]=0;
  d[k]=C[k]=Beta_x[k]=Beta_y[k]=Lam[k]=0;
  }
for(k=0;k<PSMAX;k++) X[k]=Y[k]=0;
for(k=0;k<EMAX;k++)
  {
  for(i=0;i<PMAX;i++) beta_kx[k][i]=beta_ky[k][i]=0;
  pk[k]=koef[k]=0;
  }

for(j=0;j<=PSMAX;j++)
  {
  for(i=0;i<=ep;i++)
    {
    switch(i)
       {
       case 0: pkm[i][j]=1.0;
       break;
       case 1: pkm[i][j]=1.0-2.0*j/((float)(q-1));
       break;
       default:
         pkm[i][j]=(pkm[1][j]*pkm[i-1][j]*(2*i-1)*(q-1)
                      /((float)(i*(q-i))))-pkm[i-2][j]*(i-1)*(q+i-1)
                      /((float)(i*(q-i)));
       break;
       }
    }
  }
for(pk[0]=q,i=1;i<=ep;i++)
        pk[i]=pk[i-1]*(2*i-1)*(q+i)/((float)((2*i+1)*(q-i)));
for(k=0;k<=ep;k++)
  {
  _k1=((k%2)?-1:1);
  koef[k]=(float)(_k1*(2*k+1)*(q+1+k+k*k))/(float)(q*(q+1));
  dk[k]=pkm[k][q+shift]/pk[k];
  }

 Reset();
 return;
}

void ModelARSS::Reset(void)
{
 cnt=0;
 ok=0;
 m=0;
 Bld=0;
 return;
}



void ModelARSS::xBuild(void)
{
 double s1,s2,s3,s4,s5,s6;
 long j1,j2,j3,j4;
 int i,q,v,j,k;
 double ax[PMAX_ARSS+1][PMAX_ARSS+1],ay[PMAX_ARSS+1][PMAX_ARSS+1],
         axy[PMAX_ARSS+1][PMAX_ARSS+1];

 if(cnt<PS) return; // еще рано строить модель


#ifdef DEBUG
 if(LOG) LOG->put('D',"1");
#endif
  /*......................................*/
for(v=0;v<=p;v++)
  {
  for(i=0;i<=p;i++)
    {
    j1=m-qq-shift-v; j2=m-qq-shift-i;
    j3=m-qq-v;       j4=m-qq-i;

    for(s1=s2=s3=0,j=0;j<=qq-1;j++)
      {
      s2+=X[M(j+j1)]*Y[M(j+j4)];
      if(i<=v)
        {
        s1+=X[M(j+j1)]*X[M(j+j2)];
        s3+=Y[M(j+j3)]*Y[M(j+j4)];
        }
      }
    for(s4=s5=s6=0,k=0;k<=ep;k++)
      {
      s5+=beta_ky[k][i]*beta_kx[k][v]/pk[k];
      if (i<=v)
        {
        s4+=beta_kx[k][v]*beta_kx[k][i]/pk[k];
        s6+=beta_ky[k][v]*beta_ky[k][i]/pk[k];
        }
      }
    axy[v][i]=s2-s5;
    if (i<=v)
      {
      ax[v][i]=s1-s4;
      ay[v][i]=s3-s6;
      }
    }
  }
#ifdef DEBUG
 if(LOG) LOG->put('D',"2");
#endif
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
    for(v=0;v<=p;v++)
      {
      for(q=0;q<=v;q++)
        {
        for(s1=0,j=0;j<=q-1;j++)
          s1+=ck[q][j]*ps[v][j];
        ps[v][q]=ay[v][q]-s1;
//        printf("\t%g\t",ps[q][q]);
        ck[v][q]=ps[v][q]/ps[q][q];
        }
      }
#ifdef DEBUG
 if(LOG)  LOG->put('D',"3");
#endif
    for(v=0;v<=p;v++)
      {
      for(q=0;q<=p;q++)
        {
        for(s1=0,i=0;i<=q-1;i++)
          s1+=ck[q][i]*dxy[i][v];
        dxy[q][v]=axy[v][q]-s1;
        }
      }
#ifdef DEBUG
 if(LOG)  LOG->put('D',"4");
#endif
    for(q=1;q<=p;q++)
      {
      for(v=1;v<=q;v++)
        {
        for(s1=0,i=1;i<=v-1;i++)
          s1+=ak[v][i]*ffi[q][i];
        for(s2=0,i=0;i<=p;i++)
          s2+=dxy[i][q]*dxy[i][v]/(ps[i][i]);
        ffi[q][v]=ax[q][v]-s1-s2;
        ak[q][v]=ffi[q][v]/(ffi[v][v]);
        }
      }
#ifdef DEBUG
 if(LOG)  LOG->put('D',"5");
#endif
    for(q=1;q<=p;q++)
      {
      for(s1=0,i=0;i<=p;i++)
        s1+=dxy[i][q]*dxy[i][0]/(ps[i][i]);
      for(s2=0,i=1;i<=q-1;i++)
        s2+=ffi[q][i]*d[i];
      d[q]=(ax[q][0]-s1-s2)/ffi[q][q];
      }
    /*--------------  Паpаметpы модели  ----------------------*/
#ifdef DEBUG
 if(LOG)  LOG->put('D',"6");
#endif
    for(v=p;v>=1;v--)
      {
      for(s1=0,i=v+1;i<=p;i++)
        s1+=ak[i][v]*Lam[i];
      Lam[v]=d[v]-s1;
      }
#ifdef DEBUG
 if(LOG)  LOG->put('D',"7");
#endif
    for(v=p;v>=0;v--)
      {
      for(s1=0,j=1;j<=p;j++)
        s1+=Lam[j]*dxy[v][j];
      for(s2=0,j=v+1;j<=p;j++)
        s2+=ps[j][v]*C[j];
      C[v]=(dxy[v][0]-s1-s2)/(ps[v][v]);
      }
  /*.................................................*/
#ifdef DEBUG
 if(LOG)  LOG->put('D',"8");
#endif

for (v=0;v<=p;v++)
  {
  for (s1=s2=0,k=0;k<=ep;k++)
    {
    s1+=koef[k]*beta_kx[k][v];
    s2+=koef[k]*beta_ky[k][v];
    }
  Beta_x[v]=s1;
  Beta_y[v]=s2;
  }

#ifdef DEBUG
  {
   int k;
 if(LOG) LOG->put('?',"\tLam[]=");for(k=1;k<=p;k++)  if(LOG) LOG->put('?',"%g ",Lam[k]);
 if(LOG) LOG->put('?',"\tC[]=");  for(k=0;k<=p;k++)  if(LOG) LOG->put('?',"%g ",C[k]);
 if(LOG) LOG->put('D'," B_x[]="); for(k=0;k<=p;k++)  if(LOG) LOG->put('D',"%g ",Beta_x[k]);
 if(LOG) LOG->put('D'," B_y[]="); for(k=0;k<=p;k++)  if(LOG) LOG->put('D',"%g ",Beta_y[k]);
  }
#endif
 Bld=1;
 return;
}

void _HUGE ModelARSS::Append(float y,float x)
{
  Y[m]=y; m=(m+1)%PSMAX;

  if(cnt>p+qq+shift)  params();

  X[m]=x;
  if(cnt<PS) cnt++;
 return;
}

float _HUGE ModelARSS::Calc(float y)
{
 int v;
 double s1,s2,s3;
 float yt;


  Y[m]=y;
  m=(m+1)%PSMAX;  yt=Tpl->getNext();

  params();
  xBuild();

  for(s1=0,v=1;v<=p;v++)       s1+=Lam[v]*(X[M(m-v)]-Beta_x[v]);
  for(s2=v=0;v<=shift;v++)     s2+=C[v]*(Tpl->get(-v+shift)-Beta_y[v]);
  for(s3=0,v=shift+1;v<=p;v++) s3+=C[v]*( Y[M(m-v+shift)]-Beta_y[v]);


  X[m]=Verify(Beta_x[0]+s1+s2+s3,y,yt);

 return X[m];
}



void ModelARSS::params(void)
{
double s1,s2;
long j1,j2,j3,j4;
int _k1;
int i,v,k;

for(k=0;k<=ep;k++)
  {
  _k1=((k%2)?-1:1);
  for (v=0;v<=p;v++)
    {
    j1=m-qq-shift-v; j2=m-1-shift-v;
    j3=m-qq-v;       j4=m-1-v;

    for (s1=s2=NU,i=0;i<=(qq/2)-1;i++)
      {
      s1+=(X[M(i+j1)]+_k1*X[M(j2-i)])*pkm[k][i];
      s2+=(Y[M(i+j3)]+_k1*Y[M(j4-i)])*pkm[k][i];
      }
     beta_kx[k][v]=s1;
     beta_ky[k][v]=s2;
     }
  }
#ifdef DEBUG
// if(LOG)   LOG->put('@'," B_kx[%d][%d]=",ep,p);
//  for(k=0;k<=ep;k++)  for (v=0;v<=p;v++) if(LOG)  LOG->put('@'," %g",beta_kx[k][v]);
// if(LOG)   LOG->put('@'," B_ky[%d][%d]=",ep,p);
//  for(k=0;k<=ep;k++)  for (v=0;v<=p;v++) if(LOG)  LOG->put('@'," %g",beta_ky[k][v]);
#endif

 return;
}


// В фоне ничего не надо делать
void ModelARSS::Build(void)
{
 return;
}

// когда набрано достаточное число данных, разрешить переключится
int ModelARSS::Update(void)
{
 if(cnt<PS) return 0;
 else return 1;
}


