#include <io.h>
#include <math.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "upi_3w.hpp"



UPI_3w::UPI_3w(char *filename,int s):UPI(s,Channel,Channel)
{
 int f,i,k;
 struct _param_3w tmp;
 struct _FuncModel xFuncModel[3]={{1,1.0},{2,1.0},{0,1.0}};
 struct _AperModel xAperModel[3]={{40,5,2},{40,5,2},{40,5,2}};
 struct _KolbModel xKolbModel[3]={{0.5,27.7},{0.5,27.7},{0.5,27.7}};
 struct _MassModel xMassModel[3]={{{0.5,0.5},{27.7,27.7}},{{0.5,0.5},{27.7,27.7}},{{0.5,0.5},{27.7,27.7}}};
 int    xZ_Type[4]={1,0,0,4};
 float  _wzk[10]={0.9,0.2,0.3,0.2,0.1,0.2,0.3,0.2,0.1,0.2};
 float  _xAmpl[3]={0.8,0.3,0.1};
 float  _xFreq[3]={3.1,12.7,34.9};
 float  _xFaza[3]={1.33,1.0,0.01};

 f=open(filename,O_BINARY|O_RDONLY);
 if(f!=-1 &&  read(f,&tmp,sizeof(tmp)) == sizeof(tmp) )
   {
   printf("Loading parameters from '%s'...",filename);
   for(i=0;i<Channel;i++)
     {
     for(k=0;k<3;k++)
       {
       memcpy(&FuncModel[i][k],&tmp.Fm[i][k],sizeof(struct _FuncModel));
       memcpy(&AperModel[i][k],&tmp.Am[i][k],sizeof(struct _AperModel));
       memcpy(&KolbModel[i][k],&tmp.Km[i][k],sizeof(struct _KolbModel));
       memcpy(&MassModel[i][k],&tmp.Mm[i][k],sizeof(struct _MassModel));
       xAmpl[i][k]=tmp.xAmpl[i][k];
       xFaza[i][k]=tmp.xFaza[i][k];
       xFreq[i][k]=tmp.xFreq[i][k];
       }
     for(k=0;k<4;k++) Z_Type[i][k]=tmp.Z_Type[i][k];
     imi_Const[i]=tmp.imi_Const[i];
     fdds[i]=tmp.fdds[i];
     for(k=0;k<Channel;k++) wzk[i][k]=tmp.wzk[i][k];
     }
   printf(" Ok.\n");
   }
 else
   {
   printf("Using default parameters.\n");
   for(i=0;i<Channel;i++)
     {
     for(k=0;k<3;k++)
       {
       memcpy(&FuncModel[i][k],&xFuncModel[k],sizeof(struct _FuncModel));
       memcpy(&AperModel[i][k],&xAperModel[k],sizeof(struct _AperModel));
       memcpy(&KolbModel[i][k],&xKolbModel[k],sizeof(struct _KolbModel));
       memcpy(&MassModel[i][k],&xMassModel[k],sizeof(struct _MassModel));
       xAmpl[i][k]=_xAmpl[k];
       xFreq[i][k]=_xFreq[k];
       xFaza[i][k]=_xFaza[k];
     }
     for(k=0;k<4;k++) Z_Type[i][k]=xZ_Type[k];
     imi_Const[i]=i*0.1+0.025;
     fdds[i]=133.3;
     for(k=0;k<Channel;k++) wzk[i][k]=_wzk[(k-i+10)%10];
     }
   }
 if(f!=-1) close(f);
 ShowConfig();
 return;
}



// Формирование отклика системы

int huge UPI_3w::CalcSignal(float *iBuf,float *oBuf)
{
  static float wy1,ww[Channel];
  float tmp;
  unsigned int i,j,k,wADC=GetWorkADC(),wDAC=GetWorkDAC();


  for(k=0;k<wADC;k++)
    {

    wy1 = iBuf[k%wDAC];

    for(i=0;i<3;i++)
      {
      switch(Z_Type[k][i])
        {
        case 1:  /* функциональное звено */
          switch(FuncModel[k][i].Type)
            {
            case 0: wy1=((double)(FuncModel[k][i].k*wy1));         break;
            case 1: wy1=((double)(FuncModel[k][i].k*wy1*wy1));     break;
            case 2: wy1=((double)(FuncModel[k][i].k*wy1*wy1*wy1)); break;
            case 3: wy1=((double)(FuncModel[k][i].k*wy1));         break;
            case 4: wy1=((double)(FuncModel[k][i].k*sqrt(wy1)));   break;
            }
          break;
        case 2:  /* апериодическое звено */
//            wy1=-0.8;
          break;
        case 3:  /* колебательное звено */
        case 4:  /* колебательное звено (перемещение) */
        case 5:  /* колебательное звено (скорость) */
          X[k][i][2]=wy1;
          switch(tik[k][i])
            {
            case 0:  wy1=0; tik[k][i]++; break;
            case 1:  wy1=C[k][i][0]*wy1; tik[k][i]++; break;
            default: wy1=Lam[k][i][0]*Y[k][i][1]-Lam[k][i][1]*Y[k][i][0]+
                         C[k][i][0]*X[k][i][1]-C[k][i][1]*X[k][i][0];
                     break;
            }
          Y[k][i][0]=Y[k][i][1]; Y[k][i][1]=wy1;
          X[k][i][0]=X[k][i][1]; X[k][i][1]=X[k][i][2];
          break;
        case 6:  /* колебательное звено (ускорение) */
          break;
        case 11: // Двухмассовое звено
          break;
        }
      }  /* end for */

    switch(Z_Type[k][3])
        {
        case 7: /* синусоида */
                wy1+=xAmpl[k][0]*sin(2.0*M_PI*delt[k]*xFreq[k][0]*(iq[k]++)+xFaza[k][0]);
                break;
        case 8: /* полигармоника */
                wy1+=xAmpl[k][0]*sin(2.0*M_PI*delt[k]*xFreq[k][0]*(iq[k]++)+xFaza[k][0]);
                wy1+=xAmpl[k][1]*sin(2.0*M_PI*delt[k]*xFreq[k][1]*iq[k]+xFaza[k][1]);
                wy1+=xAmpl[k][2]*sin(2.0*M_PI*delt[k]*xFreq[k][2]*iq[k]+xFaza[k][2]);
                break;
        case 9: /* псевдо-белый шум */
                wy1+=xAmpl[k][0]*2.0*(rand()/(float)RAND_MAX-0.5);
                break;
        case 10: /* инверсия сигнала */
                wy1=-wy1;
                break;
        }
    oBuf[k]=wy0[k];
    wy0[k]=wy1+imi_Const[k]; // задержка на 1 такт
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
int huge UPI_3w::BuildUPI(void)
{
 float  fr,freq=GetWorkFreq();
 double D,D1,Psi[2];
 int i,k,Ret=1;
 static char err[]="Неверные параметры колебательного звена";

 randomize();

 for(k=0;k<Channel;k++)
   {
   fdi[k]=freq;
   delt[k]=1/fdi[k];
   iq[k]=0;
   wy0[k]=0;

#define Betta KolbModel[k][i].Betta
#define Betta1 MassModel[k][i].Betta[0]
#define Betta2 MassModel[k][i].Betta[1]
#define Freq1  MassModel[k][i].Freq[0]
#define Freq2  MassModel[k][i].Freq[1]
   for(i=0;i<3;i++)
     {
     tik[k][i]=0;
     X[k][i][0]=X[k][i][1]=X[k][i][2]=0;
     Y[k][i][0]=Y[k][i][1]=0;

     switch(Z_Type[k][i])
     {
     case 3:  /* Колебательное простое */
     break;
     case 4: /* Колебательное по перемешению */
     break;
     case 5: /* Колебательное по скорости */
     break;
     case 6: /* Колебательное по ускорению */
     break;
     case 11: // Двухмассовое звено

     break;
     }
     } /* endfor */
   }
 return Ret;
}


int main(int argc,char *argv[])
{
 UPI_3w *Emul;

 printf("\nEmulator for UPI&External Object v1.10 (C) by"
        " \"Samara-Trend\" 1994.\n");

 if(_8087==0)   printf("\007\007I can`t found hardware FPU, aborted.\n");
 else if(argc>=3)
    {
    Emul=new UPI_3w(argv[1],1);
    if(!Emul->DoLoop(&argv[2])) printf("\nError: %s\n",Emul->ErrMsg());
    delete Emul;
    }
 else printf("\n Usage: %s <CfgFile> <ExecProgramFile> [list of parameters for <ExecProgramFile> ]\n",argv[0]);

 return 0;
}

