#include <function.hpp>

int SingleFunction::SetParam(void *P)
{
 int ret=1;
 struct _Function *F=(struct _Function *)P;

 if(F) k=F->k;
 else ret=0;

 return ret;
}
