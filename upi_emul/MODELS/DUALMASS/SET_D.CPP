#include <DualMass.hpp>

int DualMass::SetParam(void *P)
{
 int i;
 struct _DualMass *T=(struct _DualMass *)P;

 for(i=0;i<2;i++)
   {
   Betta[i]=T->Betta[i];
   Freq[i]=T->Freq[i];
   }

 return 0;
}

int DualMass::ParamSize(void) { return sizeof(struct _DualMass); }
