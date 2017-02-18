#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <malloc.h>


#define Uses_gen_Volume
#define Uses_gen_Sinus
#define Uses_gen_Template
#define Uses_gen_DatFile
#define Uses_NilBuffer

#define Uses_mdlTest

#include <adaptive.hpp>
#include <log.hpp>

#include "config.h"
#include "cfgloop.hpp"
#include "okmodel.hpp"
#include "common.h"
#include "dac_adc.h"
#include "multigen.h"

#define mdl CfgLoopGen
#define D Cfg->Def[i]


mdl::mdl(CfgT *Cfg,LoopSC *l,unsigned safeMem) : CfgLoop(Cfg,l)
{
 typedef FModel_1x1 _HUGE *mdlP;

 SafeMem=safeMem;

 for(int i=0;i<Cfg->nk;i++)
   {
   m[i]=new mdlP[2];
   if(!m[i]) goto done;
   for(int k=0;k<2;k++) m[i][k]=NULL;
   fn[i][0]=0;
   }
done:
 return;
}

mdl::~mdl()
{
 int i;

 for(i=0;i<Cfg->nk;i++)
   {
   if(fn[i][0]) unlink(fn[i]);
   }
 return;
}

int mdl::PrepareModels(void)
{
 int i,val=ERR_NOMEM,tmp;
 Word DAC;

 for(i=0;i<Cfg->nk;i++)
   {
   if(!m[i]) goto done;

   tmp = D.isActive ? D.mdlCod : mc_PASSIVE;


   switch(tmp) /* !! Все активные каналы должны быть потомками gen_vBuffer !!*/
     {
     case mc_PASSIVE:
          m[i][0]=new gen_Volume(D.PassiveVol/5.0);
          break;
     default:
          DrvGetInfoDAC(&DAC); DAC/=2;
          tprintf(WORKING_PART,"%d/%d: Подготовка сигнала нагружения",i+1,Cfg->nk);
          sprintf(fn[i],"temp-%02d.$$$",i);
          if(Template2Bin(fn[i],i,DAC)!=0) goto done;
          m[i][0]=new gen_DatFile(Cfg->nr*Cfg->cycle,fn[i],DAC);
          break;
     }
  if(!m[i][0]) goto done;
  }

 val=AllocBuffers();
done:
 return val;
}

/*
int mdl::PrepareBuffers(void)
{
 int i;

 for(i=0;i<Cfg->nk;i++)
   {
   xx[i]=new NilBuffer;
   yy[i]=new NilBuffer;
   }
 return 0;
}
*/

int mdl::PrepareIdeal(void)
{
 int i;

 for(i=0;i<Cfg->nk;i++) yyt[i]=new NilBuffer;
 return 0;
}


int mdl::Template2Bin(char *fname,int i,unsigned DAC)
{
 int ret=0;
 short int Cod;
 int f=0,nk,act=D.file_chl,m;
 float freq,val,val1;
 unsigned long nr,c,l,tik,pos;
 FILE *t;
 char buffer[1024];



  tprintf(WORKING_PART_PERSENT,"%3d",0);
  t=fopen(D.file_name,"r+t");
  if(t)
    {
    setvbuf(t,buffer,_IOFBF,1024);

    fscanf(t,"%d %f %lu",&nk,&freq,&nr);
    if(nk>=act && freq==Cfg->Freq)
       {
       pos=ftell(t);
       f=open(fname,O_RDWR|O_CREAT|O_BINARY,S_IREAD|S_IWRITE);
       if(f>0)
         {
         BufInit();

         for(tik=c=0;c<Cfg->cycle;c++) for(l=0;l<Cfg->nr;l++,tik++)
           {
           tprintf(WORKING_PART_PERSENT,"%3d",tik*100/nr);
           if(tik>=nr) goto EndOfFile;
           if(kbhit() && getch()==27)
             { ret=ERR_USERBREAK; tprintf(WORKING_PART_STATUS," Прервано оператором "); goto EndOfFile; }
           for(m=0;m<nk;m++)
               {
               fscanf(t,"%f",&val);
               if(m==act)
                  {
//                  DBGLOG('S',"\n[%03ld] %+7.3f --> ",tik,val);
                  val=(val*D.file_k+D.file_st+D.StartVol)/5.0;
                  val1=ToDAC(val,DAC);
                  Cod=VerifyDAC(val1,DAC);
//                  val=FromADC(Cod,DAC);
//                  DBGLOG('S'," %4d (0x%04X) ---> %+7.3f",Cod,Cod,val*5.0-D.StartVol);
                  BufWrite(f,&Cod,sizeof(Cod));
                  }
               }
           }
EndOfFile:
         BufFlush(f);
         close(f);
         }
       else { ret=ERR_BUILD; tprintf(WORKING_PART_STATUS," Невозможно создать временный файл %s ",fname);
         }
       }
    else { ret=ERR_TEMFILE; tprintf(WORKING_PART_STATUS," Неподходящие параметры файла эталона %s ",D.file_name); }
    fclose(t);
    }
 else { ret=ERR_ETALON; tprintf(WORKING_PART_STATUS," Отсутствует файл эталона %s ",D.file_name); }

 return ret;
}

int mdl::AllocBuffers(void)
{
 int i,k,l;
 unsigned long need,num;
 gen_vBuffer *Tmp[CHANNEL];

  for(l=i=0,need=0;i<Cfg->nk;i++) if(D.isActive)
    {
    Tmp[l]=(gen_vBuffer *)m[i][0];
    need+=Tmp[l]->NeedMemory();
    l++;
    }
  num=DosMemoryAvail();
  num-=1024*SafeMem;
//  fprintf(stdout,"Доступно памяти : %luKb,  "
//          "необходимо для работы : %luKb\n",num/1024+1,need/1024+1);
  if(need<num) for(i=0;i<l;i++) Tmp[i]->CreateBuffer(0);
  else
     {
     for(i=k=0;i<l;i++) if(Tmp[i]->NeedMemory()<num/l)
        {
        Tmp[i]->CreateBuffer(0); k++;
        }
     if(k!=l)
       {
       num=DosMemoryAvail()-1024*SafeMem;
       for(i=0;i<l;i++) Tmp[i]->CreateBuffer(num/(l-k)-1);
       }
     }
 return 0;
}
