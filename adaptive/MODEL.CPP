#define Uses_FModel_1x1
#define Uses_Mistake_1x1
#define Uses_Defence
#include <adaptive.hpp>

float FModel_1x1::R[2]={1,0.983};

float FModel_1x1::Mistake(void)
{
 if(Err) return Err->Value();
 else return 0;
}

void FModel_1x1::Reset(void)
{
 ir=0;
 if(Err) Err->Reset();
 return;
}

void FModel_1x1::setMistake(Mistake_1x1 *err,unsigned long margin,float ok)
{
 Err=err;
 Err->SetMargin(margin);
 OkErr=ok;
 return;
}

float FModel_1x1::Verify(float x,float y,float yt)
{


 // физическое ограничение по диапазону
 if(x>1) x=R[++ir%=2]; else if(x<-1) x=-R[++ir%=2];

 // внешний контроль упр.сигнала
 if(Dfn) x=Dfn->Verify(x,y,yt);

 // расчет погрешности управления
 if(Err) Err->Use(x,y,yt);

 return x;
}

