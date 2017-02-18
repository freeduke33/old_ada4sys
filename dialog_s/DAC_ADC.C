/*  */
#include <dos.h>
#include <string.h>
#include <stdio.h>

#include "dac_adc.h"

// #define TRACE         // --------- отладочная печать
// #define LOOPBACK      // --------- короткое замыкание
// #define FILTER        // --------- фильтрация АЦП-сигнала

#ifdef __386__
#  define x w
#  define int86 int386
#endif


#define _fstrcmp(a,b) strcmp(a,b)
#define _fstrcpy(a,b) strcpy(a,b)
#define _fmemcpy(a,b,c) memcpy(a,b,c)

#define min(a,b) ( (a)>(b) ? (b) : (a))

DRV_Table_T __FuncTable__;

static unsigned long DrvTimerFreq=0;
static unsigned char DrvNum=0;
static char NullErrMsg[]="Sorry, unknown description",
            NoneDrvMsg[]="Driver don't installed";

static Word Adc[50],Dac[50],NumDAC,NumADC,MaxDAC,MaxADC;

#define TEST_SUBFUN        0
#define INIT_SUBFUN        1
#define DEINIT_SUBFUN      2
#define GETTBL_SUBFUN      3
#define COPYRIGHT_SUBFUN   4
#define ERRMSG_SUBFUN      5


#define ToDAC(a)     ( (int) ((a)*(float)MaxDAC+MaxDAC)               )
#define VerifyDAC(a) ( (a<0) ? 0 : (a)>=MaxDAC*2 ? MaxDAC*2-1 : (a)   )
#define FromDAC(a)   ( ((float)(((int)(a))-(int)MaxDAC)) /(int)MaxDAC )
#define FromADC(a)   ( ((float)(((int)(a))-(int)MaxADC)) /(int)MaxADC )



int DrvEnable(float Freq)
{
 Word DivHi,DivLo,i;
 float tmp,delt,mdelt;
 int val;

#ifdef LOOPBACK
 return DRV_OK;
#endif

 if(Freq<=0) return DRV_ERRARG;
 tmp=DrvTimerFreq/Freq;
 if(tmp>65000L)
    {
    mdelt=65000U;
    for(i=(unsigned)min(65000L,tmp/2);i>1000;i--)
       {
       delt=tmp-((unsigned long)(tmp/i))*i;
       if(delt<mdelt) { mdelt=delt; DivHi=i; }
       }
    DivLo=tmp/DivHi;
    }
 else { DivLo=2; DivHi=(unsigned)tmp/2; }

#ifdef TRACE
 printf(">>>>>>>> %u %u",DivHi,DivLo);
#endif

 if(DivLo*DivHi>0) val=_DrvEnable(DivHi,DivLo);
 else              val=DRV_ERRARG;

 return val;
}

int DrvDisable(void)
{
#ifdef LOOPBACK
 return DRV_OK;
#endif

 return _DrvDisable();
}

int DrvIsPresent(void)
{
 union REGS r;
 unsigned char i;
 char far *IDstring,IDnormal[]=DRV_IDstring;

#ifdef LOOPBACK
 return 1;
#endif

 DrvNum=0;
 for(i=0xff;i>=0xc0;i--)
   {
   r.h.al=TEST_SUBFUN;
   r.h.ah=i;
   int86(0x2f,&r,&r);
   IDstring=MK_FP(r.x.bx,r.x.dx);
   if(!strcmp(IDstring,IDnormal))  { DrvNum=i; break; }
   }
 return DrvNum;
}

int DrvInitialize(void)
{
 union REGS r;
 int val;
 DRV_Table_P ptr;

#ifdef LOOPBACK
 return DRV_OK;
#endif

 if(DrvIsPresent())
   {
   r.h.al=INIT_SUBFUN;
   r.h.ah=DrvNum;
   int86(0x2f,&r,&r);
   if(r.x.ax==DRV_OK)
     {
     val=DrvGetFuncTable(&ptr);
     memcpy(&__FuncTable__,ptr,sizeof(*ptr));
     if(val==DRV_OK)
       {
       DrvGetMaxFreq(&DrvTimerFreq);
       DrvGetMaxChannel(&NumDAC,&NumADC);
       DrvGetInfoDAC(&MaxDAC); MaxDAC/=2;
       DrvGetInfoADC(&MaxADC); MaxADC/=2;
       }
     }
   else val=DRV_FAILED;
   }
 else val=DRV_NONE;

 return val;
}

int DrvDeinitialize(void)
{
 union REGS r;
 int val;

#ifdef LOOPBACK
 return DRV_OK;
#endif

 if(DrvNum)
   {
   r.h.al=DEINIT_SUBFUN;
   r.h.ah=DrvNum;
   int86(0x2f,&r,&r);
   val=r.x.ax;
   }
 else val=DRV_NONE;

 return val;
}


int DrvGetCopyright(char far *str)
{
 int val;

#ifdef LOOPBACK
 return DRV_OK;
#endif

 if(DrvNum)
   {
   strcpy(str,__FuncTable__.Copyright);
   val=DRV_OK;
   }
 else val=DRV_NONE;

 return val;
}

int DrvGetName(char far *str)
{
 int val;

#ifdef LOOPBACK
 return DRV_OK;
#endif

 if(DrvNum)
   {
   strcpy(str,__FuncTable__.NameDRV);
   val=DRV_OK;
   }
 else val=DRV_NONE;
 return val;
}

int DrvGetErrorMsg(char far *msg)
{
 union REGS r;
 char far *str=NullErrMsg;
 int val=DRV_OK;

#ifdef LOOPBACK
 return DRV_OK;
#endif

 if(DrvNum)
   {
   r.h.ah=DrvNum;
   r.h.al=ERRMSG_SUBFUN;
   int86(0x2f,&r,&r);
   str=MK_FP(r.x.bx,r.x.dx);
   if(!str[0]) str=NullErrMsg;
   }
 else
   {
   val=DRV_NONE;
   str=NoneDrvMsg;
   }
 strcpy(msg,str);

 return val;
}

int DrvGetFuncTable(DRV_Table_I tbl)
{
 union REGS r;
 int val;

#ifdef LOOPBACK
 return DRV_OK;
#endif

 if(DrvNum)
   {
   r.h.al=GETTBL_SUBFUN;
   r.h.ah=DrvNum;
   int86(0x2f,&r,&r);
   *tbl=MK_FP(r.x.bx,r.x.dx);
   val=DRV_OK;
   }
 else val=DRV_NONE;

 return val;
}

int DrvSetChannel(Word dac,Word adc)
{
 int val;

#ifdef LOOPBACK
 return DRV_OK;
#endif

 val=_DrvSetChannel(dac,adc);
 if(val==DRV_OK)
    {
    NumDAC=dac;
    NumADC=adc;
    }
 return val;
}

int DrvVolume(float far *BufDAC,float far *BufADC)
{
 int i,val;

 for(i=0;i<NumDAC;i++)
   {
   val=ToDAC(BufDAC[i]);
   Dac[i]=VerifyDAC(val);
   }

#ifdef LOOPBACK
 for(i=0;i<NumADC;i++) Adc[i]=Dac[i%NumDAC];
#else
 val=_DrvVolume(Dac,Adc);
#endif

#ifndef FILTER
 for(i=0;i<NumADC;i++) BufADC[i]=FromADC(Adc[i]);
#else
 for(i=0;i<NumADC;i++) BufADC[i]=Filter(i,FromADC(Adc[i]));
#endif
 return val;
}

int huge DrvSetVolDAC(float far *BufDAC)
{
 int i,val;

 for(i=0;i<NumDAC;i++)
  {
  val=ToDAC(BufDAC[i]);
  Dac[i]=VerifyDAC(val);
  }
#ifdef LOOPBACK
 return DRV_OK;
#endif

 return _DrvSetVolDAC(Dac);
}

int huge DrvGetVolADC(float far *BufADC)
{
 int i,val=DRV_OK;

#ifndef LOOPBACK
 val=_DrvGetVolADC(Adc);
#endif

#ifndef FILTER
 for(i=0;i<NumADC;i++) { BufADC[i]=FromADC(Adc[i]); }
#else
 for(i=0;i<NumADC;i++) { BufADC[i]=Filter(i,FromADC(Adc[i])); }
#endif
 return val;
}

int huge DrvGetVolDAC(float far *BufDAC)
{
 int i,val;

 val=_DrvGetVolDAC(Dac);
 for(i=0;i<NumDAC;i++) { BufDAC[i]=FromDAC(Dac[i]); }
 return val;
}

int huge _DrvGetVolDAC(Word far *BufDAC)
{
 Word far* dac,far* adc;
 int i,val;

#ifdef LOOPBACK
 return DRV_OK;
#endif

 val=_DrvGetBuf(&dac,&adc);
 for(i=0;i<NumDAC;i++) { BufDAC[i]=dac[i]; }

 return val;
}

