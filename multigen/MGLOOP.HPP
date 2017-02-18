#include <time.h>
#include <stdio.h>

class aLoop: public AsyncLoop
{
 public:
   inline aLoop(unsigned int ctl): AsyncLoop(ctl) {;}

 protected:
   // индикация работы контура управления
   virtual void showStart(void);
   virtual int  showProgress(void);
   virtual void showDone(void);

 private:

   int i;
   time_t bt,ct,wt;
   FILE *log;
};
