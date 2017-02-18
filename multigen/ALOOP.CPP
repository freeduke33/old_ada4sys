#include <conio.h>
#include <stdio.h>
#include <time.h>

#define Uses_AsyncLoop
#include <adaptive.hpp>

#include "multigen.h"
#include "mgloop.hpp"


#define mdl aLoop

extern "C" { extern FILE *disp; }


char *stime(time_t tm)
{
// int i;
 static char *buf,str[40];
 char tmp[10],y[6],t[10],d[5],m[10];

 buf=ctime(&tm);
 sscanf(buf,"%s %s %s %s %s",tmp,m,d,t,y);
// for(i=0;t[i];i++) if(t[i]==':') t[i]='.';
 sprintf(str,"%s %s %s, %s",d,m,y,t);

 return str;
}


void mdl::showStart()
{

 fprintf(disp,"\n");
 xprintf("Идет генерация задания -\r");
 bt=ct=wt=time(NULL);
 i=0;
 log=fopen("multigen.log","a+t");
 if(log)
   {
   fprintf(log,"\n%s -- Запуск испытания\n",stime(bt));
   }
}

int mdl::showProgress()
{
 time_t t=time(NULL);

 if(t>ct)
   {
   if(t>wt+10*1)
     {
     fprintf(log,"%s -- %f\n",stime(wt=t),0.0);
     }
   xprintf("Идет генерация задания - %-15s\r",elapsTime((ct=t)-bt));
   }
 if(kbhit() && getch()==27) return 1;
 return 0;
}

void mdl::showDone()
{
 ct=time(NULL);
 xprintf("Идет генерация задания - %-15s ",elapsTime(ct-bt));
 if(log)
   {
   fprintf(log,"%s -- испытание закончено (время работы %s).\n",stime(ct),elapsTime(ct-bt));
   fclose(log);
   }

}

