#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "dac_adc.h"
#include "run.h"

void StartUp(void)
{

 if(DrvIsPresent())
   {
   if(DrvInitialize()!=DRV_OK)
      {
      printf("\nUnable to initialize UPI driver\n");
      exit(-2);
      }
//   if(Enable()==DRV_OK) Disable();
   }
 else
   {
   printf("\nUPI driver not present !\n");
   exit(-1);
   }

 return;
}

void ShutDown(void)
{
 DrvDeinitialize();
 return;
}


void GotoNull(void)
{
}

