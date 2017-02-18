/*Программа <stat_har.c>
*Снятие статических характеристик
*       каналов
*/
#include <conio.h>
#include <math.h>

#include <tcxlwin.h>
#include <tcxlinp.h>

#include "pro_achh.h"
#include "dac_adc.h"
#define nr Cfg.nr
#define nq Cfg.nq
#define delt Cfg.delt
#define nko Cfg.nko
#define nki Cfg.nki
#define fw Cfg.fw
#define fn Cfg.fn

#ifdef SYS_TEST

float be[KAN_MAX+2][KAN_MAX+2][KAN_MAX+2][KAN_MAX+2];

static int fi2[4][3]={
              {-1,-1,+1},
              {-1,+1,-1},
              {+1,-1,-1},
              {+1,+1,+1}
              };
static int fi3[8][7]={
              {-1,-1,-1,+1,+1,+1,-1},
              {-1,-1,+1,+1,-1,-1,+1},
              {-1,+1,-1,-1,+1,-1,+1},
              {-1,+1,+1,-1,-1,+1,-1},
              {+1,-1,-1,-1,-1,+1,+1},
              {+1,-1,+1,-1,+1,-1,-1},
              {+1,+1,-1,+1,-1,-1,-1},
              {+1,+1,+1,+1,+1,+1,+1}
              };
static int fi4[16][4]={
              {-1,-1,-1,-1},
              {-1,-1,-1,+1},
              {-1,-1,+1,-1},
              {-1,-1,+1,+1},
              {-1,+1,-1,-1},
              {-1,+1,-1,+1},
              {-1,+1,+1,-1},
              {-1,+1,+1,+1},
              {+1,-1,-1,-1},
              {+1,-1,-1,+1},
              {+1,-1,+1,-1},
              {+1,-1,+1,+1},
              {+1,+1,-1,-1},
              {+1,+1,-1,+1},
              {+1,+1,+1,-1},
              {+1,+1,+1,+1},
              };


float xmin=-1.0;
float xmax=1.0;
float ymin=-1.0;
float ymax=1.0;


/*****************/
void do_stat(void)
/*****************/
{
int i,ii,j,k,v,nq_4;
float del_x=(xmax-xmin-0.2)/((float)nq/2),mxmax,mxmin,mymax,mymin;

Inf_Stat();

if(Enable()==DRV_OK)
  {
  xk[0]=xk[1]=xk[2]=xk[3]=0.1E-20;
  for(xh[0]=k=0;k<nki;k++)
    {
    xk[0]=xk[1]=xk[2]=xk[3]=0.1E-20;
    xk[k]=xh[0];
    Imit_Driv();
    yh[k][0][0]=yk[0];  yh[k][1][0]=yk[1];
    yh[k][2][0]=yk[2];  yh[k][3][0]=yk[3];
    for(i=0,ii=-1;i<=nq;i++)
      {
      if(i>=0&&i<=(nq/4)) ++ii;
      else
        {
        if((i>(nq/4))&&(i<=(3*nq/4)))  --ii;
        else if((i>(3*nq/4))&&(i<=nq)) ++ii;
        }
      xh[i]=(float)ii*del_x;
      xk[k]=xh[i];
      for(v=0;v<=10;v++)
        {
        Imit_Driv();
        if(kbhit() && getch()==27) goto end;
        }
      Wprtf(6,16,_RED|INTENSE|LGREY,"%2d%",(int)((k*nq+i)*100.0/(nki*nq)));
      for(j=0;j<nko;j++)  yh[k][j][i]=yk[j];
      }
    }
  Wprts(6,15,_RED|INTENSE|LGREY,"100% ");
end:
  Disable();
  WprtCen(8,_CYAN|BLINK|BLACK,"   Нажмите любую клавишу   ");
  KeyWait();
  }
  else printf("\007\007\007");
Wclose();
}

/*****************/
void wz_stat(void)
/*****************/
{
int i,j,k,v,m;
static float ai[KAN_MAX+1][WMAX+1];
static float beta[WMAX+1][KAN_MAX+1];
extern float be[KAN_MAX+2][KAN_MAX+2][KAN_MAX+2][KAN_MAX+2];
float su1,del_x;

del_x=(xmax-xmin-0.2)/(float)nq;

Inf_WzSt();

if(Enable()!=DRV_OK) { printf("\07\07\07"); return; }

switch(nki)
  {
  case 2:
/*    ai[0][0]=xmin; ai[1][0]=xmin;
    ai[0][1]=xmin; ai[1][1]=xmax;
    ai[0][2]=xmax; ai[1][2]=xmin;
    ai[0][3]=xmax; ai[1][3]=xmax;

    ScrSave();

    for(m=0;m<=3;m++)
      {
      for(k=0;k<nki;k++)
        {
        xk[k]=ai[k][m];
        printf("xk[%d]=%6.3f ",k,xk[k]);
        }
        printf("\n");
      Imit_Driv();
      for(j=0;j<nko;j++)
        {
        yh[0][j][m]=yk[j];
        printf("yh[0][%d][%d]=%6.3f\n",j,m,yh[0][j][m]);
        }
      }
    getch();
    for(i=0;i<nko;i++)
      {
      for(k=0;k<=3;k++)
        {
        for(su1=m=0;m<4;m++)
          su1+=(k==0)?yh[0][i][m]:fi2[m][k-1]*yh[0][i][m];
        beta[k][i]=su1/4.0;
        }
      }
//    ScrSave();
    clrscr();
    puts("Значения коэффициентов взаимосвязи для системы");
    printf("с числом входов %2d и с числом выходов %2d\n\n\n",nki,nko);
    for(i=0;i<nko;i++)
      {
      for(k=0;k<=3;k++)
        printf("beta[%1d][%1d]=%5.2f ",k,i,beta[k][i]);
      printf("\n");
      }
    puts("Нажмите любую клавишу");
    getch();
    ScrRest();
  */
  break;
  case 3:

  break;
  case 4:
    m=0;
    for(i=0;i<nq/2;i++)
      {
      xk[0]=xk[1]=xk[2]=xk[3]=-(int)i*del_x;
//      printf("7) %d %f %f %f %f\n",m,xk[0],xk[1],xk[2],xk[3]);
      Imit_Driv();
//      per_pr();
      }
    i=0;
    xk[0]=xk[1]=xk[2]=xk[3]=xmin+0.1;
    Imit_Driv();
//      printf("6) %d %f %f %f %f\n",m,xk[0],xk[1],xk[2],xk[3]);
    per_pr();
    Out_kan(i,m);

    for(m=1;m<=15;m++)
      {
//      shag(m);
      for(k=0;k<nki;k++)
          {
//        xk[k]=(fi4[m][k]==-1)?xmin:xmax;
          if(fabs(fi4[m][k]-fi4[m-1][k])>0.5)
            {
            if(fi4[m][k]==1)
              {
              for(i=0;i<nq;i++)
                {
                xk[k]=(xmin+0.1)+i*del_x;
                Imit_Driv();
//      printf("5) %d %f %f %f %f\n",m,xk[0],xk[1],xk[2],xk[3]);
//                per_pr();
                if(kbhit() && getch()==27) goto end;
                Wprtf(6,16,_RED|INTENSE|LGREY,"%2d%",(int)((m*nki*nq+k*nq+i)*100.0/(nki*nq*16)));
                }
              xk[k]=xmax-0.1;
              Imit_Driv();
//      printf("4) %d %f %f %f %f\n",m,xk[0],xk[1],xk[2],xk[3]);
              per_pr();
//              Out_y(m);
              }
            else
              {
              for(i=0;i<nq;i++)
                {
                xk[k]=(xmax-0.1)-i*del_x;
                Imit_Driv();
//      printf("3) %d %f %f %f %f\n",m,xk[0],xk[1],xk[2],xk[3]);
//                per_pr();
                if(kbhit() && getch()==27) goto end;
                Wprtf(6,16,_RED|INTENSE|LGREY,"%2d%",(int)((m*nki*nq+k*nq+i)*100.0/(nki*nq*16)));
                }
              xk[k]=xmin+0.1;
              Imit_Driv();
//      printf("2) %d %f %f %f %f\n",m,xk[0],xk[1],xk[2],xk[3]);
              per_pr();
//              Out_y(m);
              }
            }
          else
              {
              xk[k]=(fi4[m][k]==-1)?xmin+0.1:xmax-0.1;
              Imit_Driv();
//      printf("1)  %d %f %f %f %f\n",m,xk[0],xk[1],xk[2],xk[3]);
//            Out_kan(0,m);
              per_pr();
//              Out_y(m);
              Wprtf(6,16,_RED|INTENSE|LGREY,"%2d%",(int)((m*nki+k)*100.0/(nki*16)));
              }
          }
      Imit_Driv();
      per_pr();
//      printf("%d %f %f %f %f\n",m,xk[0],xk[1],xk[2],xk[3]);
//    printf("%d %f %f %f %f\n",m,yk[0],yk[1],yk[2],yk[3]);
      Out_kan(0,m);
      }
for(k=0;k<=3;k++)
  {
  if(fi4[15][k]==1)
    {
    for(i=0;i<nq/2;i++)
      {
      xk[k]=(xmax-0.1)-i*del_x;
      Imit_Driv();
      if(kbhit() && getch()==27) goto end;
      }
    xk[k]=0.0;
    Imit_Driv();
    }
  else
    {
    for(i=0;i<nq/2;i++)
      {
      xk[k]=(xmin+0.1)+i*del_x;
      Imit_Driv();
      if(kbhit() && getch()==27) goto end;
      }
    xk[k]=0.0;
    Imit_Driv();
    }
  }

// Вычисление величин <beta>
    for(v=0;v<=4;v++)
      {
      for(k=0;k<nko;k++)
        {
        for(su1=0,m=0;m<=15;m++)
          su1=(v==0)?su1+yh[0][k][m]:su1+yh[0][k][m]*(int)fi4[m][v-1];
        be[0][0][v][k]=su1/16.0;
        }
      }
    for(j=1;j<=3;j++)
      {
      for(v=j+1;v<=4;v++)
        {
        for(k=0;k<nko;k++)
          {
          for(su1=m=0;m<=15;m++)
            su1+=((int)fi4[m][j-1])*((int)fi4[m][v-1])*yh[0][k][m];
          be[0][j][v][k]=su1/16.0;
          }
        }
      }
    for(i=1;i<=2;i++)
      {
      for(j=i+1;j<=3;j++)
        {
        for(v=j+1;v<=4;v++)
          {
          for(k=0;k<nko;k++)
            {
            for(su1=m=0;m<=15;m++)
              su1+=((int)fi4[m][j-1])*((int)fi4[m][v-1])*((int)fi4[m][i-1])*yh[0][k][m];
            be[i][j][v][k]=su1/16.0;
            }
          }
        }
      }
  for(k=0;k<nko;k++)
    {
    for(su1=m=0;m<=15;m++)
      su1+=((int)fi4[m][0])*((int)fi4[m][1])*((int)fi4[m][2])*((int)fi4[m][3])*yh[0][k][m];
    be[3][3][3][k]=su1/16.0;
    }
  break;

  }
 Wprts(6,15,_RED|INTENSE|LGREY,"100% ");
end:
 Disable();
 WprtCen(8,_CYAN|BLINK|BLACK,"   Нажмите любую клавишу   ");
 KeyWait();
 tab_beta();
 Wclose();

}

// Функция вычисления АЧХ всех каналов системы
//****************
void do_achh(void)
//****************
{

int i,j,k,v,kk;
float fi,psii,lam,amp,ga,cc,xt[3],e[3],ps[3],tmp;
int aa0,aa1,aa2;

 psii=0.5;
 fdisk=1.0/delt;
 amp=0.08;

if(!AllocBuf()) return;

 Inf_Achh();

if(Enable()==DRV_OK)
{
xk[0]=xk[1]=xk[2]=xk[3]=0;
for(kk=0;kk<nki;kk++)
  {
  tmp=(xk[kk]-amp*sin(psii))/(float)30;
  for(k=0;k<30;k++)
    {
    xk[kk]-=tmp;
    Imit_Driv();
    if(kbhit() && getch()==27) goto end;
    }
  for(k=0;k<=nq;k++)
    {
    fi=2.0*M_PI*delt*(fn+((float)k*(fw-fn)/((float)(nq))));
    xh[k]=fi/(2.0*M_PI*delt);
//    printf("xh[%3d]=%8.3f\n",k,fi/(2.0*M_PI*delt));
    lam=2.0*cos(fi);
    xt[0]=amp*sin(psii);
    xk[kk]=xt[0];
    Imit_Driv();
    xt[1]=amp*sin(fi+psii);
    xk[kk]=xt[1];
    Imit_Driv();
    for(v=2;v<=200;v++)
      {
      xk[kk]=xt[v%3]=lam*xt[(v-1)%3]-xt[(v-2)%3];
      Imit_Driv();
      if(kbhit() && getch()==27) goto end;
      }
    Wprtf(6,16,_RED|INTENSE|LGREY,"%2d%",(int)((kk*nq+k)*100.0/(nki*nq)));
    xk[kk]=x[kk][0]=xt[(v-3)%3];
    Imit_Driv();
    Out_y(0);
    xk[kk]=x[kk][1]=xt[(v-2)%3];
    Imit_Driv();
    Out_y(1);
    xk[kk]=x[kk][2]=xt[(v-1)%3];
    Imit_Driv();
    Out_y(2);
    for(v=3;v<=nr-1;v++)
      {
      xk[kk]=x[kk][v]=lam*x[kk][v-1]-x[kk][v-2];
      Imit_Driv();
      Out_y(v);
//      printf("v=%d  xk[%d]=%f  yk[%d]=%f y[kk][v]=%f\n",v,kk,xk[kk],kk,yk[kk],y[kk][v]);
      if(kbhit() && getch()==27) goto end;
      }
//    ga=sin((float)nr*fi)/((float)nr*sin(fi));
    ga=0;
    cc=2.0/(amp*(float)nr*(1.0-ga*ga));
    for(v=0;v<=nr-1;v++)
      {
      aa0=v%3;  aa1=(v-1)%3; aa2=(v-2)%3;
      ps[v%2]=y[kk][nr-1-v]-ga*y[kk][v];
      if(v==0)
        e[aa0]=ps[v%2];
      else
      if(v==1)
        e[aa0]=lam*e[aa1]+ps[v%2];
      else
        {
//      e[v%3]=lam*e[(v-1)%3]-e[(v-2)%3]+ps[v%2];
//      aa0=v%3;  aa1=(v-1)%3; aa2=(v-2)%3;
        e[aa0]=lam*e[aa1]-e[aa2]+ps[v%2];

        v=v;
        }
      }
    aa0=(nr-1)%3; aa1=(nr-2)%3; aa2=(nr-3)%3;
//  yh[kk][0][k]=cc*sqrt(e[(nr-1)%3]*(ps[(nr-1)%2]-e[(nr-3)%3])+e[(nr-2)%3]*e[(nr-2)%3]);
    yh[kk][kk][k]=cc*sqrt(fabs(e[aa0]*(ps[(nr-1)%2]-e[aa2])+e[aa1]*e[aa1]));
//    printf("xh[%3d]=%8.3f  yh[kk][0][k]=%8.3f\n",k,fi/(2.0*M_PI*delt),yh[kk][0][k]);

    }
//  printf("kk=%3d\n",kk);
  }
 Wprts(6,15,_RED|INTENSE|LGREY,"100% ");
end:
  Disable();
  WprtCen(8,_CYAN|BLINK|BLACK,"   Нажмите любую клавишу   ");
  KeyWait();
  graf_achh();
 }
 else printf("\007\007\007");

 Wclose();

}
#endif