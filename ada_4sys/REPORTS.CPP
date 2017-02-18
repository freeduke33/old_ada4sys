#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#include "tcxlwin.h"
#include "pro_achh.h"
#include "common.h"

#define Uses_Buffer
#include <adaptive.hpp>

#define ufile_name Cfg.ufile_name
#define nq Cfg.nq
#define ifile_k Cfg.ifile_k
#define ifile_st Cfg.ifile_st
#define wfile_name Cfg.wfile_name
#define ifile_name Cfg.ifile_name
#define afile_name Cfg.afile_name
#define sfile_name Cfg.sfile_name


void MakeEtalon(int,Buffer **);


int setExt(char *file_name,char *ext);



extern "C" void write_upr(void)
/*{
 FILE *f;
 int k,ff,nk,i;
 unsigned long nr,len[CHANNEL],l;
 short int isPresent[CHANNEL][3];
 float delt,val;




 ff=open("lastwork.dat",O_BINARY|O_RDONLY);
 BufInit();
 BufRead(ff,&k,sizeof(k));
 if(k!=PID) { Wperror("Нет данных для сохранения в файл"); goto done; }

 Inf_File();

 BufRead(ff,&nk,sizeof(nk));
 BufRead(ff,&delt,sizeof(delt));

 for(k=0,nr=0;k<nk;k++)
     {
     BufRead(ff,&len[k],sizeof(len[k])); nr= nr>len[k] ? nr : len[k];
     for(i=0;i<3;i++) BufRead(ff,&isPresent[k][i],sizeof(isPresent[k][i]));
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
        if(isPresent[k][i] && l<len[k]) BufRead(ff,&val,sizeof(val));
        else val=0;
        fprintf(f,"\t%+7.3f",val*5.0);
        }
     fprintf(f,"\n");
     }
   fclose(f);
   }
 else Wperror(" Не могу открыть файл для записи протокола управления ");

 Wclose();

done:
 close(ff);

 return;
}

extern "C" void write_tem(void) */
{
 FILE *f;
 int k,ff,nk,i;
 unsigned long nr,len[CHANNEL],l;
 short int isPresent[CHANNEL][3];
 float delt,val;


 ff=open("lastwork.dat",O_BINARY|O_RDONLY);
 BufInit();
 BufRead(ff,&k,sizeof(k));
 if(k!=PID) { Wperror("Нет данных для сохранения в файл"); goto done; }

 Inf_File();

 BufRead(ff,&nk,sizeof(nk));
 BufRead(ff,&delt,sizeof(delt));

 for(k=0,nr=0;k<nk;k++)
     {
     BufRead(ff,&len[k],sizeof(len[k])); nr= nr>len[k] ? nr : len[k];
     for(i=0;i<3;i++) BufRead(ff,&isPresent[k][i],sizeof(isPresent[k][i]));
     }

 setExt(ufile_name,".TEM");

 f=fopen(ufile_name,"w+t");
 if(f)
   {
   fprintf(f,"%d %8.3f %lu\n",nk,delt,nr);

   for(l=0;l<nr;l++)
     {
     fprintf(f,"%lu",l);
     for(k=0;k<nk;k++) for(i=0;i<3;i++)
        {
        if(isPresent[k][i] && l<len[k]) BufRead(ff,&val,sizeof(val));
        else val=0;
        if(i==0) fprintf(f,"\t%+7.3f",val*5.0);
        }
     fprintf(f,"\n");
     }
   fclose(f);
   }
 else Wperror(" Не могу открыть файл для записи файла эталонного сигнала ");

 Wclose();

done:
 close(ff);

 return;
}

extern "C" void write_et(void)
{
 Buffer *yt[CHANNEL];
 FILE *f;
 unsigned long l;
 int k,i;


 Inf_File();

 for(i=0;i<Cfg.nk;i++)
   {
   MakeEtalon(i,&yt[i]);
   yt[i]->reset();
   }

 setExt(ifile_name,".TEM");

 f=fopen(ifile_name,"w+t");
 if(f)
   {
   fprintf(f,"%d %8.3f %lu\n",Cfg.nk,Cfg.Freq,Cfg.nr);
   for(l=0;l<Cfg.nr;l++)
     {
     for(k=0;k<Cfg.nk;k++) fprintf(f,"%6.2f ",yt[k]->getNext()*5.0);
     fprintf(f,"\n");
     }
   fclose(f);
   }
 else Wperror(" Не могу открыть файл для записи эталонного сигнала ");

 Wclose();

 return;
}


int setExt(char *file_name,char *ext)
{
 char d[5],d1[100],nm[10],ex[5];
 int i;

 for(i=strlen(file_name);file_name[i]==' ';i--) file_name[i]=0;
 _splitpath(file_name,d,d1,nm,ex);
 _makepath(file_name,d,d1,nm,ext);
 return 0;
}


