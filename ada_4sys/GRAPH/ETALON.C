#include "ada_graf.h"
#include "common.h"
/*#include "tabfile.hpp"

class etalonFile:public tabTextFile
{
 public:
 etalonFile(char *filename,int begCol,int numCol)

}
*/

/* ==================================================================== */

void GetE(float *x,float *y,triadGRF *p,int num)
{
 *x=(unsigned)num*delt;
 if(p->Array1) *y=5.0*p->Array1[(unsigned)num];
 else *y=0;
 return;
}

//*********************************************
static triadGRF et_time[8]={
{BOX_1_8_T,  0,-5,1,5,  GetE,NULL,   0,0, 1,0,    0,100, "Канал 0","[Время]","f[%5.2f]=%+6.3f",NULL,grfPARAM_ALL,grfRANGE|grfLeftTTL},
{BOX_2_8_T,  0,-5,1,5,  GetE,NULL,   1,1, 1,0,    0,100, "Канал 1","[Время]","f[%5.2f]=%+6.3f",NULL,grfPARAM_ALL,grfRANGE|grfLeftTTL},
{BOX_3_8_T,  0,-5,1,5,  GetE,NULL,   2,2, 1,0,    0,100, "Канал 2","[Время]","f[%5.2f]=%+6.3f",NULL,grfPARAM_ALL,grfRANGE|grfLeftTTL},
{BOX_4_8_T,  0,-5,1,5,  GetE,NULL,   3,3, 1,0,    0,100, "Канал 3","[Время]","f[%5.2f]=%+6.3f",NULL,grfPARAM_ALL,grfRANGE|grfLeftTTL},
{BOX_5_8_T,  0,-5,1,5,  GetE,NULL,   4,4, 1,0,    0,100, "Канал 4","[Время]","f[%5.2f]=%+6.3f",NULL,grfPARAM_ALL,grfRANGE|grfLeftTTL},
{BOX_6_8_T,  0,-5,1,5,  GetE,NULL,   5,5, 1,0,    0,100, "Канал 5","[Время]","f[%5.2f]=%+6.3f",NULL,grfPARAM_ALL,grfRANGE|grfLeftTTL},
{BOX_7_8_T,  0,-5,1,5,  GetE,NULL,   6,6, 1,0,    0,100, "Канал 6","[Время]","f[%5.2f]=%+6.3f",NULL,grfPARAM_ALL,grfRANGE|grfLeftTTL},
{BOX_8_8_T,  0,-5,1,5,  GetE,NULL,   7,7, 1,0,    0,100, "Канал 7","[Время]","f[%5.2f]=%+6.3f",NULL,grfPARAM_ALL,grfRANGE|grfLeftTTL}
};



void vi_et_graph(int chl)
{
 int i;

 for(i=0;i<chl;i++)
   {
   memcpy(&et_time[i].scr,&ChlScr[chl-1][i],sizeof(struct _scr_params));
   et_time[i].beg=0; et_time[i].end=nr-1;
   et_time[i].Xmax=nr*delt; et_time[i].Xmin=0;
   et_time[i].Ymax=5;       et_time[i].Ymin=-5;
   et_time[i].Array1=yt[i];
   }

 ScrSave();
 InitGRF(grfGRAPHICS);
 SetLook_Clr(putTitle); strcpy(TitleStr,"Эталонный сигнал");
 LookGRF(chl,et_time);
 DeinitGRF(grfTEXT);
 ScrRest();
 return;
}


int ViewEtalon(int code,int OkPID)
{
 short ret=0,PID,chl;
 int k,f;
 unsigned int tpos,pos,t_len,i;
 float tmp;

 for(i=0;i<CHANNEL;i++) yt[i]=NULL;


 f=open("last_tem.dat",O_RDONLY|O_BINARY);
 if(f)
   {
   BufInit();
   BufRead(f,&PID,sizeof(PID));
   if(PID==OkPID || OkPID==0)
     {
     BufRead(f,&chl,sizeof(chl)); nki=chl;
     BufRead(f,&tmp,sizeof(tmp)); delt=1.0/tmp;
     BufRead(f,&nr,sizeof(nr));
     for(i=0;i<nki;i++)
       {
       yt[i]=calloc(sizeof(float),nr);
       if(!yt[i]) ret=Err_MEM;
       }

     if(ret==0) for(i=0;i<nki;i++)
       {
       BufRead(f,&t_len,sizeof(t_len));
       for(k=0;k<t_len;k++)
               BufRead(f,&yt[i][k],sizeof(float));
       for(;k<nr;k++) yt[i][k]=0;
       }
     if(ret==0) vi_et_graph(chl);
     }
   else ret=Err_PID;
   }
 else ret=Err_DatFile;

 return ret;
}

int ViewEtalonReport(int code,char *name)
{
 FILE *f;
 float freq;
 unsigned i,k,nk;
 int ret=0;

 f=fopen(name,"r+t");
 if(f)
   {
   fscanf(f,"%d %f %d",&nk,&freq,&nr); delt=1.0/freq;
   for(i=0;i<nk;i++)
     {
     yt[i]=calloc(sizeof(float),nr);
     if(!yt[i]) { ret=Err_MEM; goto done; }
     }
   for(i=0;i<nr;i++) for(k=0;k<nk;k++)
     {
     fscanf(f,"%f",&yt[k][i]);
     yt[k][i]/=5;
     }
   fclose(f);
   vi_et_graph(nk);
   }
 else ret=Err_DatFile;
done:
 return ret;
}
