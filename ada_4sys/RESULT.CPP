#include <io.h>
#include <sys/stat.h>
#include <fcntl.h>

#define Uses_Buffer
#define Uses_LoopSC
#include <adaptive.hpp>

#include "result.hpp"

#include "config.h"
#include "common.h"

static void writeInfo(int f,Buffer *x,CfgT *,LoopStat *Info);

int StoreResults(int PID,Buffer **x,Buffer **y,Buffer **yt,CfgT *Cfg,LoopStat *Info)
{
 int f,k,n,i;
 short int _PID=PID;
 Buffer **buf[3]={x,y,yt};
 float val;

 f=open("lastwork.dat",O_BINARY|O_TRUNC|O_CREAT|O_RDWR,S_IREAD|S_IWRITE);
 BufInit();
 BufWrite(f,&_PID,sizeof(_PID));
 BufWrite(f,&Cfg->nk,sizeof(Cfg->nk));
 BufWrite(f,&Cfg->Freq,sizeof(Cfg->Freq));

// BufWrite(f,&Cfg.nr,sizeof(Cfg.nr));
// BufWrite(f,&Info.OkCycle,sizeof(Info.OkCycle));
 for(k=0;k<Cfg->nk;k++)
   {
   BufWrite(f,&Cfg->nr_view,sizeof(Cfg->nr_view));
   for(n=0;n<3;n++) writeInfo(f,buf[n][k],Cfg,Info);
   }
// for(k=0;k<Cfg->nk;k++) for(n=0;n<3;n++) if(buf[n][k])
//      {
//      val=buf[n][k]->get(0);
//      BufWrite(f,&val,sizeof(val));
//      }

 for(i=0;i<Cfg->nr_view;i++) for(k=0;k<Cfg->nk;k++) for(n=0;n<3;n++)
     {
     if(buf[n][k])
        {
        val=buf[n][k]->getNext();
        BufWrite(f,&val,sizeof(val));
        }
     }
 BufFlush(f);
 close(f);

 return 0;
}

int LoadResultBanner(int PID,int MaxChl,int *fh,ResultInfo *rInfo)
{
 int f,ret=0,k,n;
 short int _PID,nk;
 float Freq;

 f=open("lastwork.dat",O_BINARY|O_TRUNC|O_CREAT|O_RDONLY);
 if(f)
   {
   BufInit();
   BufRead(f,&_PID,sizeof(_PID));
   if(PID == _PID || !PID)
      {
      BufRead(f,&nk,sizeof(nk)); nk= MaxChl<nk ? MaxChl : nk;
      BufRead(f,&Freq,sizeof(Freq));

      for(k=0;k<nk;k++)
        {
        rInfo[k].isPresent=1;

        rInfo[k].Freq=Freq;
        rInfo[k].BegTime=0;
        BufRead(f,&rInfo[k].nr,sizeof(rInfo[k].nr));
        for(n=0;n<3;n++) BufRead(f,&rInfo[k].is[n],sizeof(rInfo[k].is[n]));
        }
      rInfo[nk].isPresent=0;
      }
   else ret=2;
   }
 else ret=1;

 if(ret) { close(f); f=0; }

 if(fh) *fh=f;
 return ret;
}

int LoadResult(int f,ResultInfo *rInfo,Buffer **x,Buffer **y,Buffer **yt)
{
 int ret=0,k,n;
 unsigned long i,nr;
 Buffer **buf[3]={x,y,yt};
 float val;

 if(f)
   {
   for(k=0,nr=0;rInfo[k].isPresent;k++) if(rInfo[k].nr>nr) nr=rInfo[k].nr;
   for(i=0;i<nr;i++) for(k=0;rInfo[k].isPresent;k++) for(n=0;n<3;n++)
        {
        if(rInfo[k].is[n] && rInfo[k].nr>i) BufRead(f,&val,sizeof(val));
        else val=0;

        if(buf[n][k]) buf[n][k]->putNext(val);
        }
   close(f);
   }
 else ret=1;

 return ret;
}


int StoreEtalon(int PID,CfgT *Cfg,Buffer **yt)
{
 int f,k;
 float val;
 unsigned int num,i;

 f=open("last_tem.dat",O_BINARY|O_TRUNC|O_CREAT|O_RDWR,S_IREAD|S_IWRITE);
 BufInit();
 BufWrite(f,&PID,sizeof(PID));
 BufWrite(f,&Cfg->nk,sizeof(Cfg->nk));
 BufWrite(f,&Cfg->Freq,sizeof(Cfg->Freq));
 BufWrite(f,&Cfg->nr,sizeof(Cfg->nr));

 for(k=0;k<Cfg->nk;k++)
   {
   num=(unsigned int)yt[k]->getSize();
   yt[k]->setMargin(0);
   yt[k]->reset();
   BufWrite(f,&num,sizeof(num));
   val=yt[k]->get(0);
   for(i=0;i<num;i++)
       {
       BufWrite(f,&val,sizeof(val));
       val=yt[k]->getNext();
       }
   }
 BufFlush(f);
 close(f);

 return 0;
}

int LoadEtalonBanner(int ,int *,EtalonInfo *)
{
 int ret=0;

 return ret;
}

int LoadEtalon(int ,EtalonInfo *,Buffer **)
{
 return 0;
}

static void writeInfo(int f,Buffer *x,CfgT *Cfg,LoopStat *Info)
{
 unsigned long num,i;
 short int act;

 act= x ? 1 : 0; BufWrite(f,&act,sizeof(act));

 if(x)
   {
   num=x->getSize();
   if(num>Cfg->nr_view) num=Cfg->nr_view;
//   x->setMargin(0);
//   x->reset();

   i= num ? Info->OkCycle*Cfg->nr+Info->OkTik :0;

   x->gotoPos(i>num ? -num : -i);
   }
 return;
}





