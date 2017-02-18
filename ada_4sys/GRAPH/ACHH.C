#include "ada_graf.h"
#include "common.h"

/* ==================================================================== */

static void GetE(float *x,float *y,triadGRF *p,unsigned long num)
{
 if(p->Array1) *x=p->Array1[(unsigned)num];
 else *x=0;
 if(p->Array2) *y=p->Array2[(unsigned)num];
 else *y=0;
 return;
}

//*********************************************
static triadGRF ach[8]={
{BOX_1_8_T,  0,-5,1,5,  GetE,NULL,   0,0, 1,0,    0,100, "Канал 0","[Частота]","f[%5.2f]=%+6.3f",NULL,grfPARAM_ALL,grfRANGE|grfLeftTTL},
{BOX_2_8_T,  0,-5,1,5,  GetE,NULL,   1,1, 1,0,    0,100, "Канал 1","[Частота]","f[%5.2f]=%+6.3f",NULL,grfPARAM_ALL,grfRANGE|grfLeftTTL},
{BOX_3_8_T,  0,-5,1,5,  GetE,NULL,   2,2, 1,0,    0,100, "Канал 2","[Частота]","f[%5.2f]=%+6.3f",NULL,grfPARAM_ALL,grfRANGE|grfLeftTTL},
{BOX_4_8_T,  0,-5,1,5,  GetE,NULL,   3,3, 1,0,    0,100, "Канал 3","[Частота]","f[%5.2f]=%+6.3f",NULL,grfPARAM_ALL,grfRANGE|grfLeftTTL},
{BOX_5_8_T,  0,-5,1,5,  GetE,NULL,   4,4, 1,0,    0,100, "Канал 4","[Частота]","f[%5.2f]=%+6.3f",NULL,grfPARAM_ALL,grfRANGE|grfLeftTTL},
{BOX_6_8_T,  0,-5,1,5,  GetE,NULL,   5,5, 1,0,    0,100, "Канал 5","[Частота]","f[%5.2f]=%+6.3f",NULL,grfPARAM_ALL,grfRANGE|grfLeftTTL},
{BOX_7_8_T,  0,-5,1,5,  GetE,NULL,   6,6, 1,0,    0,100, "Канал 6","[Частота]","f[%5.2f]=%+6.3f",NULL,grfPARAM_ALL,grfRANGE|grfLeftTTL},
{BOX_8_8_T,  0,-5,1,5,  GetE,NULL,   7,7, 1,0,    0,100, "Канал 7","[Частота]","f[%5.2f]=%+6.3f",NULL,grfPARAM_ALL,grfRANGE|grfLeftTTL}
};



int ViewACH(int chl,int OkPID)
{
 int i,PID,f,k;
 float ma=1;
 unsigned short int nr;

 for(i=0;i<CHANNEL;i++) x[i]=y[i]=NULL;


 f=open("last_ach.dat",O_RDONLY|O_BINARY);
 if(f)
   {
   BufInit();
   BufRead(f,&PID,sizeof(PID));
   if(PID==OkPID || OkPID==0)
     {
     BufRead(f,&chl,sizeof(chl));
     for(k=0;k<chl;k++)
       {
       BufRead(f,&nr,sizeof(nr));
       if(nr>0)
         {
          ma=1;

          x[k]=calloc(sizeof(float),nr);
          y[k]=calloc(sizeof(float),nr);
          if(!(x[k] && y[k] )) return Err_MEM;
          for(i=0;i<nr;i++)
             {
             BufRead(f,&x[k][i],sizeof(float));
             BufRead(f,&y[k][i],sizeof(float));
             ma= (y[k][i]+0.5>ma) ?  y[k][i]+0.5 :ma;
             }
          ach[k].beg=0;   ach[k].end=nr-1;
          ach[k].Ymax=ma; ach[k].Ymin=0;
          ach[k].Xmax=x[k][ach[k].end]; ach[k].Xmin=x[k][0];
          }
        else
          {
          x[k]=NULL; y[k]=NULL;
          ach[k].beg=0;   ach[k].end=1;
          ach[k].Ymax=1; ach[k].Ymin=0;
          ach[k].Xmax=1; ach[k].Xmin=0;
          }
       }

 for(i=0;i<chl;i++)
   {
   memcpy(&ach[i].scr,&ChlScr[chl-1][i],sizeof(struct _scr_params));
   ach[i].Array1=x[i];
   ach[i].Array2=y[i];
   }

 ScrSave();
 InitGRF(grfGRAPHICS);
 SetLook_Clr(putTitle); strcpy(TitleStr,"АЧХ");
 LookGRF(chl,ach);
 DeinitGRF(grfTEXT);
 ScrRest();
  } /* PID */
 } /* file */
 return 0;
}


int ViewACHReport(int chl,char *name)
{
 FILE *f;
 char tmp[20],tmp1[20],tmp2[20];
 float freq,val,ma,mi;
 int k,i,m;

 f=fopen(name,"r+t");
 if(f)
   {
   fscanf(f,"%s %s %s",tmp,tmp1,tmp2);
   fscanf(f,"%s %s %f",tmp,tmp1,&freq);
   fscanf(f,"%s %s %d %s %d",tmp,tmp1,&chl,tmp2,&nko);
   for(k=0;k<chl;k++)
     {
     fscanf(f,"%s %s %s",tmp,tmp1,tmp2);
     fscanf(f,"%s %s %d",tmp,tmp1,&nr);
     x[k]=calloc(nr,sizeof(float));
     y[k]=calloc(nr,sizeof(float));
     if( !x[k] || !y[k]) goto done;
     for(i=0;i<nr;i++)
       {
       fscanf(f,"%s %f",tmp,&x[k][i]);
       for(m=0;m<chl;m++)
          { fscanf(f,"%f",&val); if(m==k) y[k][i]=val; }
       ma= (y[k][i]+0.5>ma) ?  y[k][i]+0.5 :ma;
       }
     ach[k].beg=0;   ach[k].end=nr-1;
     ach[k].Ymax=ma; ach[k].Ymin=0;
     ach[k].Xmax=x[k][ach[k].end]; ach[k].Xmin=x[k][0];
     }

 for(i=0;i<chl;i++)
   {
   memcpy(&ach[i].scr,&ChlScr[chl-1][i],sizeof(struct _scr_params));
   ach[i].Array1=x[i];
   ach[i].Array2=y[i];
   }

 ScrSave();
 InitGRF(grfGRAPHICS);
 SetLook_Clr(putTitle); strcpy(TitleStr,"АЧХ");
 LookGRF(chl,ach);
 DeinitGRF(grfTEXT);
 ScrRest();

done:
   fclose(f);
   }
 return 0;
}

