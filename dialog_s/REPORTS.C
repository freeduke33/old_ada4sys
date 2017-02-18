#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "reports.h"
#define CHANNEL MAX_CHANNEL
#define PID 0

#define ufile_name Cfg.ufile_name
#define ifile_k Cfg.ifile_k
#define ifile_st Cfg.ifile_st
#define wfile_name Cfg.wfile_name
#define ifile_name Cfg.ifile_name
#define afile_name Cfg.afile_name
#define sfile_name Cfg.sfile_name

void MakeEtalon(int,float *);


int setExt(char *file_name,char *ext)
{
 char d[5],d1[100],nm[10],ex[5];
 int i;

 for(i=strlen(file_name);file_name[i]==' ';i--) file_name[i]=0;
 _splitpath(file_name,d,d1,nm,ex);
 _makepath(file_name,d,d1,nm,ext);
 return 0;
}

void write_upr(void)
{
 FILE *f;
 int k,ff,nk,i;
 unsigned long nr,len[CHANNEL],l;
 short int isPresent[CHANNEL][3];
 float delt,val;

 ff=open("lastwork.dat",O_BINARY|O_RDONLY);
 if(ff<1) return;

 read(ff,&k,sizeof(k));
 // if(k!=PID) { /* Wperror("Нет данных для сохранения в файл"); */ goto done; }

// Inf_File();

 read(ff,&nk,sizeof(nk));
 read(ff,&delt,sizeof(delt));

 for(k=0,nr=0;k<nk;k++)
     {
     read(ff,&len[k],sizeof(len[k])); nr= nr>len[k] ? nr : len[k];
     for(i=0;i<3;i++) read(ff,&isPresent[k][i],sizeof(isPresent[k][i]));
     }

 setExt(ufile_name,".UPR");

 f=fopen(ufile_name,"w+t");
 if(f)
   {
   fprintf(f,"+++ Заголовок_: \"ПРОТОКОЛ_УПРАВЛЕНИЯ_СТЕНДОМ\"\n"
             "+++ Длинна_ряда_: %lu\n"
             "+++ Частота_дискретизации_: %8.3f\n"
             "+++ Число_каналов_: %d\n\n",nr,delt,nk);

   for(l=0;l<nr;l++)
     {
     fprintf(f,"%lu",l);
     for(k=0;k<nk;k++) for(i=0;i<3;i++)
        {
        if(isPresent[k][i] && l<len[k]) read(ff,&val,sizeof(val));
        else val=0;
        fprintf(f,"\t%+7.3f",val*5.0);
        }
     fprintf(f,"\n");
     }
   fclose(f);
   }
// else Wperror(" Не могу открыть файл для записи протокола управления ");

done:
 close(ff);

 return;
}

void write_tem(void)
{
 FILE *f;
 int k,ff,nk,i;
 unsigned long nr,len[CHANNEL],l;
 short int isPresent[CHANNEL][3];
 float delt,val;

 ff=open("lastwork.dat",O_BINARY|O_RDONLY);
 if(ff<1) return;

 read(ff,&k,sizeof(k));
 // if(k!=PID) { /* Wperror("Нет данных для сохранения в файл"); */ goto done; }

// Inf_File();

 read(ff,&nk,sizeof(nk));
 read(ff,&delt,sizeof(delt));

 for(k=0,nr=0;k<nk;k++)
     {
     read(ff,&len[k],sizeof(len[k])); nr= nr>len[k] ? nr : len[k];
     for(i=0;i<3;i++) read(ff,&isPresent[k][i],sizeof(isPresent[k][i]));
     }

 setExt(ufile_name,".TEM");

 f=fopen(ufile_name,"w+t");
 if(f)
   {
   fprintf(f,"%d %f %lu\n",nk,delt,nr);

   for(l=0;l<nr;l++)
     {
     for(k=0;k<nk;k++) for(i=0;i<3;i++)
        {
        if(isPresent[k][i] && l<len[k]) read(ff,&val,sizeof(val));
        else val=0;
        if(i==0) fprintf(f,"%+7.3f\t",val*5.0);
        }
     fprintf(f,"\n");
     }
   fclose(f);
   }
// else Wperror(" Не могу открыть файл для записи протокола управления ");

done:
 close(ff);

 return;
}

void write_et(void)
{
 float *yt[CHANNEL];
 FILE *f;
 unsigned long l;
 int k,i;


// Inf_File();

 for(i=0;i<Cfg.nk;i++)
   {
//   MakeEtalon(i,&yt[i]);
//   yt[i]->reset();
   }

 setExt(ifile_name,".TEM");

 f=fopen(ifile_name,"w+t");
 if(f)
   {
   fprintf(f,"%d %8.3f %lu\n",Cfg.nk,Cfg.Freq,Cfg.nr);
   for(l=0;l<Cfg.nr;l++)
     {
     for(k=0;k<Cfg.nk;k++) fprintf(f,"%6.2f ",yt[k][l]*5.0);
     fprintf(f,"\n");
     }
   fclose(f);
   }
// else Wperror(" Не могу открыть файл для записи эталонного сигнала ");

 return;
}



void write_wzs(void)
{
 FILE *f;
 int k,i;

 f=fopen(wfile_name,"w+t");
 if(f)
   {
   fprintf(f,"+++ Заголовок_: \"ВЗАИМОДЕЙСТВИЕ_В_СТАТИКЕ\"\n"
             "+++ Частота_дискретизации_: %8.3f\n"
             "+++ Число_каналов_: %d x %d\n",Cfg.Freq,Cfg.nk,Cfg.nk);

/*   fprintf(f,"\n+++ Значения_выходных_сигналов_системы\n");
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
  */
   fclose(f);
   }
// else Wperror(" Не могу открыть файл для записи взаимодействия в статике ");

 return;
}


void write_sth(void)
{
 FILE *f;
 unsigned long i;
 int k,j,ff,_PID,nk,nq,mode;
 float *buf=NULL,val;

 ff=open("last_sth.dat",O_BINARY|O_RDONLY);
 if(ff<1) return;
 read(ff,&_PID,sizeof(_PID));
 read(ff,&nk,sizeof(nk));

 f=fopen(sfile_name,"w+t");
 if(f)
   {
   fprintf(f,"+++ Заголовок_: \"СТАТИЧЕСКИЕ_ХАРАКТЕРИСТИКИ\"\n"
             "+++ Частота_дискретизации_: %8.3f\n"
             "+++ Число_каналов_: %d x %d\n",Cfg.Freq,nk,nk);
   for(j=0;j<nk;j++)
     {
     read(ff,&nq,sizeof(nq));
     read(ff,&mode,sizeof(mode));
     fprintf(f,"\n+++ Управляемый_канал_: %d\n"
                 "+++ Длинна_ряда_: %u\n",j+1,nq);
     buf=calloc(nq,sizeof(float));
     if(buf==NULL) goto done;
     for(i=0;i<nq;i++) read(ff,&buf[i],sizeof(float));
     for(i=0;i<nq*2;i++)
       {
       fprintf(f,"%-6lu %6.2f ",i,buf[i%nq]*5.0);
       read(ff,&val,sizeof(val));
       for(k=0;k<nk;k++) fprintf(f,"%6.2f ", k==j ? val*5.0 : 0.0);
       fprintf(f,"\n");
       }
     if(buf) free(buf);
     }
done:
   fclose(f);
   }
// else Wperror(" Не могу открыть файл для записи статических характеристик ");

 close(ff);
 return;
}

void write_achh(void)
{
 FILE *f;
 unsigned long i;
 int k,j,nq,ff,_PID,nk;
 float valX,valY;

 ff=open("last_ach.dat",O_BINARY|O_RDONLY);
 if(ff<1) return;
 read(ff,&_PID,sizeof(_PID));
 read(ff,&nk,sizeof(nk));

 f=fopen(afile_name,"w+t");
 if(f)
   {
   fprintf(f,"+++ Заголовок_: \"АЧХ\"\n"
             "+++ Частота_дискретизации_: %8.3f\n"
             "+++ Число_каналов_: %d x %d\n",Cfg.Freq,nk,nk);
   for(j=0;j<nk;j++)
     {
     read(ff,&nq,sizeof(nq));
     fprintf(f,"\n+++ Управляемый_канал_: %d\n"
                 "+++ Длинна_ряда_: %u\n",j+1,nq);
     for(i=0;i<nq;i++)
       {
       read(ff,&valX,sizeof(valX));
       read(ff,&valY,sizeof(valY));
       fprintf(f,"%-6lu %6.2f ",i,valX);
       for(k=0;k<nk;k++) fprintf(f,"%6.2f ", k==j ? valY : 0.0);
       fprintf(f,"\n");
       }
     }
   fclose(f);
   }
// else Wperror(" Не могу открыть файл для записи АЧХ ");
 close(ff);
 return;
}

