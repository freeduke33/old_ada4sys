#include <stdio.h>

#include "tcxlwin.h"
#include "pro_achh.h"
#ifdef SYS_TEST

#define ufile_name Cfg.ufile_name
#define nr Cfg.nr
#define nq Cfg.nq
#define delt Cfg.delt
#define nko Cfg.nko
#define nki Cfg.nki
#define ifile_k Cfg.ifile_k
#define ifile_st Cfg.ifile_st
#define wfile_name Cfg.wfile_name
#define ifile_name Cfg.ifile_name
#define afile_name Cfg.afile_name
#define sfile_name Cfg.sfile_name







void CTYP Inf_file(NOARG);



void write_wzs(void)
{
 FILE *f;
 int k,i;

 Inf_file();

 f=fopen(wfile_name,"w+t");
 if(f)
   {
   fprintf(f,"+++ Заголовок_: \"ВЗАИМОДЕЙСТВИЕ_В_СТАТИКЕ\"\n"
             "+++ Частота_дискретизации_: %8.3f\n"
             "+++ Число_каналов_: %d x %d\n",1.0/delt,nko,nki);

   fprintf(f,"\n+++ Значения_выходных_сигналов_системы\n");
   for(i=0;i<15;i++)
     {
     fprintf(f,"%d ",i);
     for(k=0;k<nko;k++) fprintf(f,"%6.2f ",yh[0][k][i]*5.0);
     fprintf(f,"\n");
     }

   fprintf(f,"\n+++ Значения_коэффициентов_взаимосвязи\n");
   for(i=0;i<4;i++)
     {
     for(k=0;k<nko;k++) fprintf(f,"%7.4f  ",be[0][0][k][i]);
     fprintf(f,"\n");
     }

   fclose(f);
   }
 else Wperror(" Не могу открыть файл для записи взаимодействия в статике ");

 Wclose();

 return;
}


void write_st(void)
{
 FILE *f;
 unsigned long i;
 int k,j;

 Inf_file();

 f=fopen(sfile_name,"w+t");
 if(f)
   {
   fprintf(f,"+++ Заголовок_: \"СТАТИЧЕСКИЕ_ХАРАКТЕРИСТИКИ\"\n"
             "+++ Частота_дискретизации_: %8.3f\n"
             "+++ Длина_ряда_: %lu\n"
             "+++ Число_каналов_: %d x %d\n",1.0/delt,(long)nq,nko,nki);
   for(j=0;j<nki;j++)
     {
     fprintf(f,"\n+++ Управляемый_канал_: %d\n",j+1);
     for(i=0;i<nq;i++)
       {
       fprintf(f,"%-6lu %6.2f ",i,xh[i]*5.0);
       for(k=0;k<nko;k++) fprintf(f,"%6.2f ",yh[j][k][i]*5.0);
       fprintf(f,"\n");
       }
     }
   fclose(f);
   }
 else Wperror(" Не могу открыть файл для записи статических характеристик ");

 Wclose();

 return;
}

void write_achh(void)
{
 FILE *f;
 unsigned long i;
 int k,j;

 Inf_file();

 f=fopen(afile_name,"w+t");
 if(f)
   {
   fprintf(f,"+++ Заголовок_: \"АЧХ\"\n"
             "+++ Частота_дискретизации_: %8.3f\n"
             "+++ Длина_ряда_: %lu\n"
             "+++ Число_каналов_: %d x %d\n",1.0/delt,(long)nq,nko,nki);
   for(j=0;j<nki;j++)
     {
     fprintf(f,"\n+++ Управляемый_канал_: %d\n",j+1);
     for(i=0;i<nq;i++)
       {
       fprintf(f,"%-6lu %6.2f ",i,xh[i]);
       for(k=0;k<nko;k++) fprintf(f,"%6.2f ",yh[j][k][i]);
       fprintf(f,"\n");
       }
     }
   fclose(f);
   }
 else Wperror(" Не могу открыть файл для записи АЧХ ");

 Wclose();

 return;
}
#else

void write_wzs(void) { return; }
void write_achh(void) { return; }
void write_st(void) { return; }

#endif

