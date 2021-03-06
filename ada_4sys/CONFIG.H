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
 /* �᫮ ������� �ࠢ�����, ���������� ���ଠ樮���� */
 short int nk,nkInfo;
 /* ��஢� ������ */
 short int nkIN,nkOUT;

 /* ���� ����⨧�樨 ����������� ᨣ���� */
 float Freq;
 /* ���⥫쭮��� ࠡ��� */
 unsigned long nr,nr_view,cycle;
 /* �६� ��室� �� ��砫��� ��� ��। ����᪮� */
 float iniTime;
 /* ����⢨� �� �ॢ�襭�� �����⨬�� ����譮�� */
 short int AftErr; /* 0-�⪫�祭�� 1-��१���� 2-�ਮ�⠭���� */

 /* ��⮪�� ࠡ��� */
 short int LogLevel; /* 0-��������� 1-��ଠ��� 2-��⠫�� 3-�⫠��� */
 unsigned long LogTime,LogSize; char DebugLevel[35];

 /* �⠫�� / ���� */
 float ifile_k,ifile_st;
 char  ifile_name[MAX_FILENAME],ufile_name[MAX_FILENAME];

 /* ��⠫�� ��娢� */
 char ArchivePath[MAX_DIRNAME];
 /* ��⠫�� ���⮢ */
 char ReportPath[MAX_DIRNAME];
 /* ��⠫�� �⠫����� ᨣ����� */
 char TemplatePath[MAX_DIRNAME];

 struct {  /* ------------------ ���ᠭ�� ��� ������ �ࠢ����� */
    short int isActive;
    /* ������ �ࠢ����� */
    short int mdlCod; /* �. DefCodModel */
    short int mdlType,p,q,Q; /* 0-��६�襭�� 1-᪮���� 2-�᪮७�� */
    /* �७� �ࠢ����� */
    short int trCod; short int trP; /* 0-���஥��� 1-ࠧ����� 2-������������ */
    /* ����譮��� �ࠢ����� */
    short int mstCod; /* 0-�������᪠� 1-���.�������� 2-������ ����� */
    float mstMax,mstTime,mstMargin;

    /* �⠫���� ᨣ��� */
    short int tplCod; /* 0-��� 1-������ମ���� 2-�� 䠩�� */
    short int isEMS; /* �࠭���� �⠫��� � EMS 0-��� 1-�� */
    /* ����� �� 䠩�� */ char file_name[MAX_FILENAME]; float file_k,file_st; short int file_chl;
    /* ������ମ����   */ float u; float a[3],f[3],fa[3];

    /* 㤠����� �७�� �� ᨣ���� �ࠢ����� */
    short int delTr;
    unsigned short delTr_size; float delTr_vol,delTr_speed; /* size==0 - �⪫�祭� */

    /* ��࠭�祭�� �ࠢ���饣� ᨣ���� */
    float MaxStatic,MinStatic,MaxSpeed,MaxAcceleration;

    /* ���室�� ����� */
    short int begCod; /* 0-ᨭ�� 1-�⠫�� */
    float begFreq,begAmpl,begZoom;

    /* �஢��� �����, ���⮢� �஢��� �� ����᪥ �ࠢ����� */
    float PassiveVol,StartVol;

    /* 䨧��᪨� ����稭� */
    struct {
      short int T;   /* ⨯ ������⢮������ ���稪� (䨧��᪠� ����稭�) */
      float A,D;     /* �����樥��� ������ = A + (D * ������) */
    } usrDAC,usrADC; /* ���稪 �ࠢ����� � ���稪 �⪫��� */

    } Def[MAX_CHANNEL];



  /* ---------------------------------- �ࠪ���⨪� ������� �ࠢ����� */
  char afile_name[20];
  struct { /* ����� �ࠪ���⨪� */
      short int Mode;  /* 0-��� 1-��� */
      short int Periods,Nq;
      float Ampl,FreqMin,FreqMax,Const;
      } Ach[MAX_CHANNEL];

  char sfile_name[20];
  struct { /* ����᪠� */
      short int Mode; /* 0-��६�饭�� 1-᪮���� 2-�᪮७�� */
      short int Nq;
      float Wait,Lo,Hi,HiSpeed,HiAcceleration;
      } Sth[MAX_CHANNEL];

  char wfile_name[20];
  struct { /* ����������⢨� � ��⨪� */
      short int Nq;
      float Lo,Hi;
      } Wzst[MAX_CHANNEL];

  /* ---------------------------------- ���ଠ樮��� ������ */
  struct {
    short int isActive;
    /* ०�� ࠡ��� */
    short int infCod; /* 0-���祭�� 1-����஫� �࠭�� 2-���� ��-�� */
    /* �����᪨� ����� ������� */
    short int Chl[2];
     struct {  /* =========== ���祭�� */
       /* ����஫���� ���ᨢ */
       unsigned long size; /* 0-�।��� 1-⥪�饥 2..N-����饥 �।��� */
       } Average;
     struct {  /* =========== ����஫� �࠭�� */
       struct {
         /* ॠ��� �� ��室 �� �࠭��� */
         short int outCod;      /* 0-�⪫�祭�� 1-��१���� 2-�ਮ�⠭���� */
         /* ��㪮��� ᮯ஢������� */
         short int isMute;
         /* �࠭��� ��������� */
         float MinVol,MaxVol;
         } Normal,Warning,Crash;
       } Defence;
     struct {  /* =========== �ࠪ���⨪� */
       /* ��� �ࠪ���⨪� */
       short int exmCod; /* 0-???? */
       } Examine;
     } Inform[MAX_infoCHANNEL];

  /* ---------------------------------- ��஢� ������ */

  // ??????????????

  /* ---------------------------------- ������⨢��� �ࠢ����� */
  struct
    {
    short int isBatch; /* ������ ०�� (��� �।���⥫쭮�� ��ᬮ��) */
    unsigned Cycles;   /* �᫮ 横��� �।���⥫쭮�� ��ᯥਬ��� */
    struct {
      short int q;          /* ����প� ��� */
      float Mistake;        /* �����⨬�� ����譮��� �� ������ */
      unsigned long Margin; /* ��砫�� ����� */
      unsigned long nr;     /* ࠧ��� ���� (0=max) */
      unsigned long nq;     /* �᫮ ������ �祪/���� (0=nr) */
      } Def[MAX_CHANNEL];
    } NMDL;

 /* ���न���� ���� ��� ������樨 ����� ࠡ��� */
 short int ScreenX,ScreenY;

 /* ���짮��⥫�᪨� ����䥩� */
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