#include "look_def.h"
#include "stdlib.h"


static void far __GetPtr__(float *x,float *y,triadGRF *p,unsigned long num)
{
 *x=num*p->dX;
 *y=p->Array1[num];
 return;
}

static void far __GetMaxMin__(triadGRF *p)
{
 unsigned long i;
 float step;

 p->Ymin=p->Ymax=p->Array1[p->beg];
 for(i=p->beg;i<=p->end;i++)
  {
  if(p->Ymin>p->Array1[i]) p->Ymin=p->Array1[i];
  if(p->Ymax<p->Array1[i]) p->Ymax=p->Array1[i];
  }
 p->Xmin=p->beg*p->dX;
 p->Xmax=p->end*p->dX;
 if(p->Ymin==p->Ymax)
  {
  step=p->Ymax/10000.0;
  p->Ymin-=step;
  p->Ymax+=step;
  }
 if(p->Xmax==p->Xmin)
  {
  step=p->Xmax/10000.0;
  p->Xmax+=step;
  p->Xmin-=step;
  }
 return;
}

static triadGRF p[3]={
   {HOR_2_2,0,0,1,1,__GetPtr__,__GetMaxMin__,NULL,NULL,
    0,0, 0,0,"График 1","[ms]","f(%+7.4f)=%+7.4f",
    NULL,grfSCALE_ALL|grfPARAM_ALL,grfBLABEL|grfRANGE},
   {HOR_1_2,0,0,1,1,__GetPtr__,__GetMaxMin__,NULL,NULL,
    0,0, 0,0,"График 2","[ms]","f(%+7.4f)=%+7.4f(%+7.4f)",
    NULL,grfSCALE_ALL|grfPARAM_ALL,grfBLABEL|grfRANGE},
   {{0,0,0,0,LOOK_DEF_COL3},0,0,0,0,__GetPtr__,__GetMaxMin__}
    };

struct _scr_params _HOR_2_2=HOR_2_2,_FULL_SCR=FULL_SCR;

int far LookGRF2(float *y1,char *name1,int num1,float delt1,
                 float *y2,char *name2,int num2,float delt2)
{
 int val;

 val=InitGRF(grfGRAPHICS);
 if(val==okGRF)
   {
   p[0].scr=_HOR_2_2;
   p[0].end=num1-1; p[1].end=num2-1;
   p[0].dX=delt1; p[1].dX=delt2;
   p[0].Array1=y1;p[1].Array1=y2;
   p[1].ctrl=grfPARAM_ALL|grfSCALE_ALL;

   if(name1!=NULL) p[0].title=name1;
   if(name2!=NULL) p[1].title=name2;

   LookGRF(2,p);
   DeinitGRF(grfTEXT);
   }
 return(val);
}

int far LookGRF3(float *y1,char *name1,int num1,float delt1,
                 float *y2,char *name2,int num2,float delt2,
                 float *y3)
{
 int val;

 val=InitGRF(grfGRAPHICS);
 if(val==okGRF)
   {
   p[0].scr=_HOR_2_2;
   p[0].end=num1-1; p[1].end=num2-1; p[2].end=num2-1;
   p[0].dX=delt1;   p[1].dX=delt2;  p[2].dX=delt2;
   p[0].Array1=y1;  p[1].Array1=y2; p[2].Array1=y3;
   p[1].ctrl=grfDOUBLE|grfPARAM_ALL|grfSCALE_ALL;

   if(name1!=NULL) p[0].title=name1;
   if(name2!=NULL) p[1].title=name2;

   LookGRF(3,p);
   DeinitGRF(grfTEXT);
   }

 return(val);
}

int far LookGRF1(float *y,char *name,int num,float delt)
{
 int val;

 val=InitGRF(grfGRAPHICS);
 if(val==okGRF)
   {
   p[0].scr=_FULL_SCR;
   p[0].end=num-1;
   p[0].dX=delt;
   p[0].Array1=y;
   if(name!=NULL) p[0].title=name;

   LookGRF(1,p);
   DeinitGRF(grfTEXT);
   }
 return(val);
}

int far SetLook_Col(unsigned int num,struct _scr_params *col)
{
 p[num%3].scr=*col;
 return(okGRF);
}
