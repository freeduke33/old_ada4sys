#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "run.h"
#include "run.hpp"
#include "config.h"
#include "common.h"

#define Uses_MemBuf
#define Uses_exm_Static
#define Uses_exm_Achh
#define Uses_Examine
#include <adaptive.hpp>

class txt_Achh:public exm_Achh
{
 protected:
  virtual void showStart(void);
  virtual void showProcess(void);
  virtual void showDone(void);
};

void txt_Achh::showStart(void)   { tprintf(WORKING_PART_PERSENT,"%3u",0); return; }
void txt_Achh::showDone(void)    { tprintf(WORKING_PART_PERSENT,"%3u",100); return; }
void txt_Achh::showProcess(void) { tprintf(WORKING_PART_PERSENT,"%3u",TIK*100/Def->Nq); return;}

class txt_Stat:public exm_Static
{
 protected:
  virtual void showStart(void);
  virtual void showProcess(void);
  virtual void showDone(void);
};

void txt_Stat::showStart(void)   { tprintf(WORKING_PART_PERSENT,"%3u",0); return; }
void txt_Stat::showDone(void)    { tprintf(WORKING_PART_PERSENT,"%3u",100); return; }
void txt_Stat::showProcess(void) { tprintf(WORKING_PART_PERSENT,"%3u",TIK*100/(Def->Nq*2)); return; }

int doExamine(int Mode)
{
 Examine *exm=NULL;
 Buffer *buf[CHANNEL];
 Buffer *sign=NULL;
 DefACHH ach;
 DefSTAT stat;
 short int i,k,f=0,isBreak=0;
 float val,ActualFreq;

 for(i=0;i<CHANNEL;i++) buf[i]=NULL;

 switch(Mode)
   {
   case runACH:
      tprintf(WORKING_TITLE,"Расчет амплитудно-частотных характеристик");
      exm=new txt_Achh();
      ach.X=new MemBuf();
      ach.Y=buf;
      for(i=0;i<Cfg.nk;i++) ach.Y[i]=new MemBuf();
      f=open("last_ach.dat",O_WRONLY|O_BINARY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
      write(f,&PID,sizeof(PID));

      for(i=k=0;i<Cfg.nk;i++) if(Cfg.Def[i].isActive) k++;
      write(f,&k,sizeof(k));
//      write(f,&Cfg.nk,sizeof(Cfg.nk));
      break;
   case runSTH:
      tprintf(WORKING_TITLE,"Расчет статических характеристик");
      exm=new txt_Stat();
      stat.X=new MemBuf();
      stat.Y=buf;
      for(i=0;i<Cfg.nk;i++) stat.Y[i]=new MemBuf();
      f=open("last_sth.dat",O_WRONLY|O_BINARY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
      write(f,&PID,sizeof(PID));

      for(i=k=0;i<Cfg.nk;i++) if(Cfg.Def[i].isActive) k++;
      write(f,&k,sizeof(k));

      break;
   case runWZS:
      tprintf(WORKING_TITLE,"Расчет взаимовлияния в статике");
      break;
   }
 if(exm==NULL) goto done;
 exm->setChannel(Cfg.nk);

 for(i=0;i<Cfg.nk;i++) if(Cfg.Def[i].isActive)
   {
   tprintf(WORKING_PART,"Активизация канала %d",i+1);
//   tprintf(Cfg.ScreenX,Cfg.ScreenY,"%u/%u",i+1,Cfg.nk);
//   tprintf(Cfg.ScreenX,Cfg.ScreenY+1,"Ini");
   for(k=0;k<Cfg.nk;k++)
      exm->setVolume(k,Cfg.Def[k].StartVol/5.0,Cfg.Def[k].PassiveVol/5.0);

   ActualFreq=Cfg.Freq;
   switch(Mode)  // --------- подготовка к снятию характеристики с канала
     {
     case runACH: // =======================
       ach.Chl=Cfg.nk;
       ach.iniTime=Cfg.iniTime;
       ach.PassiveVol=Cfg.Def[i].PassiveVol/5.0;

       ach.Nq=Cfg.Ach[i].Nq;
       ach.Ampl=Cfg.Ach[i].Ampl/5.0;
       ach.Const=Cfg.Ach[i].Const/5.0;
       ach.Periods=Cfg.Ach[i].Periods;
       ach.FreqMin=Cfg.Ach[i].FreqMin;
       ach.FreqMax=Cfg.Ach[i].FreqMax;

       ActualFreq= (ach.FreqMax*10>Cfg.Freq) ? ach.FreqMax*10 : Cfg.Freq;

       ach.X->setSize(ach.Nq);
       if(ach.X->getSize()<ach.Nq) goto done;

       for(k=0;k<Cfg.nk;k++) if(ach.Y[k])
         {
         ach.Y[k]->setSize(ach.Nq);
         if(ach.Y[k]->getSize()<ach.Nq) goto done;
         }
       ((exm_Achh *)exm)->Setup(&ach);
       break;
     case runSTH:
       stat.Chl=Cfg.nk;
       stat.iniTime=Cfg.iniTime;
       stat.PassiveVol=Cfg.Def[i].PassiveVol/5.0;

       stat.Mode=Cfg.Sth[i].Mode;
       stat.Nq=Cfg.Sth[i].Nq;
       stat.Waits=Cfg.Sth[i].Wait;
       stat.Hi=Cfg.Sth[i].Hi/5.0;
       stat.Lo=Cfg.Sth[i].Lo/5.0;
       stat.HiSpeed=Cfg.Sth[i].HiSpeed/5.0;
       stat.HiAcceleration=Cfg.Sth[i].HiAcceleration/5.0;

       stat.X->setSize(stat.Nq);
       if(stat.X->getSize()<stat.Nq) goto done;
       for(k=0;k<Cfg.nk;k++) if(stat.Y[k])
         {
         stat.Y[k]->setSize(stat.Nq*2);
         if(stat.Y[k]->getSize()<stat.Nq*2) goto done;
         }
       ((exm_Static *)exm)->Setup(&stat);
       break;
     case runWZS:
       break;
     }

   // ------------------------- снятие характеристики
   tprintf(WORKING_PART,"Исследование канала %d",i+1);
   if(!isBreak) switch(exm->doExamine(i,ActualFreq))
     {
     case RC_UPI:       isBreak=1; break;
     case RC_USERBREAK: isBreak=1; break;
     case RC_DONE:      break;
     }

   // ------ запись в файл
   tprintf(WORKING_PART,"Сохранение результатов по каналу %d",i+1);
   tprintf(WORKING_PART_PERSENT,"%3u",0);
//   tprintf(Cfg.ScreenX,Cfg.ScreenY,"Write");
   switch(Mode)
     {
     case runACH:
       write(f,&ach.Nq,sizeof(ach.Nq));

       if(ach.X) ach.X->reset();
       for(k=0;k<Cfg.nk;k++) if(ach.Y[k]) ach.Y[k]->reset();

       for(k=0;k<ach.Nq;k++)
         {
         tprintf(WORKING_PART_PERSENT,"%3u",k*100/ach.Nq);
         val= ach.X    ? ach.X->getNext()    : k;
         write(f,&val,sizeof(val));
         val= ach.Y[i] ? ach.Y[i]->getNext() : 0;
         write(f,&val,sizeof(val));
         }
       break;
     case runSTH:
       write(f,&stat.Nq,sizeof(stat.Nq));
       write(f,&stat.Mode,sizeof(stat.Mode));

       if(ach.X) stat.X->reset();
       for(k=0;k<Cfg.nk;k++) if(stat.Y[k]) stat.Y[k]->reset();

       for(k=0;k<stat.Nq;k++)
         {
         tprintf(WORKING_PART_PERSENT,"%3u",k*100/(stat.Nq*3));
         val= stat.X ? stat.X->getNext() : k;
         write(f,&val,sizeof(val));
         }

       for(k=0;k<stat.Nq*2;k++)
         {
         tprintf(WORKING_PART_PERSENT,"%3u",(k+stat.Nq)*100/(stat.Nq*3));
         val= stat.Y[i] ? stat.Y[i]->getNext() : 0;
         write(f,&val,sizeof(val));
         }
       break;
     case runWZS:
       break;
     }
   tprintf(WORKING_PART_PERSENT,"%3u",100);
   }
done:
 if(f) close(f);
 return 0;
}


