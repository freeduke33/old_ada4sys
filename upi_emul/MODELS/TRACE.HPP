// �஢�ઠ �ࠢ��쭮�� �ନ஢���� ��ࠬ��஢ ������
// �� 29.8.96

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

   // ࠡ�� � �������
   virtual float _HUGE Calc(float );

   // ࠡ�� � ��ࠬ��ࠬ� ������
   virtual int SetParam(void *);
   virtual int ParamSize(void);
   virtual int GetParam(void *);

   // ���� ����� ��ࠬ��஢ ������
   virtual int UserEnter(void);

 protected:

  int k;
  float x[NMAX],y[NMAX];

  // ���譨� ��ࠬ����

  int p;
  float E;
  double Lam[N],C[N];
};
