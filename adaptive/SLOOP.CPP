//  #pragma options -N-

// #define DEBUG

#define Uses_FModel_1x1
#define Uses_Buffer
#define Uses_SyncLoop
#include <adaptive.hpp>

#include <dac_adc.h>

SyncLoop::SyncLoop(unsigned int chl):LoopSC(chl)
{
 num=new int[chl];
 ErrTik=0;
 Info.IsTiksExist=1;

 return;
}

SyncLoop::~SyncLoop(void)
{
 if(num) delete num;
 return;
}


int SyncLoop::doStart(void)
{
 int i;

 if(!num) return RC_INVALIDBUILD;

 for(i=0;i<Chl;i++) { num[i]=0; iBuf[i]=0; }
 Info.OkCycle=0;
 Info.OkTik=0;
 Info.IsTiksExist=1;
 doErr=0;

 return RC_DONE;
}


int SyncLoop::doLoop(void)
{
 unsigned int m,i;

 for(m=0;m<Chl;m++)
   {
   i=num[m];
#ifdef DEBUG
 if(LOG)    LOG->put('!',"^1");
#endif
   // рассчитываем сигнал прогноза
   Y[m]->putNext(iBuf[m]);

#ifdef DEBUG
 if(LOG)    LOG->put('!',"^2");
#endif
   Model[i][m]->Build();
#ifdef DEBUG
 if(LOG)    LOG->put('!',"^3");
#endif
   Model[i][m]->Update();
#ifdef DEBUG
 if(LOG)    LOG->put('!',"^4");
#endif
   oBuf[m]=Model[i][m]->Calc(iBuf[m]);

#ifdef DEBUG
 if(LOG)    LOG->put('!',"^5");
#endif
   X[m]->putNext(oBuf[m]);

#ifdef DEBUG
 if(LOG)    LOG->put('!',"^6");
#endif
   if(Model[++i][m])
     {
#ifdef DEBUG
 if(LOG)    LOG->put('!',"^6.1");
#endif
     Model[i][m]->Append(oBuf[m],iBuf[m]);
#ifdef DEBUG
 if(LOG)    LOG->put('!',"^6.2");
#endif
     Model[i][m]->Build();
#ifdef DEBUG
 if(LOG)    LOG->put('!',"^6.3");
#endif
     if(Model[i][m]->Update()) num[m]++;
#ifdef DEBUG
 if(LOG)    LOG->put('!',"^6.4");
#endif
     }
   }
#ifdef DEBUG
 if(LOG)    LOG->put('!',"^7");
#endif
 DrvVolume(oBuf,iBuf);

#ifdef DEBUG
 if(LOG)    LOG->put('!',"^8");
#endif
 if(Info.OkTik>Tiks) { Info.OkTik=0; Info.OkCycle++; }
 else                { Info.OkTik++;                 }

// for(i=0;i<Chl;i++) iBuf[i]=oBuf[i];
 return 0;
}


int SyncLoop::isWorkTime(void)
{
 return (Info.OkCycle<Cycles) && (Info.ResultCode==RC_PROCESS);
}

int SyncLoop::isOkMistake(void)
{
 int i,ok=1;

 for(i=0;i<Chl;i++) if(!Model[num[i]][i]->isOkMistake()) ok=0;

 if(!ok)
   {
   if(doErr<ErrTik) { doErr+=2; ok=1; }
   }
 else if(doErr) doErr--;

 return ok;
}

void SyncLoop::SetMistakeWait(unsigned int tiks)
{
 doErr=0;
 ErrTik=tiks*2;
 return;
}

float SyncLoop::Mistake(void)
{
 int i;
 float su,val;

 for(i=su=0;i<Chl;i++)
   {
   val=Model[num[i]][i]->Mistake();
   if(val>su) su=val;
   }

 return su;
}


