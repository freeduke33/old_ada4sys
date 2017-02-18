#define DEBUG_LOOP


#ifdef DEBUG_LOOP
#define OwnerLoop SyncLoop
#else
#define OwnerLoop AsyncLoop
#endif

class txtLoop:public OwnerLoop
{
 public:
   txtLoop(unsigned int ctl,unsigned int info): OwnerLoop(ctl,info) {;}

 protected:

   // индикация работы контура управления
   virtual int  showProcess(LoopShow );
   void showStart(void);
   void showDone(void);

   protected:
     unsigned long tim,cycles,old,cycle,tik;
};
