#define EMUL_ARSS
// #define EMUL_NUL

#include <stdio.h>
#include <math.h>

#define Uses_arss_qTS270392
#define Uses_ss_qTS270392
#define Uses_mdl_0TR031095
#define Uses_mdl_arss211196


#define Uses_MemBufPSinus

#include <adaptive.hpp>
#include <log.hpp>
// #include <models.hpp>

#define NR 1000

#define FREQ 254.00

float X[1000],Y[1000],Yt[1000];

#define M(a) (((a)+50)%50)

float Emul(float x)
{
static int tik=0;
static float tmp[3]={0,0,0};

 double C[10]={1,2.8,2.94,1.372,0.2401},Lam[10]={1,-3.2,3.84,-2.048,0.4096};
// double C[10]={1,0.7},Lam[10]={1,-0.8};
 int p=4,q=2,v;
 static double X[50],Y[50];
 double su1=0,su2=0;

 if((++tik)%20==0) printf("\r %u",tik);


#ifdef EMUL_NUL

 tmp[0]=tmp[1]; tmp[1]=tmp[2];

 tmp[2]=x/2+0.01;
// tmp[0]+=0.01*sin(2.0*M_PI*0.2*tik/FREQ);

 return tmp[0];

#else

 X[M(tik)]=x;
// if(tik<98) return Y[M(tik)]=x;

 for(v=0,su1=0;v<=p;v++)
   {
   if(tik-q-v<0) continue;
   su1+=Lam[v]*X[M(tik-q-v)];
   }

#ifdef EMUL_ARSS
 for(v=1,su2=0;v<=p;v++)
   {
   if(tik-v<0) continue;
   su2+=C[v]*Y[M(tik-v)];
   }
#else
  su2=0;
#endif

 tmp[0]=Y[M(tik)]=(su1-su2)/C[0];


 return tmp[0];
#endif

}

int main()
{
 FILE *f;
 unsigned i;
 double x,y,yt;
 char buf[20];

 Log2TxtFile l("debug.log");

 float a[3]={0.3,0,0},fr[3]={6.5,0,0},fi[3]={0,0,0};
 Buffer *b=new MemBufPSinus(NR,FREQ,a,fr,fi,0);
 //========== (порядок, регуляризация, сдвиг, тип управления)
//  ss_qTS270392 m(4,10,2,0);
 FModel_1x1 *m=new arss_qTS270392(4,60,2,0);
//  mdl_arss211196 m(0,0,60,2);


// --------------------- подготовка
 m->setIdeal(b);
 m->Reset();

 b->reset();

 x=0;
 yt=0;

 l.setLevel(".?");
 LOG=&l;

// --------------------- первоначальное построение модели
 for(i=0;i<220;i++)
   {
   LOG->put('.',"\n[%u]\t",i);
   y=Emul(x);
   x=b->getNext();


   m->Append(x,y);
   m->Build();

   yt=b->get(0);
   X[i+1]=x; Y[i]=y; Yt[i+1]=yt;
   }

// --------------------- замыкание обратной связи
 m->Update();
 if(!m->isBuild()) { printf("Can't build.\n"); return 1;}

 for(;i<NR;i++)
   {
   LOG->put('.',"\n[%u]\t",i);
   y=Emul(x);

   m->Build(); m->Update();
   x=m->Calc(y);

   yt=b->get(0);
   X[i+1]=x; Y[i]=y; Yt[i+1]=yt;
   }

// --------------------- сохранение массивов
 f=fopen("test.upr","w+t");
 if(f)
   {
   printf("\nWriting ...\n");
   fprintf(f,"+++ Заголовок_: \"ПРОТОКОЛ_УПРАВЛЕНИЯ_СТЕНДОМ\"\n");
   fprintf(f,"+++ Длинна_ряда_: %d\n",i);
   fprintf(f,"+++ Частота_дискретизации_: %g\n",FREQ);
   fprintf(f,"+++ Число_каналов_: %dx%d\n",1,1);
   fprintf(f,"\n+++ Канал_N 1\n");

   for(i=0;i<NR;i++)  fprintf(f,"%u\t %f\t %f\t %f\n",i,X[i]*5,Y[i]*5,Yt[i]*5);

   fclose(f);
   }

 return 0;
}


