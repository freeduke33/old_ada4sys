#ifndef __COMMON_H__
#define __COMMON_H__

#define ERR_HARDWARE    1 // ��९������� �⥪�, �訡�� ������饩 ��䬥⨪� � �.�.
#define ERR_START       2 // ����।����� ������ ���室���� �����
#define ERR_MISTAKE     3 // ����।���� ������ ���� ����譮��
#define ERR_MODEL       4 // ����।����� ������ �ࠢ�����
#define ERR_TREND       5 // ����।����� ������ ���� �७��
#define ERR_ETALON      6 // ����।������ �⠫��
#define ERR_TEMFILE     7 // ������ 䠩� �⠫��� *.TEM
#define ERR_CHANNEL     8 // ����୮� �᫮ �������
#define ERR_TEMLENGTH   9 // ����ୠ� ������ �鸞
#define ERR_BUILD      10 // �訡�� �� ����᪥ ��ᯥਬ���
#define ERR_LOOP       11 // �஡���� � ����� ��ᯥਬ���
#define ERR_USERBREAK  12 // ��ᯥਬ��� ��ࢠ� �����஬
#define ERR_MODE       13 // ����� ������ ०�� ࠡ���
#define ERR_NOCFGFILE  14 // ��� 䠩�� ���䨣��樨
#define ERR_CFG        15 // ������ 䠩� ���䨣��樨
#define ERR_UPIDRV     16 // �ࠩ��� ��� �� ������/�訡�� ���
#define ERR_CMDLINE    17 // ������ �ଠ� ��ࠬ��஢ ����᪠
#define ERR_NOMEM      18 // �������筮 ᢮������ ����� ��� ࠡ���
#define ERR_FREQ       19 // ����ୠ� ���� ����⨧�樨 � ��ᯥਬ���


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

// ��࠭��� ������ �����
void tpreserv(void);
// ����⠭����� ������ �����
void trestore(void);
// �뢥�� ��ப�
void tprintf(unsigned x,unsigned y,unsigned len,unsigned col,char *fmt, ...);
// �뢥�� ���� ��࠭�
void tprintDump(unsigned width,unsigned height,dumpType buf[]);

unsigned long DosMemoryAvail(void);


char *txtClock(unsigned long tik);
char *txtTime(char *s);

#ifdef __cplusplus
 }
#endif

//                              x,y,length,color
// ������������ ���⠭��
#define WORKING_TITLE           23,3,51,0x20
// ������������ ����㧪�
#define WORKING_ETALON          23,4,51,0x20
// ����⭮� �६� ࠡ���
#define WORKING_ALL_TIME        22,5,16,0x7E
// ��⠫��� �६��� ࠡ����
#define WORKING_LEFT_TIME       59,5,16,0x7E
// �⠯ ࠡ���
#define WORKING_PART            23,7,47,0x20
// ����⭮� �६� �믮������ �⠯�
#define WORKING_PART_TIME       59,8,16,0x20
// ��業� �믮������ �⠯�
#define WORKING_PART_PERSENT    72,7,3,0x7F
// ����� �믮������ ࠡ���
#define WORKING_PART_STATUS     3,9,73,0xCF

// ���ᠭ�� ���ଠ樮���� �������
#define WORKING_INFO_DSC(a)     41,12+(a),25,0x71
#define WORKING_INFO_VAL(a)     66,12+(a),10,0x20
#define WORKING_INFO_WARNING(a) 66,12+(a),10,0x40
#define WORKING_INFO_CRASH(a)   66,12+(a),10,0x4F
// ���ᠭ�� ������� ����譮�� �ࠢ�����
#define WORKING_MISTAKE_DSC(a)  3,12+(a),26,0x71
#define WORKING_MISTAKE_VAL(a)      29,12+(a),9,0x20
#define WORKING_MISTAKE_WARNING(a)  29,12+(a),9,0x40
#define WORKING_MISTAKE_CRASH(a)    29,12+(a),9,0x4F

// ��ப� ���᪠���
#define WORKING_HOTLINE         2,23,76,0x30
#define HOTLINE_DEFSTR          "   \027\x3F<Space>\027\xFF - ��㧠,  \027\x3F<Esc>\027\xFF - ��⠭�� ���⠭��"

#endif


