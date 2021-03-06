#ifndef __UPI_3W_HPP__
#define __UPI_3W_HPP__

#include "upi.hpp"
#include "upi_3w.h"


#ifndef TEST
# define owner UPI
#else
# define owner UPI_test
#endif

class NilClass;

class UPI_3w:public owner
{
  public:
    UPI_3w(char *filename,int ctrl);

    owner::DoLoop;
    owner::ErrMsg;

    virtual void huge StartUPI(void);
    virtual void huge StopUPI(void);

    virtual int huge BuildUPI(void);
    virtual int huge CalcSignal(float *iBuf,float *oBuf);

    int Complete;

  private:

    void ShowConfig();


    // ���ᠭ�� �������
    struct
      {

      // ���䨣���� ������
      int Z_Type[4],subZ_Type[3],wait;
      float fdi,fdds,imi_Const;

      NilZveno *Zveno[4];

      // ���� ��� �࣠����樨 ����প� ᨣ����
      float wy[MAX_WAIT];

      // ��ࠬ���� ��� ���� �����
      unsigned long iq;
      float delt;
      // ��ࠬ���� �����
      float xAmpl[3],xFreq[3],xFaza[3];
      } D[Channel];

      // �����樥��� ���������ﭨ� �������
    float wzk[Channel][Channel];

};



#endif

