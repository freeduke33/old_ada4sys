#define TEST

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <dos.h>

#include "lcard.h"
#define CHANNEL 16

#ifdef TEST

volatile unsigned int val,dac[CHANNEL],adc[CHANNEL];
volatile unsigned long Tik=0;

void interrupt test(void)
{
 int i;

// putch('<');
 Tik++;

 putData(0x300,1);

 for(i=0;i<8;i++) dac[i]=(Tik%1000)+i*1000;

 for(i=0;i<CHANNEL;i++) getData(0x300,(unsigned *)&adc[i]);
 for(i=0;i<CHANNEL;i++) putData(0x300,dac[i]);

 val=inport(0x308);
 outport(0x20,0x20);
 outport(0xA0,0x20);

// putch('>');

 return;
}

int doTest(float freq)
{
 float F;
 clock_t tb,te;
 int i;


   SetHook((intrProc)test);

   printf("Passed: %lu\n",Tik);
   tb=clock();
//   Enable(100,10);
   fEnable(freq);
   while(!(kbhit() && getch()==27))
     {
     te=clock(); if(tb!=te) F=(Tik*CLK_TCK)/(te-tb); else F=0;
     printf("\r%-7lu (%5.1f),",Tik,F);
     for(i=0;i<5;i++) printf("%4d ",dac[i]);
     printf("--> ");
     for(i=0;i<5;i++) printf("%4d ",adc[i]);
     }
   Disable();
   te=clock();
   if(tb!=te) F=(Tik*CLK_TCK)/(te-tb);
   else F=0;
   printf("\nDone (by freq %.1fHz).\n",F);
 return 0;
}

#endif





int main(int argc ,char *argv[])
{
 int val;
 float freq;

 if(argc!=3) { printf("Can't specified parameter\n"); return -1; }
 switch(val=LoadBios(0x300,argv[1]))
   {
   case lc_NONE:        printf("Successful loaded.\n"); break;
   case lc_NOBIOSFILE:  printf("file not found.\n"); break;
   case lc_BADBIOSFILE: printf("invalid file structure or file too long.\n"); break;
   case lc_NOTAIL:      printf("tail can't found.\n"); break;
   case lc_NORESPONSE:  printf("can't response.\n"); break;
   case lc_UNKNOWN:     printf("error detected.\n"); break;
   }

#ifdef TEST
 if(val==lc_NONE)
   {
   freq=atof(argv[2]);
   doTest(freq);
   }
#endif

 return val;
}
