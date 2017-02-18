#include <zveno.hpp>

struct _ArSS
{
 int p,q;
 float Lam[2],C[2];
};


class ArSS: public NilZveno
{
 public:
   ArSS(void);

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
  struct _ArSS Def;

  //
  long m;
  float X[100],Y[100];
};
