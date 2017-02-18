#define SAFEMEM 100

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include <math.h>
#include <malloc.h>
#include <conio.h>

#include "dac_adc.h"
#include "config.h"
#include "common.h"
#include "multigen.h"

#define Uses_gen_Const
#define Uses_gen_Sinus
#define Uses_gen_PSinus
#define Uses_gen_DatFile
#define Uses_gen_PrgFile
#define Uses_AsyncLoop
#define Uses_NilBuffer
#define Uses_MemBuf
#include "adaptive.hpp"

#include "config.h"
#include "cfgloop.hpp"
#include "okmodel.hpp"
#include "mgloop.hpp"


#include "..\\working.inc"

#define min(a,b) ((a)<(b) ? (a) : (b))

extern "C" { FILE *of,*inf,*disp; }
unsigned int __stklen=1024*20;

int DoGenerate(char *file)
{
 LoopSC *Ctrl;
 CfgLoop *Def;
 int val=1;

 tprintDump(80,25,WorkingPanel);
 tprintf(WORKING_TITLE,"Неадаптивное управление");

 tprintf(WORKING_PART,"Подготовка к испытанию");
 if(loadConfig(file)==CFG_OK)
   {
   tprintf(WORKING_ETALON,"Файл данных <%s>",Cfg.Def[0].file_name);
   Ctrl=new aLoop(Cfg.nk,Cfg.nkInfo);
   Def=new CfgLoopGen(&Cfg,Ctrl,SAFEMEM);

   if(Def && Ctrl && Def->PrepareLoop()==0)
     {
//     printf("Осталось свободной памяти: %uKb\n",DosMemoryAvail()/1024);
     if(DosMemoryAvail()>1024*5) val=Ctrl->PassLoop(Cfg.Freq,Cfg.cycle,Cfg.nr);
     else val=ERR_NOMEM;

     switch(val)
         {
         case RC_USERBREAK:
            tprintf(WORKING_PART_STATUS," Прервано оператором ");
            val=ERR_USERBREAK;
         case RC_DONE:
            tprintf(WORKING_PART,"Сохранение результатов испытания");
            Def->WriteResult(0);
            break;
         case RC_INVALIDBUILD:
            tprintf(WORKING_PART_STATUS," Проблемы при запуске задания ");
            val=ERR_BUILD;
            break;
         case RC_NOMEM:
            tprintf(WORKING_PART_STATUS," Нужно больше свободной памяти ");
            val=ERR_NOMEM;
            break;
         case RC_UPI:
            tprintf(WORKING_PART_STATUS," Ошибка интерфейса УСО ");
            val=ERR_UPIDRV;
            break;
         }
     }
   if(Def)  delete Def;
   if(Ctrl) delete Ctrl;
   }
 else  { val=ERR_CFG; tprintf(WORKING_PART_STATUS," Неверный файл конфигурации <%s>",file); }

 return val;
}

int main(int argc,char *argv[])
{
 int ret;

 fprintf(stdout,"Модуль генератора сигнала нагружения v1.01 (C) \"Системы Триал\" 1995-97.\n");
 if(argc!=2)
     {
     fprintf(stdout,"Запуск:\n?:\\> %s <CfgFile>\n",argv[0]);
     return ERR_CMDLINE;
     }

 if(!DrvIsPresent() || DrvInitialize()!=DRV_OK)
   {
   fprintf(stdout,"Драйвер УСО не найден.\n");
   return ERR_UPIDRV;
   }

#ifdef __WATCOMC__
  _heapgrow();
#endif

 ret=DoGenerate(argv[1]);
 tprintf(WORKING_HOTLINE,"Сеанс работы закончен, нажмите любую клавишу для продолжения ...");
 getch();
 DrvDeinitialize();

 return ret;
}
