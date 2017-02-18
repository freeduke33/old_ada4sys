#define Uses_exm_StatAccelerate_1x1
#define Uses_Buffer
#include <adaptive.hpp>


#define mdl   exm_StatAccelerate_1x1
#define owner exm_StatSpeed_1x1

void mdl::Reset(void)
{
 unsigned long n=(nq/2)-1;

 owner::Reset();
// delt=2.0*hi/(n*(n+1));
 old[0]=old[1]=beg;
 return;
}

void mdl::setVolume(void)
{
 vol=2.0*old[0]-old[1]+ddelt;

 old[1]=old[0]; old[0]=vol;
 return;
}

void mdl::setStep(void)
{
 owner::setStep();
 if(sign==-1) { vol=old[1]; old[1]=old[0]; old[0]=vol; }  // разворот
 if(sign==1)  old[0]=beg;
 return;
}
