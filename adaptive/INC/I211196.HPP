//
// 20 - 21 ����� 1996
//

#ifdef Uses_mdl_arss211196


#define DMAX_211196 DMAX_ARSS
#define PMAX_211196 PMAX_ARSS
#define QMAX_211196 (DMAX_211196-50)

class mdl_arss211196:public TemplateARSS_1x1
{
  public:
    // type - ��� �ࠢ�����
    //          0- ��६�饭��, 1-᪮����, 2-�᪮७��
    // p - ���冷� ������        [0..PMAX]
    // Q - ��㡨�� ॣ��ਧ�樨 [1..QMAX]
    // q - ����稭� ������뢠��� >=0
    mdl_arss211196(int type,unsigned p,unsigned Q,unsigned q);


   virtual void Build(void);
   virtual int Update(void);
   virtual float _HUGE Calc(float);
   virtual void _HUGE Append(float,float);

   virtual int isBuild(void) { return okBld; }

  protected:

   virtual void params(void);
   virtual void xbuild(void);
   inline float _y_(long );

    // ��室�� ��ࠬ���� ������
    int Typ,Mode;

//    double Betta[PMAX_211196],C[PMAX_211196];
    double psi_x[PMAX_211196][PMAX_211196],
           psi_y[PMAX_211196][PMAX_211196],
           psi_xy[PMAX_211196][PMAX_211196];

    // ࠡ�稥 ��ࠬ���� ������
//    float Y[DMAX_211196],X[DMAX_211196];
    unsigned long cnt;
    int okBld,okUpd,PS;

};

#endif
