#include <string.h>
#include <stdio.h>

#include "tcxlwin.h"
#include "view.h"



char sth_filename[70],tem_filename[70],wzs_filename[70];
int Sel;

float be[KAN_MAX+2][KAN_MAX+2][KAN_MAX+2][KAN_MAX+2];
int etalon(void) { return 1;}

char buf[5][40],tmp[40];

void ViewSTH(void)
{
 FILE *f;
 int i,j;
 unsigned long  n;

 f=fopen(sth_filename,"r+t");
 if(f)
   {
   fscanf(f,"%s %s %s  %s %s",buf[0],tmp,buf[1],buf[2],tmp);
   if(!strcmp(buf[0],"+++") && !strcmp(buf[2],"+++") &&
      !strcmp(buf[1],"\"СТАТИЧЕСКИЕ_ХАРАКТЕРИСТИКИ\""))
     {
     fscanf(f,"%f %s %s %d %s %d %s %s %s",&fdisk,tmp,tmp,&nki,tmp,&nko,tmp,tmp,tmp);
     delt=1.0/fdisk;
     for(i=0;i<nki;i++)
      {
      n=0;
      do {
         fscanf(f,"%s %f",tmp,&xh[n]); xh[n]/=5.0;
         for(j=0;j<nko;j++) { fscanf(f,"%f",&yh[i][j][n]); yh[i][j][n]/=5.0; }
         n++;
         } while(strcmp(tmp,"+++") && !feof(f));
      fscanf(f,"%s %s",tmp,tmp);
      }
     nq=n; graf_stat();
     }
   else  Wperror("Неверный формат файла");
   }
 else Wperror("Не могу открыть файл данных");
 fclose(f);
 return;
}

void ViewTEM(void)
{
 FILE *f;
 int i;
 unsigned long n;

 f=fopen(tem_filename,"r+t");
 if(f)
   {
   fscanf(f,"%s %s %s  %s %s",buf[0],tmp,buf[1],buf[2],tmp,tmp);
   if(!strcmp(buf[0],"+++") && !strcmp(buf[2],"+++") &&
      !strcmp(buf[1],"\"ПРОТОКОЛ_УПРАВЛЕНИЯ_СТЕНДОМ\""))
     {
     fscanf(f,"%ld %s %s %f %s %s %d %s %d",&nr,tmp,tmp,&fdisk,tmp,tmp,&nki,tmp,&nko);

     if(AllocBuf())
       {
       delt=1.0/fdisk;
       for(i=0;i<nki;i++)
         {
         fscanf(f,"%s %s %s",tmp,tmp,tmp);
         for(n=0;n<nr;n++)
           {
           fscanf(f,"%s %f %f %f",tmp,&x[i][n],&y[i][n],&yt[i][n]);
           x[i][n]/=5.0; y[i][n]/=5.0; yt[i][n]/=5.0;
           }
         }
       if(Sel==1) graf_time();
       else       graf_2time();
       }
     }
   else Wperror("Неверный формат файла");
   }
 else Wperror("Не могу открыть файл данных");
 fclose(f);
 return;
}

void ViewWZS(void)
{
 FILE *f;
 int i,j;
 unsigned long  n;

 f=fopen(wzs_filename,"r+t");
 if(f)
   {
   fscanf(f,"%s %s %s  %s %s",buf[0],tmp,buf[1],buf[2],tmp);
   if(!strcmp(buf[0],"+++") && !strcmp(buf[2],"+++") &&
      !strcmp(buf[1],"\"АЧХ\""))
     {
     fscanf(f,"%f %s %s %d %s %d %s %s %s",&fdisk,tmp,tmp,&nki,tmp,&nko,tmp,tmp,tmp);
     delt=1.0/fdisk;
     for(i=0;i<nki;i++)
      {
      n=0;
      do {
         fscanf(f,"%s %f",tmp,&xh[n]);
         for(j=0;j<nko;j++) { fscanf(f,"%f",&yh[i][j][n]); }
         n++;
         } while(strcmp(tmp,"+++") && !feof(f));
      fscanf(f,"%s %s",tmp,tmp);
      }
     nq=n-1; graf_achh();
     }
   else  Wperror("Неверный формат файла");
   }
 else Wperror("Не могу открыть файл данных");
 fclose(f);
 return;
}




