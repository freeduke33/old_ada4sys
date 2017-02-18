#include <process.h>
#include <string.h>
#include "upi.hpp"

extern "C" void TestDriver(void);


int UPI_test::DoLoop(char *prg[])
{
 int val;


   EnableDriver();
   TestDriver();
   DisableDriver();

   val=1;
 return val;
}



