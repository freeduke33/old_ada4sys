/* Подготовка к запуску с эксперимента (испытания) */
/*        на основании файла конфигурации          */
#include <malloc.h>
#include <string.h>

// ===============================[ Защита по управлению
#define Uses_Defence_1x1
#define Uses_dfnSpeed
#define Uses_dfnAcceleration
#define Uses_dfnFull

// ===============================[ Удаление тренда из сигнала прогноза
#define Uses_delTrend_270395
#define Uses_delTr0_270395
#define Uses_delTr1_270395

// ===============================[ Расчет погрешности управления
#define Uses_Mistake_1x1
#define Uses_mst_Square
#define Uses_mst_Shenk
#define Uses_mst_MathWait

// ===============================[ Информационные каналы
#define Uses_Anatomy
#define Uses_ant_AverageValue
#define Uses_ant_Defence

// ===============================[ Разное
#define Uses_Buffer
#define Uses_MemBuf

#define Uses_FModel_1x1
#define Uses_Model_1x1
#define Uses_SyncLoop
#define Uses_AsyncLoop


#include <Adaptive.hpp>
#include <Log.hpp>

#include "common.h"

#include "etalon.hpp"
#include "config.h"

#include "cfgloop.hpp"
#include "log.hpp"
#include "result.hpp"


#define mdl  CfgLoop


mdl::mdl(CfgT *cfg,LoopSC *lp)
{
 int i;

 Cfg=cfg;
 Ctrl=lp;

 LOG=NULL;
 InitLog();
 if(LOG)
   {
   char lvl[20];
   //
   // S - запуск/останов, время работы
   // A - пред-аварийные ситуации
   // T - контрольные точки
   // M - погрешность работы (по каналам)
   // I - статистика о сеансе работы
   //
   switch(Cfg->LogLevel)
     {
     case 0:  strcpy(lvl,   "S" ); break;
     case 1:  strcpy(lvl,  "AS" ); break;
     case 2:  strcpy(lvl, "TAS" ); break;
     case 3:  strcpy(lvl,"TMAS" ); break;
     case 4:  strcpy(lvl,"TMASI"); break;
     default: strcpy(lvl,"TMAS" ); break;
     }
   LOG->setLevel(lvl);
   }

 for(i=0;i<Cfg->nk;i++)
    {
    yyt[i]=NULL;     yy[i]=NULL;      xx[i]=NULL;
    d[i]=NULL;       e[i]=NULL;       dt[i]=NULL;
    dd[i]=NULL;      a[i]=NULL;       m[i]=NULL;
    }
 for(i=0;i<Cfg->nkInfo;i++)
    {
    a[i]=NULL;
    }

 return;
}

mdl::~mdl(void)
{
 int i,k;

 for(i=0;i<Cfg->nk;i++)
    {
    if(m[i])
      {
      for(k=0;m[i][k];k++) delete m[i][k];
      delete m[i];
      }
    if(yyt[i])    delete yyt[i];
    if(yy[i])     delete yy[i];
    if(xx[i])     delete xx[i];
    if(d[i])      delete d[i];
    if(dd[i])     delete dd[i];
    if(e[i])      delete e[i];
    if(dt[i])     delete dt[i];
    }
 for(i=0;i<Cfg->nkInfo;i++)
    {
    if(a[i])      delete a[i];
    }

 if(LOG) delete LOG;
 LOG=NULL;
 return;
}


int mdl::PrepareLoop(void)
{
 unsigned long mem;
 int i,val=ERR_NOMEM;

 mem=DosMemoryAvail();
 if(mem<25*1024) goto done;

 // время выхода на начальную точку генерации
 Ctrl->SetPassMargin(Cfg->iniTime);
 // время работы с превышением погрешности
 Ctrl->SetMistakeWait(Cfg->Def[0].mstTime*Cfg->Freq);

 Cfg->nk= Cfg->nk>MAX_CHANNEL ? MAX_CHANNEL : Cfg->nk<0  ? 0 : Cfg->nk ;



 Cfg->nr_view = Cfg->nr*Cfg->cycle<Cfg->nr_view ? Cfg->nr*Cfg->cycle : Cfg->nr_view;

 val=PrepareModels(); if(val) return val;

#define D Cfg->Def[i]

 for(i=0;i<Cfg->nk;i++) if(D.isActive)
   {
   if(D.MaxSpeed || D.MaxAcceleration)
     {
     if(D.MaxSpeed && D.MaxAcceleration)
           d[i]=new dfnFull(Cfg->Freq,D.MaxSpeed/5.0,D.MaxAcceleration/5.0);
     else if(D.MaxSpeed)
           d[i]=new dfnSpeed(Cfg->Freq,D.MaxSpeed/5.0);
     else  d[i]=new dfnAcceleration(Cfg->Freq,D.MaxAcceleration/5.0);
     if(!d[i]) goto done;
     }
   if(D.delTr_size>0)
     {
     dd[i]=d[i]; d[i]=NULL;
     switch(D.delTr)
       {
       case 0: d[i]=new delTr0_270395(D.delTr_size,D.delTr_vol/5,D.delTr_speed/5,dd[i]); break;
       case 1:
       default:
               d[i]=new delTr1_270395(D.delTr_size,D.delTr_vol/5,D.delTr_speed/5,dd[i]); break;
       }
     if(!d[i]) goto done;
     }
   if(d[i])
     {
     m[i][0]->setDefence(d[i]);
     if(m[i][1]) m[i][1]->setDefence(d[i]);
     }

   e[i]=NULL;
   if(D.mstMax>0 && D.isActive)
     {
     switch(D.mstCod)
       {
       case 0: e[i]=new mst_Square(); break;
       case 1: e[i]=new mst_MathWait(); break;
       case 2: e[i]=new mst_Shenk(); break;
       default:
//          Wperror("Неизвестный алгоритм расчета погрешности");
          val=ERR_MISTAKE;
          goto done;
       }
     if(e[i])
       {
       m[i][0]->setMistake(e[i],D.mstMargin*Cfg->Freq,D.mstMax);
       if(m[i][1]) m[i][1]->setMistake(e[i],D.mstMargin*Cfg->Freq,D.mstMax);
       }
     }

   } /* for i=0;i<Cfg->nk */

 val=PrepareBuffers(); if(val) return val;
 val=PrepareIdeal();   if(val) return val;
 val=PrepareInform();  if(val) return val;

 for(i=0;i<Cfg->nk;i++)
   {
   Ctrl->SetBuffer(i,xx[i],yy[i],yyt[i]);
   Ctrl->SetModel(i,m[i]);

   Ctrl->SetPassiveVol(i,D.PassiveVol/5.0);
//   Ctrl->SetStartVol(i, D.StartVol/5.0);
   Ctrl->SetStartVol(i, (D.isActive) ? D.StartVol/5.0 : D.PassiveVol/5.0);
   }

 val=0;
done:
 return val;
}


int mdl::PrepareIdeal(void)
{
 int i,val=0;

 for(i=0;i<Cfg->nk;i++)
   {
   switch(MakeEtalon(i,&yyt[i]))
      {
      case 0:  break;
      case 1:  val=ERR_NOMEM;    break;
      case 2:  val=ERR_TEMFILE;  tprintf(WORKING_PART_STATUS,"Отсутствует/нельзя открыть файл нагрузки <%s> для канала N%d",Cfg->Def[i].file_name,i+1); break;
      case 3:  val=ERR_CHANNEL;  tprintf(WORKING_PART_STATUS,"Нет данных для канала N%d в файле нагрузки <%s>",i+1,Cfg->Def[i].file_name); break;
      case 4:  val=ERR_TEMLENGTH;tprintf(WORKING_PART_STATUS,"Слишком короткий файл нагрузки <%s> для канала N%d",Cfg->Def[i].file_name,i+1); break;
      default: val=ERR_ETALON;   tprintf(WORKING_PART_STATUS,"Неверный вид эталонного сигнала для канала N%d",i+1); break;
      }
   }
 return val;
}



int mdl::PrepareInform(void)
{
 int i,val=RC_DONE;

#define INF Cfg->Inform[i]

 for(i=0;i<Cfg->nkInfo;i++) if(INF.isActive)
   {
   switch(INF.infCod)
     {
     case 0:
       a[i]=new ant_AverageValue(INF.Chl[0],INF.Average.size);
       break;
     case 1:
       {
       ant_DefenceSetup ads;

       ads.Normal.outCod= INF.Defence.Normal.outCod;
       ads.Normal.isMute= INF.Defence.Normal.isMute;
       ads.Normal.MaxVol= INF.Defence.Normal.MaxVol/5;
       ads.Normal.MinVol= INF.Defence.Normal.MinVol/5;

       ads.Warning.outCod=INF.Defence.Warning.outCod;
       ads.Warning.isMute=INF.Defence.Warning.isMute;
       ads.Warning.MaxVol=INF.Defence.Warning.MaxVol/5;
       ads.Warning.MinVol=INF.Defence.Warning.MinVol/5;

       ads.Crash.outCod=INF.Defence.Crash.outCod;
       ads.Crash.isMute=INF.Defence.Crash.isMute;
       ads.Crash.MaxVol=INF.Defence.Crash.MaxVol/5;
       ads.Crash.MinVol=INF.Defence.Crash.MinVol/5;

       a[i]=new ant_Defence(INF.Chl[0],&ads);
       }
       break;
     case 2:
       a[i]=NULL;
       break;
     }
   if(!a[i]) { val=RC_NOMEM; goto done; }
   Ctrl->SetInfoModel(i,a[i]);
   }

done:

 return val;
}

int mdl::PrepareBuffers(void)
{
 int i,val=ERR_NOMEM;

 for(i=0;i<Cfg->nk;i++)
   {
   yy[i]=new MemBuf;
   if(yy[i])
     {
     yy[i]->setSize(Cfg->nr_view);
     if(yy[i]->getSize()!=Cfg->nr_view) goto done;
     }
   else goto done;
   xx[i]=new MemBuf;
   if(xx[i])
     {
     xx[i]->setSize(Cfg->nr_view);
     if(xx[i]->getSize()!=Cfg->nr_view) goto done;
     }
   else goto done;
   }

 val=0;
done:
 return val;
}


int mdl::WriteResult(int PID)
{
 LoopStat info;

 Ctrl->GetStatictics(&info);
 StoreResults(PID,xx,yy,yyt,Cfg,&info);

 return 0;
}
