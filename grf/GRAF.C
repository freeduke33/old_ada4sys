
#include "look_def.h"
#include "..\upi\dac_adc.h"

#include "conio.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "dos.h"
/* #include "malloc.h" */

#define MAXSIZE 2000


void myFun(float *x,float *y,triadGRF *p,int num)
{
 *x=num;
 *y=p->Array1[num];
 return;
}
void myFun1(float *x,float *y,triadGRF *p,int num)
{
 *x=p->Xmin+num*p->dX;
 *y=p->Array1[num];
 return;
}

void Get(float *x,float *y,triadGRF *p,int num)
{
 *x=p->Xmin+num*p->dX;
 *y=sin(2.0*3.1415*(*x)*2.0);
 return;
}

void MaxXY(triadGRF *p)
{
 p->dX=(p->Xmax-p->Xmin)/100;
 p->beg=0;
 p->end=100;
 return;
}

float *y,*x,*z,maxx,maxy,minx,miny;

triadGRF param[10]={{HOR_1_2,
                 0,-1.0,MAXSIZE,1.0,
                 myFun,NULL,
                 0,0,0.01,0,
                 0,MAXSIZE,
                 "Title 1","[1]","f[%05.3f]=%8.6f(%8.6f)",
                 "mark=%4g",grfPARAM_ALL|grfSCALE_ALL|grfDOUBLE,
                 grfBLABEL|grfRANGE,"%+5.3f:%+5.3f,%+5.3f,%+5.3f,%+5.3f"},
         {ADD_1,
                 0,-1,MAXSIZE,1,
                 myFun,NULL,
                 0,0,0.01,0,
                 0,MAXSIZE,
                 "Title 2","[2]"," f(%8g)=%8g ",NULL,grfDOUBLE},
         {ADD_2,
                 0,-1,MAXSIZE,1,
                 myFun,NULL,
                 0,0,0.01,0,
                 0,MAXSIZE,
                 "Title 2","[2]"," f(%g)=%g ",NULL,grfDOUBLE},
         {ADD_9,
                 0,-1,MAXSIZE,1,
                 myFun,NULL,
                 0,0,0.01,0,
                 0,MAXSIZE,
                 "Title 2","[2]"," f(%g)=%g "},
         {HOR_2_2,
                 0,-0.1,MAXSIZE,0.1,
                 myFun,NULL,
                 0,0,1,0,
                 0,MAXSIZE,
                 "Title 3","[3]"," f(%g)=%g ",NULL,grfPARAM_ALL,0,
                 "F[%g]=%g !!! "}};

main(int argc,char *argv[])
{
 int i=0;
 float nn;
 static char str[40]="";
 FILE *f;

  x=calloc(6001,sizeof(float));
  y=calloc(6001,sizeof(float));
  z=calloc(6001,sizeof(float));

  if(!x || !y || !z)
    {
    printf("No more free memory");
    exit(2);
    }


 if(argc>1) strncpy(str,argv[1],40);
 else
   {
   printf(">");
   scanf("%s",str);
   }

 f=fopen(str,"r");
 if(f!=NULL)
 {
  for(i=0;i<MAXSIZE;i++)
  {
   z[i]=x[i]=y[i]=0;
   fscanf(f,"%e %e",&x[i],&y[i]);
   maxx= maxx>x[i] ? maxx : x[i];  minx= minx<x[i] ? minx : x[i];
   maxy= maxy>y[i] ? maxy : y[i];  miny= miny<y[i] ? miny : y[i];
   z[i]=FromADC(ToDAC(x[i],2048),2048);
  }
  fclose(f);
 }
 else
 {
  printf("\nRandomize mode ...\n");//getch();
  for(i=0;i<MAXSIZE;i++)
  {
   x[i]=rand()/(float)RAND_MAX-0.5;
   y[i]=rand()/(float)RAND_MAX-0.5;
   z[i]=rand()/(float)RAND_MAX-0.5;
   maxx= maxx>x[i] ? maxx : x[i];  minx= minx<x[i] ? minx : x[i];
   maxy= maxy>y[i] ? maxy : y[i];  miny= miny<y[i] ? miny : y[i];
  }
 }


 param[0].Array1=x; param[0].Ymax=maxx; param[0].Ymin=minx;
 param[1].Array1=z; param[1].Ymax=maxx; param[1].Ymin=minx;
 param[2].Array1=z;
 param[3].Array1=z;
 param[4].Array1=y; param[4].Ymax=maxy; param[4].Ymin=miny;

 InitGRF(grfGRAPHICS);
 TextGRF("ABCDEFGH АБВГДЕЖЗИК",
          500,500,7,0,100,100,grfAxisY,SOLID_FILL,grfCENTER);
 scanf("%s",str);
// system("c:\command.com");
// LookGRF3(x," График N1 ",MAXSIZE,1,y," График N2 ",MAXSIZE,1,z);
// LookGRF(5,&param);
 DeinitGRF(grfTEXT);
 return(0);
}





