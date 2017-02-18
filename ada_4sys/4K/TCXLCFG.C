#include <tcxlwin.h>
#include "pro_achh.h"

void xLoadCfg(char *fn)
{
 switch(loadConfig(fn))
   {
   case 0: /* success */  break;
   case 1: Wperror("Неверный формат файла конфигурации"); break;
   case 2:  Wperror("Ошибка чтения файла конфигурации"); break;
   }
 return;
}

void xSaveCfg(char *name)
{
 switch(saveConfig(name))
   {
   case 0: /* success */ break;
   case 1: Wperror("Ошибка записи файла конфигурации"); break;
   }
 return;
}
