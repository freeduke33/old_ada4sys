#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <dir.h>

char *fn(char *file,int num)
{
 static char buf[100];
 char disk[3],path[100],name[10],ext[5];

 fnsplit(file,disk,path,name,ext);
 name[5]=0; strcpy(buf,name);
 sprintf(name,"%s%02d",buf,num);
 fnmerge(buf,disk,path,name,ext);
 return buf;
}

char *nm(char *file)
{
 static char path[100];
 char disk[3],name[10],ext[5];

 fnsplit(file,disk,path,name,ext);
 fnmerge(path,"","",name,ext);
 return path;
}

void help(char *fn)
{
 printf("\nUsage: %s /UPR|/TEM <MaxVol> <iFile> <oFile>.\n",nm(fn));
 exit(-1);
}

int upr2bin(int MaxVol,char *ifn,char *ofn)
{
 FILE *inf;
 int outf,err=0,i,nki,nko;
 float fdisk,x,y,yt;
 unsigned long nr,k,tik;
 unsigned int num;
 char sign[10],token[50],buf[50];

 MaxVol/=2;

  inf=fopen(ifn,"r+t");
  if(!inf) { printf("\n I can't open input file <%s>\n",ifn); return(-1); }

  fscanf(inf,"%s %s %s",sign,token,buf);
  if(strcmp(sign,"+++") || strcmp(token,"Заголовок_:") || strcmp(buf,"\"ПРОТОКОЛ_УПРАВЛЕНИЯ_СТЕНДОМ\"")) err=1;

  fscanf(inf,"%s %s %lu",sign,token,&nr);
  if(strcmp(sign,"+++") || strcmp(token,"Длинна_ряда_:")) err=1;

  fscanf(inf,"%s %s %f",sign,token,&fdisk);
  if(strcmp(sign,"+++") || strcmp(token,"Частота_дискретизации_:")) err=1;

  fscanf(inf,"%s %s %d %s %d",sign,token,&nki,buf,&nko);
  if(strcmp(sign,"+++") || strcmp(token,"Число_каналов_:") || strcmp(buf,"x")) err=1;

  if(err) { printf("Invalid format of file <%s>\n",ifn); return(-2); }

  for(i=0;i<max(nki,nko);i++)
    {
    strcpy(buf,fn(ofn,i+1));
    outf=open(buf,O_CREAT|O_BINARY|O_TRUNC|O_WRONLY,S_IREAD|S_IWRITE);
    printf("\nChannel %d ...",i+1);
    if(outf==-1) { printf("I can`t create file <%s>, channel skipped.\n",buf); continue; }
    fscanf(inf,"%s %s %d",sign,token,&num);
    if(strcmp(sign,"+++") || strcmp(token,"Канал_N") || num!=i+1) { printf("Invalid channel %d",i+1); return(-3); }
    for(k=0;k<nr;k++)
      {
      fscanf(inf,"%lu %f %f %f",&tik,&x,&y,&yt);
      num=(unsigned)((x*MaxVol/5.0)+MaxVol+0.5);
//      printf("\n %f --> %u",x,num);
      write(outf,&num,sizeof(num));
      }
    close(outf);
    printf(" Ok");
    }

 fclose(inf);
 printf("\nDone.\n");
 return 0;
}

int tem2bin(int MaxVol,char *ifn,char *ofn)
{
 FILE *inf;
 int outf,i,nk,j;
 float fdisk,x,tmp;
 unsigned long nr,k,beg;
 unsigned int num;
 char buf[100];

  inf=fopen(ifn,"r+t");
  if(!inf) { printf("\n I can't open input file <%s>\n",ifn); return(-1); }

  fscanf(inf,"%u %f %lu",&nk,&fdisk,&nr);
  beg=ftell(inf);

  for(i=0;i<nk;i++)
    {
    strcpy(buf,fn(ofn,i+1));
    outf=open(buf,O_CREAT|O_BINARY|O_TRUNC|O_WRONLY,S_IREAD|S_IWRITE);
    printf("\nChannel %d ...",i+1);
    if(outf==-1) { printf("I can`t create file <%s>, channel skipped.\n",buf); continue; }

    fseek(inf,beg,SEEK_SET);
    for(k=0;k<nr;k++)
      {
      for(j=0;j<nk;j++) fscanf(inf,"%f", j==i ? &x : &tmp);
      num=(unsigned)((x*MaxVol/5.0)+MaxVol+0.5);
//      printf("\n %f --> %u",x,num);
      write(outf,&num,sizeof(num));
      }
    close(outf);
    printf(" Ok");
    }

 return 0;
}

int main(int argc,char *argv[])
{
 int val,MaxVol;

 printf("\nChannel Data File Builder (C) Samara-\"Trial\" " __DATE__ ".\n");

  if(argc!=5) help(argv[0]);

  MaxVol=atoi(argv[2]);
  if(MaxVol<=0) {printf("\n MaxVol mast be great then zero.\n"); exit(-2); }

  val=-3;
  if(!strcmp(strupr(argv[1]),"/UPR")) val=upr2bin(MaxVol,argv[3],argv[4]);
  if(!strcmp(strupr(argv[1]),"/TEM")) val=tem2bin(MaxVol,argv[3],argv[4]);
  if(val==-3) help(argv[0]);

 return 0;
}

