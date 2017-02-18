#include <time.h>
#include <stdio.h>

class aLoop: public AsyncLoop
{
 public:
   aLoop(unsigned int ctl,unsigned int info=0);

 protected:
   // индикация работы контура управления
   virtual int showProcess(LoopShow );
   void showStart(void);
   void showDone(void);

 private:

   int i;
   time_t bt,ct,wt;
   FILE *log;
};
