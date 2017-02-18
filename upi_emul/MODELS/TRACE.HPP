// проверка правильности формирования параметров модели
// от 29.8.96

#include <zveno.hpp>


#define N    10
#define NMAX 10

struct _TestBuildModel
{
 int p;
 float E;
 double Lam[N],C[N];
};


class TestBuildModel: public NilZveno
{
 public:
   TestBuildModel(void);

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

  int k;
  float x[NMAX],y[NMAX];

  // внешние параметры

  int p;
  float E;
  double Lam[N],C[N];
};
