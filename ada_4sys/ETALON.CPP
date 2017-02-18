#define DEFINE_CHANNEL 4
#include <stdio.h>

#include "config.h"
#include "common.h"

#define Uses_Buffer
#define Uses_NilBuffer
#define Uses_MemBuf
#define Uses_StaticMemBuf
#define Uses_MemBufPSinus

#ifndef __NOTCXL__
# include "emsbuf.hpp"
#else
# include <adaptive.hpp>
#endif

#define D Cfg.Def[i]


static int psin(int i,Buffer **buf)
{
 int val=0,j;
 float a[3];
 float u;

 for(j=0;j<3;j++) a[j]=D.a[j]/5; u=(D.u+D.StartVol)/5;
  *buf=new MemBufPSinus(Cfg.nr,Cfg.Freq,a,D.f,D.fa,u);
  if(*buf==NULL || (*buf)->getSize()==0) val=1;

 return val;
}

static int file(int i,Buffer **buf)
{
 FILE *f;
 int _nk,k,val=0;
 float _freq,yt;
 unsigned long _nr,l;


#ifndef __NOTCXL__
  if(D.isEMS) *buf=new EMSMemBuf(Cfg.nr,i,0,3);
  else
#endif
    {
    *buf=new MemBuf;
    if(*buf) (*buf)->setSize(Cfg.nr);
    }
// загрузка эталонного сигнала из файла
  if( !(*buf==NULL || (*buf)->getSize()==0))
    {
    f=fopen(D.file_name,"r+t");
    if(f==NULL) { val=2; goto done; }
    else
      {
      fscanf(f,"%u %f %lu",&_nk,&_freq,&_nr);
      if(_nk<i || _nr<Cfg.nr || _freq!=Cfg.Freq)
        {
        fclose(f);
        if(_nk<i)           val=3;    // неверное число каналов
        else if(_nr<Cfg.nr) val=4;    // неверная длинна ряда
        else                val=5;    // неверная частата дискретизации
        goto done;
        }
      else
        {
        (*buf)->reset();
        (*buf)->gotoPos(-1);

        for(l=0;l<Cfg.nr;l++)
          {
          for(k=0;k<_nk;k++)
            {
            fscanf(f,"%f",&yt);
            if(k==D.file_chl)
                {
                yt=(yt*D.file_k+D.file_st+D.StartVol)/5.0;
                (*buf)->putNext(yt);
                }
            }
          }
        (*buf)->reset();
        val=0; // все нормально
        }
      fclose(f);
      }
    }
  else val=1; // нет памяти

done:
 return val;
}

int MakeEtalon(int chl,Buffer **buf)
{
 int val=1;

 if(Cfg.Def[chl].isActive)
   {
   switch(Cfg.Def[chl].tplCod)
     {
     case 1: // полигармоника
       val=psin(chl,buf);
       break;
     case 2: // из файла
       val=file(chl,buf);
       break;
     case 3: // пила
     default: val=10; goto done;
     }
   }
 else // passive channel
   {
   val=0;
   *buf=new NilBuffer;
   if(*buf) (*buf)->putNext(Cfg.Def[chl].PassiveVol/5.0);
   else val=1;
   }
done:
 return val;
}



