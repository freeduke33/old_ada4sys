// #define FULL_EMS_USAGE

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <malloc.h>

#define Uses_SyncLoop
#define Uses_AsyncLoop
#define Uses_Anatomy
#define Uses_ExtTrendARSS_1x1

#include <adaptive.hpp>

// #include "ems\\emsbuf.hpp"
#include "txtLoop.hpp"

#include "config.h"
#include "cfgLoop.hpp"
#include "okmodel.hpp"

#include "common.h"
#include "run.h"

#define bufMem StaticMemBuf
//#define bufMem(a,b) EMSMemBuf(a)


unsigned int _stklen=1024*50U;

// �ᨭ�஭��� �ࠢ�����
int Loop(void)
{
 LoopSC *Ctrl;
 CfgLoop *Def;
 int val=RC_NOMEM;

 tprintf(WORKING_TITLE,"����⨢��� �ࠢ����� �⥭���");
 tprintf(WORKING_PART,"�����⮢�� � ����⨢���� �ࠢ�����");
 Ctrl=new txtLoop(Cfg.nk,Cfg.nkInfo);
 if(Ctrl)
   {
   Def=new CfgLoopAda(&Cfg,Ctrl);
   if(Def &&  (val=Def->PrepareLoop())==0)
     {
     if(DosMemoryAvail()>1024*10)
       {
       tprintf(WORKING_ETALON,"�� 蠡����");
       val=Ctrl->PassLoop(Cfg.Freq,Cfg.cycle,Cfg.nr);
       } /* if DosMemoryAvail() */
     } /* if Def */
   } /* if Ctrl*/

 switch(val)
    {
    case RC_USERBREAK:
        tprintf(WORKING_PART_STATUS," ��ࠢ����� ��ࢠ�� �����஬ ");
        val=ERR_USERBREAK;
    case RC_DONE:
        tprintf(WORKING_PART,"���࠭���� ��⮪��� ���⠭��");
        tprintf(WORKING_PART_PERSENT,"%3d",0);
        Def->WriteResult(PID);
        tprintf(WORKING_PART_PERSENT,"%3d",100);
        break;
    case RC_INVALIDBUILD:
        tprintf(WORKING_PART_STATUS," �訡�� �� �����⮢�� � ࠡ�� ");
        val=ERR_BUILD; break;
    case RC_NOMEM:
        tprintf(WORKING_PART_STATUS," ��� ᢮������ ����� ��� �믮������ ࠡ��� ");
        val=ERR_NOMEM; break;
    default:
//        tprintf(WORKING_PART_STATUS," �஡���� � ����� �஢������ ���⠭�� ");
        val=ERR_LOOP;
    }
 return val;
}

// ======================= �ਥ� ᨣ����� � �⥭��
int from_stend(void)
{
 return 0;
}

