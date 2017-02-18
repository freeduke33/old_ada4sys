#include <Oscill.hpp>

int TransferOscill::SetParam(void *P)
{
 int ret=0;
 struct _Oscill *B=(struct _Oscill *)P;

 if(B)
   {
   Freq=B->Freq;
   Betta=B->Betta;
   ret=1;
   }
 return ret;
}

