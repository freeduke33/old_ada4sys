// #define TEST


#ifndef __UPI_3W_H__
#define __UPI_3W_H__

#ifdef __cplusplus
  extern "C" {
#endif

#define upiVERSION "v1.7"

#define Channel  10
#define MAX_WAIT 10

#pragma pack(1)

struct _FuncModel
{
        short int Type;
        float k;   /* Тип звена , коэффициент */
};

struct _AperModel
{
        float FV,FN;  /* Верхняя, нижняя граница */
        short int p;        /* Порядок */
};

struct _KolbModel
{
        float Betta,Freq;  /* Степень успокоения, резонансная частота */
};

struct _MassModel
{
  float Betta[2],Freq[2];
};

struct _TestModel
{
 short int p;
 float E;
 double C[11],Lam[11];
};

struct _ArssModel
{
 short int p,q;
 double C[11],Lam[11];
};

struct _param_3w
{
 char ID[10];

 struct _FuncModel Fm[Channel][3];
 struct _AperModel Am[Channel][3];
 struct _KolbModel Km[Channel][3];
 struct _MassModel Mm[Channel][3];
 struct _TestModel Tm[Channel][3];
 struct _ArssModel Sm[Channel][3];

 float  fdds[Channel],imi_Const[Channel];
 short int    wait[Channel];
 short int    Z_Type[Channel][4];
 float  wzk[Channel][Channel];
 float  xAmpl[Channel][3],xFreq[Channel][3],xFaza[Channel][3];
};

#ifdef __cplusplus
  }
#endif

/* для конфигуратора */
extern struct _param_3w def;
extern int Z_curr,Chl;

#endif



