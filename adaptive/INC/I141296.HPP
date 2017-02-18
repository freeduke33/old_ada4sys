//
//        13 - 14 декабря 1996
//       ======================
//  Управление с использованием тренда
//
// Тренды 0,1,2 порядков, для АР-СС моделей

#ifdef Uses_etr_PL141296

// с использованием ортогональных полиномов
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

// по критерию совпадения
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
