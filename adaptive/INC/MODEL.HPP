//
// =========================[ модели управления ]==========================
//

#ifdef Uses_Model

// одноканальные
class Model_1x1
{
 public:
   virtual float _HUGE Calc(float) =0;
   virtual int isSuccessful(char *msg) { msg=0; return 1; }
};

// многоканальные
class Model_Nx1
{
 public:
   virtual float _HUGE Calc(float* ) =0;
   virtual int isSuccessful(char *msg) { msg=0; return 1; }
};

class Model_NxN
{
 public:
   virtual float* _HUGE Calc(float* ) =0;
   virtual int isSuccessful(char *msg) { msg=0; return 1; }
};

#endif




#ifdef Uses_FModel_1x1

// -------- базовый класс моделей управления 1x1
class Buffer;
class Defence_1x1;
class Mistake_1x1;

// model single channel
class FModel_1x1:public Model_1x1
{
 public:
  FModel_1x1(void) { Dfn=0; Err=0; Tpl=0; OkErr=0; Reset(); }

  // защита от перегрузок
  virtual void setDefence(Defence_1x1 *def) { Dfn=def; }
  // шаблонный сигнал
  virtual void setIdeal(Buffer *buf)    { Tpl=buf; }
  // погрешность управления
  virtual void setMistake(Mistake_1x1 *err,unsigned long margin,float ok);
  inline int isOkMistake(void) { return Mistake()>OkErr ? 0 : 1; }
  virtual float Mistake(void);

  // сбросить модель в начальное состояние
  virtual void Reset(void);

  // расчитать параметры модели
  virtual void Build(void)  { return; }
  // обновить рабочие параметры модели
  virtual int  Update(void) { return 1; }
  // расчитать сигнал прогноза
  virtual float _HUGE Calc(float y) =0;
  // оприходовать отклик объекта и сигнал прогноза
  virtual void _HUGE Append(float x,float y) { x=x; y=y; return; }

  // признак готовности расчитать сигнал прогноза
  virtual int isBuild(void) { return 1; }

 protected:

  virtual float Verify(float,float,float);

  Buffer   *Tpl;  // шаблонный сигнал
  Defence_1x1 *Dfn;  // защита от перегрузок
  Mistake_1x1 *Err;  // погрешность управления
  float OkErr;       // допустимая погрешность

  static float R[];  // сигнал дребезжания при выходе за диапазон +/-1
  unsigned short ir; // тек. положение в массиве R
};

class NilModel_1x1:public FModel_1x1
{
 public:
  // расчитать сигнал прогноза
  virtual float _HUGE Calc(float y) { return 0;}
  // признак готовности расчитать сигнал прогноза
  virtual int isBuild(void) { return 0; }
  // обновить рабочие параметры модели
  virtual int  Update(void) { return 0; }
};


#endif


#ifdef Uses_mdlTest

class mdlTest_1x1:public FModel_1x1
{
 public:
   virtual float _HUGE Calc(float);
};

#endif

#ifdef Uses_TemplateARSS_1x1

#define PMAX_ARSS 30
#define DMAX_ARSS 200

class ExtTrendARSS_1x1;

class TemplateARSS_1x1: public FModel_1x1
{
 public:
  TemplateARSS_1x1(void);

  virtual float _HUGE Calc(float y) =0;

  void setExtTrend(ExtTrendARSS_1x1 *et);

// --------------------------------------------
   // параметры АР-СС модели
   double Betta[PMAX_ARSS],C[PMAX_ARSS],A;
   // сигналы управления/отклики
   float X[DMAX_ARSS],Y[DMAX_ARSS];
   // положение в массивах X и Y
   int l;
   // порядок модели, задержка, глубина регуляризации
   int p,q,Q;
// --------------------------------------------

 protected:

   inline int isUseExtTrend(void)  { return (ExtTrend!=NULL); }

   // внешний тренд
   ExtTrendARSS_1x1 *ExtTrend;
};

#endif


#ifdef Uses_Container_1x1


class Container_1x1: public FModel_1x1
{
 public:
   Container_1x1(FModel_1x1& mdl) { Mdl=&mdl; }

   virtual float _HUGE Calc(float y) { return Mdl->Calc(y); }
   virtual void _HUGE Append(float x,float y) { Mdl->Append(x,y); }

   virtual void Build() { Mdl->Build(); }
   virtual int Update() { return Mdl->Update(); }


   virtual void setDefence(Defence_1x1 *def) { Mdl->setDefence(def); }
   virtual void setMistake(Mistake_1x1 *err,unsigned long tm,float ok) { Mdl->setMistake(err,tm,ok); }
   virtual void setIdeal(Buffer *buf) { Mdl->setIdeal(buf); }

   virtual float Mistake() { return Mdl->Mistake();}
   virtual void Reset() { Mdl->Reset(); }

   virtual int isBuild() { return Mdl->isBuild(); }
   virtual int isSuccessful(char *msg) { return Mdl->isSuccessful(msg); }

 protected:

  FModel_1x1 *Mdl;
};


#endif
