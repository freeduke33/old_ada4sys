
/*
   Application program inteface for UPI`s drivers

   History version of driver`s table:

     v1 -- base revision
     v2 -- first public release
     v3 -- added support for digital & mappable analog channel

 */


#ifndef __DAC__ADC__
#define __DAC__ADC__

/* Next line disable validate table version in UPI's driver */
// #define NOCHECK_TABLE_VER


#ifdef __cplusplus
  extern "C" {
#endif

typedef unsigned char      Byte;
typedef unsigned short int Word;
typedef unsigned long      DWord;

typedef void interrupt (far *HookPtr)(void);

typedef struct { Word Unit,Slot,Channel; } UnitUPI;

#define DRV_OK         0
#define DRV_FAILED    -1
#define DRV_NONE      -2
#define DRV_ERRARG    -3
#define DRV_ERRFUN    -4
#define DRV_NOSUPPORT -5  /* <<3>>  */


                /* Проверить присутствие драйвера ЦАП/АЦП в памяти */
int DrvIsPresent(void);
                /* Инициализировать драйвер ЦАП/АЦП */
int DrvInitialize(void);
                /* Деинициализировать драйвер ЦАП/АЦП */
int DrvDeinitialize(void);
                /* Взять строку авторских прав */
int DrvGetCopyright(char far *str);
                /* Взять строку названия драйвера */
int DrvGetName(char far *str);
                /* Взять версию драйвера */
int DrvGetVersion(short int far *ver);
                /* Дать описание последней ошибки драйвера */
int DrvGetErrorMsg(char far *str);

                /* Включить систему ОС */
int DrvEnable(float Freq);
int _DrvEnable(Word DivHi,Word DivLo);
                /* Выключить систему ОС */
int DrvDisable(void);
int _DrvDisable(void);
                /* Установить функцию слежения в ОС */
int DrvSetHook(HookPtr *HookFun);
                /* Снять функцию слежения с ОС */
int DrvClrHook(void);

/* ------------------------- Аналоговые каналы ----------------------- */
                /* Прочитать текущие значения на всех каналах АЦП */
int huge _DrvGetVolADC(Word far *ADCbuf);
int huge DrvGetVolADC(float far *ADCbuf);
                /* Установить начальные значения на всех каналах ЦАП */
int huge _DrvSetVolDAC(Word far *DACbuf);
int huge DrvSetVolDAC(float far *DACbuf);
                /* Прочитать текущие значения на всех каналах ЦАП */
int huge _DrvGetVolDAC(Word far *DACbuf);
int huge DrvGetVolDAC(float far *DACbuf);


                /* Передать на ЦАП / снять с АЦП новые значения */
int _DrvVolume(Word far *DACbuf,Word far *ADCbuf);
int DrvVolume(float far *DACbuf,float far *ADCbuf);
                /* Взять адреса областей хранения данных ЦАП/АЦП */
int _DrvGetBuf(Word far *far *DACbuf,Word far *far *ADCbuf);
int DrvGetBuf(float far *far *DACbuf,float far *far *ADCbuf);
                /* Установить новые значения ЦАП и прочитать АЦП */
int DrvWaitDAC_ADC(void);

                /* Установить число используемых каналов ЦАП/АЦП */
int _DrvSetChannel(Word NumDAC,Word NumADC);
int DrvSetChannel(Word NumDAC,Word NumADC);

                /* Обнулить число пропущенных тактов */
int DrvClrSkipped(void);
                /* Взять число пропущенных тактов */
int DrvGetSkipped(unsigned long far *val);

                /* Взять максимально возможную частоту */
int DrvGetMaxFreq(unsigned long far *val);
                /* Взять максимальное число каналов ЦАП/АЦП */
int DrvGetMaxChannel(Word far *NumDAC,Word far *NumADC);
                /* Взять мах возможный уровень ЦАП */
int DrvGetInfoDAC(Word far *NumDAC);
                /* Взять мах возможный уровень АЦП */
int DrvGetInfoADC(Word far *NumADC);


                /* <<3>> Установить отображение на физический ЦАП */
int DrvSetupDAC(Word Chl, UnitUPI *hdwDAC);
                /* <<3>> Установить отображение на физический АЦП */
int DrvSetupADC(Word Chl, UnitUPI *hdwADC);

/* ------------------------- Цифровые каналы ----------------------- */
                /* <<3>> прочитать значения с входных цифровых каналов */
int DrvGetDigIN(Word Chl,Byte far *INbuf);
                /* <<3>> установить значения на выходных цифровых каналах */
int DrvSetDigOUT(Word Chl,Byte OUTbuf);
                /* <<3>> прочитать текущие значения на выходных цифровых каналах */
int DrvGetDigOUT(Word Chl,Byte far *OUTbuf);
                /* <<3>> дать число цифровых каналов */
int DrvGetMaxDigital(Word far *NumIN,Word far *NumOUT);

                /* <<3>> Установить отображение на физические входные цифровые каналы */
int DrvSetupDigIN(Word Chl, UnitUPI *hdwIN);
                /* <<3>> Установить отображение на физические выходные цифровые каналы */
int DrvSetupDigOUT(Word Chl, UnitUPI *hdwOUT);


/* ==============[ internal ]================= */

#define DRV_IDstring "Stend driver for DAC/ADC"

typedef struct
{
 short int version;

 /* --- identification --- */
 char far *TableID;

 /* --- copyright,name --- */
 char far *Copyright;
 char far *NameDRV;

 /* --- initialize --- */
 int pascal (far *_Enable)(Word DivHi,Word DivLo);
 int pascal (far *_Disable)(void);
 int pascal (far *_GetVolADC)(Word far *ADCbuf);  /* current ADC vol */
 int pascal (far *_SetVolDAC)(Word far *DACbuf);  /* initialize DAC vol */

 /* --- usage --- */
 int pascal (far *_SetHook)(HookPtr HookFun);
 int pascal (far *_ClrHook)(void);
 int pascal (far *_Volume)(Word far *DACbuf,Word far *ADCbuf);
 int pascal (far *_GetBuf)(Word far *far *DACbuf,Word far *far *ADCbuf);
 int pascal (far *_WaitDAC_ADC)(void);

 /* --- channel --- */
 int pascal (far *_SetChannel)(Word numDAC,Word numADC);

 /* --- Skipped --- */
 int pascal (far *_ClrSkipped)(void);
 int pascal (far *_GetSkipped)(DWord far *val);

 /* --- Info --- */
 int pascal (far *_GetMaxFreq)(DWord far *val);
 int pascal (far *_GetMaxChannel)(Word far *dac,Word far *adc);
 int pascal (far *_GetInfoDAC)(Word far *dac);
 int pascal (far *_GetInfoADC)(Word far *adc);

 /* ============================================ Added from version 3.0 === */

 /* --- Setup analog part --- */
 int pascal (far *_DrvSetupDAC)(Word Chl, UnitUPI *hdwDAC);
 int pascal (far *_DrvSetupADC)(Word Chl, UnitUPI *hdwADC);

 /* --- usage digital --- */
 int pascal (far *_DrvGetDigIN)(Word Chl,Word far *INbuf);
 int pascal (far *_DrvSetDigOUT)(Word Chl,Word OUTbuf);
 int pascal (far *_DrvGetDigOUT)(Word Chl,Word far *OUTbuf);

 /* --- digital info --- */
 int pascal (far *_DrvGetMaxDigital)(Word far *NumIN,Word far *NumOUT);

 /* --- Setup digital part --- */
 int pascal (far *_DrvSetupDigIN)(Word Cnt, UnitUPI *hdwIN);
 int pascal (far *_DrvSetupDigOUT)(Word Cnt, UnitUPI *hdwOUT);

} DRV_Table_T,far *DRV_Table_P,far *far *DRV_Table_I;


extern DRV_Table_T __FuncTable__;


int DrvGetFuncTable(DRV_Table_I tbl);


#ifdef __cplusplus
  }
#endif


/* ==============[ macroses ]================= */

#ifdef NOCHECK_TABLE_VER
# define DoCheckTableVer(a,b) b
#else
# define DoCheckTableVer(a,b) ((__FuncTable__.version>=a) ? b : DRV_NOSUPPORT)
#endif


#ifndef LOOPBACK

#define _DrvEnable(a,b)        __FuncTable__._Enable(a,b)
#define _DrvDisable()          __FuncTable__._Disable()
#define _DrvGetVolADC(a)       __FuncTable__._GetVolADC(a)
#define _DrvSetVolDAC(a)       __FuncTable__._SetVolDAC(a)

#define DrvSetHook(a)          __FuncTable__._SetHook(a)
#define DrvClrHook()           __FuncTable__._ClrHook()
#define _DrvVolume(a,b)        __FuncTable__._Volume(a,b)

#define _DrvGetBuf(a,b)        __FuncTable__._GetBuf(a,b)
#define DrvWaitDAC_ADC()       __FuncTable__._WaitDAC_ADC()

#define _DrvSetChannel(a,b)    __FuncTable__._SetChannel(a,b)

#define DrvClrSkipped()        __FuncTable__._ClrSkipped()
#define DrvGetSkipped(a)       __FuncTable__._GetSkipped(a)

#define DrvGetMaxFreq(a)       __FuncTable__._GetMaxFreq(a)
#define DrvGetMaxChannel(a,b)  __FuncTable__._GetMaxChannel(a,b)
#define DrvGetInfoDAC(a)       __FuncTable__._GetInfoDAC(a)
#define DrvGetInfoADC(a)       __FuncTable__._GetInfoADC(a)

/* v3.0 */
#define DrvSetupDAC(a,b)       DoCheckTableVer(3,__FuncTable__._DrvSetupDAC(a,b))
#define DrvSetupADC(a,b)       DoCheckTableVer(3,__FuncTable__._DrvSetupADC(a,b))

#define DrvGetDigIN(a,b)       DoCheckTableVer(3,__FuncTable__._DrvGetDigIN(a,b))
#define DrvSetDigOUT(a,b)      DoCheckTableVer(3,__FuncTable__._DrvSetDigOUT(a,b))
#define DrvGetDigOUT(a,b)      DoCheckTableVer(3,__FuncTable__._DrvGetDigOUT(a,b))

#define DrvGetMaxDigital(a,b)  DoCheckTableVer(3,__FuncTable__._DrvGetMaxDigital(a,b))

#define DrvSetupDigIN(a,b)     DoCheckTableVer(3,__FuncTable__._DrvSetupDigIN(a,b))
#define DrvSetupDigOUT(a,b)    DoCheckTableVer(3,__FuncTable__._DrvSetupDigOUT(a,b))

#else

#define _DrvEnable(a,b)        ((1) ? DRV_OK : DRV_OK)
#define _DrvDisable()          ((1) ? DRV_OK : DRV_OK)
#define _DrvGetVolADC(a)       ((1) ? DRV_OK : DRV_OK)
#define _DrvSetVolDAC(a)       ((1) ? DRV_OK : DRV_OK)

#define DrvSetHook(a)          ((1) ? DRV_OK : DRV_OK)
#define DrvClrHook()           ((1) ? DRV_OK : DRV_OK)
#define _DrvVolume(a,b)        ((1) ? DRV_OK : DRV_OK)

#define _DrvGetBuf(a,b)        ((1) ? DRV_OK : DRV_OK)
#define DrvWaitDAC_ADC()       ((1) ? DRV_OK : DRV_OK)

#define _DrvSetChannel(a,b)    ((1) ? DRV_OK : DRV_OK)

#define DrvClrSkipped()        ((1) ? DRV_OK : DRV_OK)
#define DrvGetSkipped(a)       ((1) ? DRV_OK : DRV_OK)

#define DrvGetMaxFreq(a)       ((1) ? DRV_OK : DRV_OK)
#define DrvGetMaxChannel(a,b)  ((1) ? DRV_OK : DRV_OK)
#define DrvGetInfoDAC(a)       ((1) ? DRV_OK : DRV_OK)
#define DrvGetInfoADC(a)       ((1) ? DRV_OK : DRV_OK)

/* v3.0 */
#define DrvSetupDAC(a,b)       ((1) ? DRV_OK : DRV_NOSUPPORT)
#define DrvSetupADC(a,b)       ((1) ? DRV_OK : DRV_NOSUPPORT)
#define DrvGetDigIN(a,b)       ((1) ? DRV_OK : DRV_NOSUPPORT)
#define DrvSetDigOUT(a,b)      ((1) ? DRV_OK : DRV_NOSUPPORT)
#define DrvGetDigOUT(a,b)      ((1) ? DRV_OK : DRV_NOSUPPORT)

#define DrvGetMaxDigital(a,b)  ((1) ? DRV_OK : DRV_NOSUPPORT)

#define DrvSetupDigIN(a,b)     ((1) ? DRV_OK : DRV_NOSUPPORT)
#define DrvSetupDigOUT(a,b)    ((1) ? DRV_OK : DRV_NOSUPPORT)

#endif

#endif
