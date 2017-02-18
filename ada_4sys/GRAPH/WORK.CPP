#include <alloc.h>

#define Uses_Buffer
#define Uses_MemBuf
#define Uses_LoopSC

#include <adaptive.hpp>

#include "ada_graf.h"
#include "result.hpp"

Buffer *Signal[3][CHANNEL];
ResultInfo rInf[CHANNEL+1];

static void tGetX(float *xx,float *yy,triadGRF *p,unsigned long num)
{
 *xx=num*delt;
 if(x[(int)p->Array2]) *yy=5.0*x[(int)p->Array2][num];
 else *yy=0;
 return;
}

static void tGetY(float *xx,float *yy,triadGRF *p,unsigned long num)
{
 *xx=num*delt;
 if(y[(int)p->Array2]) *yy=5.0*y[(int)p->Array2][num];
 else *yy=0;
 return;
}



static triadGRF intime[]={
{HOR_1_2_T,  -1,-1,1,1,     tGetX,NULL,  0,x[0], 1,0,    0,100,
 "Управление","[Время]","f[%5.2f]=%+6.3f,%+6.3f,%+6.3f,%+6.3f",NULL,grfPARAM_ALL|grfDOUBLE,grfRANGE|grfLeftTTL},
{ADD_2,    -1,-1,1,1,     tGetX,NULL,    0,x[1], 1,0,    0,200,NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_3,    -1,-1,1,1,     tGetX,NULL,    0,x[2], 1,0,    0,200,NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_4,    -1,-1,1,1,     tGetX,NULL,    0,x[3], 1,0,    0,200,NULL,NULL,NULL,NULL},

{HOR_2_2_T,  -1,-1,1,1,     tGetY,NULL,  0,y[0], 1,0,    0,100,
 "Отклик","[Время]","f[%5.2f]=%+6.3f,%+6.3f,%+6.3f,%+6.3f",NULL,grfPARAM_ALL|grfDOUBLE,grfRANGE|grfLeftTTL},
{ADD_2,    -1,-1,1,1,     tGetY,NULL,    0,y[1], 1,0,    0,200,NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_3,    -1,-1,1,1,     tGetY,NULL,    0,y[2], 1,0,    0,200,NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_4,    -1,-1,1,1,     tGetY,NULL,    0,y[3], 1,0,    0,200,NULL,NULL,NULL,NULL}
};


static int graf_time1(int chl)
{
 int i,val;

 for(i=0;i<8;i++)
    {
    intime[i].beg=0;  intime[i].end=nr-1;
    intime[i].Xmin=0; intime[i].Xmax=nr*delt;
    intime[i].Ymax=5; intime[i].Ymin=-5;
    }

 ScrSave();
 val=InitGRF(grfGRAPHICS);
 if(val==okGRF)
   {
   SetLook_Clr(putTitle); strcpy(TitleStr,"Сигнал управления/Отклик объекта");
   val=LookGRF(8,intime);
   DeinitGRF(grfTEXT);
   }
 ScrRest();
 return val==okGRF ? 0 : 7;
}

/* ==================================================================== */

static void Get1(float *x,float *y,triadGRF *p,unsigned long num)
{
 if(p->Array1)
   {
   *x=(unsigned)num/((ResultInfo *)(p->Array2))->Freq;
//   ((Buffer *)(p->Array1))->gotoPos(num);
   *y=5.0*((Buffer *)(p->Array1))->get(num+1);
   }
 else { *y=0; *x=0; }
 return;
}

//*********************************************
static triadGRF in2time[]={
{BOX_1_8_T,  0,-5,1,5,  Get1,NULL,   0,0, 1,0,    0,100, "Канал 0","[Время]","f[%5.2f]=%+6.3f(%+6.3f)",NULL,grfDOUBLE,grfRANGE|grfLeftTTL},
{ADD_3,      0,-5,1,5,  Get1,NULL,   0,0, 1,0,    0,200,NULL,NULL,NULL,NULL},

{BOX_2_8_T,  0,-5,1,5,  Get1,NULL,   0,0, 1,0,    0,100, "Канал 1","[Время]","f[%5.2f]=%+6.3f(%+6.3f)",NULL,grfDOUBLE,grfRANGE|grfLeftTTL},
{ADD_3,      0,-5,1,5,  Get1,NULL,   0,0, 1,0,    0,200,NULL,NULL,NULL,NULL},

{BOX_3_8_T,  0,-5,1,5,  Get1,NULL,   0,0, 1,0,    0,100, "Канал 2","[Время]","f[%5.2f]=%+6.3f(%+6.3f)",NULL,grfDOUBLE,grfRANGE|grfLeftTTL},
{ADD_3,      0,-5,1,5,  Get1,NULL,   0,0, 1,0,    0,200,NULL,NULL,NULL,NULL},

{BOX_4_8_T,  0,-5,1,5,  Get1,NULL,   0,0, 1,0,    0,100, "Канал 3","[Время]","f[%5.2f]=%+6.3f(%+6.3f)",NULL,grfDOUBLE,grfRANGE|grfLeftTTL},
{ADD_3,      0,-5,1,5,  Get1,NULL,   0,0, 1,0,    0,200,NULL,NULL,NULL,NULL},

{BOX_5_8_T,  0,-5,1,5,  Get1,NULL,   0,0, 1,0,    0,100, "Канал 4","[Время]","f[%5.2f]=%+6.3f(%+6.3f)",NULL,grfDOUBLE,grfRANGE|grfLeftTTL},
{ADD_3,      0,-5,1,5,  Get1,NULL,   0,0, 1,0,    0,200,NULL,NULL,NULL,NULL},

{BOX_6_8_T,  0,-5,1,5,  Get1,NULL,   0,0, 1,0,    0,100, "Канал 5","[Время]","f[%5.2f]=%+6.3f(%+6.3f)",NULL,grfDOUBLE,grfRANGE|grfLeftTTL},
{ADD_3,      0,-5,1,5,  Get1,NULL,   0,0, 1,0,    0,200,NULL,NULL,NULL,NULL},

{BOX_7_8_T,  0,-5,1,5,  Get1,NULL,   0,0, 1,0,    0,100, "Канал 6","[Время]","f[%5.2f]=%+6.3f(%+6.3f)",NULL,grfDOUBLE,grfRANGE|grfLeftTTL},
{ADD_3,      0,-5,1,5,  Get1,NULL,   0,0, 1,0,    0,200,NULL,NULL,NULL,NULL},

{BOX_8_8_T,  0,-5,1,5,  Get1,NULL,   0,0, 1,0,    0,100, "Канал 7","[Время]","f[%5.2f]=%+6.3f(%+6.3f)",NULL,grfDOUBLE,grfRANGE|grfLeftTTL},
{ADD_3,      0,-5,1,5,  Get1,NULL,   0,0, 1,0,    0,200,NULL,NULL,NULL,NULL}

};




static int graf_time2(int chl)
{
 int i,val;

 for(i=0;i<chl*2;i++)
   {
   if(i%2==0)
     memcpy(&in2time[i].scr,&ChlScr[chl-1][i/2],sizeof(struct _scr_params));
   in2time[i].beg=0; in2time[i].end=rInf[i/2].nr-1;
   in2time[i].Xmax=(float)rInf[i/2].nr/rInf[i/2].Freq;
   in2time[i].Xmin=0;
   in2time[i].Ymax=5;
   in2time[i].Ymin=-5;
   in2time[i].Array1= (i%2)==0 ? (float *)Signal[2][i/2] : (float *)Signal[1][i/2];
   in2time[i].Array2= (float *)&rInf[i/2];
   }

 ScrSave();
 val=InitGRF(grfGRAPHICS);
 if(val==okGRF)
   {
   SetLook_Clr(putTitle); strcpy(TitleStr,"Эталонный/Реальный отклик объекта");
   val=LookGRF(chl*2,in2time);
   DeinitGRF(grfTEXT);
   }
 ScrRest();
 return val==okGRF ? 0 : 7;
}

static int graf_time3(int chl)
{
 int i,val;

 for(i=0;i<chl*2;i++)
   {
   if(i%2==0)
     memcpy(&in2time[i].scr,&ChlScr[chl-1][i/2],sizeof(struct _scr_params));
   in2time[i].beg=0; in2time[i].end=rInf[i/2].nr-1;
   in2time[i].Xmax=(float)rInf[i/2].nr/rInf[i/2].Freq;
   in2time[i].Xmin=0;
   in2time[i].Ymax=5;
   in2time[i].Ymin=-5;
   in2time[i].Array1= (i%2)==0 ? (float *)Signal[0][i/2] : (float *)Signal[1][i/2];
   in2time[i].Array2= (float *)&rInf[i/2];
   }

 ScrSave();
 val=InitGRF(grfGRAPHICS);
 if(val==okGRF)
   {
   SetLook_Clr(putTitle); strcpy(TitleStr,"Сигнал управления/Отклик объекта");
   val=LookGRF(chl*2,in2time);
   DeinitGRF(grfTEXT);
   }
 ScrRest();
 return val==okGRF ? 0 : 7;
}


extern "C" int ViewWork(int code,int OkPID)
{
 short ret=0,PID,chl;
 int k,f,n;
 unsigned long xpos,ypos,tpos,pos,x_len,y_len,t_len,i;
 float tmp;

 if((ret=LoadResultBanner(OkPID,CHANNEL,&f,rInf))==0)
   {
   for(i=chl=0;rInf[i].isPresent;i++,chl++)
       for(n=0;n<3;n++) if(rInf[i].is[n])
            {
            Signal[n][i]=new MemBuf();
               if(!Signal[n][i]) { ret=Err_MEM; goto done; }

            Signal[n][i]->setSize(rInf[i].nr);
               if(Signal[n][i]->getSize()!=rInf[i].nr) { ret=Err_MEM; goto done; }
            }
   if((ret=LoadResult(f,rInf,Signal[0],Signal[1],Signal[2]))==0)
      switch(code)
         {
         case 1: ret=graf_time1(chl); break;
         case 2: ret=graf_time2(chl); break;
         case 3: ret=graf_time3(chl); break;
         }
   }

done:

 return ret;
}

extern "C" int ViewWorkReport(int code,char *name)
{
 FILE *f;
 unsigned long nr,l,tik;
 unsigned short nk,n,i;
 char tmp1[30],tmp2[30],tmp3[30];
 float freq,val;
 int ret=0;


 f=fopen(name,"r+t");
 if(f)
   {
   fscanf(f,"%s %s %s",tmp1,tmp2,tmp3);
   fscanf(f,"%s %s %lu",tmp1,tmp2,&nr);
   fscanf(f,"%s %s %f",tmp1,tmp2,&freq);
   fscanf(f,"%s %s %u",tmp1,tmp2,&nk);
   for(i=0;i<nk;i++)
     {
     rInf[i].nr=nr;
     rInf[i].Freq=freq;
     }

   for(i=0;i<nk;i++) for(n=0;n<3;n++)
      {
      Signal[n][i]=new MemBuf();
      if(!Signal[n][i]) { ret=Err_MEM; goto done; }

      Signal[n][i]->setSize(rInf[i].nr);
      if(Signal[n][i]->getSize()!=rInf[i].nr) { ret=Err_MEM; goto done; }
      }
   for(l=0;l<nr;l++)
      {
      fscanf(f,"%lu",&tik);
      for(i=0;i<nk;i++) for(n=0;n<3;n++)
         {
         fscanf(f,"%f",&val);
         Signal[n][i]->putNext(val/5.0);
         }
      }
    switch(code)
         {
         case 101: ret=graf_time1(nk); break;
         case 102: ret=graf_time2(nk); break;
         case 103: ret=graf_time3(nk); break;
         }
done:
   }
 else ret=Err_DatFile;

 return ret;
}
