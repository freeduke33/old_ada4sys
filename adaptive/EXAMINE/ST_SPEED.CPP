// #define DEBUG

#define Uses_exm_StatSpeed_1x1
#define Uses_Buffer
#include <adaptive.hpp>
#include <log.hpp>


#define mdl   exm_StatSpeed_1x1
#define owner exm_Static_1x1

void mdl::Reset(void)
{
 owner::Reset();
 delt=hiVol*2/(nq-2);
 ddelt=0;
 Cycle=0;
 return;
}

float _HUGE mdl::Calc(float y)
{
 float old;

 last_y=y;

#ifdef DEBUG
 if(LOG)      LOG->put('D'," < ");
#endif
 if(ok && Cycle<nq/2 && act)
   {
   if(--Sleep==0)
     {
Next:
#ifdef DEBUG
 if(LOG)      LOG->put('D',"Новое значение (%f\t,%f\t)",y,delt*sign*Cycle);
#endif
     Y->putNext(y);
     X->putNext(delt*sign*Cycle);

     if(Top) Top->storePassive();
#ifdef DEBUG
 if(LOG)      LOG->put('D'," + ");
#endif

     setStep();
     }
#ifdef DEBUG
 if(LOG)      LOG->put('D',"\t%f\t--> ",y);
#endif
   old=vol;
   setVolume();
#ifdef DEBUG
 if(LOG)      LOG->put('D',"%f",vol);
#endif
   if(vol>hi || (sign==-1 && vol>old))
     {
     vol=old;
     if(Sleep==wait) // невозможно продолжать эксперимент
       {
       old=delt*sign*Cycle;
       for(;Cycle<nq;Cycle++)
         {
         Y->putNext(y); X->putNext(old);
         if(Top) Top->storePassive();
         }
       ok=0;
       }
     else goto Next;
     }
   y=vol;
   }
 else y=beg;

#ifdef DEBUG
 if(LOG)      LOG->put('D'," > ");
#endif
 return y;
}

void mdl::setVolume(void)
{
 vol+=(ddelt*sign);
 return;
}


void mdl::setStep(void)
{
 Sleep=wait;
 sign=-sign;

 if(sign==1) // перейдем к исследованию след. скорости, если у подножья пирамиды
   {
   Cycle++;
   ddelt= Cycle<nq/2 ? delt*Cycle/freq :0;
   vol=beg;

   if(Cycle==nq)
        {
        setPassive();
        if(Next) Next->setActive();
        }
   }
 return;
}


