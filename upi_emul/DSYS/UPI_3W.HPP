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

    // ���� ��� �࣠����樨 ����প� ᨣ���� �� 1 蠣
    float wy0[Channel];

    // ��ࠬ���� ��� ���� ������⥫쭮�� �����
    float C[Channel][3][2],Lam[Channel][3][2];
    float X[Channel][3][3],Y[Channel][3][2];
    int tik[Channel][3];

    // ��ࠬ���� ��� ���� ���嬠�ᮢ��� ������⥫쭮�� �����
    float Psi[Channel][3],E[Channel][3],Et[Channel][3]]


    // ��ࠬ���� ��� ���� �����
    unsigned long iq[Channel];
    float delt[Channel];
    // �����樥��� ���������ﭨ� �������
    float wzk[Channel][Channel];

    // ��ࠬ���� �����
    float xAmpl[Channel][3],xFreq[Channel][3],xFaza[Channel][3];
};



#endif

