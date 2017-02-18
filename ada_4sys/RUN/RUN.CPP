#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include <adaptive.hpp>

#include "run.hpp"
#include "run.h"
#include "..\config.h"
#include "dac_adc.h"
#include "common.h"

// идентификатор сеанса
int PID;
// панель рабочего состояния
#include "..\working.inc"

#ifndef DETECTIVE

int main(int argc,char  *argv[])
{
 int Mode,val=0;
 char *CfgFile;

 fprintf(stdout,"Модуль управления испытаниями v2.05 (C) \"Системы Триал\" 1995-97.\n");

 if(DrvInitialize()==DRV_OK)
 {
 if(argc==4) /* <PID> <Mode> <CfgFile> */
   {
   tprintDump(80,25,WorkingPanel);

   PID=atoi(argv[1]);
   Mode=atoi(argv[2]);
   CfgFile=argv[3];

   tprintf(WORKING_PART,"Чтение конфигурации");
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
        tprintf(WORKING_PART_STATUS," Файл конфигурации не найден ");
        break;
     default:
        val=ERR_CFG;
        tprintf(WORKING_PART_STATUS," Файл конфигурации поврежден ");
        break;
     }
   tprintf(WORKING_HOTLINE," Сеанс работы закончен, нажмите любую клавишу для продолжения ...");
   getch();
   }
 else
   {
   fprintf(stdout,"Запуск: \n"
          "  ?:\\> %s <PID> <Mode> <CfgFile> \n"
          "\t <PID>  - идентификатор сеанса работы\n"
          "\t <Mode> - режим работы:\n"
          "\t\t 1 - адаптивное управление обьектом\n"
          "\t\t 2 - исследование АЧХ обьекта\n"
          "\t\t 3 - исследование статических характеристик обьекта\n"
          "\t\t 5 - управление обьектом с предварительным расчетом\n"
          "\t <CfgFile> - файл с рабочими настройками"
          ,argv[0]);
   val=ERR_CMDLINE;
   }
 DrvDeinitialize();
 }
 else { val=ERR_UPIDRV; fprintf(stdout,"Драйвер УСО не найден."); }

 return val;
}

#endif
