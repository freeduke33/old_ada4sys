

#ifdef Uses_LoopSC
// симметричные несвязанные каналы
// базовый класс управления контуром M:M
class Buffer;
class FModel_1x1;

// Состояние системы управления после проведения эксперимента
typedef struct
{
 // код состояния (возврата)
 int ResultCode;

 // число отработанных циклов и отсчетов в последнем цикле
 int IsTiksExist;
 unsigned long OkCycle,OkTik;

 // время работы системы управления
 int IsTimeExist;
 float WorkTime;

 // погрешность управления
 int IsMistakeExist;
 float MaxMistake,MinMistake,AveMistake;

 // число пропущенных отсчетов
 unsigned long Skipped;

} LoopStat;


class LoopSC
{
 public:
    LoopSC(unsigned int chl);
    virtual ~LoopSC(void);

   // источники и приемники данных
   int SetBuffer(unsigned int chl,Buffer *x,Buffer *y,Buffer *yt);
   // модели управления (до цикла, цикл, после цикла)
   int SetModel(unsigned int chl,FModel_1x1 *Bef,
                FModel_1x1 *Act,FModel_1x1 *Aft);

   // длительность выхода на стартовую позицию
   int SetPassMargin(unsigned long len);

   // уровень сигнала в пассивном состоянии
   void SetPassiveVol(unsigned int chl,float vol);
   float GetPassiveVol(unsigned int chl);

   // стартовый уровень сигнала перед запуском
   void SetStartVol(unsigned int chl,float vol);
   float GetStartVol(unsigned int chl);

   // получить информацию об эксперименте
   int GetStatictics(LoopStat *s);

   // выполнить циклы управления объектом
   // с плавным выходом на начало и на конец каждого цикла
   virtual int PassLoop(float freq,unsigned long cycles,unsigned long time);

   // погрешность управления
   virtual float Mistake(void) =0;
   virtual void SetMistakeWait(unsigned int tiks) =0;

 protected:

   // включение/выключение контура управления
   virtual int Enable();
   virtual int Disable();

   // работа с контуром управления
   virtual int doStart(void) =0;
   virtual int doLoop(void) =0;
   virtual int doDone(void) =0;

   virtual void doRewind(float* valB,float* valE);

   // TRUE если время работы еще не кончилось
   virtual int isWorkTime(void) =0;
   // TRUE если погрешность управления не превышает заданную
   virtual int isOkMistake(void) =0;

   // индикация работы контура управления
   virtual void showStart(void) =0;
   virtual int  showProgress(void) =0;
   virtual void showDone(void) =0;


   float *iBuf,*oBuf,*Buf; // массивы для работы с УСО
   float *Buf1,*Buf2;

   unsigned int Chl;       // число каналов в контуре управления
   Buffer _HUGE **X,_HUGE **Y,_HUGE **Yt;    // указатели на массивы данных
   FModel_1x1 _HUGE **Model[3];  // указатели на модели управления

   // частота,число циклов,длинна 1 цикла управления
   unsigned long Freq,Cycles,Tiks,Margin;

   // уровень сигналов в стартовых точках
   float *StartVol,*PassiveVol;

   LoopStat Info;
};

#endif


#ifdef Uses_SyncLoop


class SyncLoop: public LoopSC
{
 public:
    SyncLoop(unsigned int chl);
    virtual ~SyncLoop(void);

   virtual float Mistake(void);
   virtual void SetMistakeWait(unsigned int tiks);

 protected:
   // работа с контуром управления
   virtual int doStart(void);
   virtual int doLoop(void);
   virtual int doDone(void)  { return 0;}
   virtual int isWorkTime(void);
   virtual int isOkMistake(void);

   int *num;

   // время работы с ошибкой, максим.время работы с ошибкой
   unsigned int doErr,ErrTik;
};


#endif

#ifdef Uses_AsyncLoop

class AsyncLoop: public SyncLoop
{
 public:
   inline AsyncLoop(unsigned int chl):SyncLoop(chl) {;}

 protected:
   // работа с контуром управления
   virtual int doStart(void);
   virtual int doLoop(void);

   virtual int Enable(void);
   virtual int Disable(void);

   friend void interrupt hook(void);

 private:
   int Do;
};

#endif


#ifdef Uses_LoopMC
// симметричные взаимосвязанные каналы
// базовый класс управления контуром M:M

class LoopMC: public LoopSC
{
 public:
   LoopMC(unsigned int chl);
  ~LoopMC(void);

   // модели управления (до цикла, цикл, после цикла)
   int SetModel(FModel_NxN *Bef,FModel_NxN *Act,FModel_NxN *Aft);

   // выполнить циклы управления объектом
   // с плавным выходом на начало и на конец каждого цикла
   virtual int PassLoop(float freq,unsigned long cycles,unsigned long time);

   // погрешность управления
   virtual float Mistake(void) =0;
   virtual void SetMistakeWait(unsigned int tiks) =0;

 protected:
   // включение/выключение контура управления
   virtual int Enable();
   virtual int Disable();

   FModel_NxN *ModelN[3];
};

#endif
