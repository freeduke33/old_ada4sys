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

   // ࠡ�� � �������
   virtual float _HUGE Calc(float );
   virtual int _HUGE Build(void);

   // ࠡ�� � ��ࠬ��ࠬ� ������
   virtual int SetParam(void *);
   virtual int ParamSize(void);
   virtual int GetParam(void *);

   // ���� ����� ��ࠬ��஢ ������
   virtual int UserEnter(void);

 protected:
  // ���譨� ��ࠬ����
  struct _ArSS Def;

  //
  long m;
  float X[100],Y[100];
};
