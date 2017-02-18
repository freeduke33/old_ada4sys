//
//     24 - 27 марта 1995
//    ====================
//

#ifdef Uses_delTrend_270395
// Удаление полиномиального тренда для обеспечения
// возможности управления "скоростью" (тренд 0-го порядка)
// и "ускорением" (тренд 1-го порядка)
class delTrend_270395: public Defence_1x1
{
 public:
   // Q- коэффициент точности удаления тренда
   // Vol - пьедистал
   // Speed - максимальная скорость удаления тренда
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
// модель для управления по перемещению/скорости/ускорению
// без тренда

#define PMAX_270395 PMAX_ARSS
#define DMAX_270395 DMAX_ARSS
#define QMAX_270395 100

class mdl_270395:public TemplateARSS_1x1
{
  public:
    // type- вид управления
    //          0- перемещение, 1-скорость, 2-ускорение
    // p - порядок модели        [0..PMAX]
    // q - задержка              [0.. ?? ]
    // Q - глубина регуляризации [1..QMAX]
    mdl_270395(unsigned int type,unsigned int p,unsigned int q,
               unsigned int Q);


   virtual void Build(void);
   virtual int Update(void);
   virtual float _HUGE Calc(float);
   virtual void _HUGE Append(float,float);

   virtual int isBuild(void) { return okBld; }

  protected:

    // исходные параметры модели
    int Typ;

//    double Beta[PMAX_270395];
    double dx[DMAX_270395],dy[DMAX_270395];

    virtual void params(void);
    virtual float calc(void);

    // рабочие параметры модели
//    float X[DMAX_270395],Y[DMAX_270395];
    unsigned long cnt;
    int okBld,okUpd,PS;

    // копия параметров модели для расчета
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

