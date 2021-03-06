//
//     24 - 27 ���� 1995
//    ====================
//

#ifdef Uses_delTrend_270395
// �������� ����������쭮�� �७�� ��� ���ᯥ祭��
// ���������� �ࠢ����� "᪮�����" (�७� 0-�� ���浪�)
// � "�᪮७���" (�७� 1-�� ���浪�)
class delTrend_270395: public Defence_1x1
{
 public:
   // Q- �����樥�� �筮�� 㤠����� �७��
   // Vol - �쥤��⠫
   // Speed - ���ᨬ��쭠� ᪮���� 㤠����� �७��
    delTrend_270395(unsigned int Q,float vol,float speed,Defence_1x1 *dfn);
    virtual ~delTrend_270395(void);

   virtual float Verify(float x,float y,float yt);

 protected:
   float *buf,Vol,MaxSpeed;
   unsigned int Q,cur,cnt;

   virtual float calc(float) =0;
   Defence_1x1 *Dfn;
};


#endif

#ifdef Uses_delTr0_270395


class delTr0_270395:public delTrend_270395
{
 public:
   inline delTr0_270395(unsigned int Q,float vol,float speed,Defence_1x1 *M):
              delTrend_270395(Q,vol,speed,M) {;}

 protected:

   virtual float calc(float);

};

#endif

#ifdef Uses_delTr1_270395


class delTr1_270395:public delTrend_270395
{
 public:
   inline delTr1_270395(unsigned int Q,float vol,float speed,Defence_1x1 *M):
                delTrend_270395(Q,vol,speed,M) {;}

 protected:
   virtual float calc(float);

};

#endif

#ifdef Uses_mdl_270395
// ������ ��� �ࠢ����� �� ��६�饭��/᪮���/�᪮७��
// ��� �७��

#define PMAX_270395 PMAX_ARSS
#define DMAX_270395 DMAX_ARSS
#define QMAX_270395 100

class mdl_270395:public TemplateARSS_1x1
{
  public:
    // type- ��� �ࠢ�����
    //          0- ��६�饭��, 1-᪮����, 2-�᪮७��
    // p - ���冷� ������        [0..PMAX]
    // q - ����প�              [0.. ?? ]
    // Q - ��㡨�� ॣ��ਧ�樨 [1..QMAX]
    mdl_270395(unsigned int type,unsigned int p,unsigned int q,
               unsigned int Q);


   virtual void Build(void);
   virtual int Update(void);
   virtual float _HUGE Calc(float);
   virtual void _HUGE Append(float,float);

   virtual int isBuild(void) { return okBld; }

  protected:

    // ��室�� ��ࠬ���� ������
    int Typ;

//    double Beta[PMAX_270395];
    double dx[DMAX_270395],dy[DMAX_270395];

    virtual void params(void);
    virtual float calc(void);

    // ࠡ�稥 ��ࠬ���� ������
//    float X[DMAX_270395],Y[DMAX_270395];
    unsigned long cnt;
    int okBld,okUpd,PS;

    // ����� ��ࠬ��஢ ������ ��� ����
    double beta[PMAX_270395];

  private:
   virtual float _y_(int);
   inline double calc_su(int);
};


#endif


#ifdef Uses_gen_270395

class gen_270395: public mdl_270395
{
 public:
    inline gen_270395(unsigned int type,unsigned int p,unsigned int q,
               unsigned int Q):
       mdl_270395(type,p,q,Q) {;}

 private:
  virtual float _y_(int);
};

#endif

#ifdef Uses_mdl_TR270395

class mdl_TR270395:public mdl_270395
{
 public:
   mdl_TR270395(unsigned int type,unsigned int p,unsigned int q,
                unsigned int Q,unsigned int N1);

 protected:
   int N;

   virtual void params(void);
   virtual float calc(void);

 private:
   virtual float _y_(int);
   inline double calc_su(int);
   inline double calc_ddy(int);
   inline double calc_ddx(int,double *);
};

#endif

#ifdef Uses_gen_TR270395

class gen_TR270395: public mdl_TR270395
{
 public:
    inline gen_TR270395(unsigned int type,unsigned int p,unsigned int q,
               unsigned int Q,unsigned int N1):
       mdl_TR270395(type,p,q,Q,N1) {;}

 private:
  virtual float _y_(int);
};

#endif

