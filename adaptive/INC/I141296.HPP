//
//        13 - 14 ������� 1996
//       ======================
//  ��ࠢ����� � �ᯮ�짮������ �७��
//
// �७�� 0,1,2 ���浪��, ��� ��-�� �������

#ifdef Uses_etr_PL141296

// � �ᯮ�짮������ ��⮣������� ���������
//

class etr_PL141296: public ExtTrendARSS_1x1
{
 public:

  etr_PL141296(int p,int T,int q);

  virtual float Calc(int l);

  protected:

  int T,Q,q,k;
  double fi[DMAX_ARSS],psi[DMAX_ARSS];
};

#endif

#ifdef Uses_etr_EQ141296

// �� ����� ᮢ�������
//

class etr_EQ141296: public ExtTrendARSS_1x1
{
 public:

  etr_EQ141296(int T,int Q,int q);

  virtual float Calc(int l);

 protected:
  int T,Q,q,k;
  double fi[DMAX_ARSS];

};

#endif
