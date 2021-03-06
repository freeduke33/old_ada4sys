// �㭪樮����� ������

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

   // ࠡ�� � �������
   virtual float _HUGE Calc(float );

   // ࠡ�� � ��ࠬ��ࠬ� ������
   virtual int SetParam(void *);
   virtual int ParamSize(void);
   virtual int GetParam(void *);

   // ���� ����� ��ࠬ��஢ ������
   virtual int UserEnter(void);

 protected:
  // ���譨� ��ࠬ����
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

   // ����७���
   float X[3];
};

class DSpeedFunction: public SpeedFunction
{
 public:
   static const char NikName[];

   virtual float _HUGE Calc(float );
};

