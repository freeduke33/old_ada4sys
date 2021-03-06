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
 /* �᫮ ������� �ࠢ�����, ���������� ���ଠ樮���� */
// short int nk,nkADC;
 6,6,
 /* ��஢� ������ */
// short int nkIN,nkOUT;
 0,0,
 /* ���� ����⨧�樨 ����������� ᨣ���� */
// float Freq;
 256.0,
 /* ���⥫쭮��� ࠡ��� */
// unsigned long nr,nr_view,cycle;
 1000,1000,1,
 /* ��室 �� ��砫��� ��� ��। ����᪮� */
// short int iniTime;
 5,
 /* ����⢨� �� �ॢ�襭�� �����⨬�� ����譮�� */
// short int AftErr;
 0,
 /* ��⮪�� ࠡ��� */
// short int LogLevel;
 2,
// unsigned long LogTime,LogSize; char DebugLevel[35];
 1,0,"",

 /* �⠫�� / ���� */
// float ifile_k,ifile_st;
 1,1,
// char  ifile_name[20],ufile_name[20];
 "NONAME.TEM","NONAME.UPR",

 /* ��⠫�� ��娢� */
// char ArchivePath[80];
 ".\\",
 /* ��⠫�� ���⮢ */
// char ReportPath[80];
 ".\\",
 /* ��⠫�� �⠫����� ᨣ����� */
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
    /* ������ �ࠢ����� */
//    short int mdlCod; short int mdlType,p,q,Q; -----------------1
    /* �७� �ࠢ����� */
//    short int trCod; short int trP;-----------------------------2
    /* ����譮��� �ࠢ����� */
//    short int mstCod; float mstMax,mstTime,mstMargin;-----------3
    /* �⠫���� ᨣ��� */
//    short int tplCod; shortint isEMS;---------------------------4
    /* ����� �� 䠩�� */
        // char file_name[20]; float file_k,file_st,file_chl;-----5
    /* ������ମ����   */
        // float u,a[3],f[3],fa[3];-------------------------------6
    /* 㤠����� �७�� �� ᨣ���� �ࠢ����� */
//    short int delTr;--------------------------------------------7
//    unsigned short delTr_size; float delTr_vol,delTr_speed;
    /* ��࠭�祭�� �ࠢ���饣� ᨣ���� */
//    float MaxStatic,MinStatic,MaxSpeed,MaxAcceleration;---------8
    /* ���室�� ����� */
//    short int begCod; float begFreq,begAmpl,begZoom;------------9
    /* �஢��� �����, ���⮢� �஢��� �� ����᪥ �ࠢ����� */
//    float PassiveVol,StartVol;----------------------------------10
    /* 䨧��᪨� ����稭� */
//    struct { ---------------------------------------------------11
//      short int T;   /* ⨯ ������⢮������ ���稪� */
//      float A,D;     /* �����樥��� ������ = A + (D * ������) */
//    } usrDAC,usrADC; /* ���稪 �ࠢ����� � ���稪 �⪫��� */
  },

  /* ---------------------------------- �ࠪ���⨪� ������� �ࠢ����� */
//  char afile_name[20];
  "NONAME.ACH",
//  struct { /* ��� */
//      short int Mode;
//      short int Periods,Nq;
//      float Ampl,FreqMin,FreqMax,Const;
//      } Ach[MAX_CHANNEL];
  {{0,10,50,1,0.5,30,0},{0,10,50,1,0.5,30,0},{0,10,50,1,0.5,30,0},{0,10,50,1,0.5,30,0}},

//  char sfile_name[20];
  "NONAME.STH",
//  struct { /* ����᪠� */
//      short int Mode
//      short int statNq;
//      float Wait,Lo,Hi,HiSpeed,HiAcceleration;
//      } Sth[MAX_CHANNEL];
  {{0,50,2,-5,5,50,3},{0,50,2,-5,5,50,3},{0,50,2,-5,5,50,3},{0,50,2,-5,5,50,3}},

//  char wfile_name[20];
  "NONAME.WZS",
//  struct { /* ����������⢨� � ��⨪� */
//      short int Nq;
//      float Lo,Hi;
//      } Wzst[MAX_CHANNEL];
  {{50,-5,5},{50,-5,5},{50,-5,5},{50,-5,5}},


  { /* ---------------------------------- ���ଠ樮��� ������ */
   {0,1, {0,0},{0},{{0,0,-0.5,0.5},{0,1,-1,1},{1,1,-4,4}},{0}},
   {0,0, {1,1},{0},{{0,0,-0.5,0.5},{0,1,-1,1},{1,1,-4,4}},{0}},
   {0,1, {2,2},{0},{{0,0,-0.5,0.5},{0,1,-1,1},{1,1,-4,4}},{0}},
   {0,0, {3,3},{0},{{0,0,-0.5,0.5},{0,1,-1,1},{1,1,-4,4}},{0}},
   {0,1, {4,4},{0},{{0,0,-0.5,0.5},{0,1,-1,1},{1,1,-4,4}},{0}},
   {0,1, {5,5},{0},{{0,0,-0.5,0.5},{0,1,-1,1},{1,1,-4,4}},{0}}

//  struct {
//    short int isActive;
    /* ०�� ࠡ��� */
//    short int infCod; /* 0-���祭�� 1-����஫� �࠭�� 2-���� ��-�� */
    /* ����� ������� */
//    short int Chl[2];
//       struct {  /* =========== ���祭�� */
         /* ����஫���� ���ᨢ */
//         unsigned long size; /* 0-�।��� 1-⥪�饥 2..N-����饥 �।��� */
//         } Average;
//       struct {  /* =========== ����஫� �࠭�� */
//         struct {
           /* ॠ��� */
//           short int outCod;      /* 0-�������� 1-�⪫�祭�� 2-��१���� 3-�ਮ�⠭���� */
           /* ��㪮��� ᮯ஢������� */
//           short int isMute;
           /* �࠭��� ��������� */
//           float MinVol,MaxVol;
//           } Normal,Warning,Crach;
//         } Defence;
//       struct {  /* =========== �ࠪ���⨪� */
         /* ��� �ࠪ���⨪� */
//         short int exmCod; /* 0-???? */
//         } Examine;
//      } Inform[MAX_infoCHANNEL];
   },

  /* ---------------------------------- ������⨢��� �ࠢ����� */
//  struct
  {
//    {
//    int isBatch;     /* ������ ०�� (��� �।���⥫쭮�� ��ᬮ��) */
     1,
//    unsigned Cycles; /* �᫮ 横��� �।���⥫쭮�� ��ᯥਬ��� */
     5,
//    struct {
//0     int q;                /* ����প� ��� */
//1     float Mistake;        /* �����⨬�� ����譮��� �� ������ */
//2     unsigned long Margin; /* ��砫�� ����� */
//3     unsigned long nr;     /* ࠧ��� ���� (0=max) */
//4     unsigned long nq;     /* �᫮ ������ �祪/���� (0=nr) */
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

 /* ���न���� ���� ��� ������樨 ����� ࠡ��� */
// short int ScreenX,ScreenY;
  47,10,

 /* ���짮��⥫�᪨� ����䥩� */
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
     else ret=CFG_CORRUPT; /* 䠩� ���०��� */
     }
   else ret=CFG_FORMAT; /* ������ �ଠ� ��� �� 䠩� ���䨣��樨 */
   close(f);
   }
 else if(name) ret=CFG_RDOPEN; /* ��� 䠩�� ��� ���������� ������ */

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
      write(f,&Cfg,sizeof(Cfg))!=sizeof(Cfg)) ret=CFG_WRITE; /* ���������� ������� */
   close(f);
   }
 else ret=CFG_WROPEN; /* ���������� �������ᮧ���� */

 return ret;
}



