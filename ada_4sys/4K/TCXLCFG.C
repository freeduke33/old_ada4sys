#include <tcxlwin.h>
#include "pro_achh.h"

void xLoadCfg(char *fn)
{
 switch(loadConfig(fn))
   {
   case 0: /* success */  break;
   case 1: Wperror("������ �ଠ� 䠩�� ���䨣��樨"); break;
   case 2:  Wperror("�訡�� �⥭�� 䠩�� ���䨣��樨"); break;
   }
 return;
}

void xSaveCfg(char *name)
{
 switch(saveConfig(name))
   {
   case 0: /* success */ break;
   case 1: Wperror("�訡�� ����� 䠩�� ���䨣��樨"); break;
   }
 return;
}
