#include <string.h>

#define Uses_mdl_Anatomy
#include <adaptive.hpp>

#define mdl mdl_Anatomy_1x1

float _HUGE mdl::Calc(float y)
{
 float x=Mdl->Calc(y);

 store(x,y);

 return x;
}

void _HUGE mdl::Append(float x,float y)
{
 Mdl->Append(x,y);
 store(x,y);
 return;
}

int mdl::isSuccessful(char *msg)
{
 if(!Mdl) /* strcpy(msg,"Undefined parent model") */;
 return (Mdl!=0) && Mdl->isSuccessful(msg);
}
