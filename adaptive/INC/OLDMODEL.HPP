

#ifdef Uses_mdl_ARSS

#define PMAX_ARSS92  5
// #define QQMAX_ARSS  50
#define EMAX_ARSS    6
#define PSMAX_ARSS   100


class mdl_ARSS: public TemplateARSS_1x1
{
 public:

  mdl_ARSS(int p,int qq,int shift,int ep);

  virtual void  Build(void);
  virtual int   Update(void);
  virtual float _HUGE Calc(float y);
  virtual void _HUGE  Append(float y,float x);

  virtual void Reset(void);

 protected:

  virtual void xBuild(void);
  virtual void params(void);

  // описание модели
  int p;     // порядок
  int shift; // запаздывание
  int qq;    // регуляризация
  int ep;    // тренд

  // параметры модели
//  double C[PMAX_ARSS+1],Lam[PMAX_ARSS+1];
  double Beta_x[PMAX_ARSS+1],Beta_y[PMAX_ARSS+1];


  // рабочие переменные для расчета модели
  int ok,Bld;

//  double c[PMAX_ARSS+1];
//  double beta_x[PMAX_ARSS+1],beta_y[PMAX_ARSS+1];
//  double lam[PMAX_ARSS+1];


  double pkm[EMAX_ARSS+1][PSMAX_ARSS],pk[EMAX_ARSS+1];

  double ps[PMAX_ARSS+1][PMAX_ARSS+1],dxy[PMAX_ARSS+1][PMAX_ARSS+1];
  double ck[PMAX_ARSS+1][PMAX_ARSS+1],ak[PMAX_ARSS+1][PMAX_ARSS+1];
  double ffi[PMAX_ARSS+1][PMAX_ARSS+1];
  double d[PMAX_ARSS+1],a;
  double beta_kx[EMAX_ARSS+1][PMAX_ARSS+1],
              beta_ky[EMAX_ARSS+1][PMAX_ARSS+1];
  double koef[EMAX_ARSS+1],dk[EMAX_ARSS+1];

  unsigned long cnt,PS;

//  double X[PSMAX_ARSS],Y[PSMAX_ARSS];

};


#endif

#ifdef Uses_mdl_SS

class mdl_SS: public mdl_ARSS
{
 public:
  inline mdl_SS(int p,int qq,int shift,int ep):
       mdl_ARSS(p,qq,shift,ep) {;}

  virtual float _HUGE Calc(float y);

 protected:

  virtual void xBuild(void);
};

#endif
