#include <stdio.h>
#include <alloc.h>
#include <conio.h>
#include <time.h>
#include <dos.h>
#include <math.h>

#include "dac_adc.h"

float huge *fBuf;
Word  huge *wBuf;
enum {CONST=0, SIN=1, MEANDR=2, DELTA=3} Test;
enum {isWord=0, isFloat=1 } TestBuf;
unsigned long TestTime,Tik;
void (*TestFunc)(void);

static void DoTestWord(void)
{
 Word iBuf[10],oBuf[10];
 int i;

 for(i=0;i<10;i++) switch(Test)
   {
   case CONST: iBuf[i]=1024; break;
   case SIN:   iBuf[i]=500+500*sin(2.0*M_PI*Tik*0.0097);
   case MEANDR:
   case DELTA:
   default: break;
   }
 _DrvVolume(iBuf,oBuf);
 for(i=0;i<10;i++) wBuf[i*TestTime+Tik]=oBuf[i];
 Tik++;
 return;
}

static void DoTestFloat(void)
{
 switch(Test)
   {
   case CONST:
   case SIN:
   case MEANDR:
   case DELTA:
   default: break;
   }
 return;
}

static void DoTest(void)
{
 char led[]=".oOo";
 int i=0;
 clock_t clk=0;

 Tik=0;
 printf("Testing ...  ");
 if(TestFunc)
   {
   while(Tik<TestTime)
     {
     if(kbhit() && getch()==27) { cprintf("*** User Break *** "); break; }
     (*TestFunc)();
     if(clock()>=clk+3) { putch(led[i=(++i)%4]); putch('\b'); clk=clock(); }
     }
   printf("Ok.\n");
   }
 else cprintf("\007FAILED.\n");
 return;
}

static void SetTestMode(void)
{
/* int i; */
 char nSignal[4][10]={"CONST","SIN","MEANDR","DELTA"};
 char nBuf[2][10]={"WORD","FLOAT"};

 while(kbhit()) getch();  // clears keyboard buffer

 printf("\nEnter tiks for doing test (>0)? ");
 TestTime=100;
 scanf("%lu",&TestTime);

 printf("\nSelect test mode [0-Const, 1-Sin, 2-Meandr, 3-Delta]? ");
 switch(getch())
   {
   case '0': Test=CONST;  break;
   case '1': Test=SIN;	  break;
   case '2': Test=MEANDR; break;
   case '3': Test=DELTA;   break;
   default: Test=CONST;
   }
 printf("\nSelect output mode [0-Word, 1-Float]? ");
 if(getch()=='1') TestBuf=isFloat;
 else		  TestBuf=isWord;

 switch(TestBuf)
  {
  case isWord:
	  TestFunc=DoTestWord;
	  wBuf=farcalloc(10*sizeof(Word),TestTime);
	  if(!wBuf) TestFunc=NULL;
	  break;
  case isFloat:
	  TestFunc=DoTestFloat;
	  fBuf=farcalloc(10*sizeof(float),TestTime);
	  if(!fBuf) TestFunc=NULL;
	  break;
  default: TestFunc=NULL; break;
  }
 printf("\n\nYou selection"
	"\n\ttiks for doing test :%lu"
	"\n\ttest's signal :%s"
	"\n\toutput mode :%s"
	"\n ===[ Press any key ]===\n"
	,TestTime,nSignal[Test],nBuf[TestBuf]);
 while(kbhit()) getch();
 getch();
 return;
}


static SaveTestResult(void)
{
 struct date d;
 FILE *f;
 int i;

 f=fopen("test.out","w_t");
 if(f)
   {
   printf("Write results of test ... ");
   getdate(&d);
   fprintf(f,"++1++ Дата_тестирования_эмулятора: %d.%d.%d\n",d.da_day,d.da_mon,d.da_year);
   fprintf(f,"++2++ Частота_выборки_(Гц): %g\n",18.2);
   fprintf(f,"++3++ Число_отсчетов_(штук): %lu\n",TestTime);
   fprintf(f,"++4++ Число_каналов_сбора_информации: %u\n",10);
   for(Tik=0;Tik<TestTime;Tik++)
     {
     fprintf(f,"\n%010lu",Tik+1);
     switch(TestBuf)
       {
       case isWord:  for(i=0;i<10;i++) fprintf(f,"\t%10.4g",(float)wBuf[i*TestTime+Tik]);
		     break;
       case isFloat: for(i=0;i<10;i++) fprintf(f,"\t%10.4g",fBuf[i*TestTime+Tik]);
		     break;
       }
     }
   fclose(f);
   printf("Ok.\n");
   }
 return;
}


void TestDriver(void)
{
 if(DrvIsPresent())
   {
   if(DrvInitialize()==DRV_OK)
     {
     SetTestMode();
     DrvSetChannel(10,10);
     if(DrvEnable(18)==DRV_OK)
       {
       DoTest();
       DrvDisable();
       SaveTestResult();
       }
     else printf("\007FAILED to enable UPI's emulator.\n");
     DrvDeinitialize();
     }
   else printf("\007FAILED to initialize UPI's driver or UPI's emulator.\n");
   }
 else printf("\007FAILED to detect UPI's driver.\n");
 return;
}

