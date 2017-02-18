#ifndef __EMUL_UPI_HPP__
#define __EMUL_UPI_HPP__

#include <dos.h>
#include "table.hpp"

class UPI:public Table
{
 public:
   UPI(int state,int Imax=50,int Omax=50);
  ~UPI(void);

   char huge *ErrMsg(void);
   int  DoLoop(char *prg[]); // инициализировать драйвер,
                             // запустить программу управления,
                             // деинициализировать драйвер

   virtual void huge StartUPI(void) =0;
   virtual void huge StopUPI(void) =0;

   virtual int huge BuildUPI(void) =0;
   virtual int huge CalcSignal(float *iBuf,float *oBuf) =0;


 protected:
   char huge *Err;
   int state;

   int EnableDriver(void);
   int DisableDriver(void);

   void SetError(char huge *Msg)  { Err=Msg; }

/*   InfoUPI::OpenInfoW;
   InfoUPI::CloseInfoW;
   InfoUPI::ResetInfoW;*/
};

class UPI_test:public UPI
{
 public:

   inline UPI_test(int state,int Imax=50,int Omax=50): UPI(state,Imax,Omax) {;}
   int  DoLoop(char *prg[]);

};

#endif
