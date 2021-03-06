#ifndef __UPI_pipe_HPP__
#define __UPI_pipe_HPP__

#define INCL_DOSNMPIPES
#include <os2.h>

// #include "upi.hpp"
#include "UPI_3w.h"

class NilClass;

class UPI_pipe
{
  public:
    UPI_pipe(char *filename,int ctrl);

    virtual float _HUGE StartUPI(float f);
    virtual void _HUGE StopUPI(void);

    virtual int _HUGE BuildUPI(void);
    virtual int _HUGE CalcSignal(float *iBuf,float *oBuf);

    virtual int doEmulate(void);

    int Complete;

    inline int GetWorkADC(void) { return chlADC; }
    inline int GetWorkDAC(void) { return chlDAC; }
    inline float GetWorkFreq(void) { return Freq; }

  protected:
    float Freq;
    int chlADC,chlDAC;
    HPIPE pipe_handle;

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

