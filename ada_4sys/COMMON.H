#ifndef __COMMON_H__
#define __COMMON_H__

#define ERR_HARDWARE    1 // переполнение стека, ошибка плавающей арифметики и т.д.
#define ERR_START       2 // неопределена модель переходного процесса
#define ERR_MISTAKE     3 // неопределен алгоритм расчета погрешности
#define ERR_MODEL       4 // неопределена модель управления
#define ERR_TREND       5 // неопределена модель расчета тренда
#define ERR_ETALON      6 // неопределенный эталон
#define ERR_TEMFILE     7 // неверный файл эталона *.TEM
#define ERR_CHANNEL     8 // неверное число каналов
#define ERR_TEMLENGTH   9 // неверная длинна ряда
#define ERR_BUILD      10 // ошибка при запуске эксперимента
#define ERR_LOOP       11 // проблемы в процессе эксперимента
#define ERR_USERBREAK  12 // эксперимент прерван оператором
#define ERR_MODE       13 // задан неверный режим работы
#define ERR_NOCFGFILE  14 // нет файла конфигурации
#define ERR_CFG        15 // неверный файл конфигурации
#define ERR_UPIDRV     16 // драйвер УСО не найден/ошибка УСО
#define ERR_CMDLINE    17 // неверный формат параметров запуска
#define ERR_NOMEM      18 // недостаточно свободной памяти для работы
#define ERR_FREQ       19 // неверная частота дискретизации в эксперименте


#ifdef __cplusplus
 extern "C" {
#endif

int InitLog(void);
int DoneLog(void);

int BufWrite(int f,void *buf,int size);
int BufRead(int f,void *buf,int size);
int BufInit(void);
int BufFlush(int f);

typedef struct { unsigned char Symbol,Attr; } dumpType;

// сохранить позицию курсора
void tpreserv(void);
// восстановить позицию курсора
void trestore(void);
// вывести строку
void tprintf(unsigned x,unsigned y,unsigned len,unsigned col,char *fmt, ...);
// вывести дамп экрана
void tprintDump(unsigned width,unsigned height,dumpType buf[]);

unsigned long DosMemoryAvail(void);


char *txtClock(unsigned long tik);
char *txtTime(char *s);

#ifdef __cplusplus
 }
#endif

//                              x,y,length,color
// Наименование испытаний
#define WORKING_TITLE           23,3,51,0x20
// Наименование нагрузки
#define WORKING_ETALON          23,4,51,0x20
// Расчетное время работы
#define WORKING_ALL_TIME        22,5,16,0x7E
// Осталось времени работать
#define WORKING_LEFT_TIME       59,5,16,0x7E
// Этап работы
#define WORKING_PART            23,7,47,0x20
// Расчетное время выполнения этапа
#define WORKING_PART_TIME       59,8,16,0x20
// Процент выполнения этапа
#define WORKING_PART_PERSENT    72,7,3,0x7F
// статус выполнения работы
#define WORKING_PART_STATUS     3,9,73,0xCF

// Описание информационных каналов
#define WORKING_INFO_DSC(a)     41,12+(a),25,0x71
#define WORKING_INFO_VAL(a)     66,12+(a),10,0x20
#define WORKING_INFO_WARNING(a) 66,12+(a),10,0x40
#define WORKING_INFO_CRASH(a)   66,12+(a),10,0x4F
// Описание каналов погрешности управления
#define WORKING_MISTAKE_DSC(a)  3,12+(a),26,0x71
#define WORKING_MISTAKE_VAL(a)      29,12+(a),9,0x20
#define WORKING_MISTAKE_WARNING(a)  29,12+(a),9,0x40
#define WORKING_MISTAKE_CRASH(a)    29,12+(a),9,0x4F

// Строка подсказки
#define WORKING_HOTLINE         2,23,76,0x30
#define HOTLINE_DEFSTR          "   \027\x3F<Space>\027\xFF - Пауза,  \027\x3F<Esc>\027\xFF - Останов испытаний"

#endif


