#include <stdlib.h>

#define Uses_exm_Static_1x1
#define Uses_Buffer

#include <adaptive.hpp>


#define mdl exm_Static_1x1


inline unsigned int min(unsigned int a,unsigned int b)
{
 return a>b ? a : b;
}

mdl::mdl(void)
{
 hi=1; lo=-1;
 X=Y=NULL;
 ok=0;
 sign=1;
 beg=0;
 act=0;
 Next=Top=NULL;
 return;
}

int mdl::Setup(float f,unsigned long nr,unsigned long w,Buffer *x,Buffer *y)
{
 freq=f;
 wait=w;
 X=x; Y=y;
 if(!Y || !Y) return 0;
 nq=nr; //-2;
 ok=1;
 Reset();

 return ok;
}

void mdl::Reset(void)
{
 if(ok)
   {
   X->reset();
   Y->reset();
   hi= hi<beg ? beg : hi;
   lo= lo>beg ? beg : lo;
   sign=1;
   delt=(hi-lo)*2.0/nq;
   Sleep=wait;
   vol=beg;
   Tik=0;
   act=0;
   }
 return;
}

float _HUGE mdl::Calc(float y)
{
 last_y=y;

 if(ok && act)
   {
   // ждем успокоения
   if(--Sleep==0)
     {
     Sleep=wait;
     // сохраняем значения статической хар-ки
     Y->putNext(y); X->putNext(vol);
     if(Top) Top->storePassive();

     // переходим к следующей точке
     vol+=(delt*sign); Tik++;
     if(vol>hi) { vol=hi-delt; sign=-1; }
     if(vol<lo) { vol=lo+delt; sign= 1; }

     if(Tik==nq)
        {
        setPassive();
        if(Next) Next->setActive();
        }
     }
   y=vol;
   }
 else y=beg;

 return y;
}


void mdl::storePassive(void)
{
 if(!act) { Y->putNext(last_y); X->putNext(beg); }
 if(Next) Next->storePassive();
 return;
}


void mdl::NextChannel(exm_Static_1x1 *N)
{
 Next=NULL;
 if(N)
   {
   Top=N->Top;
   N->Next=this;
   }
 else  Top=this;
}
