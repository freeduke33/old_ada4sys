//
//     2 - 3 ������ 1995
//    ====================
//

#ifdef Uses_mdl_0TR031095


#define DMAX_031095  DMAX_ARSS
#define PMAX_031095  PMAX_ARSS
#define QMAX_031095 50

// class mdl_0TR031095:public FModel_1x1
class mdl_0TR031095:public TemplateARSS_1x1
{
  public:
    // type - ��� �ࠢ�����
    //          0- ��६�饭��, 1-᪮����, 2-�᪮७��
    // p - ���冷� ������        [0..PMAX]
    // Q - ��㡨�� ॣ��ਧ�樨 [1..QMAX]
    // q - ����稭� ������뢠��� >=0
    mdl_0TR031095(unsigned int type,unsigned int p,unsigned int Q,unsigned int q);


   virtual void Build(void);
   virtual int Update(void);
   virtual float _HUGE Calc(float);
   virtual void _HUGE Append(float,float);

   virtual int isBuild(void) { return okBld; }

  protected:

    // ��室�� ��ࠬ���� ������
    int Typ,Mode;

//    double Beta[PMAX_031095];
    double Ax[DMAX_031095],Ay[DMAX_031095];

    virtual void params(void);

    // ࠡ�稥 ��ࠬ���� ������
//    float x[3],Y[DMAX_031095],X[DMAX_031095],Yt[DMAX_031095];
    float x[3];
    unsigned long cnt;
//    int okBld,okUpd,l,PS;
    int okBld,okUpd,PS;

    // ����� ��� ���� ��ࠬ��஢ ������ � �ᨭ�஭��� ०���
    double XX[DMAX_031095],YY[DMAX_031095],YYt[DMAX_031095],
           beta[PMAX_031095];

  private:
   inline float _y_(int);
   inline float calc_su(int);
};

#endif

#ifdef Uses_mdl_0TR031095

class mdl_1TR031095:public mdl_0TR031095
{
 public:
    inline mdl_1TR031095(unsigned int type,unsigned int p,unsigned int Q,unsigned q):
                mdl_0TR031095(type,p,Q,q) {;}

 protected:
  virtual void params(void);

};

#endif
