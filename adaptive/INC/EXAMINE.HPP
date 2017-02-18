//
// Исследование обьекта в режиме прямого воздействия
//
//

#ifdef Uses_exm_Static_1x1

//  Исследование статической характеристики обьекта 1x1
//

class Buffer;

class exm_Static_1x1:public FModel_1x1
{
 public:
  exm_Static_1x1(void);

  virtual void Reset(void);
  virtual float _HUGE Calc(float y);

  virtual void SetParam(float Beg,float Lo,float Hi) { beg=Beg;lo=Lo;hi=Hi; }
  int Setup(float freq,unsigned long Nq,unsigned long wait,Buffer *x,Buffer *y);

  void NextChannel(exm_Static_1x1 *N);

  inline void setActive(void)  { act=1; }
  inline void setPassive(void) { act=0; }
  void storePassive(void);

 protected:


  int ok,act;
  float hi,lo,beg;
  unsigned long wait,Tik;
  Buffer *X,*Y;

  float delt,vol,freq,last_y;
  unsigned long Sleep,nq;
  int sign;

  exm_Static_1x1 *Next,*Top;
};

#endif

#ifdef Uses_exm_StatSpeed_1x1

class exm_StatSpeed_1x1:public exm_Static_1x1
{
 public:
  virtual void Reset(void);
  virtual float _HUGE Calc(float y);

  virtual void SetParam(float Beg,float Lo,float Hi)
     { exm_Static_1x1::SetParam(Beg,Lo,1); hiVol=Hi; }

  exm_Static_1x1::storePassive;

 protected:
  float ddelt,hiVol,Cycle;


  virtual void setVolume(void);
  virtual void setStep(void);

};


#endif

#ifdef Uses_exm_StatAccelerate_1x1

class exm_StatAccelerate_1x1:public exm_StatSpeed_1x1
{
 public:
  virtual void Reset(void);

 protected:
  float old[2];

  virtual void setVolume(void);
  virtual void setStep(void);

};
#endif


#ifdef Uses_exm_Achh

class exm_Achh: public FModel_1x1
{
 public:
  exm_Achh(void);
  virtual ~exm_Achh(void);

  virtual void Reset(void);
  virtual float _HUGE Calc(float y);

  inline void SetParam(float Beg,float Ampl,float loFreq,float hiFreq)
        { beg=Beg;ampl=Ampl; hi=hiFreq, lo=loFreq; }
  int Setup(float Freq,unsigned long Nq,unsigned long wait,Buffer *x,Buffer *y);

 protected:
  float hi,lo,beg,ampl,freq,delt,df;
  float *xx,*yy;

  int state,Cycle,ok;
  unsigned long wait,nq,nr,Sleep,nn,Tik;
  Buffer *X,*Y;
};

#endif
