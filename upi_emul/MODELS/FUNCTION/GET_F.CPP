#include <function.hpp>

int SingleFunction::GetParam(void *P)
{
 int ret=1;
 struct _Function *F=(struct _Function *)P;

 if(F) F->k=k;
 else ret=0;

 return ret;
}



