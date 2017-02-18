#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include <log.hpp>

Log::Log()
{
 Level[0]=0;
 Ini=0;
 Repack=0;
 return;
}

void Log::put(char level,const char *fmt, ...)
{
 va_list va;
 int i,ok=0;

 va_start(va,fmt);

 if(Level[0]==0) ok=1;
 else for(i=0;Level[i] && !ok;i++) if(Level[i]==level) ok=1;


 if(ok)
   {
   vsprintf(buf,fmt,va);
   writeLog(buf);
   }

 va_end(va);
}


void Log::setLevel(const char *level)
{
 strncpy(Level,level,sizeof(Level));
 Level[sizeof(Level)]=0;
}

void Log::setPackSize(const unsigned long init,const unsigned long repack)
{
 Ini=init;
 Repack=repack;
 if(isNeedPack()) Pack();
}

