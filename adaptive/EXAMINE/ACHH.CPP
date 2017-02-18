#include <math.h>
#include <stdlib.h>

#define Uses_exm_Achh
#define Uses_Buffer
#include <adaptive.hpp>



#define mdl exm_Achh

mdl::mdl(void)
{
 xx=NULL;
 yy=NULL;
 return;
}

mdl::~mdl(void)
{
 if(xx) delete xx;
 if(yy) delete yy;
 return;
}


int mdl::Setup(float Freq,unsigned long Nq,unsigned long w,Buffer *x,Buffer *y)
{
 delt=1.0/Freq;
 nq=Nq;
 wait=w;
 X=x; Y=y;
 if(xx) delete xx;
 if(yy) delete yy;

 xx=new float[(int)(wait*hi*delt)];
 yy=new float[(int)(wait*hi*delt)];
 ok=0;

 if(x && y) ok=1;
 return ok;
}

void mdl::Reset(void)
{
 df=(hi-lo)/nq;
 Tik=0;
 Sleep=1;
 state=1;
 Cycle=0;
 return;
}

float _HUGE mdl::Calc(float y)
{
 float ga,cc,e[3],ps[3],lam;
 int aa0,aa1,aa2,v;

 if(ok)
   {
   if(--Sleep==0)
     {
     switch(state)
       {
       case 0:
          // расчет значения АЧХ
          ga=0;
          lam=2.0*cos(freq);
          cc=2.0/(ampl*(float)nr*(1.0-ga*ga));
          for(v=0;v<=nr-1;v++)
            {
            aa0=v%3;  aa1=(v-1)%3; aa2=(v-2)%3;
            ps[v%2]=yy[nr-1-v]-ga*yy[v];
            if(v==0)
              e[aa0]=ps[v%2];
            else
            if(v==1)
              e[aa0]=lam*e[aa1]+ps[v%2];
            else
              {
              e[aa0]=lam*e[aa1]-e[aa2]+ps[v%2];
              v=v;
              }
            }
          aa0=(nr-1)%3; aa1=(nr-2)%3; aa2=(nr-3)%3;
          X->putNext(lo+df*Cycle);
          Y->putNext(cc*sqrt(fabs(e[aa0]*(ps[(nr-1)%2]-e[aa2])+e[aa1]*e[aa1])));

          nn=(lo+df*Cycle)/delt;
          state++;
          Sleep=nn;
          break;
       case 1:
          state=0;
          Sleep=nr=nn*wait;
          freq=2.0*M_PI*nn;
          Tik=0;
          Cycle++;
          break;
       }
     }
   y=beg+ampl*sin(freq*Tik++);
   }
 return y;
}



/*
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
*/