#pragma options -N-

#define Uses_FModel_1x1
#define Uses_AsyncLoop
#define Uses_Buffer

#include <adaptive.hpp>
#include <dac_adc.h>



#define CHANNEL 20
void interrupt hook(void);
static AsyncLoop _HUGE *CLoop;


int AsyncLoop::doStart(void)
{
 SyncLoop::doStart();

 Do=0; CLoop=this;
 DrvSetHook(hook);
 return 0;
}

int AsyncLoop::Enable(void)
{
 int val=SyncLoop::Enable();
 Do=1;
 return val;
}


int AsyncLoop::doLoop(void)
{
 unsigned int m,k;

 for(m=0;m<Chl;m++)
   {
   k=num[m];
   Model[k][m]->Build();
   k++;
   if(Model[k][m]) Model[k][m]->Build(); // пытаемся построить следующую модель
   }
 DrvWaitDAC_ADC();
 for(m=0;m<Chl;m++)
   {
   k=num[m];
   if(Model[k+1][m] && Model[k+1][m]->Update()) num[m]++;
   else Model[k][m]->Update();
   }

 return RC_DONE;
}

static void interrupt hook(void)
{
 static float buf[CHANNEL],val;
 register short i,k;

 if(!CLoop->Do) return;

 DrvGetVolADC(buf);

 for(i=0;i<CLoop->Chl;i++)
    {
    k=CLoop->num[i];
    val=buf[i];
    CLoop->Y[i]->putNext(val);  // отклик системы
    buf[i]=CLoop->Model[k][i]->Calc(val);
    CLoop->X[i]->putNext(buf[i]);
    if(CLoop->Model[++k][i]) CLoop->Model[k][i]->Append(buf[i],val);
    }
 DrvSetVolDAC(buf);  // управляющее воздействие

 if(CLoop->Info.OkTik > CLoop->Tiks)
   {
   CLoop->Info.OkTik=0;
   CLoop->Info.OkCycle++;
   }
 else CLoop->Info.OkTik++;
 return;
}

int AsyncLoop::Disable(void)
{
 DrvClrHook();
 return SyncLoop::Disable();
}
