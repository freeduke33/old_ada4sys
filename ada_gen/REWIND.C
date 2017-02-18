#include "dac_adc.h"

#define CHANNEL 64

void doRewind(float Freq,unsigned int Tm,unsigned int Chl, float *valB, float *valE)
{
 unsigned long i,k,d,time,nx;
 static float oBuf[CHANNEL],Buf[CHANNEL],iBuf[CHANNEL];

 nx= Tm ? Tm*Freq :1;

 for(k=0;k<Chl;k++) oBuf[k]=valB[k];

 for(k=0;k<Chl;k++) Buf[k]=(oBuf[k]-valE[k])/(float)(nx+1);

 for(i=0;i<nx;i++)
     {
     DrvVolume(oBuf,iBuf);
     for(k=0;k<Chl;k++) oBuf[k]-=Buf[k];
     }
 for(k=0;k<Chl;k++) oBuf[k]=valE[k];
 DrvVolume(oBuf,iBuf);
 return;
}

