#ifndef __UPI_ZVENO_HPP__
#define __UPI_ZVENO_HPP__

#include <NilClass.hpp>

class NilZveno:public NilClass
{
 public:

   // ࠡ�� � �������
   virtual void _HUGE Reset(void);
   virtual float _HUGE Calc(float );
   virtual int _HUGE Build(void);
   inline float Build(void *P) { if(SetParam(P)) return Build(); else return 0; }

   // ࠡ�� � ��ࠬ��ࠬ� ������
   NilClass::SetParam;

   // ���� ����� ��ࠬ��஢ ������
   virtual int UserEnter(void);

  // ����७��� ���� �������
  float delt;

  NilZveno *next;

  static const char NikName[];
};

#endif
