#define DEBUG

#include <time.h>
#include <dos.h>
#include <stdlib.h>

#include <tcxlwin.h>
#include <tcxlinp.h>
#include <tcxlcod.h>

#define Uses_SyncLoop
#define Uses_AsyncLoop
#include <adaptive.hpp>

#include "tcxloop.hpp"
#include "pro_achh.h"
#include "dac_adc.h"


char *txtClock(unsigned long tik)
{
 static char buf[50];
 unsigned int ss,mm,hh;
 unsigned long dd;

 tik/=CLK_TCK;
 ss=(int)(tik%60); tik/=60;
 mm=(int)(tik%60); tik/=60;
 hh=(int)(tik%24); tik/=24;
 dd=tik;


 if(dd) sprintf(buf,"%lu, %02u:%02u:%02u",dd,hh,mm,ss);
 else
 if(hh) sprintf(buf,"%02u:%02u:%02u",hh,mm,ss);
 else   sprintf(buf,"%02u:%02u",mm,ss);

 return buf;
}

char *txtTime(char *s)
{
 static char buf[50];
 char w[5],m[5],d[5],t[10],y[10];


 sscanf(s,"%s %s %s %s %s",w,m,d,t,y);
 sprintf(buf,"%s-%s-%s %s",d,m,y,t);
 return buf;
}


void tcxlLoop::showStart(void)
{
 time_t t;
 Inf_Rab();
 old=clock();
 t=time(NULL);
 LOG->put('S',"\n%s\tЗапуск системы управления.\t",txtTime(ctime(&t)));
 _progress(1);
 return;
}

int tcxlLoop::showProgress(void)
{
 return _progress(0);
}

void tcxlLoop::showDone(void)
{
 time_t t;
 _progress(1);
 t=time(NULL);
 Wprtf(7,4,_RED,"Время работы       :  %-12s",txtClock(clock()-old));
 WprtCen(9,_CYAN|BLACK|_BLINK,"  Нажмите любую клавишу  ");
 LOG->put('S',"\n%s\tОстанов системы управления.\t",txtTime(ctime(&t)));
 KeyWait();
 Wclose();
 return;
}


int tcxlLoop::_progress(int flg)
{
 unsigned long skip,cur;
 unsigned percent;
 static time_t clk=0;
 static sec=60;
 float m;

 if(flg || (clk+CLK_TCK)<clock())
   {
   m=Mistake();
   if(!sec--)
     {
     sec=60;
     clk=time(NULL);
     LOG->put('T',"\n%s\t ",txtTime(ctime(&clk)));
     LOG->put('E',"E=%6.3f\t",m);
     LOG->put('M',"(%6.3f/%6.3f)\t",m,m);
     LOG->put('S',"[%6.3f,%6.3f,%6.3f,%6.3f]\t",m,m,m,m);
     }

   clk=clock();

   DrvGetSkipped(&skip);

   if(Info.OkCycle==Cycles) { cur=Cycles;  percent=100; }
   else  { cur=Info.OkCycle+1; percent=(unsigned)(Info.OkTik*100/Tiks); }

   Wprtf(3,24,_RED,"%lu/%lu",cur,Cycles);
   Wprtf(4,24,_RED,"%02u% ",percent);
   Wprtf(5,24,_RED,"%lu",skip);
   Wprtf(6,24,_RED,"%06.3f",m);

   while(KeyEvent())
     switch(KeyGetc())
       {
       case Key_Esc: Info.ResultCode=RC_USERBREAK; return 1;
       case Key_C_S: Info.ResultCode=RC_USERPAUSE; return 1;
       }
   }
 return 0;
}

