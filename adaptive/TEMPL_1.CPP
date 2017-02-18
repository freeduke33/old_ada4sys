#define Uses_TemplateARSS_1x1
#define Uses_ExtTrendARSS_1x1

#include <adaptive.hpp>

#define mdl TemplateARSS_1x1

mdl::mdl(void)
{
 int i;
 
 A=0; for(i=0;i<PMAX_ARSS;i++) C[i]=Betta[i]=0;
 C[0]=1;
 l=p=Q=q=0;

 for(i=0;i<DMAX_ARSS;i++) X[i]=Y[i]=0;

 ExtTrend=NULL;

 return;
}

void mdl::setExtTrend(ExtTrendARSS_1x1 *et)
{
 ExtTrend=et;
 if(et) et->Connect(this);
 return;
}

