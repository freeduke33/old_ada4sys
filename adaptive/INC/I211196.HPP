//
// 20 - 21 ноября 1996
//

#ifdef Uses_mdl_arss211196


#define DMAX_211196 DMAX_ARSS
#define PMAX_211196 PMAX_ARSS
#define QMAX_211196 (DMAX_211196-50)

class mdl_arss211196:public TemplateARSS_1x1
{
  public:
    // type - вид управления
    //          0- перемещение, 1-скорость, 2-ускорение
    // p - порядок модели        [0..PMAX]
    // Q - глубина регуляризации [1..QMAX]
    // q - величина запаздывания >=0
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

    // исходные параметры модели
    int Typ,Mode;

//    double Betta[PMAX_211196],C[PMAX_211196];
    double psi_x[PMAX_211196][PMAX_211196],
           psi_y[PMAX_211196][PMAX_211196],
           psi_xy[PMAX_211196][PMAX_211196];

    // рабочие параметры модели
//    float Y[DMAX_211196],X[DMAX_211196];
    unsigned long cnt;
    int okBld,okUpd,PS;

};

#endif
