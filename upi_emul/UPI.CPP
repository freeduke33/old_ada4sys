#define TEST_EMUL

#include <process.h>
#include <string.h>
#include "upi.hpp"

extern "C"
 {
 int far pascal _EnableDriver(void far *,int ctrl);
 int far pascal _DisableDriver(void);
 }

extern "C" void TestDriver(void);


int UPI::EnableDriver(void)
{
 _EnableDriver(GetTable(),state);
 SetTblActive();
 return 1;
}

int UPI::DisableDriver(void)
{
 _DisableDriver();
 SetTblPassive();
 return 1;
}

int UPI::DoLoop(char *prg[])
{
 int val;


   EnableDriver();
   spawnvp(P_WAIT,prg[0],&prg[0]);
   DisableDriver();

   val=1;
 return val;
}

UPI::UPI(int s,int Imax,int Omax):Table(Imax,Omax)
{
 Err=NULL;
 state=s;
 return;
}

UPI::~UPI(void) { DisableDriver(); return; }


char huge *UPI::ErrMsg(void) { return Err; }
