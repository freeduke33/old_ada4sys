// #define PREPARE_FPE
#define DEBUG


#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

#define Uses_FModel_1x1
#define Uses_Buffer
#define Uses_LoopSC
#define Uses_FPError


#include <adaptive.hpp>
#include <dac_adc.h>

#include <log.hpp>

Log *LOG=NULL;


typedef Buffer     _HUGE *BufferP;
typedef FModel_1x1 _HUGE *CtrlModelP;

#define max(a,b) ((a)>(b) ? (a):(b))
#define min(a,b) ((a)<(b) ? (a):(b))

LoopSC::LoopSC(unsigned int chl)
{
 int i;

 Chl=chl;

 Model[0]=new CtrlModelP[chl];
 Model[1]=new CtrlModelP[chl];
 Model[2]=new CtrlModelP[chl];

 iBuf=new float[chl];
 oBuf=new float[chl];
  Buf=new float[chl];
  Buf1=new float[chl];
  Buf2=new float[chl];

  StartVol=new float[chl];
  PassiveVol=new float[chl];

  for(i=0;i<chl;i++)
    {
    StartVol[i]=0;
    PassiveVol[i]=-1;
    }


 X=new BufferP[chl];
 Y=new BufferP[chl];
 Yt=new BufferP[chl];

 Margin=0;

 Info.IsTiksExist=0;
 Info.IsTimeExist=0;
 Info.IsMistakeExist=0;

 return;
}

LoopSC::~LoopSC(void)
{
 if(X) delete X;
 if(Y) delete Y;
 if(Yt) delete Yt;
 if(Model[0]) delete Model[0];
 if(Model[1]) delete Model[1];
 if(Model[2]) delete Model[2];

 if(iBuf) delete iBuf;
 if(oBuf) delete oBuf;
 if( Buf) delete  Buf;

 return;
}

int LoopSC::GetStatictics(LoopStat *s)
{
 *s=Info;
 return 0;
}



int LoopSC::SetBuffer(unsigned int chl,Buffer *x,Buffer *y,Buffer *yt)
{
 if( !X || !Y || !Yt) return RC_INVALIDBUILD;
 if(chl>Chl) return RC_CHANNEL;

 X[chl]=x; Y[chl]=y; Yt[chl]=yt;

 return RC_DONE;
}

int LoopSC::SetModel(unsigned int chl,FModel_1x1 *Bef,FModel_1x1 *Act,FModel_1x1 *Aft)
{
 if( !Model ) return RC_INVALIDBUILD;
 if(chl>Chl) return RC_CHANNEL;

 Model[0][chl]=Bef;
 Model[1][chl]=Act;
 Model[2][chl]=Aft;

 return RC_DONE;
}

int LoopSC::SetPassMargin(unsigned long m)
{
 Margin=m;
 return RC_DONE;
}


int LoopSC::PassLoop(float freq,unsigned long cycles,unsigned long time)
{
 unsigned int m,i,val=RC_DONE;
#ifdef DEBUG
 static unsigned long Tik=0;
#endif

 Freq=freq; Cycles=cycles; Tiks=time;


 if(!DrvIsPresent()) return RC_UPI;

 if(!X || !Y || !Yt || !Model[0] || !Model[1]
       || !Model[2] || !iBuf || !oBuf || !Buf) return RC_INVALIDBUILD;

 for(i=0;i<Chl;i++)
  {
  if( !X[i] || !Y[i] || !Yt[i] || !Model[0][i]) return RC_INVALIDBUILD;

  Y[i]->setMargin(1);
  X[i]->reset();
  Y[i]->reset();
  Yt[i]->reset();

  Model[0][i]->Reset();
  if(Model[1][i]) Model[1][i]->Reset();
  if(Model[2][i]) Model[2][i]->Reset();

  Model[0][i]->setIdeal(Yt[i]);
  if(Model[1][i]) Model[1][i]->setIdeal(Yt[i]);

  oBuf[i]=0;
  }

#ifdef DEBUG
 Tik=0;
 if(LOG)  LOG->put('.',"\n Begin work (%gHz, %lu cycles, %lu tiks)\n",freq,cycles,time);
#endif

 if((val=doStart())==RC_DONE) {

 showStart();

 if(Enable()==DRV_OK)   {


#ifdef PREPARE_FPE
 signal(SIGFPE,GlobalFPE);
 if(!setjmp(FPE_ERR)) {
#endif

  for(m=0;m<Chl;m++)
    {
     X[m]->reset(); Y[m]->reset(); Yt[m]->reset();
    }

  // теперь цикл управления
  Info.ResultCode=RC_PROCESS;
  while(isWorkTime() && isOkMistake())
    {
#ifdef DEBUG
 if(LOG)  LOG->put('.',"\n[%lu] ",Tik++);
#endif
    showProgress();
    doLoop();
    }

#ifdef PREPARE_FPE
  } /*if(signal)*/
  signal(SIGFPE,SIG_DFL);
#endif

  Disable();
  } /* if(Enable()) */
  else  val=RC_UPI;

  } /* if(doStart()) */

 showDone();
 doDone();

#ifdef DEBUG
 if(LOG)  LOG->put('.',"\nDone.\n");
#endif

 return val;
}



int LoopSC::Enable(void)
{
 int k;

 if(!X || !Y || !Yt || !Model[0] || !Model[1]
       || !Model[2] || !iBuf || !oBuf || !Buf) return DRV_FAILED;

 DrvSetChannel(Chl,Chl);

 if((k=DrvEnable(Freq))==DRV_OK)
   {
   for(k=0;k<Chl;k++) { Buf1[k]=PassiveVol[k];Buf2[k]=StartVol[k]; }
   DrvGetVolDAC(Buf1);

   doRewind(Buf1,Buf2);
   k=DRV_OK;
   }

 return k;
}

int LoopSC::Disable(void)
{
 int k;

 for(k=0;k<Chl;k++) Buf2[k]=PassiveVol[k];
 DrvGetVolDAC(Buf1);

 doRewind(Buf1,Buf2);
 return DrvDisable();
}

void LoopSC::doRewind(float *valB,float *valE)
{
 unsigned long i,k,d,time,nx=Margin;

 if(nx) time=(Freq+0.5)/10;
 else   time=1;

 nx*=time;

 for(k=0;k<Chl;k++) oBuf[k]=valB[k];

 for(k=0;k<Chl;k++) Buf[k]=(oBuf[k]-valE[k])/(float)(nx+1);

 for(i=0;i<nx;i++)
     {
     DrvVolume(oBuf,iBuf);
     for(k=0;k<Chl;k++)
       {
//       X[k]->putNext(oBuf[k]);
//       Y[k]->putNext(iBuf[k]);
//       Yt[k]->getNext();
       oBuf[k]-=Buf[k];
       }
     }
 for(k=0;k<Chl;k++) oBuf[k]=valE[k];
 for(d=0;d<time;d++) DrvVolume(oBuf,iBuf);
 return;
}


void LoopSC::SetStartVol(unsigned int chl,float vol)
{
 chl%=Chl;
 vol=max(-1,min(1,vol));
 StartVol[chl]=vol;
 return;
}

void LoopSC::SetPassiveVol(unsigned int chl,float vol)
{
 chl%=Chl;
 vol=max(-1,min(1,vol));
 PassiveVol[chl]=vol;
 return;
}

float LoopSC::GetStartVol(unsigned int chl)
{
 chl%=Chl;
 return StartVol[chl];
}

float LoopSC::GetPassiveVol(unsigned int chl)
{
 chl%=Chl;
 return PassiveVol[chl];
}
