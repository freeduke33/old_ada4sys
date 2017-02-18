#include <stdarg.h>
#include <conio.h>
#include <stdio.h>

#include "multigen.h"


extern FILE *disp;


char *elapsTime(unsigned long sec)
{
 static char buf[40];

 sprintf(buf,"%02lu:%02u:%02u",(unsigned long)(sec/3600),(unsigned)((sec/60)%60),(unsigned)(sec%60));
 return buf;
}

void xprintf(const char *fmt, ...)
{
 va_list ap;
 char buf[200];
 int i;

 if(disp!=stdout)
   {
   gotoxy(1,1); for(i=0;i<80;i++) putch(' ');
   gotoxy(1,1); cprintf("MGEN: ");
   }
 va_start(ap,fmt);
 vsprintf(buf,fmt,ap);
 if(disp!=stdout)
   {
   buf[75]=0;
   for(i=0;buf[i];i++) if(buf[i]=='\n') buf[i]=' ';
   cprintf(buf);
   }
 else fprintf(disp,buf);
 va_end(ap);
 return;
}
void xwait(void)
{
 if(disp!=stdout)
   {
   while(kbhit()) getch();
   getch();
   }
 return;
}


