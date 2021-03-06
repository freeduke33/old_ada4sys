#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <conio.h>
#include <math.h>
#include <sys/stat.h>

#define Uses_Buffer
#ifdef DETECTIVE
#  define Uses_StaticMemBuf
#endif
#include "adaptive.hpp"

#include "run.h"
#include "run.hpp"
#include "dac_adc.h"
#include "config.h"
#include "common.h"

extern dumpType WorkingPanel[];

#define  UNARY(a)  (((a)+1)/2.0)
#define  SIGNED(a) (((a)*2.0)-1)

int Calc(int k,long N,float *x,float *y,Buffer *yt,long first)
{
 double *X;
 double su,su1,val,Y;
 long m,v;
 int q;
// FILE *F;

#ifdef DETECTIVE
 float x1,x2;
#endif

// F=fopen("debug.log","a+t");


 X=new double[N];
 if(!X) /* no memory */
  { tprintf(WORKING_PART_STATUS," �������筮 ����� ��� ࠡ��� "); return 0; }


 q=Cfg.NMDL.Def[k].q;
 Y=UNARY(y[q]); if(fabs(Y)<1e-60) Y+=1e-30;


 for(m=0;m<N-q;m++)
   {
   tprintf(WORKING_PART_PERSENT,"%3d",(int)((first+m)*100/Cfg.nr));
   if(kbhit() && getch()==27) /* STOP */
     { tprintf(WORKING_PART_STATUS," ��ࢠ�� �����஬ "); return 0; }

   for(v=0,su=0;v<=m;v++)
     {
     val=yt->get(v+q);
     su+=UNARY(val)*UNARY(x[m-v]);
     }
   for(v=1,su1=0;v<=m;v++) su1+=UNARY(y[v+q])*UNARY(X[m-v]);

   X[m]=SIGNED( (su-su1)/Y );

   if(X[m]>1) X[m]=1; if(X[m]<-1) X[m]=-1;

//   fprintf(F,"\n%lu \t%f\t%f\t%f\t%f |\t%f %f",m,X[m],x[m],y[m],yt->get(m),su,su1);

   }

 for(m=0;m<N-q;m++) x[m]=X[m];

// fclose(F);

 delete X;
 return 1; /* continue */
}


int doNomodel(void)
{
 Buffer *yt[CHANNEL];
 float xk[CHANNEL],yk[CHANNEL];
 float *x[CHANNEL],*y[CHANNEL];
 float val;
 int i,k,f;
 unsigned long l,t,N;
 short int _PID=0,act=1;

 tprintf(WORKING_TITLE,"��ࠢ����� � �।���⥫�� ���⮬");


 for(i=0;i<Cfg.nk;i++) if(Cfg.Def[i].isActive)
   {
   MakeEtalon(i,&yt[i]);
   x[i]= new float[Cfg.nr];
   y[i]= new float[Cfg.nr];
   if(!x[i] || !y[i] || !yt[i] || Cfg.nr>(64000/sizeof(float)))
      { tprintf(WORKING_PART_STATUS," ��� ᢮������ ����� ��� �믮������ ����樨 "); goto done; }
   for(l=0;l<Cfg.nr;l++) x[i][l]=yt[i]->get(l);
//   if(!x[i][0]) x[i][0]=FIRST;
   yt[i]->reset();
   }

 tprintf(WORKING_PART,"���樠������");
 DrvSetChannel(Cfg.nk,Cfg.nk);

 if(DrvEnable(Cfg.Freq)==DRV_OK)
   {
   for(k=0;k<=Cfg.NMDL.Cycles;k++)
     {
     if(k)
        {
        for(i=0;i<Cfg.nk;i++) if(Cfg.Def[i].isActive)
           {
           tprintf(WORKING_PART,"%d/%d: ����� ����㦥��� ������ %d",k,Cfg.NMDL.Cycles,i+1);
           tprintf(WORKING_PART_PERSENT,"%3d",0);
           yt[i]->reset();
           yt[i]->gotoPos(Cfg.NMDL.Def[i].Margin);
           N=Cfg.NMDL.Def[i].nr ? Cfg.NMDL.Def[i].nr : Cfg.nr;
           for(t=Cfg.NMDL.Def[i].Margin;t<Cfg.nr;t+=N)
             {
             if(t+N>Cfg.nr) N=Cfg.nr-t;
             if(!Calc(i,N,&x[i][t],&y[i][t],yt[i],t)) goto stop;
             yt[i]->gotoPos(N);
             }
           }
        /* � ०��� � �।���⥫�� ��ᬮ�஬ �ᥣ�� ⮫쪮 1 横� */
        /* ��� ����㦥��� १���⠬� ���� */
        if(!Cfg.NMDL.isBatch) { goto stop; }
        }

     tprintf(WORKING_PART,"%d/%d: �����",k,Cfg.NMDL.Cycles);
     tprintf(WORKING_PART_PERSENT,"%3d",0);
     if(kbhit() && getch()==27) goto stop;
     for(i=0;i<Cfg.nk;i++) xk[i]=Cfg.Def[i].StartVol;
     DrvGetVolDAC(yk);
     doRewind(Cfg.Freq,Cfg.iniTime,Cfg.nk,yk,xk);

     tprintf(WORKING_PART,"%d/%d: ����㦥���",k,Cfg.NMDL.Cycles);
     for(l=0;l<Cfg.nr;l++)
       {
       if(kbhit() && getch()==27) goto stop;
       tprintf(WORKING_PART_PERSENT,"%3d",l*100/Cfg.nr);
       for(i=0;i<Cfg.nk;i++) if(Cfg.Def[i].isActive) xk[i]=x[i][l];
       DrvVolume(xk,yk);
       for(i=0;i<Cfg.nk;i++) if(Cfg.Def[i].isActive) y[i][l]=yk[i];
       }

     if(k<Cfg.NMDL.Cycles)
       {
       tprintf(WORKING_PART,"%d/%d: ��⠭��",k,Cfg.NMDL.Cycles);
       for(i=0;i<Cfg.nk;i++) xk[i]=Cfg.Def[i].PassiveVol;
       DrvGetVolDAC(yk);
       doRewind(Cfg.Freq,Cfg.iniTime,Cfg.nk,yk,xk);
       if(kbhit() && getch()==27) goto stop;
       }
     }

stop:
   tprintf(WORKING_PART,"����ࢠ��");
//   tprintf(Cfg.ScreenX,Cfg.ScreenY+1,"Done");
   for(i=0;i<Cfg.nk;i++) xk[i]=Cfg.Def[i].PassiveVol;
   DrvGetVolDAC(yk);
   doRewind(Cfg.Freq,Cfg.iniTime,Cfg.nk,yk,xk);

   DrvDisable();

   tprintf(WORKING_PART,"���࠭���� १���⮢ ���⠭��");
   tprintf(WORKING_PART_PERSENT,"%3d",0);

   f=open("lastwork.dat",O_BINARY|O_TRUNC|O_CREAT|O_RDWR,S_IREAD|S_IWRITE);
   BufInit();
   BufWrite(f,&_PID,sizeof(_PID));
   BufWrite(f,&Cfg.nk,sizeof(Cfg.nk));
   BufWrite(f,&Cfg.Freq,sizeof(Cfg.Freq));

   for(k=0;k<Cfg.nk;k++)
     {
     l=Cfg.nr-1-Cfg.NMDL.Def[k].q;
     BufWrite(f,&l,sizeof(Cfg.nr));
     for(i=0;i<3;i++) BufWrite(f,&act,sizeof(act));
     if(Cfg.Def[k].isActive) yt[k]->reset();
     }

   N=Cfg.nr-1-Cfg.NMDL.Def[k].q;
   for(l=0;l<N;l++) for(k=0;k<Cfg.nk;k++)
       {
       tprintf(WORKING_PART_PERSENT,"%3d",(l*Cfg.nk+k)*100/(Cfg.nk*N));
       val=0;
       if(Cfg.Def[k].isActive) val=x[k][l];       BufWrite(f,&val,sizeof(float));
       if(Cfg.Def[k].isActive) val=y[k][l];       BufWrite(f,&val,sizeof(float));
       if(Cfg.Def[k].isActive) val=yt[k]->get(l); BufWrite(f,&val,sizeof(float));
       }
   BufFlush(f);
   close(f);


   for(i=0;i<Cfg.nk;i++) if(Cfg.Def[i].isActive)
     {
     if(yt[i]) delete yt[i];
     if(x[i])  delete x[i];
     if(y[i])  delete y[i];
     }
   tprintf(WORKING_PART_PERSENT,"%3d",100);
   }
 else tprintf(WORKING_PART_STATUS," �訡�� �ࠩ��� ��� ");

done:
 return 0;
}

#ifdef DETECTIVE

int main(int argc,char *argv[])
{
 FILE *f,*fo;
 float *x[16],*y[16],*yt[16];

 Buffer *YT[16];
 char tmp1[30],tmp2[30],tmp3[50];
 unsigned nr;
 float freq,val,val1,val2;
 unsigned nk,winSize,skipped,i,t,N,q;
 int ret=0;

 setvbuf(stdout,NULL,_IONBF,0);

 printf("�஢�ઠ ࠡ��ᯮᮡ���� �����⬠ �।���⥫쭮�� ���� �����饭��.\n"
        "\"���⥬� �ਠ�\" (C) 1997.\n\n");
 if(argc!=6)
   {
   printf("\n�����:\n"
          " :\> ada_test <UPR-䠩�> <TEM-䠩�> <�ய��> <����> <�����>\n");
   exit(-1);
   }

 printf("��⮪�� �ࠢ����� : <%s>\n"
        "����⭮� ����㦥���: <%s>\n",argv[1],argv[2]);

 skipped=atoi(argv[3]);
 winSize=atoi(argv[4]);
 q=atoi(argv[5]);


 f=fopen(argv[1],"r+t");
 fo=fopen(argv[2],"w+t");
 if(f && fo)
   {
   printf("�����⮢�� � ࠡ�� ...");
   fscanf(f,"%s %s %s",tmp1,tmp2,tmp3);
   fscanf(f,"%s %s %d",tmp1,tmp2,&nr);
   fscanf(f,"%s %s %f",tmp1,tmp2,&freq);
   fscanf(f,"%s %s %d",tmp1,tmp2,&nk);

//   for(i=0;i<nk;i++)
   i=0;
     {
     x[i]=new float[nr];
     y[i]=new float[nr];
     yt[i]=new float[nr];
     YT[i]=new StaticMemBuf(nr,yt[i]);
     if( !x[i] || !y[i] || !yt[i] || !YT[i]) { ret=1; goto done; }
     }
   for(t=0;t<nr;t++)
     {
     fscanf(f,"%d",&N);
     for(i=0;i<nk;i++)
       {
       fscanf(f,"%f %f %f",&val,&val1,&val2);
       if(i==0) { x[i][t]=val/5.0; y[i][t]=val1/5.0; yt[i][t]=val2/5.0; }
       }
     }
   fclose(f);

   Cfg.nr=nr;
   Cfg.NMDL.Def[0].q=q;

   printf("Ok\n����� ᨣ���� ����㦥��� ...");
   tpreserv();
//   for(i=0;i<Cfg.nk;i++)
   i=0;
      {
      YT[i]->reset();
      YT[i]->gotoPos(skipped);
      N=winSize ? winSize : Cfg.nr;
      for(t=skipped;t<Cfg.nr;t+=N)
        {
        if(t+N>Cfg.nr) N=Cfg.nr-t;
        if(!Calc(i,N,&x[i][t],&y[i][t],YT[i],t)) { ret=1; goto done; }
        YT[i]->gotoPos(N);
        }
      }
   trestore();
   printf("Ok\n���࠭���� 䠩�� ����㦥��� ...");
   fprintf(fo,"%d %f %d\n",nk,freq,nr);
   for(t=0;t<nr;t++)
     {
     for(i=0;i<nk;i++) fprintf(fo,"%f\t", i ? 0:x[i][t]*5.0);
     fprintf(fo,"\n");
     }
   fprintf(fo,"\n"); fclose(fo);
   printf("Ok\n");
   }
done:
  switch(ret)
    {
    case 0: printf("������ �����訫��� �ᯥ譮.\n"); break;
    case 1: printf("��� ����� ��� �믮������ ����樨.\n"); break;
    }
 return ret;
}

#endif
