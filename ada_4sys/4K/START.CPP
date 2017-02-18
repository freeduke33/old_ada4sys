#ifdef __BORLANDC__
#  define ENABLE_SWAP
#endif

#include <process.h>
#include <stdlib.h>
#include <dos.h>
#include <errno.h>


extern "C" {
// #include "look_grf.h"
#include <tcxlwin.h>
#include <tcxlinp.h>
 }

#ifdef ENABLE_SWAP
# include "swap.h"
#endif
#include "pro_achh.h"

#define loop_process "ada_loop.ovl"

void PrepareConfig(void)
{
 int i;
#define MAX_DEFINED 4

 for(i=MAX_DEFINED;i<CHANNEL;i++) Cfg.Def[i]=Cfg.Def[i%MAX_DEFINED];
}

extern "C" void Loop(void)
{
 int ret;
 char buf[20];

 if(ScrSave())
   {
   Inf_Rab();

   PrepareConfig(); // перенос конфигурации из младших каналов в старшие

   itoa(PID,buf,10);
   xSaveCfg("lastwork.ini");
#ifdef ENABLE_SWAP
   while(!swap_ison()) swap_on();
#endif
   ret=spawnl(P_WAIT,loop_process,loop_process,buf,"1","lastwork.ini",NULL);
   xLoadCfg("lastwork.ini");

   Wprtf(7,4,_RED|LGREY,"Время работы       :");
   WprtCen(9,_CYAN|BLACK|_BLINK,"  Нажмите любую клавишу  ");
   if(ret==0 || ret==12) KeyWait();
   Wclose();
   ScrRest();
   }
 else ret=1;

 switch(ret)
  {
  case  0: /*все нормально*/
  case 12: /*прервано оператором*/
     graf_2time();
   break;
  case -1:
   switch(errno)
     {
     case E2BIG:
     case EINVAL:
       Wperror("DOS: Проблемы при запуске эксперимента");
       break;
     case ENOENT:
       Wperror("DOS: Отсутствует модуль управления экспериментом \"" loop_process "\"");
       break;
     case ENOEXEC:
       Wperror("DOS: Модуль управления экспериментом \"" loop_process "\" поврежден");
       break;
     case ENOMEM:
       Wperror("DOS: Недостаточно свободной памяти для проведения эксперимента");
       break;
     }
   break;
  case 1: Wperror("EXE: Внутренняя ошибка периода выполения"); break;
  case 2:
  case 3:
  case 4:
  case 5:
  case 13:
  case 15:
  case 16:
  case 17:
     Wperror("Несовместимая версия модуля проведения эксперимента"); break;
  case 6: Wperror("Ошибка при формировании эталона"); break;
  case 7: Wperror("Нет файла эталона"); break;
  case 8: Wperror("Неподходящее число каналов для выбранного эталона"); break;
  case 9: Wperror("Неподходящая длинна ряда для выбранного эталона"); break;
  case 10:Wperror("Проблемы в процессе запуска эксперимента"); break;
  case 11:Wperror("Ошибка в процессе проведения эксперимента"); break;
  case 14:Wperror("Ошибка при передаче параметров в модуль проведения эксперимента"); break;
  case 18: Wperror("Недостаточно свободной памяти для проведения эксперимента"); break;
  default: Wperror("Проблемы в процессе проведения эксперимента");
  }
 return;
}


// ======================= прием сигналов со стенда
extern "C" void from_stend(void)
{

}

