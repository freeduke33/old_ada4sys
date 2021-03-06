#define DEBUG
#define USE_DOSTEXT


#include <time.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#ifdef USE_DOSTEXT
#ifndef __TURBOC__
#include <graph.h>
#endif
#endif


#define Uses_SyncLoop
#define Uses_AsyncLoop
#define Uses_Anatomy
#include <adaptive.hpp>

#include "txtloop.hpp"
#include "dac_adc.h"
#include "config.h"
#include "common.h"



int txtLoop::showProcess(LoopShow Mode)
{
 unsigned long skip,cur;
 unsigned percent;
 int i,k,flg=0;
 float m;

 static time_t clk=0;
 static sec=600;

 switch(Mode)
   {
   case INITPROCESS: showStart(); break;
   case FINAL:       showDone();  break;
   case INITPART:    tprintf(WORKING_PART,"����㦥���"); flg=1; break;
   case PASSPART:    flg=0; break;
   }

 if(flg || (clk+CLK_TCK)<clock())
   {
//   m=Mistake();
   if(!sec--)
     {
     sec=600;
     clk=time(NULL);
     PUTLOG('T',".");
     PUTLOG('M',"����譮��� [%6.4f, %6.4f, %6.4f, %6.4f]",m,m,m,m);
     }
   clk=clock();

   DrvGetSkipped(&skip);

   if(Info.OkCycle==Cycles) { cur=Cycles; }
   else  { cur=Info.OkCycle+1; }

    percent=(unsigned)((Info.OkTik+Info.OkCycle*Tiks)*100/(Tiks*Cycles));

    tprintf(WORKING_PART_PERSENT,"%3u%",percent);

    tprintf(WORKING_INFO_VAL(0)," %ld",skip);
    for(i=0;i<Chl;i++)
       tprintf(WORKING_MISTAKE_VAL(i)," %6.4f",Mistake(i));

    if(infoModel) for(i=k=0;i<infoChl;i++) if(infoModel[i])
      switch(infoModel[i]->getState())
        {
        case 0: tprintf(WORKING_INFO_VAL(1+k++)," ��ଠ"); break;
        case 1: tprintf(WORKING_INFO_WARNING(1+k++)," ��������"); break;
        case 2: tprintf(WORKING_INFO_CRASH(1+k++),"  �����"); break;
        }

  if(kbhit())
    {
    int c=getch();
    switch( c ? c : getch()*0x100 )
      {
      case 0x1B:   Info.ResultCode=RC_USERBREAK; return 1;
      case 0x1F00: Info.ResultCode=RC_USERPAUSE; return 1;
      case 0x20:
          tprintf(WORKING_HOTLINE," ���⠭�� �ਮ�⠭������:  \027\x3F<Enter>\027\xFF - �த������ \027\x3F<Esc>\027\xFF - ��ࢠ��");
          while(c) switch(getch())
            {
            case 0x0D: c=0; break; /* �த������ */
            case 0x1B: Info.ResultCode=RC_USERBREAK; return 1;
            }
          tprintf(WORKING_HOTLINE,HOTLINE_DEFSTR);
          break;
      }
    }
   }

 return 0;
}


void txtLoop::showStart(void)
{
 time_t t;
 int k,i;

 tprintf(WORKING_PART,"���樠������ ��⥬� �ࠢ�����");
 tprintf(WORKING_PART_PERSENT,"%3u%",0);
 tprintf(WORKING_HOTLINE,HOTLINE_DEFSTR);

 old=clock();
 t=time(NULL);
// LOG->put('S',"\n%s\t����� ��⥬� �ࠢ�����.\t",txtTime(ctime(&t)));
 PUTLOG('S',"�����  ��⥬� �ࠢ����� (����������).");

    tprintf(WORKING_INFO_DSC(0),"�ய�饭� �������⢨�");
    for(i=0;i<Chl;i++)
       tprintf(WORKING_MISTAKE_DSC(i),"����� %d",i);

    if(infoModel) for(i=k=0;i<infoChl;i++) if(infoModel[i])
            tprintf(WORKING_INFO_DSC(1+k++),"���ଠ樮��� ����� %d",i+1);

 return;
}

void txtLoop::showDone(void)
{
 time_t t;

 t=time(NULL);
 PUTLOG('S',"��⠭�� ��⥬� �ࠢ�����.");
 if(Info.ResultCode==RC_USERBREAK)
    tprintf(WORKING_PART_STATUS,"���⠭�� ��ࢠ�� �����஬");


 tprintf(WORKING_ALL_TIME,"%-12s",txtClock(clock()-old));
 tprintf(WORKING_PART,"����ࢠ��");
 tprintf(WORKING_PART_PERSENT,"%3u%",0);

 return;
}


