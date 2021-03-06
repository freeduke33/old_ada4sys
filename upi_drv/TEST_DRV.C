/*  */
/* #define USE_GRF */

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <time.h>
#include <sys\\stat.h>
#include <alloc.h>

#ifdef USE_GRF
#include "look_grf.h"
#include "look_def.h"
#endif

#include "DAC_ADC.h"

/***********************************************************************/

struct
  {
  float Freq,SinFreq,MeandrFreq;
  Word  Act,NumDAC,NumADC,Sig;
  float LineAmpl,SinAmpl,ConstVol,MeandrAmpl;
  } Cfg={10.0,0.6,0.1,  1,1,1,0, 1,1,1,1};

float SinArg;
char SigMsg[][10]={"�������","�����","������","����"};
volatile float BufDAC[50],BufADC[50];
Word tik,MaxVol,NumMAX;
double doit;


int TestLen=0;

float far *yBuf,*xBuf;

/***********************************************************************/

int ExitDrvError(int val)
{
 char msg[180];

 DrvGetErrorMsg(msg);
 printf("Fatal DAC/ADC driver error: %s\n",msg);
 DrvDeinitialize();
 exit(val);
 return 0;
}

int DrvError(char *fmt)
{
 char msg[180];

 DrvGetErrorMsg(msg);
 printf(fmt,msg);
 return 0;
}

int ClrScr(void)
{
 char str[80];

  clrscr();
  DrvGetName(str);
  printf("�������� ��� : %s\n",str);
  DrvGetCopyright(str);
  printf("����᪮� �ࠢ� : %s\n",str);

  return 0;
}

int ShowBuf(int mode)
{
 int i,x,y;

 x=wherex();
 y=wherey();

 cprintf("\n\r---------   --���--   --���--\n\r");
 for(i=0;i<NumMAX;i++)
   {
   cprintf("����� %02d:  ",i+1);
   if(i<Cfg.NumDAC) cprintf("%7.4f   ",BufDAC[i]);
   else         cprintf("%7s   ","");
   if(i<Cfg.NumADC) cprintf("%7.4f   \n\r",BufADC[i]);
   else         cprintf("%7s   \n\r","");
   }

 if(mode) gotoxy(x,y);
 return 0;
}

void ShowWorkInfo(clock_t t1,clock_t t2)
{
 unsigned long i;
 float WorkFreq;

   DrvGetSkipped(&i);

   if(t2!=t1) WorkFreq=(doit+i)*CLK_TCK/(t2-t1);
   else WorkFreq=0;

   printf("\n��\n������ ���� ������� ....");
   while(kbhit()) getch(); getch();

   ClrScr();
   printf("\n"
          "    ������������      ���祭��\n"
          "-------------------- ---------------\n"
          "�������� ����    :%12.4f ��\n"
          "�����᪠� ���� :%12.4f ��\n"
          "�ய�饭� ⠪⮢ ��:%7lu ��.\n\n"
          "������ <SPACE> ��� �த������� ...",Cfg.Freq,WorkFreq,i);
 return;
}

/***********************************************************************/

int huge CalcSignal(void)
{
 int i;
 float tmp,vol;

 tik++; doit++;
 switch(Cfg.Sig)
   {
   case 0:  /* Const */  tmp=Cfg.ConstVol;                         break;
   case 1:  /* Sin   */  tmp=Cfg.SinAmpl*sin(SinArg*tik/Cfg.Freq); break;
   case 2:  /* Meandr*/  tmp= tik%2 ? -Cfg.MeandrAmpl/2 : Cfg.MeandrAmpl/2; break;
   case 3:  /* Line  */  tmp=(Cfg.LineAmpl*(tik%101)/100);         break;
   }
 vol=Cfg.Act ? 0 : tmp;
 for(i=0;i<Cfg.NumDAC;i++) BufDAC[i]=vol;
 if(Cfg.Act) BufDAC[Cfg.Act-1]=tmp;
 return 0;
}

/***********************************************************************/

int TestA(void)
{
 unsigned long num,i;
 int step,val,j;
 clock_t t1,t2;

 ClrScr();
 printf("\n ******* ��אַ� �ࠢ����� ��� *******"
        "\n ������ �᫮ �祪 (0 �᫨ ����࠭�祭�) :");
 scanf("%ld",&num);

 step = num ? 1 : 0;
 if(step)
   {
   if(yBuf) farfree(yBuf);
   if(xBuf) farfree(xBuf);
   yBuf=farcalloc(Cfg.NumADC,num*sizeof(float));
   xBuf=farcalloc(Cfg.NumDAC,num*sizeof(float));
   TestLen=num;
   }
 num-=step;

 printf("\n����஢���� ...");
 val=DrvEnable(Cfg.Freq);
 if(val==DRV_OK)
   {
   tik=0; doit=0; t1=clock(); DrvClrSkipped();
   for(i=0;i<=num;i+=step)
      {
      CalcSignal();
      DrvVolume(BufDAC,BufADC);
      ShowBuf(1);
      if(step)
        {
        if(xBuf) for(j=0;j<Cfg.NumDAC;j++) xBuf[i*Cfg.NumDAC+j]=BufDAC[j];
        if(yBuf) for(j=0;j<Cfg.NumADC;j++) yBuf[i*Cfg.NumADC+j]=BufADC[j];
        }
      if(kbhit() && getch()==27) { t2=clock(); printf(" *** ��ࢠ�� *** ");break; }
      }
   DrvDisable(); if(i>=num) t2=clock();
   ShowBuf(0);
   ShowWorkInfo(t1,t2);
   }
 else DrvError("\007\n�訡�� ���樠����樨 ��� :%s\n������ <SPACE> ...");
 while(kbhit()) getch();
 while(getch()!=' ') putch('\007');
 return 0;
}

void interrupt HookFunc(void)
{
 int i,j;
// float k;

 DrvGetVolADC(BufADC);
// ShowBuf(1);
 CalcSignal();
// k=1;
// for(i=0;i<4000;i++) k=485.0*(4578.0+i)+k*0;

 DrvSetVolDAC(BufDAC);
 return;
}

int TestB(void)
{
 unsigned long num,step,i,val;
 int j;
 clock_t t1,t2;

 ClrScr();
 printf("\n **** ��ࠢ����� ��� � 䮭���� ०��� ****"
        "\n ������ �᫮ �祪 (0 �᫨ ����࠭�祭�) :");
 scanf("%ld",&num);

 /* ---------------------[ step 1 ]-----------------*/
 step = num ? 1 : 0;
 if(step)
   {
   if(yBuf) farfree(yBuf);
   if(xBuf) farfree(xBuf);
   yBuf=farcalloc(Cfg.NumADC,num*sizeof(float));
   xBuf=farcalloc(Cfg.NumDAC,num*sizeof(float));
   TestLen=num;
   }
 num-=step;

 DrvSetHook(HookFunc);
 /* ---------------------[ step 2 ]-----------------*/
 printf("\n����஢���� ...");
 val=DrvEnable(Cfg.Freq);
 if(val==DRV_OK)
   {
   tik=0; doit=0; t1=clock(); DrvClrSkipped();
   for(i=0;i<=num;i+=step)
     {
     DrvWaitDAC_ADC(); // cprintf("$");
     ShowBuf(1);
     if(step)
       {
       if(xBuf) for(j=0;j<Cfg.NumDAC;j++) xBuf[i*Cfg.NumDAC+j]=BufDAC[j];
       if(yBuf) for(j=0;j<Cfg.NumADC;j++) yBuf[i*Cfg.NumADC+j]=BufADC[j];
       }
     if(kbhit() && getch()==27) { t2=clock(); printf(" *** ��ࢠ�� *** ");break; }
     }
   /* ---------------------[ step 3 ]-----------------*/
   DrvDisable(); t2=clock();
   ShowBuf(0); DrvClrHook();
   ShowWorkInfo(t1,t2);
   }
 else DrvError("\007\n�訡�� ���樠����樨 ��� :%s\n������ <SPACE> ...");

 DrvClrHook();
 while(kbhit()) getch();
 getch();
 return 0;
}

#ifdef USE_GRF
void getX(float *x,float *y,triadGRF *p,int pos)
{
 *y=pos;
 if(Cfg.NumDAC<=p->dX) *x=0;
 else *x=xBuf[Cfg.NumDAC*pos+p->dX];
 return;
}

void getY(float *x,float *y,triadGRF *p,int pos)
{
 *y=pos;
 if(Cfg.NumADC<=p->dX) *x=0;
 else *x=yBuf[Cfg.NumADC*pos+p->dX];
 return;
}

#include "test_drv.inc"
#endif

int ShowTest()
{
#ifdef USE_GRF
 int i;

 for(i=0;i<20;i++) grf[i].end=grf[i].Xmax=TestLen;

 if(TestLen>0 && InitGRF(grfGRAPHICS)==okGRF)
   {
   LookGRF(20,grf);
   DeinitGRF(grfTEXT);
   }
#else
 printf("\n Graphics currently not available !!!");
#endif
 return 0;
}


int Signal(void)
{
 char val;

 ClrScr();
 printf("\n   ��⨢�� ��⮢� ᨣ��� : %s\n"
        "\n    �롥�� ���� ᨣ��� ��� �஢�ન ���:"
        "\n      1) ����ﭭ� �஢���"
        "\n              �஢���=%g"
        "\n      2) ����ᮨ��"
        "\n              ����=%g"
        "\n              ������㤠=%g"
        "\n      3) ������"
        "\n              ����=%g"
        "\n              ������㤠=%g"
        "\n      4) ����"
        "\n              ������㤠=%g"
        "\n      0) ��������� ��ࠬ��஢ ��⨢���� ᨣ����\n"
        "\n   ��� �롮�> ",SigMsg[Cfg.Sig],
        Cfg.ConstVol,Cfg.SinFreq,Cfg.SinAmpl,
        Cfg.MeandrFreq,Cfg.MeandrAmpl,Cfg.LineAmpl);
 val=getche()-'0';
 if(val>=0 && val<5)
    {
    if(val!=0) Cfg.Sig=val-1;
    else  switch(Cfg.Sig)
       {
       case 0: /*CONST*/
           printf("  ������ �஢��� ����ﭭ��� ᨣ���� :");
           scanf("%f",&Cfg.ConstVol);
           break;
       case 1: /*SIN*/
           printf("  ������ ����� � �������� ᨭ�ᮨ��: ");
           scanf("%f %f",&Cfg.SinFreq,&Cfg.SinAmpl);
           break;
       case 2: /*MEANDR*/
           printf("  ������ ����� � �������� ������: ");
           scanf("%f %f",&Cfg.MeandrFreq,&Cfg.MeandrAmpl);
           break;
       case 3: /*LINE*/
           printf(" ������ �������� :");
           scanf("%f",&Cfg.LineAmpl);
       }
    }
 else putch(7);


 return 0;
}

int Channel(void)
{
 TestLen=0;
 printf("\n ������ ����� �᫮ ������� ��� : ");
 scanf("%d",&Cfg.NumDAC);
 printf(" ������ ����� �᫮ ������� ��� : ");
 scanf("%d",&Cfg.NumADC);

 NumMAX= Cfg.NumDAC>Cfg.NumADC ? Cfg.NumDAC : Cfg.NumADC;
 if(NumMAX>15) NumMAX=15;
 return 0;
}

int SetFreq(void)
{
 printf("\n ������ ����� ����� �ࠢ����� (>0): ");
 scanf("%f",&Cfg.Freq);
 return 0;
}

int ActChannel(void)
{
 printf("\n ������ ����� �ࠢ�塞��� ������ ��� (0 - ��):");
 scanf("%u",&Cfg.Act);
 if(Cfg.Act>Cfg.NumDAC) Cfg.Act=1;
 return 0;
}

int TestDigital(void)
{
 int i;
 Byte buf1[16]={1,0,1,1,0,0,0,0,0,0,0,0},buf2[16];

 ClrScr();
 printf("\n***************** ���஢� ������ *******************");
 while(!kbhit())
   {
   for(i=0;i<9;i++)
      { DrvSetDigOUT(i,buf1[i]); delay(10); }
   for(i=0;i<9;i++)
      { DrvGetDigIN(i,&buf2[i]); delay(10); }
   gotoxy(1,6);
   for(i=0;i<9;i++) printf("����� %d:\t%d --> %x\t\n\r",i,buf1[i] ? 1:0,(int)buf2[i]);

   }
 return 0;
}

/***********************************************************************/

int main()
{
 int val;
 Word tmpDAC,tmpADC;
 UnitUPI set;

 printf("\nTesting UPI's driver written by D.Kokorev (C) Samara 1995.\n");

 val=open("test_drv.ini",O_RDONLY|O_BINARY);
 if(val>0)
   {
   read(val,&Cfg,sizeof(Cfg));
   close(val);
   }

 if(DrvIsPresent())
   {
   val=DrvInitialize();
   if(val!=DRV_OK) ExitDrvError(val);
   DrvGetMaxChannel(&tmpDAC,&tmpADC);
   Cfg.NumDAC= Cfg.NumDAC>tmpDAC ? tmpDAC : Cfg.NumDAC;
   Cfg.NumADC= Cfg.NumADC>tmpADC ? tmpADC : Cfg.NumADC;
   NumMAX= Cfg.NumDAC>Cfg.NumADC ? Cfg.NumDAC : Cfg.NumADC;
   if(NumMAX>15) NumMAX=15;
   DrvGetInfoDAC(&MaxVol);

   for(val=0;val<8;val++)
     {
     set.Unit=0; set.Slot=0; set.Channel=val;
     DrvSetupDAC(val,&set);
     }

   for(val=0;val<NumMAX;val++)
     {
     set.Unit=0; set.Slot=1; set.Channel=val;
     DrvSetupADC(val,&set);
     }

   val=1;
   while(val)
      {
      SinArg=(2.0*3.1416)*Cfg.SinFreq;
      ClrScr();
      printf("\n\n  --- ������� ���� --- \n"
          "\n1) ���� �: ��אַ� �ࠢ����� ���"
          "\n2) ���� �: ��ࠢ����� ��� � 䮭���� ०���"
          "\n3) ��ᬮ�� १���⮢ ��⮢ �� ��䨪��"
          "\n4) �������� ����� ����� �ࠢ����� ᨣ�����"
          "\n5) �������� �᫮ ��⨢��� ������� ���"
          "\n6) �������� �ࠪ�� ��⮢��� ᨣ����"
          "\n7) �������� ����� �ࠢ�塞��� ������ ���"
          "\n8) ���� �: ���஢� ������"
        "\n\n0) ��� � DOS"
          "\n -------------------------------------"
          "\n �����=%7.3f, ���=%u(%u), ���=%u, ������ :%s"
          "\n -------------------------------------"
          "\n\n��� �롮� >",Cfg.Freq,Cfg.NumDAC,Cfg.Act,Cfg.NumADC,SigMsg[Cfg.Sig]);
      while(kbhit()) getch();
      switch(getche())
         {
         case '1':  TestA(); break;
         case '2':  TestB(); break;
         case '3':  ShowTest(); break;
         case '4':  SetFreq(); break;
         case '5':  Channel(); break;
         case '6':  Signal();  break;
         case '7':  ActChannel(); break;
         case '8':  TestDigital(); break;
         case '0':
              DrvDeinitialize();
              printf("\n\n ****** ����� ࠡ��� *******\n\n");
              val=0;
              break;
         default: cprintf("\b \007");
         }
      }
   }
 else printf("DAC/ADC driver mast be installed previosly.\n");

 val=open("test_drv.ini",O_BINARY|O_WRONLY|O_CREAT,S_IREAD|S_IWRITE);
 if(val>0)
   {
   write(val,&Cfg,sizeof(Cfg));
   close(val);
   }
 return 0;
}

/* ====== END OF FILE ====== */
