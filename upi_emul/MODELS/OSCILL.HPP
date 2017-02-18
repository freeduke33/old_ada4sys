// Колебательные звенья

#include <zveno.hpp>

struct _Oscill
{
 float Freq,Betta;
};

class TransferOscill: public NilZveno // по перемещению
{
 public:
   TransferOscill(void);

   static const char NikName[];

   // работа с моделью
   virtual void _HUGE Reset(void);
   virtual float _HUGE Calc(float );
   virtual int _HUGE Build(void);

   // работа с параметрами модели
   virtual int SetParam(void *);
   virtual int ParamSize(void);
   virtual int GetParam(void *);

   // окно ввода параметров модели
   virtual int UserEnter(void);

 protected:
  // внешние параметры
  float Freq,Betta;
  // внутренние параметры и переменные
  float C[3],Lam[3],X[3],Y[3];
  int tik;
};

class TransferOscillA: public TransferOscill // по перемещению (аналоговое)
{
 public:
   TransferOscillA() { ; }
   static const char NikName[];

   virtual int _HUGE Build(void);
};

class SpeedOscillA: public TransferOscill // по скорости (аналоговое)
{
 public:
   SpeedOscillA() {;}

   static const char NikName[];

   virtual int _HUGE Build(void);
};

class DSpeedOscillA: public TransferOscill // по ускорению (аналоговое)
{
 public:
   DSpeedOscillA() {;}

   static const char NikName[];

   virtual float _HUGE Calc(float );
   virtual int _HUGE Build(void);
};
