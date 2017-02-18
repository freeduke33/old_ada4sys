#include <zveno.hpp>

struct _DualMass
{
 float Betta[2],Freq[2];
};


class DualMass: public NilZveno
{
 public:
   DualMass(void);

   static const char NikName[];

   // работа с моделью
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
  float Freq[2],Betta[2];


  double Psi[3],Et[2],E[3],Lam[3],X[3],Y[3];
};
