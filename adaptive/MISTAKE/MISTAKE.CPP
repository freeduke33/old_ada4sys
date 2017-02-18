#define Uses_Mistake_1x1

#include <adaptive.hpp>


#define mdl Mistake_1x1

mdl::mdl()
{
 isE=1;
 Margin=Tik=0;
 return;
}

void mdl::Reset(void)
{
 Tik=0;
 isE= Margin ? 0: 1;
 return;
}

void mdl::Wait(void)
{
 if(!isOn())
   {
   if(Margin<Tik) On();
   else Tik++;
   }
 return;
}

