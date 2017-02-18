#include <string.h>
#include <io.h>

#include "log.hpp"
#include "config.h"
#include "common.h"

extern "C" {

int InitLog(void)
{
 char lvl[51];


 LOG=new Log2TxtFile("working.log");
 if(LOG)
   {
   switch(Cfg.LogLevel)
     {
     case 0: strcpy(lvl,""); break;
     case 1: strcpy(lvl,"S"); break;            // запускёостанов управления
     case 2: strcpy(lvl,"ST"); break;           // + временные точки
     case 3: strcpy(lvl,"STE"); break;          // + средняя погрешность
     case 4: strcpy(lvl,"STEM"); break;         // + махёмин погрешность
     case 5: strcpy(lvl,"STEMC"); break;        // + погрешность по каналам
     default:
             strcpy(lvl,"STEMC"); break;        // + погрешность по каналам
     }
   strncat(lvl,Cfg.DebugLevel,50); lvl[50]=0;
   LOG->setLevel(lvl);
   if(Cfg.LogSize) LOG->setPackSize(Cfg.LogSize/3,Cfg.LogSize);
   else            LOG->setPackSize(1024*50L,1024*1000L);
   }

 return LOG!=NULL;
}


int DoneLog(void)
{
 if(LOG) delete LOG;
 LOG=NULL;
 return 1;
}


}