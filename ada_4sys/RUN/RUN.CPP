#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include <adaptive.hpp>

#include "run.hpp"
#include "run.h"
#include "..\config.h"
#include "dac_adc.h"
#include "common.h"

// �����䨪��� ᥠ��
int PID;
// ������ ࠡ�祣� ���ﭨ�
#include "..\working.inc"

#ifndef DETECTIVE

int main(int argc,char  *argv[])
{
 int Mode,val=0;
 char *CfgFile;

 fprintf(stdout,"����� �ࠢ����� ���⠭�ﬨ v2.05 (C) \"���⥬� �ਠ�\" 1995-97.\n");

 if(DrvInitialize()==DRV_OK)
 {
 if(argc==4) /* <PID> <Mode> <CfgFile> */
   {
   tprintDump(80,25,WorkingPanel);

   PID=atoi(argv[1]);
   Mode=atoi(argv[2]);
   CfgFile=argv[3];

   tprintf(WORKING_PART,"�⥭�� ���䨣��樨");
   switch(loadConfig(CfgFile))
     {
     case 0:
        switch(Mode)
          {
          case runDEBUG: /* debug */
             break;
          case runLOOP: /* work: adaptive */
             val=Loop();
             break;
          case runNMDL: /* work: calculation template */
             val=doNomodel();
             break;
          case runACH: /* work: examine ACHH */
          case runSTH: /* work: examine STAT */
          case runWZS: /* work: examine WZS */
             val=doExamine(Mode);
             break;
          default:
             val=ERR_MODE;
             break;
          }
        saveConfig(CfgFile);
        break;
     case 1:
        val=ERR_NOCFGFILE;
        tprintf(WORKING_PART_STATUS," ���� ���䨣��樨 �� ������ ");
        break;
     default:
        val=ERR_CFG;
        tprintf(WORKING_PART_STATUS," ���� ���䨣��樨 ���०��� ");
        break;
     }
   tprintf(WORKING_HOTLINE," ����� ࠡ��� �����祭, ������ ���� ������� ��� �த������� ...");
   getch();
   }
 else
   {
   fprintf(stdout,"�����: \n"
          "  ?:\\> %s <PID> <Mode> <CfgFile> \n"
          "\t <PID>  - �����䨪��� ᥠ�� ࠡ���\n"
          "\t <Mode> - ०�� ࠡ���:\n"
          "\t\t 1 - ����⨢��� �ࠢ����� ��쥪⮬\n"
          "\t\t 2 - ��᫥������� ��� ��쥪�\n"
          "\t\t 3 - ��᫥������� ����᪨� �ࠪ���⨪ ��쥪�\n"
          "\t\t 5 - �ࠢ����� ��쥪⮬ � �।���⥫�� ���⮬\n"
          "\t <CfgFile> - 䠩� � ࠡ�稬� ����ன����"
          ,argv[0]);
   val=ERR_CMDLINE;
   }
 DrvDeinitialize();
 }
 else { val=ERR_UPIDRV; fprintf(stdout,"�ࠩ��� ��� �� ������."); }

 return val;
}

#endif
