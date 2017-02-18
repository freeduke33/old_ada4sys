#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include "table.hpp"

// extern unsigned _stklen=6000U;


extern "C" {
   int pascal _EnableTimer(Word DivHi,Word DivLo,void interrupt (far *DrvVector)(union REGS *r));
   int pascal _DisableTimer(void);
   int pascal _WaitTimer(void);
   int pascal _InstallDriver(void);
   extern char far * pascal far _EmulErrMsg;
};


char Table::TableID[]=DRV_IDstring;
char Table::Copyright[]="(C) written by D.Kokorev RU";
char Table::NameDRV[]="UPI's emulator (MultiChannel version) v1.0";
Table *Table::Tbl=NULL;


DRV_Table_T Table::table={
            0,Table::TableID,Table::Copyright,Table::NameDRV,
            Enable,Disable,GetVolADC,SetVolDAC,
            SetHook,
            ClrHook,
            Volume,
            GetBuf,
            WaitDAC_ADC,
            SetChannel,ClrSkipped,GetSkipped,
            GetMaxFreq,GetMaxChannel,GetInfoDAC,GetInfoADC
            };




Table::Table(int maxDAC,int maxADC,Word rangeDAC,Word rangeADC,Word maxFreq)
{
 int i;

 HookFunc=NULL; WaitFlg=0;

 MaxFreq=maxFreq;
 ActDAC=MaxDAC=maxDAC;  RangeDAC=rangeDAC;
 ActADC=MaxADC=maxADC;  RangeADC=rangeADC;

 BufDAC=new Word[MaxDAC];
 if(BufDAC) for(i=0;i<MaxDAC;i++) BufDAC[i]=0;

 BufADC=new Word[MaxADC];
 if(BufADC) for(i=0;i<MaxADC;i++) BufADC[i]=0;

 TmpDAC=new float[MaxDAC];
 TmpADC=new float[MaxADC];

 _InstallDriver();
 SetDrvErrMsg(NULL);

 return;
}

Table::~Table()
{
 Disable();

 if(BufDAC) delete BufDAC; if(BufADC) delete BufADC;
 if(TmpDAC) delete TmpDAC; if(TmpADC) delete TmpADC;
 return;
}

DRV_Table_P Table::GetTable(void)
{
 if(BufDAC && BufADC && TmpDAC && TmpADC) return &table;
 else return NULL;
}

void Table::SetDrvErrMsg(char far *msg) { _EmulErrMsg=msg; }


#define DAC (Tbl->RangeDAC/2)
#define ADC (Tbl->RangeADC/2)

#define ToADC(a)     ( (int) ((a)*(float)ADC+ADC)               )
#define VerifyADC(a) ( (a<0) ? 0 : (a)>=ADC*2 ? ADC*2-1 : (a)   )
#define FromDAC(a)   ( ((float)(((int)(a))-(int)DAC)) /(int)DAC )

#define Tbl Table::Tbl
//#define rUp(a,b) ((a)> 1 ? 1 : (b) )
//#define rDn(a,b) ((a)<-1 ?-1 : (b) )
//#define ToWord(a,b) ( (int)( (rUp(a,rDn(a,a)) )*((b)/2)+((b)/2)+0.5) )
//                         ( (                )                    )
//#define FromWord(a,b)  ( ((float) ( (int)(a) - (int)((b)/2) ) ) / ((b)/2) )

void interrupt DrvVector(union REGS far *r)
{
 int i;

 if(Tbl)
   {
   for(i=0;i<Tbl->ActDAC;i++)
     {
//     Tbl->TmpDAC[i]=FromWord(Tbl->BufDAC[i],Tbl->RangeDAC);
     Tbl->TmpDAC[i]=FromDAC(Tbl->BufDAC[i]);
     }

   Tbl->CalcSignal(Tbl->TmpDAC,Tbl->TmpADC);
//   for(i=0;i<Tbl->ActADC;i++) Tbl->TmpADC[i]=Tbl->TmpDAC[i];

   for(i=0;i<Tbl->ActADC;i++)
//        Tbl->BufADC[i]=ToWord(Tbl->TmpADC[i],Tbl->RangeADC);
        Tbl->BufADC[i]=VerifyADC(ToADC(Tbl->TmpADC[i]));

   if(Tbl->HookFunc) Tbl->HookFunc();
//   cprintf("+");
   }
 return;
}

// ==================[ Table's functions ]======================



int pascal huge Enable(Word DivHi,Word DivLo)
{
 if(Tbl && Tbl->BufDAC && Tbl->BufADC && Tbl->TmpDAC && Tbl->TmpADC && DivHi*DivLo)
   {
   Tbl->Freq=Tbl->MaxFreq/((float)DivLo*DivHi);

   Tbl->StartUPI();

   if(Tbl->BuildUPI())
     {
     _EnableTimer(DivHi,DivLo,DrvVector);
     return DRV_OK;
     }
   }
 return DRV_FAILED;
}
int pascal huge Disable(void)
{
 if(Tbl)
   {
//   WaitDAC_ADC();
   Tbl->HookFunc=NULL;
   _DisableTimer();

   Tbl->StopUPI();
   return DRV_OK;
   }
 else return DRV_FAILED;
}
int pascal huge GetVolADC(Word far *ADCbuf)
{
 int i;
 if(!Tbl) return DRV_FAILED;
 for(i=0;i<Tbl->ActADC;i++) ADCbuf[i]=Tbl->BufADC[i];
 return DRV_OK;
}
int pascal huge SetVolDAC(Word far *DACbuf)
{
 int i;
 if(!Tbl) return DRV_FAILED;
 for(i=0;i<Tbl->ActDAC;i++) Tbl->BufDAC[i]=DACbuf[i];
 return DRV_OK;
}
int pascal huge SetHook(HookPtr Hook)
{
 Tbl->HookFunc=Hook;
 return DRV_FAILED;
}
int pascal huge ClrHook(void)
{
 Tbl->HookFunc=NULL;
 return DRV_OK;
}
int pascal huge Volume(Word far *DACbuf,Word far *ADCbuf)
{
 if(!Tbl) return DRV_FAILED;
 SetVolDAC(DACbuf);
 WaitDAC_ADC();
 GetVolADC(ADCbuf);
 return DRV_OK;
}
int pascal huge GetBuf(Word far *far *DACbuf,Word far *far *ADCbuf)
{
 if(!Tbl) return DRV_FAILED;
 *DACbuf=Tbl->BufDAC;
 *ADCbuf=Tbl->BufADC;
 return DRV_OK;
}


int pascal huge WaitDAC_ADC()
{
 if(_WaitTimer()) cprintf("*");
 return DRV_OK;
}
int pascal huge SetChannel(int numDAC,int numADC)
{
 if(!Tbl) return DRV_FAILED;
 Tbl->ActDAC=numDAC<Tbl->MaxDAC ? numDAC: Tbl->MaxDAC;
 Tbl->ActADC=numADC<Tbl->MaxADC ? numADC: Tbl->MaxADC;
 return DRV_OK;
}
int pascal huge ClrSkipped(void)
{
 return DRV_OK;
}
int pascal huge GetSkipped(unsigned long far *val)
{
 *val=13;
 return DRV_OK;
}
int pascal huge GetMaxFreq(unsigned long far *val)
{
 if(!Tbl) return DRV_FAILED;
 *val=Tbl->MaxFreq;
 return DRV_OK;
}
int pascal huge GetMaxChannel(Word far *dac,Word far *adc)
{
 if(!Tbl) return DRV_FAILED;
 *dac=Tbl->MaxDAC; *adc=Tbl->MaxADC;
 return DRV_OK;
}
int pascal huge GetInfoDAC(Word far *dac)
{
 if(!Tbl) return DRV_FAILED;
 *dac=Tbl->RangeDAC;
 return DRV_OK;
}
int pascal huge GetInfoADC(Word far *adc)
{
 if(!Tbl) return DRV_FAILED;
 *adc=Tbl->RangeADC;
 return DRV_OK;
}

