#include <stdlib.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>

#include "config.h"

#ifndef __TURBOC__
#define M_PI 3.1415
#endif

CfgT Cfg={
 /* число каналов управления, аналоговых информационных */
// short int nk,nkADC;
 6,6,
 /* цифровые каналы */
// short int nkIN,nkOUT;
 0,0,
 /* частота дискретизации аналогового сигнала */
// float Freq;
 256.0,
 /* длительность работы */
// unsigned long nr,nr_view,cycle;
 1000,1000,1,
 /* выход на начальную точку перед запуском */
// short int iniTime;
 5,
 /* действия по превышению допустимой погрешности */
// short int AftErr;
 0,
 /* протокол работы */
// short int LogLevel;
 2,
// unsigned long LogTime,LogSize; char DebugLevel[35];
 1,0,"",

 /* эталон / отчет */
// float ifile_k,ifile_st;
 1,1,
// char  ifile_name[20],ufile_name[20];
 "NONAME.TEM","NONAME.UPR",

 /* каталог архива */
// char ArchivePath[80];
 ".\\",
 /* каталог отчетов */
// char ReportPath[80];
 ".\\",
 /* каталог эталонных сигналов */
// char TemplatePath[80];
 ".\\",

 {
// 0__1___________2____3________4____5___________________6__________________________7________8__________9_____________10____11_____________
  {1, 0,0,0,1,25, 0,0, 0,0,1,5, 1,0, "NONAME.TEM",1,0,0, 0,{1,0,0},{3,6,9},{0,0,0}, 0,0,0,5, 5,-5,50,0, 0,27.7,0.3,1, -5,0, {0,0,1},{0,0,1}},
  {1, 0,0,0,1,25, 0,0, 0,0,1,5, 1,0, "NONAME.TEM",1,0,1, 0,{1,0,0},{3,6,9},{0,0,0}, 0,0,0,5, 5,-5,50,0, 0,27.7,0.3,1, -5,0, {0,0,1},{0,0,1}},
  {1, 0,0,0,1,25, 0,0, 0,0,1,5, 1,0, "NONAME.TEM",1,0,2, 0,{1,0,0},{3,6,9},{0,0,0}, 0,0,0,5, 5,-5,50,0, 0,27.7,0.3,1, -5,0, {0,0,1},{0,0,1}},
  {1, 0,0,0,1,25, 0,0, 0,0,1,5, 1,0, "NONAME.TEM",1,0,3, 0,{1,0,0},{3,6,9},{0,0,0}, 0,0,0,5, 5,-5,50,0, 0,27.7,0.3,1, -5,0, {0,0,1},{0,0,1}},
  {1, 0,0,0,1,25, 0,0, 0,0,1,5, 1,0, "NONAME.TEM",1,0,4, 0,{1,0,0},{3,6,9},{0,0,0}, 0,0,0,5, 5,-5,50,0, 0,27.7,0.3,1, -5,0, {0,0,1},{0,0,1}},
  {1, 0,0,0,1,25, 0,0, 0,0,1,5, 1,0, "NONAME.TEM",1,0,5, 0,{1,0,0},{3,6,9},{0,0,0}, 0,0,0,5, 5,-5,50,0, 0,27.7,0.3,1, -5,0, {0,0,1},{0,0,1}},
  {1, 0,0,0,1,25, 0,0, 0,0,1,5, 1,0, "NONAME.TEM",1,0,6, 0,{1,0,0},{3,6,9},{0,0,0}, 0,0,0,5, 5,-5,50,0, 0,27.7,0.3,1, -5,0, {0,0,1},{0,0,1}},
  {1, 0,0,0,1,25, 0,0, 0,0,1,5, 1,0, "NONAME.TEM",1,0,7, 0,{1,0,0},{3,6,9},{0,0,0}, 0,0,0,5, 5,-5,50,0, 0,27.7,0.3,1, -5,0, {0,0,1},{0,0,1}}
//    short int isActive; ----------------------------------------0
    /* модель управления */
//    short int mdlCod; short int mdlType,p,q,Q; -----------------1
    /* тренд управления */
//    short int trCod; short int trP;-----------------------------2
    /* погрешность управления */
//    short int mstCod; float mstMax,mstTime,mstMargin;-----------3
    /* эталонный сигнал */
//    short int tplCod; shortint isEMS;---------------------------4
    /* данные из файла */
        // char file_name[20]; float file_k,file_st,file_chl;-----5
    /* полигармоника   */
        // float u,a[3],f[3],fa[3];-------------------------------6
    /* удаление тренда из сигнала управления */
//    short int delTr;--------------------------------------------7
//    unsigned short delTr_size; float delTr_vol,delTr_speed;
    /* ограничения управляющего сигнала */
//    float MaxStatic,MinStatic,MaxSpeed,MaxAcceleration;---------8
    /* переходный процесс */
//    short int begCod; float begFreq,begAmpl,begZoom;------------9
    /* уровень покоя, стартовый уровень при запуске управления */
//    float PassiveVol,StartVol;----------------------------------10
    /* физические величины */
//    struct { ---------------------------------------------------11
//      short int T;   /* тип задействованого датчика */
//      float A,D;     /* коэффициенты пересчета = A + (D * вольты) */
//    } usrDAC,usrADC; /* датчик управления и датчик отклика */
  },

  /* ---------------------------------- характеристики каналов управления */
//  char afile_name[20];
  "NONAME.ACH",
//  struct { /* АЧХ */
//      short int Mode;
//      short int Periods,Nq;
//      float Ampl,FreqMin,FreqMax,Const;
//      } Ach[MAX_CHANNEL];
  {{0,10,50,1,0.5,30,0},{0,10,50,1,0.5,30,0},{0,10,50,1,0.5,30,0},{0,10,50,1,0.5,30,0}},

//  char sfile_name[20];
  "NONAME.STH",
//  struct { /* статическая */
//      short int Mode
//      short int statNq;
//      float Wait,Lo,Hi,HiSpeed,HiAcceleration;
//      } Sth[MAX_CHANNEL];
  {{0,50,2,-5,5,50,3},{0,50,2,-5,5,50,3},{0,50,2,-5,5,50,3},{0,50,2,-5,5,50,3}},

//  char wfile_name[20];
  "NONAME.WZS",
//  struct { /* взаимодействие в статике */
//      short int Nq;
//      float Lo,Hi;
//      } Wzst[MAX_CHANNEL];
  {{50,-5,5},{50,-5,5},{50,-5,5},{50,-5,5}},


  { /* ---------------------------------- информационные каналы */
   {0,1, {0,0},{0},{{0,0,-0.5,0.5},{0,1,-1,1},{1,1,-4,4}},{0}},
   {0,0, {1,1},{0},{{0,0,-0.5,0.5},{0,1,-1,1},{1,1,-4,4}},{0}},
   {0,1, {2,2},{0},{{0,0,-0.5,0.5},{0,1,-1,1},{1,1,-4,4}},{0}},
   {0,0, {3,3},{0},{{0,0,-0.5,0.5},{0,1,-1,1},{1,1,-4,4}},{0}},
   {0,1, {4,4},{0},{{0,0,-0.5,0.5},{0,1,-1,1},{1,1,-4,4}},{0}},
   {0,1, {5,5},{0},{{0,0,-0.5,0.5},{0,1,-1,1},{1,1,-4,4}},{0}}

//  struct {
//    short int isActive;
    /* режим работы */
//    short int infCod; /* 0-значение 1-контроль границ 2-расчет хар-ки */
    /* номера каналов */
//    short int Chl[2];
//       struct {  /* =========== значение */
         /* контролирумый массив */
//         unsigned long size; /* 0-среднее 1-текущее 2..N-бегущее среднее */
//         } Average;
//       struct {  /* =========== контроль границ */
//         struct {
           /* реакция */
//           short int outCod;      /* 0-индикация 1-отключение 2-перезапуск 3-приостановка */
           /* звуковое сопровождение */
//           short int isMute;
           /* границы диапазона */
//           float MinVol,MaxVol;
//           } Normal,Warning,Crach;
//         } Defence;
//       struct {  /* =========== характеристики */
         /* код характеристики */
//         short int exmCod; /* 0-???? */
//         } Examine;
//      } Inform[MAX_infoCHANNEL];
   },

  /* ---------------------------------- неадаптивное управление */
//  struct
  {
//    {
//    int isBatch;     /* пакетный режим (без предварительного просмотра) */
     1,
//    unsigned Cycles; /* число циклов предварительного эксперимента */
     5,
//    struct {
//0     int q;                /* задержка УСО */
//1     float Mistake;        /* допустимая погрешность по каналу */
//2     unsigned long Margin; /* начальный отсчет */
//3     unsigned long nr;     /* размер окна (0=max) */
//4     unsigned long nq;     /* число расчетных точек/окно (0=nr) */
//      } Def[MAX_CHANNEL];
//
//    0- 1-- 2- 3- 4-
    {{ 1,0.5, 5, 0, 0},
     { 1,0.5, 5, 0, 0},
     { 1,0.5, 5, 0, 0},
     { 1,0.5, 5, 0, 0},
     { 1,0.5, 5, 0, 0},
     { 1,0.5, 5, 0, 0}}
//   } NMDL;
  },

 /* координаты окна для индикации процесса работы */
// short int ScreenX,ScreenY;
  47,10,

 /* Пользовательский интерфейс */
// short int LastMode,LastField;
  0,0,

// char itEOF;
  0xFF
};

/* --------------------------------------------------------------------- */

void getFile(char *fn,char *ext,char *buf)
{
 char d[5],p[80],n[10],e[6];

 _splitpath(fn,d,p,n,e);
 _makepath(buf,"","",n,ext);
 return;
}

static char defaultCfg[40]="lastwork.ini";

int loadConfig(char *name)
{
 char buf[100];
 int f,ret=CFG_OK;
 CfgT tmp;

 if(!name) getFile(defaultCfg,"ini",buf);
 else getFile(name,"ini",buf);

 f=open(buf,O_RDONLY|O_BINARY);
 if(f>0)
   {
   read(f,buf,sizeof(IDstr));
   if(!strncmp(buf,IDstr,sizeof(IDstr)))
     {
     if(read(f,&tmp,sizeof(tmp))==sizeof(Cfg)) memcpy(&Cfg,&tmp,sizeof(Cfg));
     else ret=CFG_CORRUPT; /* файл поврежден */
     }
   else ret=CFG_FORMAT; /* неверный формат или не файл конфигурации */
   close(f);
   }
 else if(name) ret=CFG_RDOPEN; /* нет файла или невозможно открыть */

 return ret;
}


int saveConfig(char *name)
{
 char buf[100];
 int f,ret=0;

 if(!name) getFile(defaultCfg,"ini",buf);
 else getFile(name,"ini",buf);

 f=open(buf,O_WRONLY|O_CREAT|O_BINARY,S_IREAD|S_IWRITE);
 if(f>0)
   {
   if(write(f,IDstr,sizeof(IDstr))!=sizeof(IDstr) ||
      write(f,&Cfg,sizeof(Cfg))!=sizeof(Cfg)) ret=CFG_WRITE; /* невозможно записать */
   close(f);
   }
 else ret=CFG_WROPEN; /* невозможно открытьёсоздать */

 return ret;
}



