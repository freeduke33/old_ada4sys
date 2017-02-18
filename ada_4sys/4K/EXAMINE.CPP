#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <alloc.h>
#include <sys/stat.h>
#include <math.h>
#include <conio.h>
#include <tcxlwin.h>

#include "examine.h"
#include "dac_adc.h"

#define RC_NOMEM 1
#define RC_DONE  0

// #define CHANNEL 4
void saveSTAT(float **x,float **y);
void saveACHH(float **x,float **y);


int nk;
unsigned long nr_view,nr;
void doRewind(int Margin,float *valB,float *valE);

/* Buffer *b[CHANNEL],*bb[CHANNEL]; */

extern "C" void doExamine(int mode)
{
 static float *xx[CHANNEL],*yy[CHANNEL];
 int ret=RC_NOMEM;
 Inf_Rab();
 int i;
 static float xk[CHANNEL],yk[CHANNEL];

 nk=Cfg.nk;

DrvSetChannel(nk,nk);
if(DrvEnable(Cfg.Freq)==DRV_OK)
{
 float df;
 int sign;


switch(mode)
  {
  case exmSTAT:
    {
    float vol;
    int k,w;

    nr_view=2*Cfg.Sth[0].Nq;
    for(i=0;i<nk;i++)
     {
     xx[i]=new float[(unsigned)Cfg.nr_view];
     yy[i]=new float[(unsigned)Cfg.nr_view];
     if (!xx[i] || !yy[i]) goto done;
     }

    switch(Cfg.Sth[0].Mode)
      {
      case 0:
         df=(Cfg.Sth[0].Hi-Cfg.Sth[0].Lo)/nr_view;
         vol=Cfg.Def[0].StartVol;
         sign=1;
         for(k=0;k<nk;k++)
         {
         for(w=0;w<nk;w++) xk[w]=yk[w]=0;
         for(i=0;i<Cfg.nr_view;i++)
            {
            // ждем конца переходного процесса
            xk[k]=vol; for(w=0;w<Cfg.Sth[0].Wait*Cfg.Freq;w++) DrvVolume(xk,yk);
            // сохраняем значения
            for(w=0;w<nk;w++) { yy[k][w]=yk[w]; xx[k][w]=xk[w]; }
            // переходим к следуюей точке
            vol+=df*sign;
            if(vol>Cfg.Sth[0].Hi) sign=-1;
            if(vol<Cfg.Sth[0].Lo) sign=1;
            }
         }
         break;
      case 1: break;
      case 2: break;
      }
    }
    ret=RC_DONE;
    break;
  case exmACHH:
    {
    float df,fi[3],CC,freq,A;
    long N,w;
    int i,k;


    nr_view=Cfg.Ach[0].Nq;
    for(i=0;i<nk;i++)
     {
     xx[i]=new float[(unsigned)Cfg.nr_view];
     yy[i]=new float[(unsigned)Cfg.nr_view];
     if (!xx[i] || !yy[i]) goto done;
     }
    if(Cfg.Ach[0].FreqMin==0) Cfg.Ach[0].FreqMin=0.5;

    df=(Cfg.Ach[0].FreqMax-Cfg.Ach[0].FreqMin)/nr_view;
    for(k=0;k<nk;k++)
     {
     freq=Cfg.Ach[0].FreqMin;
     Wprtf(3,4,_RED|LGREY,"Цикл управления    :  %lu/%lu",(long)k+1,(long)nk);

    // -------------- плавное выведение
    for(i=0;i<nk;i++) yk[i]=Cfg.Def[k].PassiveVol; yk[k]=Cfg.Ach[k].Const;
    DrvGetVolDAC(xk);
    doRewind(Cfg.iniTime,xk,yk);

     for(i=0;i<nr_view;i++)
       {
       fi[0]=fi[1]=fi[2]=0;
       CC=2.0*cos(2.0*M_PI*freq/Cfg.Freq);
       N=(Cfg.Ach[k].Periods*Cfg.Freq)/freq;
//       A=Cfg.achAmpl/5;
       A=Cfg.Ach[k].Ampl/5;

       for(w=0;w<nk;w++) xk[w]=Cfg.Def[w].StartVol;

       Wprtf(4,4,_RED|LGREY,"Выполнено в цикле  :  %02u% ",(int)(i*100/nr_view));
       if(kbhit() && getch()==27) { k=nk; ret=RC_DONE; goto doneACHH; }

       for(w=0;w<N;w++)
          {
          fi[2]=fi[1]; fi[1]=fi[0];
//        xk[k]=Cfg.achConst/5.0+A*sin(2.0*M_PI*freq*Cfg.delt*w);
          xk[k]=Cfg.Ach[k].Const+A*sin(2.0*M_PI*freq*w/Cfg.Freq);
          DrvVolume(xk,yk); // yk[k]=xk[k];
          fi[0]=CC*fi[1]-fi[2]+yk[k];
          } /* for(w) */
       yy[k][i]= A*N ? 2.0*sqrt(fi[0]*fi[0]-CC*fi[0]*fi[1]+fi[1]*fi[1])/(N*A) : 0;
       xx[k][i]=freq;
       freq+=df;
       } /* for(i) */
doneACHH:
    // -------------- плавное выведение
     for(i=0;i<nk;i++) { yk[i]=Cfg.Def[i].PassiveVol; }  DrvGetVolDAC(xk);
     doRewind(Cfg.iniTime,xk,yk);
     } /* for(k) */
    } /* case exmACHH: */
    ret=RC_DONE;
    break;
  case exmWZST:
    break;
  }
}
done:
DrvDisable();

 Wclose();


 switch(ret)
   {
   case RC_NOMEM:
       Wperror("Нет памяти для проведения эксперимента");
       break;
   case RC_DONE:
       switch(mode)
         {
         case exmSTAT: saveSTAT(xx,yy); break;
         case exmACHH: saveACHH(xx,yy); break;
         case exmWZST: break;
         }
   }

 if(ret==0) graphExamine(mode);

 return;
}

static void saveSTAT(float **x,float **y)
{
 int f,i;
 unsigned long m;
 float valx,valy;
 FILE * ff;

 ff=fopen("last_sth.txt","w+t");
 f=open("last_sth.dat",O_WRONLY|O_BINARY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
 if(f)
   {
   write(f,&PID,sizeof(PID));
   write(f,&Cfg.Sth[0].Mode,sizeof(Cfg.Sth[0].Mode));
   write(f,&nk,sizeof(nk));
   write(f,&nr_view,sizeof(nr_view));
   for(i=0;i<nk;i++)
     {
     switch(Cfg.Sth[0].Mode)
       {
       case 0:  // перемещение
         for(m=0;m<nr_view;m++)
           { valx=x[i][m]; write(f,&valx,sizeof(valx)); }
         for(m=0;m<nr_view;m++)
           { valy=y[i][m]; write(f,&valy,sizeof(valy)); }
         break;
       case 1: // скорость
       case 2: // ускорение
         for(m=0;m<nr_view/2;m++)
           { valx=x[i][nr_view-m*2]; write(f,&valx,sizeof(valx)); }
         for(m=0;m<nr_view/2;m++)
           { valx=x[i][m*2+1]; write(f,&valx,sizeof(valx)); }
         for(m=0;m<nr_view/2;m++)
           { valy=y[i][nr_view-m*2]; write(f,&valy,sizeof(valy)); }
         for(m=0;m<nr_view/2;m++)
           { valy=y[i][m*2+1]; write(f,&valy,sizeof(valy)); }
         break;
       }
     }
   }
 close(f);
 fclose(ff);

 return;
}

static void saveACHH(float **x,float **y)
{
 int f,k;
 unsigned long i;
 float val,val1;
 FILE *ff;

 ff=fopen("last_ach.txt","w+t");
 f=open("last_ach.dat",O_WRONLY|O_BINARY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
 if(f)
   {
   write(f,&PID,sizeof(PID));
   write(f,&nk,sizeof(nk));
   write(f,&nr_view,sizeof(nr_view));
   for(k=0;k<Cfg.nk;k++)
     for(i=0;i<nr_view;i++)
       {
       val=x[k][i]; write(f,&val,sizeof(val));
       val=y[k][i]; write(f,&val,sizeof(val));
       }
   }
 close(f);
 fclose(ff);

 return;
}

void doRewind(int Margin,float *valB,float *valE)
{
 unsigned long i,k,d,time,nx=Margin;
 float oBuf[CHANNEL],iBuf[CHANNEL],Buf[CHANNEL];

 if(nx) time=(Cfg.Freq+0.5)/10;
 else   time=1;

 nx*=time;

 for(k=0;k<Cfg.nk;k++) oBuf[k]=valB[k];

 for(k=0;k<Cfg.nk;k++) Buf[k]=(oBuf[k]-valE[k])/(float)(nx+1);

 for(i=0;i<nx;i++)
     {
     DrvVolume(oBuf,iBuf);
     for(k=0;k<Cfg.nk;k++)
       {
//       X[k]->putNext(oBuf[k]);
//       Y[k]->putNext(iBuf[k]);
//       Yt[k]->getNext();
       oBuf[k]-=Buf[k];
       }
     }
 for(k=0;k<Cfg.nk;k++) oBuf[k]=valE[k];
 for(d=0;d<time;d++) DrvVolume(oBuf,iBuf);
 return;
}


