#ifndef __CONFIG__H__
#define __CONFIG__H__

#define MAX_CHANNEL 8
#define MAX_infoCHANNEL 32
#define MAX_digitCHANNEL 8

#define MAX_FILENAME  256
#define MAX_DIRNAME   256

#define CFG_OK      0
#define CFG_FORMAT  1
#define CFG_RDOPEN  2
#define CFG_CORRUPT 3
#define CFG_READ    4
#define CFG_WROPEN  5
#define CFG_WRITE   6



#define IDstr "Adaptive control: configuration structure v1.3.6\n\r"

typedef enum {         mc_NONE,
                       mc_mdl_270395,
                       mc_gen_270395,
                       mc_mdl_TR270395,
                       mc_gen_TR270395,
                       mc_mdl_TR031095,
                       mc_mdl_ARSS_TS,
                       mc_gen_ARSS_TS,
                       mc_mdl_SS_TS,
                       mc_gen_SS_TS,
                       mc_mdl_ARSS_1K,
                       mc_mdl_SS_1K,
                       mc_mdl_ARSS_2,
                       mc_PASSIVE,
                       mc_gen_FILE
} DefCodModel;

#pragma pack(1)

typedef struct
{
 /* число каналов управления, аналоговых информационных */
 short int nk,nkInfo;
 /* цифровые каналы */
 short int nkIN,nkOUT;

 /* частота дискретизации аналогового сигнала */
 float Freq;
 /* длительность работы */
 unsigned long nr,nr_view,cycle;
 /* время выхода на начальную точку перед запуском */
 float iniTime;
 /* действия по превышению допустимой погрешности */
 short int AftErr; /* 0-отключение 1-перезапуск 2-приостановка */

 /* протокол работы */
 short int LogLevel; /* 0-минимальный 1-нормальный 2-детальный 3-отладка */
 unsigned long LogTime,LogSize; char DebugLevel[35];

 /* эталон / отчет */
 float ifile_k,ifile_st;
 char  ifile_name[MAX_FILENAME],ufile_name[MAX_FILENAME];

 /* каталог архива */
 char ArchivePath[MAX_DIRNAME];
 /* каталог отчетов */
 char ReportPath[MAX_DIRNAME];
 /* каталог эталонных сигналов */
 char TemplatePath[MAX_DIRNAME];

 struct {  /* ------------------ описание для канала управления */
    short int isActive;
    /* модель управления */
    short int mdlCod; /* см. DefCodModel */
    short int mdlType,p,q,Q; /* 0-перемешение 1-скорость 2-ускорение */
    /* тренд управления */
    short int trCod; short int trP; /* 0-встроенный 1-разностный 2-полиномиальный */
    /* погрешность управления */
    short int mstCod; /* 0-квадратическая 1-мат.ожидание 2-алгоритм Шенка */
    float mstMax,mstTime,mstMargin;

    /* эталонный сигнал */
    short int tplCod; /* 0-нет 1-полигармоника 2-из файла */
    short int isEMS; /* хранение эталона в EMS 0-нет 1-да */
    /* данные из файла */ char file_name[MAX_FILENAME]; float file_k,file_st; short int file_chl;
    /* полигармоника   */ float u; float a[3],f[3],fa[3];

    /* удаление тренда из сигнала управления */
    short int delTr;
    unsigned short delTr_size; float delTr_vol,delTr_speed; /* size==0 - отключено */

    /* ограничения управляющего сигнала */
    float MaxStatic,MinStatic,MaxSpeed,MaxAcceleration;

    /* переходный процесс */
    short int begCod; /* 0-синус 1-эталон */
    float begFreq,begAmpl,begZoom;

    /* уровень покоя, стартовый уровень при запуске управления */
    float PassiveVol,StartVol;

    /* физические величины */
    struct {
      short int T;   /* тип задействованого датчика (физическая величина) */
      float A,D;     /* коэффициенты пересчета = A + (D * вольты) */
    } usrDAC,usrADC; /* датчик управления и датчик отклика */

    } Def[MAX_CHANNEL];



  /* ---------------------------------- характеристики каналов управления */
  char afile_name[20];
  struct { /* частотные характеристики */
      short int Mode;  /* 0-АЧХ 1-ФЧХ */
      short int Periods,Nq;
      float Ampl,FreqMin,FreqMax,Const;
      } Ach[MAX_CHANNEL];

  char sfile_name[20];
  struct { /* статическая */
      short int Mode; /* 0-перемещение 1-скорость 2-ускорение */
      short int Nq;
      float Wait,Lo,Hi,HiSpeed,HiAcceleration;
      } Sth[MAX_CHANNEL];

  char wfile_name[20];
  struct { /* взаимодействие в статике */
      short int Nq;
      float Lo,Hi;
      } Wzst[MAX_CHANNEL];

  /* ---------------------------------- информационные каналы */
  struct {
    short int isActive;
    /* режим работы */
    short int infCod; /* 0-значение 1-контроль границ 2-расчет хар-ки */
    /* логические номера каналов */
    short int Chl[2];
     struct {  /* =========== значение */
       /* контролирумый массив */
       unsigned long size; /* 0-среднее 1-текущее 2..N-бегущее среднее */
       } Average;
     struct {  /* =========== контроль границ */
       struct {
         /* реакция на выход за границу */
         short int outCod;      /* 0-отключение 1-перезапуск 2-приостановка */
         /* звуковое сопровождение */
         short int isMute;
         /* границы диапазона */
         float MinVol,MaxVol;
         } Normal,Warning,Crash;
       } Defence;
     struct {  /* =========== характеристики */
       /* код характеристики */
       short int exmCod; /* 0-???? */
       } Examine;
     } Inform[MAX_infoCHANNEL];

  /* ---------------------------------- цифровые каналы */

  // ??????????????

  /* ---------------------------------- неадаптивное управление */
  struct
    {
    short int isBatch; /* пакетный режим (без предварительного просмотра) */
    unsigned Cycles;   /* число циклов предварительного эксперимента */
    struct {
      short int q;          /* задержка УСО */
      float Mistake;        /* допустимая погрешность по каналу */
      unsigned long Margin; /* начальный отсчет */
      unsigned long nr;     /* размер окна (0=max) */
      unsigned long nq;     /* число расчетных точек/окно (0=nr) */
      } Def[MAX_CHANNEL];
    } NMDL;

 /* координаты окна для индикации процесса работы */
 short int ScreenX,ScreenY;

 /* Пользовательский интерфейс */
 short int LastMode,LastField;

 char itEOF;
} CfgT;

#pragma pack(0)

#ifdef __cplusplus
 extern "C" {
#endif

extern CfgT Cfg;

int loadConfig(char *fn);
int saveConfig(char *fn);


#ifdef __cplusplus
 }
#endif

#endif