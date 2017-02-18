#ifndef __TABLE_HPP__
#define __TABLE_HPP__

#include <dos.h>
#include "dac_adc.h"


class Table
{
 public:
   // инициализация таблицы
   Table(int MaxDAC,int MaxADC,Word RangeDAC=4096,Word RangeADC=4096,Word MaxFreq=60000U);
   virtual ~Table();

   DRV_Table_P GetTable(void);

   inline void SetTblActive(void) { Tbl=this; }
   inline void SetTblPassive(void) { Tbl=NULL; }

   inline float huge GetWorkFreq(void) { return Freq;   }
   inline int huge GetWorkDAC(void)    { return ActDAC; }
   inline int huge GetWorkADC(void)    { return ActADC; }

   void SetDrvErrMsg(char far *);
   
 private:
   HookPtr HookFunc;
   static Table *Tbl;

   Word  RangeDAC,RangeADC,MaxFreq;
   Word  *BufDAC,*BufADC;
   float *TmpDAC,*TmpADC,Freq;
   int MaxDAC,MaxADC,ActDAC,ActADC;
   volatile int WaitFlg;

   static DRV_Table_T table;
   static char TableID[];
   static char Copyright[];
   static char NameDRV[];

   virtual void huge StartUPI(void) =0;
   virtual void huge StopUPI(void) =0;

   virtual int huge BuildUPI(void) =0;
   virtual int huge CalcSignal(float *iBuf,float *oBuf) =0;


   friend void interrupt DrvVector(union REGS *r);

   // ----- табличные функции -------
   friend int pascal huge Enable(Word DivHi,Word DivLo);
   friend int pascal huge Disable(void);
   friend int pascal huge GetVolADC(Word far *ADCbuf);
   friend int pascal huge SetVolDAC(Word far *DACbuf);
   friend int pascal huge SetHook(HookPtr);
   friend int pascal huge ClrHook(void);
   friend int pascal huge Volume(Word far *DACbuf,Word far *ADCbuf);
   friend int pascal huge GetBuf(Word far *far *DACbuf,Word far *far *ADCbuf);
   friend int pascal huge WaitDAC_ADC(void);
   friend int pascal huge SetChannel(int NumDAC,int NumADC);
   friend int pascal huge ClrSkipped(void);
   friend int pascal huge GetSkipped(unsigned long far *val);
   friend int pascal huge GetMaxFreq(unsigned long far *val);
   friend int pascal huge GetMaxChannel(Word far *dac,Word far *adc);
   friend int pascal huge GetInfoDAC(Word far *dac);
   friend int pascal huge GetInfoADC(Word far *adc);
};

#endif
