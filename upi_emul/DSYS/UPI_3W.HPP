#ifndef __UPI_3W_HPP__
#define __UPI_3W_HPP__

#include "upi.hpp"
#include "upi_3w.h"


class UPI_3w:public UPI
{
  public:
    UPI_3w(char *filename,int ctrl);

    UPI::DoLoop;
    UPI::ErrMsg;

    int huge BuildUPI(void);
    int huge CalcSignal(float *iBuf,float *oBuf);

  private:

    void ShowConfig();

    float fdi[Channel],fdds[Channel];
    struct _FuncModel FuncModel[Channel][3];
    struct _AperModel AperModel[Channel][3];
    struct _KolbModel KolbModel[Channel][3];
    int Z_Type[Channel][4];
    float imi_Const[Channel];

    // буфер для организации задержки сигнала на 1 шаг
    float wy0[Channel];

    // параматры для расчета колебательного звена
    float C[Channel][3][2],Lam[Channel][3][2];
    float X[Channel][3][3],Y[Channel][3][2];
    int tik[Channel][3];

    // параметры для расчета двухмассового колебательного звана
    float Psi[Channel][3],E[Channel][3],Et[Channel][3]]


    // параметры для расчета помехи
    unsigned long iq[Channel];
    float delt[Channel];
    // коэффициенты взаимовлияния каналов
    float wzk[Channel][Channel];

    // параметры помехи
    float xAmpl[Channel][3],xFreq[Channel][3],xFaza[Channel][3];
};



#endif

