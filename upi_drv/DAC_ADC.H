
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

#include "compiler.h"
#include "typedef.h"

typedef void _INTER (*HookPtr)(void);

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
int DrvGetCopyright(char _FAR *str);
                /* Взять строку названия драйвера */
int DrvGetName(char _FAR *str);
                /* Взять версию драйвера */
int DrvGetVersion(short int _FAR *ver);
                /* Дать описание последней ошибки драйвера */
int DrvGetErrorMsg(char _FAR *str);

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
int _DrvGetVolADC(Word _FAR *ADCbuf);
int DrvGetVolADC(float _FAR *ADCbuf);
                /* Установить начальные значения на всех каналах ЦАП */
int _DrvSetVolDAC(Word _FAR *DACbuf);
int DrvSetVolDAC(float _FAR *DACbuf);
                /* Прочитать текущие значения на всех каналах ЦАП */
int _DrvGetVolDAC(Word _FAR *DACbuf);
int DrvGetVolDAC(float _FAR *DACbuf);


                /* Передать на ЦАП / снять с АЦП новые значения */
int _DrvVolume(Word _FAR *DACbuf,Word _FAR *ADCbuf);
int DrvVolume(float _FAR *DACbuf,float _FAR *ADCbuf);
                /* Взять адреса областей хранения данных ЦАП/АЦП */
int _DrvGetBuf(Word _FAR *_FAR *DACbuf,Word _FAR *_FAR *ADCbuf);
int DrvGetBuf(float _FAR *_FAR *DACbuf,float _FAR *_FAR *ADCbuf);
                /* Установить новые значения ЦАП и прочитать АЦП */
int DrvWaitDAC_ADC(void);

                /* Установить число используемых каналов ЦАП/АЦП */
int _DrvSetChannel(Word NumDAC,Word NumADC);
int DrvSetChannel(Word NumDAC,Word NumADC);

                /* Обнулить число пропущенных тактов */
int DrvClrSkipped(void);
                /* Взять число пропущенных тактов */
int DrvGetSkipped(unsigned long _FAR *val);

                /* Взять максимально возможную частоту */
int DrvGetMaxFreq(unsigned long _FAR *val);
                /* Взять максимальное число каналов ЦАП/АЦП */
int DrvGetMaxChannel(Word _FAR *NumDAC,Word _FAR *NumADC);
                /* Взять мах возможный уровень ЦАП */
int DrvGetInfoDAC(Word _FAR *NumDAC);
                /* Взять мах возможный уровень АЦП */
int DrvGetInfoADC(Word _FAR *NumADC);


                /* <<3>> Установить отображение на физический ЦАП */
int DrvSetupDAC(Word Chl, UnitUPI *hdwDAC);
                /* <<3>> Установить отображение на физический АЦП */
int DrvSetupADC(Word Chl, UnitUPI *hdwADC);

/* ------------------------- Цифровые каналы ----------------------- */
                /* <<3>> прочитать значения с входных цифровых каналов */
int DrvGetDigIN(Word Chl,Byte _FAR *INbuf);
                /* <<3>> установить значения на выходных цифровых каналах */
int DrvSetDigOUT(Word Chl,Byte OUTbuf);
                /* <<3>> прочитать текущие значения на выходных цифровых каналах */
int DrvGetDigOUT(Word Chl,Byte _FAR *OUTbuf);
                /* <<3>> дать число цифровых каналов */
int DrvGetMaxDigital(Word _FAR *NumIN,Word _FAR *NumOUT);

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
 char _FAR *TableID;

 /* --- copyright,name --- */
 char _FAR *Copyright;
 char _FAR *NameDRV;

 /* --- initialize --- */
 int _PASCAL (_FAR *_Enable)(Word DivHi,Word DivLo);
 int _PASCAL (_FAR *_Disable)(void);
 int _PASCAL (_FAR *_GetVolADC)(Word _FAR *ADCbuf);  /* current ADC vol */
 int _PASCAL (_FAR *_SetVolDAC)(Word _FAR *DACbuf);  /* initialize DAC vol */

 /* --- usage --- */
 int _PASCAL (_FAR *_SetHook)(HookPtr HookFun);
 int _PASCAL (_FAR *_ClrHook)(void);
 int _PASCAL (_FAR *_Volume)(Word _FAR *DACbuf,Word _FAR *ADCbuf);
 int _PASCAL (_FAR *_GetBuf)(Word _FAR *_FAR *DACbuf,Word _FAR *_FAR *ADCbuf);
 int _PASCAL (_FAR *_WaitDAC_ADC)(void);

 /* --- channel --- */
 int _PASCAL (_FAR *_SetChannel)(Word numDAC,Word numADC);

 /* --- Skipped --- */
 int _PASCAL (_FAR *_ClrSkipped)(void);
 int _PASCAL (_FAR *_GetSkipped)(DWord _FAR *val);

 /* --- Info --- */
 int _PASCAL (_FAR *_GetMaxFreq)(DWord _FAR *val);
 int _PASCAL (_FAR *_GetMaxChannel)(Word _FAR *dac,Word _FAR *adc);
 int _PASCAL (_FAR *_GetInfoDAC)(Word _FAR *dac);
 int _PASCAL (_FAR *_GetInfoADC)(Word _FAR *adc);

 /* ============================================ Added from version 3.0 === */

 /* --- Setup analog part --- */
 int _PASCAL (_FAR *_DrvSetupDAC)(Word Chl, UnitUPI *hdwDAC);
 int _PASCAL (_FAR *_DrvSetupADC)(Word Chl, UnitUPI *hdwADC);

 /* --- usage digital --- */
 int _PASCAL (_FAR *_DrvGetDigIN)(Word Chl,Word _FAR *INbuf);
 int _PASCAL (_FAR *_DrvSetDigOUT)(Word Chl,Word OUTbuf);
 int _PASCAL (_FAR *_DrvGetDigOUT)(Word Chl,Word _FAR *OUTbuf);

 /* --- digital info --- */
 int _PASCAL (_FAR *_DrvGetMaxDigital)(Word _FAR *NumIN,Word _FAR *NumOUT);

 /* --- Setup digital part --- */
 int _PASCAL (_FAR *_DrvSetupDigIN)(Word Cnt, UnitUPI *hdwIN);
 int _PASCAL (_FAR *_DrvSetupDigOUT)(Word Cnt, UnitUPI *hdwOUT);

} DRV_Table_T,_FAR *DRV_Table_P,_FAR *_FAR *DRV_Table_I;


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
