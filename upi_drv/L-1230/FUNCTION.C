#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#include "lcard.h"

#define IrqN 0x73


typedef void interrupt (far * IntrPtr)(void);


static IntrPtr HookPtr=NULL,oldPtr;
static unsigned oldMask,Base=0x300;


lcRet fEnable(float Freq)
{
 float DrvTimerFreq=10000000,mdelt,delt,tmp;
 unsigned i,DivLo,DivHi;

 if(Freq<=0) return lc_NOTSUPPORT;
 tmp=DrvTimerFreq/Freq;
 if(tmp>65000L)
    {
    mdelt=65000U;
    for(i=(unsigned)min(65000L,tmp);i>1000;i--)
       {
       delt=tmp-((unsigned long)(tmp/i))*i;
       if(delt<mdelt) { mdelt=delt; DivHi=i; }
       }
    DivLo=tmp/DivHi;
    }
 else { DivLo=2; DivHi=(unsigned)tmp/2; }

 printf(">>>>> %u %u\n",DivHi,DivLo);
 return Enable(DivHi,DivLo);
}


lcRet Enable(unsigned Hi,unsigned Lo)
{
 unsigned val;

 outport(Base+CONTROL,9);
 inport(Base+COMMAND);

 putCmd(Base,1);
 putData(Base,Lo);
 putData(Base,Hi);

 getData(Base,&val);
 return lc_NONE;
}

lcRet Disable(void)
{
 ClrHook();
 return putCmd(Base,2);
}

lcRet SetHook(void interrupt (*Ptr)(void))
{
 outport(Base+CONTROL,8);
 ClrHook();
 oldMask=inport(0xA1);


 oldPtr=getvect(IrqN);
 setvect(IrqN,HookPtr=Ptr);

 outport(0x20,0x20);
 outport(0xA0,0x20);

 outport(0xA1,0x94); // & oldMask);

 outport(0x20,0x20);
 outport(0xA0,0x20);



 return lc_NONE;
}


lcRet ClrHook(void)
{
 if(HookPtr)
   {
   outport(0xA1,oldMask);
   HookPtr=NULL;
   setvect(IrqN,oldPtr);

   outport(0x20,0x20);
   outport(0xA0,0x20);
   }
 return lc_NONE;
}


lcRet setBaseAddress(int base)
{
 Base=base;
 return lc_NONE;
}
