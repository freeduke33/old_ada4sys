#include <DualMass.hpp>

int DualMass::GetParam(void *P)
{
 int i;
 struct _DualMass *T=(struct _DualMass *)P;

 for(i=0;i<2;i++)
   {
   T->Betta[i]=Betta[i];
   T->Freq[i]=Freq[i];
   }

 return 0;
}
