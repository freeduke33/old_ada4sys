#include <io.h>
#include <math.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#include "upi_3w.hpp"


#define min(a,b) ( (a)>(b) ? (b) : (a) )

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
       memcpy(&D[i].FuncModel[k],&tmp.Fm[i][k],sizeof(struct _FuncModel));
       memcpy(&D[i].AperModel[k],&tmp.Am[i][k],sizeof(struct _AperModel));
       memcpy(&D[i].KolbModel[k],&tmp.Km[i][k],sizeof(struct _KolbModel));
       memcpy(&D[i].MassModel[k],&tmp.Mm[i][k],sizeof(struct _MassModel));
       D[i].xAmpl[k]=tmp.xAmpl[i][k];
       D[i].xFaza[k]=tmp.xFaza[i][k];
       D[i].xFreq[k]=tmp.xFreq[i][k];
       }
     for(k=0;k<4;k++) D[i].Z_Type[k]=tmp.Z_Type[i][k];
     D[i].imi_Const=tmp.imi_Const[i];
     D[i].fdds=tmp.fdds[i];
     D[i].wait=min(tmp.wait[i],MAX_WAIT);
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
       memcpy(&D[i].FuncModel[k],&xFuncModel[k],sizeof(struct _FuncModel));
       memcpy(&D[i].AperModel[k],&xAperModel[k],sizeof(struct _AperModel));
       memcpy(&D[i].KolbModel[k],&xKolbModel[k],sizeof(struct _KolbModel));
       memcpy(&D[i].MassModel[k],&xMassModel[k],sizeof(struct _MassModel));
       D[i].xAmpl[k]=_xAmpl[k];
       D[i].xFreq[k]=_xFreq[k];
       D[i].xFaza[k]=_xFaza[k];
     }
     for(k=0;k<4;k++) D[i].Z_Type[k]=xZ_Type[k];
     D[i].imi_Const=i*0.1+0.025;
     D[i].fdds=133.3;
     D[i].wait=1;
     for(k=0;k<Channel;k++) wzk[i][k]=_wzk[(k-i+10)%10];
     }
   }
 for(i=0;i<Channel;i++)
   {
   for(k=0;k<D[i].wait;k++) D[i].wy[k]=0;
   for(k=0;k<3;k++)
      {
      D[i].X[k][0]=D[i].X[k][1]=D[i].X[k][2]=0;
      D[i].Y[k][0]=D[i].Y[k][1]=0;
      D[i].tik[k]=0;
      }
   }
 if(f!=-1) close(f);
 ShowConfig();
 return;
}


void UPI_3w::ShowConfig(void)
{
 char name[][30]={"Апериодическое","Колебательное",
                  "Колеб.(перемещение)","Колеб.(скорость)","Колеб.(Ускорение)",
                  "СИНУСОИДА","ПОЛИГАРМОНИКА","ПСЕВДО-БЕЛЫЙ ШУМ","ИНВЕРСИЯ",
                  "Двухмассовое"};
 char fname[4][15]={"Линейное","Квадратическое","Кубическое","Гистерезис"};
 int i,k;

 printf("\n┌Канал┬── Описание канала (список звеньев + помеха) ────────────────\n");
 for(i=0;i<Channel;i++)
   {
   printf("│%4u │  ",i+1);
   for(k=0;k<4;k++) if(D[i].Z_Type[k])
     {
     if(D[i].Z_Type[k]==1) printf("%s",fname[D[i].FuncModel[k].Type]);
     else printf("%s",name[D[i].Z_Type[k]-2]);
     if(k<2) printf(", ");
     else    printf(" + ");
     }
   printf("\b\b \n");
   }
 printf(  "└─────┴─────────────────────────────────────────────────────────────\n");
 return;
}


void huge UPI_3w::StartUPI(void)
{
 return;
}

void huge UPI_3w::StopUPI(void)
{
 return;
}


// Формирование отклика системы
#define Do D[k]

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
      switch(D[k].Z_Type[i])
        {

#define Mod D[k].FuncModel[i]
        case 1:  /* функциональное звено */
          switch(Mod.Type)
            {
            case 0: wy1=((double)(Mod.k*wy1));         break;
            case 1: wy1=((double)(Mod.k*wy1*wy1));     break;
            case 2: wy1=((double)(Mod.k*wy1*wy1*wy1)); break;
            case 3: wy1=((double)(Mod.k*wy1));         break;
            case 4: wy1=((double)(Mod.k*sqrt(wy1)));   break;
            }
          break;

#undef Mod
#define Mod D[k].AperModel[i]
        case 2:  /* апериодическое звено */
//            wy1=-0.8;
          break;

#undef Mod
#define Mod D[k].KolbModel[i]
        case 3:  /* колебательное звено */
        case 4:  /* колебательное звено (перемещение) */
        case 5:  /* колебательное звено (скорость) */
          Do.X[i][2]=wy1;
          switch(Do.tik[i])
            {
            case 0:  wy1=0;              Do.tik[i]++; break;
            case 1:  wy1=Do.C[i][0]*wy1; Do.tik[i]++; break;
            default: wy1= Do.Lam[i][0]*Do.Y[i][1]
                         -Do.Lam[i][1]*Do.Y[i][0]
                         +Do.C[i][0]*Do.X[i][1]
                         -Do.C[i][1]*Do.X[i][0];
                     break;
            }
          Do.Y[i][0]=Do.Y[i][1]; Do.Y[i][1]=wy1;
          Do.X[i][0]=Do.X[i][1]; Do.X[i][1]=Do.X[i][2];
          break;
        case 6:  /* колебательное звено (ускорение) */
          tmp=wy1;
          switch(Do.tik[i])
            {
            case 0:  wy1=0;              Do.tik[i]++; break;
            case 1:  wy1=Do.C[i][0]*wy1; Do.tik[i]++; break;
            default: wy1= Do.Lam[i][0]*Do.Y[i][1]
                         -Do.Lam[i][1]*Do.Y[i][0]
                         +Do.C[i][0]*(Do.X[i][2]-2.0*Do.X[i][1]+Do.X[i][0]);
                     break;
            }
          Do.Y[i][0]=Do.Y[i][1]; Do.Y[i][1]=wy1;
          Do.X[i][0]=Do.X[i][1]; Do.X[i][1]=Do.X[i][2]; Do.X[i][2]=tmp;
          break;
        case 11: // Двухмассовое звено
          Do.X[i][2]=Do.X[i][1]; Do.X[i][1]=wy1;

          Do.Psi[i][0]=Do.Et[i][0]*Do.Psi[i][1]-
                       Do.Et[i][1]*Do.Psi[i][2]+
                       Do.X[i][1]-Do.X[i][2];
          wy1=Do.Lam[i][0]*Do.Y[i][1]-Do.Lam[i][1]*Do.Y[i][2]+
              Do.E[i][0]*Do.Psi[i][0]-Do.E[i][1]*Do.Psi[i][1]+
              Do.E[i][2]*Do.Psi[i][2];

          Do.Psi[i][2]=Do.Psi[i][1]; Do.Psi[i][1]=Do.Psi[i][0];
          Do.Y[i][2]=Do.Y[i][1]; Do.Y[i][1]=wy1;

          break;
        }
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

    oBuf[k]+=Do.imi_Const;

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
 double D0,D1,_Psi[2],w1,w2,A,A1,A2,CC,Gam1,Gam2,J[4];
 int i,k,j,Ret=1;
 static char err[]="Неверные параметры колебательного звена";

 randomize();

 for(k=0;k<Channel;k++)
   {
   Do.fdi=freq;
   Do.delt=1/Do.fdi;
   Do.iq=0;

#define Betta1 D[k].MassModel[i].Betta[0]
#define Betta2 D[k].MassModel[i].Betta[1]
#define Freq1  D[k].MassModel[i].Freq[0]
#define Freq2  D[k].MassModel[i].Freq[1]
#define Betta  D[k].KolbModel[i].Betta


   for(i=0;i<3;i++)
     {

     switch(Do.Z_Type[i])
     {
     case 3:  /* Колебательное простое */
     if(Betta!=1)
       {
       if(Betta>1)
         {
         D0=Do.delt*2.0*M_PI*Do.KolbModel[i].Freq*sqrt(Betta*Betta-1);
         _Psi[0]=(exp(D0)-exp(-D0))/(2.0*sqrt(Betta*Betta-1));
         _Psi[1]=exp(D0)+exp(-D0);
         }
       else
         {
         D0=Do.delt*2.0*M_PI*Do.KolbModel[i].Freq*sqrt(1-Betta*Betta);
         _Psi[0]=sin(D0)/sqrt(1-Betta*Betta);
         _Psi[1]=2.0*cos(D0);
         }
       D0=exp(-Do.delt*2.0*M_PI*Do.KolbModel[i].Freq*Betta);

       Do.Lam[i][0]=D0*_Psi[1]; Do.C[i][0]=(2.0-Do.Lam[i][0])/2.0-Betta*D0*_Psi[0];
       Do.Lam[i][1]=D0*D0;      Do.C[i][1]=(Do.Lam[i][0]-2.0*Do.Lam[i][1])/2.0-Betta*D0*_Psi[0];
       }
     else
       {
       SetDrvErrMsg(err);
       Ret=0; /* Ошибка !!! Betta==1 */
       }
     break;
     case 4: /* Колебательное по перемешению */
       D0=2.0*M_PI*Do.KolbModel[i].Freq*Do.delt;
       D1=sqrt(1.0-(Betta*Betta));
       Do.C[i][0]=1.0-exp(-D0*Betta)*cos(D0*D1)-((Betta*exp(-Betta*D0)*sin(D0*D1))/D1);
       Do.C[i][1]=exp(-D0*Betta)*cos(D0*D1)-exp(-2.0*D0*Betta)-((Betta*exp(-D0*Betta)*sin(D0*D1))/D1);
       Do.Lam[i][0]=2.0*exp(-Betta*D0)*cos(D0*D1);
       Do.Lam[i][1]=exp(-2.0*Betta*D0);
     break;
     case 5: /* Колебательное по скорости */
       D0=2.0*M_PI*Do.KolbModel[i].Freq*Do.delt;
       D1=sqrt(1.0-(Betta*Betta));
       Do.C[i][0]=exp(-D0*Betta)*sin(D0*D1)/D1;
       Do.C[i][1]=Do.C[i][0];
       Do.Lam[i][0]=2.0*exp(-Betta*D0)*cos(D0*D1);
       Do.Lam[i][1]=exp(-2.0*D0*Betta);
     break;
     case 6: /* Колебательное по ускорению */
       D0=2.0*M_PI*Do.KolbModel[i].Freq*Do.delt;
       D1=D0*sqrt(1.0-(Betta*Betta));
       D0*=Betta;
       Do.C[i][0]=exp(-2.0*D0);
       Do.Lam[i][0]=2.0*exp(-D0)*cos(D1);
       Do.Lam[i][1]=exp(-2.0*D0);
     break;
     case 11: // Двухмассовое звено
#undef Betta
       w1=2.0*M_PI*Freq1; w2=2.0*M_PI*Freq2;
       A=4.0*w1*w1*w2*w2*(Betta1*Betta1+Betta2*Betta2)-
         4.0*Betta1*Betta2*w1*w2*(w1*w1+w2*w2)+
         (w1*w1-w2*w2)*(w1*w1-w2*w2);
       A1=(w1*w1)*(4.0*Betta1*Betta2*w1*w2-4*Betta1*Betta1*w2*w2+
         w2*w2-w1*w1)/A;
       A2=2.0*w1*(4.0*Betta1*Betta2*Betta2*w1*w1*w2-
         4.0*Betta1*Betta1*Betta2*w1*w2*w2+Betta1*w2*w2*w2-
         Betta2*w1*w1*w1)/A;
       CC=2.0*Betta1-A2*w2/w1;

       if(Betta1>1)
         {
         J[2]=sqrt(Betta1*Betta1-1.0);
         J[0]=exp(-(Do.delt*w1)/(Betta1+J[2]));
         J[1]=exp(-Do.delt*w1*(Betta1+J[2]));

         Do.Lam[i][0]=J[0]+J[1];
         Gam1=(J[0]-J[1])/(2.0*J[2]);
         }
       else
         {
         J[0]=sqrt(1-Betta1*Betta1);
         J[2]=Do.delt*w1*J[0];
         J[3]=exp(-Do.delt*Betta1*w1);

         Do.Lam[i][0]=2.0*J[3]*cos(J[2]);
         Gam1=J[3]*sin(J[2])/J[0];
         }

       D0=(CC+A1*Betta1)*Gam1;
       Do.Lam[i][1]=exp(-2.0*Do.delt*Betta1*w1);


       if(Betta2>1)
         {
         J[2]=Betta2+sqrt(Betta2*Betta2-1.0);
         J[0]=exp(-(Do.delt*w2)/J[2]);
         J[1]=exp(-Do.delt*w2*J[2]);

         Do.Et[i][0]=J[0]+J[1];
         Gam2=J[0]-J[1];
         }
       else
         {
         J[0]=sqrt(1-Betta2*Betta2);
         J[2]=Do.delt*w2*J[0];
         J[3]=exp(-Do.delt*Betta2*w2);

         Do.Et[i][0]=2.0*J[3]*cos(J[2]);
         Gam2=Do.delt*J[3]*sin(J[2])/J[0];
         }
      D1=(A2-A1*Betta2)*Gam2;
      Do.Et[i][1]=exp(-2.0*Do.delt*Betta2*w2);

      Do.E[i][0]=D0+D1+A1*(Do.Lam[i][0]-Do.Et[i][0])/2.0;
      Do.E[i][1]=D0*Do.Et[i][0]+D1*Do.Lam[i][0]+
                 A1*(Do.Lam[i][1]-Do.Et[i][1]);
      Do.E[i][2]=D0*Do.Et[i][1]+D1*Do.Lam[i][1]+
                 A1*(Do.Et[i][0]*Do.Lam[i][1]-Do.Et[i][1]*Do.Lam[i][0])/2.0;

      for(j=0;j<3;j++) Do.Psi[i][j]=0;
//      printf("\nE=   %f %f %f",E[k][i][0],E[k][i][1],E[k][i][2]);
//      printf("\nEt=  %f %f",Et[k][i][0],Et[k][i][1]);
//      printf("\nLam= %f %f",Lam[k][i][0],Lam[k][i][1]);
     break;
     }
     } /* endfor */
   }
 return Ret;
}


int main(int argc,char *argv[])
{
 UPI_3w *Emul;

 printf("\nEmulator for UPI&Object " upiVERSION " (C) by"
        " \"Samara-Trial\" " __DATE__ ".\n");

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



