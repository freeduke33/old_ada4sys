#ifndef __UPI_3W_H__
#define __UPI_3W_H__

#ifdef __cplusplus
  extern "C" {
#endif

#define upiVERSION "v1.7"

#define Channel  10
#define MAX_WAIT 10

struct _FuncModel
{
        int Type;
        float k;   /* ��� ����� , �����樥�� */
};

struct _AperModel
{
        float FV,FN;  /* ������, ������ �࠭�� */
        int p;        /* ���冷� */
};

struct _KolbModel
{
        float Betta,Freq;  /* �⥯��� �ᯮ������, १����᭠� ���� */
};

struct _MassModel
{
        float Betta[2],Freq[2];
};

struct _param_3w
{
 char ID[10];

 struct _FuncModel Fm[Channel][3];
 struct _AperModel Am[Channel][3];
 struct _KolbModel Km[Channel][3];
 struct _MassModel Mm[Channel][3];

 float  fdds[Channel],imi_Const[Channel];
 int    wait[Channel];
 int    Z_Type[Channel][4];
 float  wzk[Channel][Channel];
 float  xAmpl[Channel][3],xFreq[Channel][3],xFaza[Channel][3];
};

#ifdef __cplusplus
  }
#endif

/* ��� ���䨣���� */
extern struct _param_3w def;

#endif
