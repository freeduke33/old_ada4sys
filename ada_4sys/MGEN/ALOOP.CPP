#include <conio.h>
#include <stdio.h>
#include <time.h>

#define Uses_AsyncLoop
#define Uses_Anatomy
#include <adaptive.hpp>

#include "multigen.h"
#include "mgloop.hpp"
#include "log.hpp"
#include "config.h"
#include "common.h"
#include "dac_adc.h"

#define mdl aLoop

mdl::mdl(unsigned int n,unsigned int inf) : AsyncLoop(n,inf)
{
 return;
}

void mdl::showStart()
{
 int k,i;

 tprintf(WORKING_PART,"���樠������");
 tprintf(WORKING_PART_PERSENT,"%3u%",0);
 tprintf(WORKING_HOTLINE,HOTLINE_DEFSTR);
 bt=ct=wt=time(NULL);

 PUTLOG('S',"����� ��⥬� �ࠢ����� (���������).");
 tprintf(WORKING_INFO_DSC(0),"�ய�饭� �������⢨�");
 tprintf(WORKING_MISTAKE_DSC(0),"�।��� �� �������");

 if(infoModel) for(i=k=0;i<infoChl;i++) if(infoModel[i])
      tprintf(WORKING_INFO_DSC(1+k++),"���ଠ樮��� ����� %d",i+1);
 return;
}

int mdl::showProcess(LoopShow Mode)
{
 int ret=0,r;
 unsigned long skip,cur;
 unsigned percent;
 int i,k;
 float m;

 static time_t clk=0;
 static sec=600;

 switch(Mode)
   {
   case INITPROCESS: r=0; showStart(); break;
   case DONEPROCESS: r=0; showDone();  break;
   case INITPART:    r=1;
                     tprintf(WORKING_PART,"����㦥���");
                     tprintf(WORKING_PART_PERSENT,"%3u",0);
                     break;
   case PASSPART:    r=0; break;
   }

 if(r || (clk+CLK_TCK)<clock())
   {
   m=Mistake();
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

    tprintf(WORKING_PART_PERSENT,"%3u",percent);

    tprintf(WORKING_INFO_VAL(0)," %ld",skip);
    tprintf(WORKING_MISTAKE_VAL(0)," %6.4f",m);

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
 return ret;
}

void mdl::showDone()
{
 ct=time(NULL);

 PUTLOG('S',"��⠭�� ��⥬� �ࠢ�����.");
 if(Info.ResultCode==RC_USERBREAK)
    tprintf(WORKING_PART_STATUS,"���⠭�� ��ࢠ�� �����஬");

 tprintf(WORKING_PART,"����ࢠ��");
 tprintf(WORKING_PART_PERSENT,"%3u",0);
 return;
}

