#define LOGGED
#include <stdarg.h>
#include "workarea.hpp"

FILE *LOGFILE=NULL;

void LOG(char *fmt,...)
{
 va_list p;

 if(LOGFILE)
   {
   va_start(p,fmt);
   vfprintf(LOGFILE,fmt,p);
   fflush(LOGFILE);
   va_end(p);
   }
 return;
}

