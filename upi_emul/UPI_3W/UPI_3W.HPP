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

    virtual void huge StartUPI(void);
    virtual void huge StopUPI(void);

    virtual int huge BuildUPI(void);
    virtual int huge CalcSignal(float *iBuf,float *oBuf);

  private:

    void ShowConfig();

    // описание каналов
    struct
      {

      // конфигурация канала
      int Z_Type[4],wait;
      float fdi,fdds,imi_Const;

      // параметры моделей в канале
      struct _FuncModel FuncModel[3];
      struct _AperModel AperModel[3];
      struct _KolbModel KolbModel[3];
      struct _MassModel MassModel[3];

      // буфер для организации задержки сигнала
      float wy[MAX_WAIT];


      // параматры для расчета колебательного звена
      double C[3][2],Lam[3][2],X[3][3],Y[3][3];
      int tik[3];

      // параметры для расчета двухмассового колебательного звана
      double Psi[3][3],E[3][3],Et[3][3];


      // параметры для расчета помехи
      unsigned long iq;
      float delt;
      // параметры помехи
      float xAmpl[3],xFreq[3],xFaza[3];
      } D[Channel];

      // коэффициенты взаимовлияния каналов
    float wzk[Channel][Channel];

};



#endif

