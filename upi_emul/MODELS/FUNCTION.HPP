// Функциональные звенья

#include <Zveno.hpp>

struct _Function
{
 float k;
};


class SingleFunction: public NilZveno
{
 public:
   SingleFunction(void);

   static const char NikName[];

   // работа с моделью
   virtual float _HUGE Calc(float );

   // работа с параметрами модели
   virtual int SetParam(void *);
   virtual int ParamSize(void);
   virtual int GetParam(void *);

   // окно ввода параметров модели
   virtual int UserEnter(void);

 protected:
  // внешние параметры
  float k;
};

class DoubleFunction: public SingleFunction
{
 public:
   static const char NikName[];

   virtual float _HUGE Calc(float );
};

class FherdFunction: public SingleFunction
{
 public:
   static const char NikName[];

   virtual float _HUGE Calc(float );
};

class SqrtFunction: public SingleFunction
{
 public:
   static const char NikName[];

   virtual float _HUGE Calc(float );
};


class SpeedFunction: public SingleFunction
{
 public:
   SpeedFunction(void);

   static const char NikName[];

   virtual float _HUGE Calc(float );

 protected:

   // внутренние
   float X[3];
};

class DSpeedFunction: public SpeedFunction
{
 public:
   static const char NikName[];

   virtual float _HUGE Calc(float );
};

