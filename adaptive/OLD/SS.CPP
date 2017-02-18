#define DEBUG

//#include <conio.h>
#include <stdio.h>

#define Uses_mdl_SS
#define Uses_Buffer
#include <adaptive.hpp>


#define model     mdl_SS
#define PMAX      PMAX_ARSS
#define EMAX      EMAX_ARSS
#define QQMAX     QQMAX_ARSS
#define PSMAX     PSMAX_ARSS
// #define NU        1e-30

#define M(a) ( ((a)+PSMAX)%PSMAX )
#define C Betta
#define Lam C
#define m l

/*------------- PƒP€ŒŒ€ ‘’P…ˆŸ ‘‘ Œ„…‹ˆ -------*/
/*--------------- ‘ ’P…„Œ ‘ “—…’Œ ‘„‚ˆƒ€ ----------*/
void model::xBuild(void)
{
 int i,q,v,j,k;
 double s1,s2,s3,s5,s6;
 long j2,j3,j4;
 double ax[PMAX_ARSS+1][PMAX_ARSS+1],ay[PMAX_ARSS+1][PMAX_ARSS+1],
         axy[PMAX_ARSS+1][PMAX_ARSS+1];

if(cnt<PS) return;


#ifdef DEBUG
 if(LOG)  LOG->put('!',"1");
#endif
for(v=0;v<=p;v++)
  {
  for(i=0;i<=v;i++)
    {
    j2=m-qq-shift;j3=m-qq-v;j4=m-qq-i;

    for(s2=s3=0,j=0;j<=qq-1;j++)
      {
      s3+=Y[M(j+j3)]*Y[M(j+j4)];
      if(i==0)
        s2+=X[M(j+j2)]*Y[M(j+j3)];
      }
    for(s5=s6=0,k=0;k<=ep;k++)
      {
      s6+=beta_ky[k][v]*beta_ky[k][i]/pk[k]; //tmp;
      if(i==0)
        s5+=beta_ky[k][v]*beta_kx[k][i]/pk[k]; //tmp;
      }
    ay[v][i]=s3-s6;
    if(i==0)
      axy[v][i]=s2-s5;
    }
  }

/*--------------------------------------------------*/
#ifdef DEBUG
 if(LOG)  LOG->put('!',"2");
#endif

for(v=0;v<=p;v++)
  {
  for(q=0;q<=v;q++)
    {
    for(s1=0,j=0;j<=q-1;j++)
      s1+=ck[v][j]*ps[v][j];
    ps[v][q]=ay[v][q]-s1;
    ck[v][q]=ps[v][q]/ps[q][q];
    }
  }
#ifdef DEBUG
 if(LOG)  LOG->put('!',"3");
#endif
for(q=0;q<=p;q++)
  {
  for(s2=0,i=0;i<=q-1;i++)
    s2+=ps[q][i]*d[i];
  d[q]=(axy[q][0]-s2)/ps[q][q];
  }
#ifdef DEBUG
 if(LOG)  LOG->put('!',"4");
#endif
/*--------------   p ¬¥âpë ¬®¤¥«¨  ----------------------*/
for(v=p;v>=0;v--)
  {
  for(s1=0,i=v+1;i<=p;i++)
    s1+=ck[i][v]*C[i];
  C[v]=d[v]-s1;
  }
/*for(s1=NU,v=0;v<=p;v++)
     s1+=C[v]*ys[p-v];
  a=(xs[p]-s1)/qd; */
/*...........................................*/
#ifdef DEBUG
 if(LOG)  LOG->put('!',"5");
#endif
for (v=0;v<=p;v++)
  {
  for (s1=s2=0,k=0;k<=ep;k++)
    {
    if(v==0)
     s1+=dk[k]*beta_kx[k][v];
    s2+=dk[k]*beta_ky[k][v];
    }
  Beta_x[v]=s1;
  Beta_y[v]=s2;
  }

#ifdef DEBUG
  {
   int k;
 if(LOG)    LOG->put('?',"\tC[]="); for(k=0;k<=p;k++)  if(LOG) LOG->put('?',"%g ",C[k]);
 if(LOG)    LOG->put('D'," B_x[]="); for(k=0;k<=p;k++) if(LOG) LOG->put('D',"%g ",Beta_x[k]);
 if(LOG)    LOG->put('D'," B_y[]="); for(k=0;k<=p;k++) if(LOG) LOG->put('D',"%g ",Beta_y[k]);
  }
#endif
 ok=1;
 return;
}

/*-----------  ‘¨£­ « ¯p®£­®§   ------------*/
float _HUGE model::Calc(float y)
{
 int v;
 double s2,s3;
 float yt;

 Y[m]=y;
 m=(m+1)%PSMAX; yt=Tpl->getNext();

 params();
 xBuild();

 for(s2=0,v=0;v<=shift;v++)    s2+=C[v]*(Tpl->get(-v+shift)-Beta_y[v]);
 for(s3=0,v=shift+1;v<=p;v++)  s3+=C[v]*( Y[M(m-v+shift)]-Beta_y[v]);

 return X[m]=Verify(Beta_x[0]+s2+s3,y,yt);
}


