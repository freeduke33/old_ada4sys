#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#include "mgen.h"

struct channel_dat *EntPtr;

Config Cfg;
Config defCfg=
 {
 ID_String,CHANNEL,13.7,300,2,2048,2048,
  {
  {DatFile,{{"NONAME1.DAT",-2},{"NONAME1.PRG",-2},{7.1,0.3,0},{{7.1,0.3,0},{14.2,0.1,0},{28.4,0.05,0}},0.5}},
  {Const  ,{{"NONAME2.DAT",-2},{"NONAME2.PRG",-2},{7.1,0.3,0},{{7.1,0.3,0},{14.2,0.1,0},{28.4,0.05,0}},0.5}},
  {DatFile,{{"NONAME3.DAT",-2},{"NONAME3.PRG",-2},{7.1,0.3,0},{{7.1,0.3,0},{14.2,0.1,0},{28.4,0.05,0}},0.5}},
  {Const,{{"NONAME4.DAT",-2},{"NONAME4.PRG",-2},{7.1,0.3,0},{{7.1,0.3,0},{14.2,0.1,0},{28.4,0.05,0}},0.5}},
  {Const,{{"NONAME5.DAT",-2},{"NONAME5.PRG",-2},{7.1,0.3,0},{{7.1,0.3,0},{14.2,0.1,0},{28.4,0.05,0}},0.5}},
  {Const,{{"NONAME6.DAT",-2},{"NONAME6.PRG",-2},{7.1,0.3,0},{{7.1,0.3,0},{14.2,0.1,0},{28.4,0.05,0}},0.5}},
  {Const,{{"NONAME7.DAT",-2},{"NONAME7.PRG",-2},{7.1,0.3,0},{{7.1,0.3,0},{14.2,0.1,0},{28.4,0.05,0}},0.5}},
  {Const,{{"NONAME8.DAT",-2},{"NONAME8.PRG",-2},{7.1,0.3,0},{{7.1,0.3,0},{14.2,0.1,0},{28.4,0.05,0}},0.5}},
  {Const,{{"NONAME9.DAT",-2},{"NONAME9.PRG",-2},{7.1,0.3,0},{{7.1,0.3,0},{14.2,0.1,0},{28.4,0.05,0}},0.5}},
  {DatFile,{{"NONAME0.DAT",-2},{"NONAME0.PRG",-2},{7.1,0.3,0},{{7.1,0.3,0},{14.2,0.1,0},{28.4,0.05,0}},0.5}}
  }
 };


int SaveCfg(void)
{
 int f;

 f=open(_argv[1],O_WRONLY|O_BINARY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
 if(f>0)
   {
   write(f,&Cfg,sizeof(Cfg));
   close(f);
   }

 return 0;
}

int LoadCfg(void)
{
 int f;

 printf("Multi-Generator config file definitor v1.00 (c) by Samara-\"Trend\" 1994.\n");
 if(_argc!=2)
   {
   printf("Usage : %s <cfgFile>",_argv[0]);
   exit(-1);
   }
 f=open(_argv[1],O_RDONLY|O_BINARY);
 if(f==-1)
   {
   printf("Init default configuration ...\n");
   memcpy(&Cfg,&defCfg,sizeof(Cfg));
   }
 else
   {
   printf("Load configuration from <%s> ...",_argv[1]);
   if(read(f,&Cfg,sizeof(Cfg))!=sizeof(Cfg) || strcmp(Cfg.ID,ID_String))
     {
     printf("ERR (invalid config file)\n");
     exit(-2);
     }
   close(f);
   }
 return 0;
}