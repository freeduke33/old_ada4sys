#define DEBUG_LOOP


#ifdef DEBUG_LOOP
#define OwnerLoop SyncLoop
#else
#define OwnerLoop AsyncLoop
#endif

class tcxlLoop:public OwnerLoop
{
 public:
   inline tcxlLoop(unsigned int ctl): OwnerLoop(ctl) {;}

 protected:

   // индикация работы контура управления
   virtual void showStart(void);
   virtual int  showProgress(void);
   virtual void showDone(void);

   protected:

     int _progress(int mode);
     unsigned long tim,cycles,old,cycle,tik;
};
