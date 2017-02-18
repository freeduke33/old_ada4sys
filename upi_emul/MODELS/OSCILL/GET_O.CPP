#include <math.h>
#include <Oscill.hpp>


int TransferOscill::GetParam(void *P)
{
 int ret=0;
 struct _Oscill *B=(struct _Oscill *)P;

 if(B)
   {
   B->Freq=Freq;
   B->Betta=Betta;
   ret=1;
   }
 return ret;
}



