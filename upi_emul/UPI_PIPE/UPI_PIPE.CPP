#include <io.h>
#include <math.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#include <oscill.hpp>
#include <function.hpp>
#include <trace.hpp>
#include <dualmass.hpp>

#include "upi_pipe.hpp"


#define min(a,b) ( (a)>(b) ? (b) : (a) )

#define ADC 2048
#define DAC 2048

#define ToADC(a)     ( (int) ((a)*(float)ADC+ADC)               )
#define VerifyADC(a) ( (a<0) ? 0 : (a)>=ADC*2 ? ADC*2-1 : (a)   )
#define FromDAC(a)   ( ((float)(((int)(a))-(int)DAC)) /(int)DAC )


#if !defined(M_PI)
#  define M_PI 3.1415
#endif

#ifdef __WATCOMC__
#include <time.h>
#define randomize() (srand(clock()))
#endif

UPI_pipe::UPI_pipe(char *filename,int s)
{
 int f,i,k,j=0;
 char pipe_name[]="\\PIPE\\UPI_Emulator";
 int ret;
static struct _Function Fm;
static struct _Oscill   Km;
static struct _DualMass Mm;
static struct _TestBuildModel Tm;
static struct _param_3w tmp;


  chlADC=chlDAC=10;
  Freq=100;

#define OPEN NP_ACCESS_DUPLEX | NP_NOINHERIT
#define PIPE NP_TYPE_BYTE | NP_READMODE_BYTE | NP_WAIT

 Complete=0;
 ret=DosCreateNPipe(pipe_name,&pipe_handle,OPEN,PIPE+s,100,100,NP_DEFAULT_WAIT);
 if(ret)
   {
   printf("I can't create named pipe.\n");
   return;
   }

 Complete=1;
 printf("Loading parameters from '%s'...",filename);
 f=open(filename,O_BINARY|O_RDONLY);
 if(f!=-1 &&  (k=read(f,&tmp,sizeof(tmp))) == sizeof(tmp) )
   {
   for(i=0;i<Channel;i++)
     {
     for(k=0;k<MAX_WAIT;k++) D[i].wy[k]=0;
     for(k=0;k<3;k++)
       {
       D[i].xAmpl[k]=tmp.xAmpl[i][k];
       D[i].xFaza[k]=tmp.xFaza[i][k];
       D[i].xFreq[k]=tmp.xFreq[i][k];
       }
     D[i].imi_Const=tmp.imi_Const[i];
     D[i].fdds=tmp.fdds[i];
     D[i].wait=min(tmp.wait[i],MAX_WAIT);

     for(k=0;k<Channel;k++) wzk[i][k]=tmp.wzk[i][k];
     for(k=0;k<4;k++)
       {
       D[i].Zveno[k]=NULL;
       D[i].Z_Type[k]=tmp.Z_Type[i][k];
       switch(D[i].Z_Type[k])
         {
         case 1: // функциональное звено
            D[i].subZ_Type[k]=tmp.Fm[i][k].Type;
            switch(tmp.Fm[i][k].Type)
              {
              case 0: D[i].Zveno[k]=new SingleFunction(); break;
              case 1: D[i].Zveno[k]=new DoubleFunction(); break;
              case 2: D[i].Zveno[k]=new FherdFunction();  break;
              case 3: D[i].Zveno[k]=new SqrtFunction();   break;
              case 4: D[i].Zveno[k]=new SpeedFunction();  break;
              case 5: D[i].Zveno[k]=new DSpeedFunction(); break;
              }
            if(D[i].Zveno[k])
              {
              Fm.k=tmp.Fm[i][k].k;
              D[i].Zveno[k]->SetParam(&Fm);
              }
            else Complete=0;
            break;
         case 2: // апериодическое звено
            break;
         case 3: // колебательное звено
            D[i].Zveno[k]=new TransferOscill();
            goto _oscill;
         case 4: //  --/-- перемещение
            D[i].Zveno[k]=new TransferOscillA();
            goto _oscill;
         case 5: //  --/-- скорость
            D[i].Zveno[k]=new SpeedOscillA();
            goto _oscill;
         case 6: //  --/-- ускорение
            D[i].Zveno[k]=new DSpeedOscillA();
_oscill:
            if(D[i].Zveno[k])
              {
              Km.Freq=tmp.Km[i][k].Freq;
              Km.Betta=tmp.Km[i][k].Betta;
              D[i].Zveno[k]->SetParam(&Km);
              }
            break;
         case 11:// двухмассовое звено
            D[i].Zveno[k]=new DualMass();
            if(D[i].Zveno[k])
              {
              int j;
              for(j=0;j<2;j++) Mm.Freq[j]=tmp.Mm[i][k].Freq[j];
              for(j=0;j<2;j++) Mm.Betta[j]=tmp.Mm[i][k].Betta[j];
              D[i].Zveno[k]->SetParam(&Mm);
              }
            break;
         case 12:// проверка расчета параметров модели
            D[i].Zveno[k]=new TestBuildModel();
            if(D[i].Zveno[k])
              {

              Tm.p=tmp.Tm[i][k].p;
              Tm.E=tmp.Tm[i][k].E;
              for(j=0;j<NMAX;j++) Tm.C[j]=tmp.Tm[i][k].C[j];
              for(j=0;j<NMAX;j++) Tm.Lam[j]=tmp.Tm[i][k].Lam[j];

              D[i].Zveno[k]->SetParam(&Tm);
              }
            break;
         }
       }
     }
   printf(" Ok.\n");
   }
 else
   {
   printf("Unable to open configuration file.\n");
   Complete=0;
   }

 if(Complete)
   {
   for(i=0;i<Channel;i++)
      {
      for(k=0;k<D[i].wait;k++) D[i].wy[k]=0;
//      for(k=0;k<3;k++)         D[i].tik[k]=0;
      }
   }
 if(f!=-1) close(f);
 if(Complete) ShowConfig();

 return;
}


void UPI_pipe::ShowConfig(void)
{
 char name[][30]={"Апериодическое","Колебательное",
                  "Колеб.(перемещение)","Колеб.(скорость)","Колеб.(Ускорение)",
                  "СИНУСОИДА","ПОЛИГАРМОНИКА","ПСЕВДО-БЕЛЫЙ ШУМ","ИНВЕРСИЯ",
                  "Двухмассовое","Тест модели"};
 char fname[6][15]={"Линейное","Квадратическое","Кубическое","Гистерезис","Скорость","Ускорение"};
 int i,k;

 printf("\n┌Канал┬── Описание канала (список звеньев + помеха) ────────────────\n");
 for(i=0;i<Channel;i++)
   {
   printf("│%4u │  ",i+1);
   for(k=0;k<4;k++) if(D[i].Z_Type[k])
     {
     if(D[i].Z_Type[k]==1) printf("%s",fname[D[i].subZ_Type[k]]);
     else printf("%s",name[D[i].Z_Type[k]-2]);
     if(k<2) printf(", ");
     else    printf(" + ");
     }
   printf("\b\b \n");
   }
 printf(  "└─────┴─────────────────────────────────────────────────────────────\n");
 return;
}


float _HUGE UPI_pipe::StartUPI(float f)
{
 return f;
}

void _HUGE UPI_pipe::StopUPI(void)
{
 return;
}


// Формирование отклика системы
#define Do D[k]

int _HUGE UPI_pipe::CalcSignal(float *iBuf,float *oBuf)
{
  static float wy1,ww[Channel];
  unsigned int i,j,k,wADC=GetWorkADC(),wDAC=GetWorkDAC();


  for(k=0;k<wADC;k++)
    {

    wy1 = iBuf[k%wDAC];

    for(i=0;i<3;i++)
      {
      if(D[k].Zveno[i]) wy1 = D[k].Zveno[i]->Calc(wy1);
      }  /* end for */

    // теперь помеха
    switch(Do.Z_Type[3])
        {
        case 7: /* синусоида */
                wy1+=Do.xAmpl[0]*sin(2.0*M_PI*Do.delt*Do.xFreq[0]*(Do.iq++)+Do.xFaza[0]);
                break;
        case 8: /* полигармоника */
                wy1+=Do.xAmpl[0]*sin(2.0*M_PI*Do.delt*Do.xFreq[0]*(Do.iq++)+Do.xFaza[0]);
                wy1+=Do.xAmpl[1]*sin(2.0*M_PI*Do.delt*Do.xFreq[1]*Do.iq+Do.xFaza[1]);
                wy1+=Do.xAmpl[2]*sin(2.0*M_PI*Do.delt*Do.xFreq[2]*Do.iq+Do.xFaza[2]);
                break;
        case 9: /* псевдо-белый шум */
                wy1+=Do.xAmpl[0]*2.0*(rand()/(float)RAND_MAX-0.5);
                break;
        case 10: /* инверсия сигнала */
                wy1=-wy1;
                break;
        }
    Do.wy[0]=wy1+Do.imi_Const;
    oBuf[k]=Do.wy[Do.wait];

    for(j=Do.wait;j>0;j--) Do.wy[j]=Do.wy[j-1];
    }

  // теперь взаимовлияние каналов
  for(k=0;k<wADC;k++)
    {
    ww[k]=0;
    for(i=0;i<wADC;i++) ww[k]+=oBuf[i]*wzk[k][i];
    }
  for(k=0;k<wADC;k++) oBuf[k]=ww[k];
  return 0;
}



/*--БЛОК ВЫЧИСЛЕНИЯ ВСПОМОГАТЕЛЬНЫХ ПАРАМЕТРОВ ДИНАМ.СИСТЕМЫ.--*/
int _HUGE UPI_pipe::BuildUPI(void)
{
 float freq=GetWorkFreq();
 int i,k,Ret=1;
// static char err[]="Неверные параметры колебательного звена";

 randomize();

 for(k=0;k<Channel;k++)
   {
   Do.fdi=freq;
   Do.delt=1/Do.fdi;
   Do.iq=0;

   for(i=0;i<3;i++)
     if(Do.Zveno[i])
       {
       Do.Zveno[i]->delt=1.0/freq;
       Do.Zveno[i]->Build();
       }
   }
 return Ret;
}

int UPI_pipe::doEmulate(void)
{
 AVAILDATA av;
 ULONG cnt,cnt1,state,tik;
 int ret,i;
 char ch=0;
static short unsigned int ibuf[Channel],obuf[Channel],tmp[Channel];
static float bufI[Channel],bufO[Channel];

 StartUPI(0);

 do {
    printf("\nWait connection ...\n");
    do {
       ret=DosConnectNPipe(pipe_handle);
       ret=DosPeekNPipe(pipe_handle,tmp,0,&cnt,&av,&state);
//       printf("(%lx)\n\r",state);
       } while(state==NP_CLOSING || state==NP_LISTENING);

    printf("Process ...\n");
    tik=0;
    while(state==NP_CONNECTED && ret==0)
       {
       ret=DosWrite(pipe_handle,obuf,10,&cnt);
       ret=DosRead(pipe_handle,ibuf,10,&cnt);
       DosPeekNPipe(pipe_handle,tmp,0,&cnt1,&av,&state);

       for(i=0;i<GetWorkDAC();i++) bufI[i]=FromDAC(ibuf[i]);
       CalcSignal(bufI,bufO);
       for(i=0;i<GetWorkADC();i++) obuf[i]=VerifyADC(ToADC(bufO[i]));

//       Convert(ibuf,obuf);
       printf("\t(%ld)\t %f\t--> %f \n",tik++,bufI[0],bufO[0]);
//       printf("(0x%02lx) %ld\r",state,tik++);
//       ch=getch();
       }
    ret=DosDisConnectNPipe(pipe_handle);
    } while(ch!=27);

 printf("Done\n");

 StopUPI();

 return 0;
}


int main(int argc,char *argv[])
{
 UPI_pipe *Emul;

 printf("\nEmulator for UPI&Object " upiVERSION " (C) by"
        " \"Samara-Trial Systems\" " __DATE__ ".\n");

 if(argc==2)
    {
    Emul=new UPI_pipe(argv[1],1);
    if(Emul->Complete)
      {
      Emul->BuildUPI();
      Emul->doEmulate();
      }
    delete Emul;
    }
 else printf("\n Usage: %s <CfgFile>\n",argv[0]);

 return 0;
}



