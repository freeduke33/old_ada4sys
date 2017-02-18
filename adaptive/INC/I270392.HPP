//
//       27 марта 1992
//    ====================
//


#define DMAX_270392 DMAX_ARSS
#define PMAX_270392 PMAX_ARSS
#define QMAX_270392 100

#ifdef Uses_arss_qTS270392


class arss_qTS270392: public TemplateARSS_1x1
{
 public:
   arss_qTS270392(unsigned int p,unsigned int Q,unsigned int q,unsigned int t);

   virtual void Build(void);
   virtual int Update(void);
   virtual float _HUGE Calc(float);
   virtual void _HUGE Append(float,float);

   virtual int isBuild(void) { return okBld; }

 protected:

   float _HUGE Result(float val);
   float _HUGE toType(float);

   virtual void _HUGE params(void);
   virtual void _HUGE params1(void);
   virtual float _HUGE calc(void);
   virtual void _HUGE xbuild(void);

   // исходные данные
   int type;

   // параметры модели
//   double Lam[PMAX_270392],C[PMAX_270392],A;

   // копия для асинхронного режима
   double _Lam[PMAX_270392],_C[PMAX_270392],_A;

   // сигналы в контуре управления
//   double X[DMAX_270392],Y[DMAX_270392];
   float _X[2],oldX;

   // параметры для расчета модели
   double A_xx[PMAX_270392][PMAX_270392],
          A_yy[PMAX_270392][PMAX_270392],
          A_xy[PMAX_270392][PMAX_270392];


   // вспомогательные параметры
   double B_x[PMAX_270392],B_y[PMAX_270392],
          G_xx0[PMAX_270392],
          G_yy0[PMAX_270392],
          G_xy0[PMAX_270392],
          G_0xy[PMAX_270392];

  int okBld,PS,cnt,okUpd;

 double fi[PMAX_270392][PMAX_270392],
        psi[PMAX_270392][PMAX_270392],
        cc[PMAX_270392][PMAX_270392],
        dd[PMAX_270392][PMAX_270392],
        d[PMAX_270392];
};

#endif

#ifdef Uses_garss_qTS270392
class garss_qTS270392:public arss_qTS270392
{
 public:
   garss_qTS270392(unsigned int p,unsigned int Q,
                   unsigned int q,unsigned int t): arss_qTS270392(p,Q,q,t)
          {;}

 protected:
   virtual float _HUGE calc(void);
};
#endif



#ifdef Uses_arss_q1TS270392

class arss_q1TS270392:public arss_qTS270392
{
 public:
   arss_q1TS270392(unsigned int p,unsigned int Q,
                   unsigned int q,unsigned int t);

 protected:
   virtual float _HUGE calc(void);
   virtual void _HUGE params1(void);
   virtual float _HUGE _y_(int);

   // вспомогательные параметры
   double BB_x[DMAX_270392],BB_y[DMAX_270392];
};

#endif

#ifdef Uses_garss_q1TS270392
class garss_q1TS270392:public arss_q1TS270392
{
 public:
   garss_q1TS270392(unsigned int p,unsigned int Q,
                   unsigned int q,unsigned int t): arss_q1TS270392(p,Q,q,t)
          {;}

 protected:
   virtual float _HUGE _y_(int);
};
#endif

#ifdef Uses_ss_qTS270392

class ss_qTS270392: public arss_qTS270392
{
 public:

  ss_qTS270392(unsigned int p,unsigned int Q,unsigned int q,
               unsigned int t);


  protected:

   virtual float _HUGE calc(void);
   virtual void _HUGE params1(void);
   virtual void _HUGE xbuild(void);

};

#endif

#ifdef Uses_ss_q1TS270392

class ss_q1TS270392:public ss_qTS270392
{
 public:
   ss_q1TS270392(unsigned int p,unsigned int Q,
                 unsigned int q,unsigned int t);


 protected:
   virtual float _HUGE calc(void);
   virtual void _HUGE params1(void);
   inline float _HUGE _y_(int);

   // вспомогательные параметры
   double BB_x[DMAX_270392],BB_y[DMAX_270392];
};

#endif

#ifdef Uses_gss_qTS270392
class gss_qTS270392:public ss_qTS270392
{
 public:
   gss_qTS270392(unsigned int p,unsigned int Q,
                 unsigned int q,unsigned int t): ss_qTS270392(p,Q,q,t)
          {;}

 protected:
   virtual float _HUGE calc(void);
};
#endif

#ifdef Uses_gss_q1TS270392
class gss_q1TS270392:public ss_q1TS270392
{
 public:
   gss_q1TS270392(unsigned int p,unsigned int Q,
                  unsigned int q,unsigned int t): ss_q1TS270392(p,Q,q,t)
          {;}

 protected:
   virtual float _HUGE _y_(int);
};
#endif


