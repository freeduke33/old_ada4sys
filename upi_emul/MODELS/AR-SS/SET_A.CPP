#include <ArSS.hpp>

int ArSS::SetParam(void *P)
{
 int i;
 struct _ArSS *T=(struct _ArSS *)P;

 *T=Def;

 return 0;
}

int ArSS::ParamSize(void) { return sizeof(struct _ArSS); }
