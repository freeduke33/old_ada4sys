// ������⥫�� ������

#include <zveno.hpp>

struct _Oscill
{
 float Freq,Betta;
};

class TransferOscill: public NilZveno // �� ��६�饭��
{
 public:
   TransferOscill(void);

   static const char NikName[];

   // ࠡ�� � �������
   virtual void _HUGE Reset(void);
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
  float Freq,Betta;
  // ����७��� ��ࠬ���� � ��६����
  float C[3],Lam[3],X[3],Y[3];
  int tik;
};

class TransferOscillA: public TransferOscill // �� ��६�饭�� (����������)
{
 public:
   TransferOscillA() { ; }
   static const char NikName[];

   virtual int _HUGE Build(void);
};

class SpeedOscillA: public TransferOscill // �� ᪮��� (����������)
{
 public:
   SpeedOscillA() {;}

   static const char NikName[];

   virtual int _HUGE Build(void);
};

class DSpeedOscillA: public TransferOscill // �� �᪮७�� (����������)
{
 public:
   DSpeedOscillA() {;}

   static const char NikName[];

   virtual float _HUGE Calc(float );
   virtual int _HUGE Build(void);
};
