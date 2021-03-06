//
// =========================[ ������ �ࠢ����� ]==========================
//

#ifdef Uses_Model

// �����������
class Model_1x1
{
 public:
   virtual float _HUGE Calc(float) =0;
   virtual int isSuccessful(char *msg) { msg=0; return 1; }
};

// ������������
class Model_Nx1
{
 public:
   virtual float _HUGE Calc(float* ) =0;
   virtual int isSuccessful(char *msg) { msg=0; return 1; }
};

class Model_NxN
{
 public:
   virtual float* _HUGE Calc(float* ) =0;
   virtual int isSuccessful(char *msg) { msg=0; return 1; }
};

#endif




#ifdef Uses_FModel_1x1

// -------- ������ ����� ������� �ࠢ����� 1x1
class Buffer;
class Defence_1x1;
class Mistake_1x1;

// model single channel
class FModel_1x1:public Model_1x1
{
 public:
  FModel_1x1(void) { Dfn=0; Err=0; Tpl=0; OkErr=0; Reset(); }

  // ���� �� ��ॣ�㧮�
  virtual void setDefence(Defence_1x1 *def) { Dfn=def; }
  // 蠡����� ᨣ���
  virtual void setIdeal(Buffer *buf)    { Tpl=buf; }
  // ����譮��� �ࠢ�����
  virtual void setMistake(Mistake_1x1 *err,unsigned long margin,float ok);
  inline int isOkMistake(void) { return Mistake()>OkErr ? 0 : 1; }
  virtual float Mistake(void);

  // ����� ������ � ��砫쭮� ���ﭨ�
  virtual void Reset(void);

  // ������ ��ࠬ���� ������
  virtual void Build(void)  { return; }
  // �������� ࠡ�稥 ��ࠬ���� ������
  virtual int  Update(void) { return 1; }
  // ������ ᨣ��� �ண����
  virtual float _HUGE Calc(float y) =0;
  // ���室����� �⪫�� ��ꥪ� � ᨣ��� �ண����
  virtual void _HUGE Append(float x,float y) { x=x; y=y; return; }

  // �ਧ��� ��⮢���� ������ ᨣ��� �ண����
  virtual int isBuild(void) { return 1; }

 protected:

  virtual float Verify(float,float,float);

  Buffer   *Tpl;  // 蠡����� ᨣ���
  Defence_1x1 *Dfn;  // ���� �� ��ॣ�㧮�
  Mistake_1x1 *Err;  // ����譮��� �ࠢ�����
  float OkErr;       // �����⨬�� ����譮���

  static float R[];  // ᨣ��� �ॡ������� �� ��室� �� �������� +/-1
  unsigned short ir; // ⥪. ��������� � ���ᨢ� R
};

class NilModel_1x1:public FModel_1x1
{
 public:
  // ������ ᨣ��� �ண����
  virtual float _HUGE Calc(float y) { return 0;}
  // �ਧ��� ��⮢���� ������ ᨣ��� �ண����
  virtual int isBuild(void) { return 0; }
  // �������� ࠡ�稥 ��ࠬ���� ������
  virtual int  Update(void) { return 0; }
};


#endif


#ifdef Uses_mdlTest

class mdlTest_1x1:public FModel_1x1
{
 public:
   virtual float _HUGE Calc(float);
};

#endif

#ifdef Uses_TemplateARSS_1x1

#define PMAX_ARSS 30
#define DMAX_ARSS 200

class ExtTrendARSS_1x1;

class TemplateARSS_1x1: public FModel_1x1
{
 public:
  TemplateARSS_1x1(void);

  virtual float _HUGE Calc(float y) =0;

  void setExtTrend(ExtTrendARSS_1x1 *et);

// --------------------------------------------
   // ��ࠬ���� ��-�� ������
   double Betta[PMAX_ARSS],C[PMAX_ARSS],A;
   // ᨣ���� �ࠢ�����/�⪫���
   float X[DMAX_ARSS],Y[DMAX_ARSS];
   // ��������� � ���ᨢ�� X � Y
   int l;
   // ���冷� ������, ����প�, ��㡨�� ॣ��ਧ�樨
   int p,q,Q;
// --------------------------------------------

 protected:

   inline int isUseExtTrend(void)  { return (ExtTrend!=NULL); }

   // ���譨� �७�
   ExtTrendARSS_1x1 *ExtTrend;
};

#endif


#ifdef Uses_Container_1x1


class Container_1x1: public FModel_1x1
{
 public:
   Container_1x1(FModel_1x1& mdl) { Mdl=&mdl; }

   virtual float _HUGE Calc(float y) { return Mdl->Calc(y); }
   virtual void _HUGE Append(float x,float y) { Mdl->Append(x,y); }

   virtual void Build() { Mdl->Build(); }
   virtual int Update() { return Mdl->Update(); }


   virtual void setDefence(Defence_1x1 *def) { Mdl->setDefence(def); }
   virtual void setMistake(Mistake_1x1 *err,unsigned long tm,float ok) { Mdl->setMistake(err,tm,ok); }
   virtual void setIdeal(Buffer *buf) { Mdl->setIdeal(buf); }

   virtual float Mistake() { return Mdl->Mistake();}
   virtual void Reset() { Mdl->Reset(); }

   virtual int isBuild() { return Mdl->isBuild(); }
   virtual int isSuccessful(char *msg) { return Mdl->isSuccessful(msg); }

 protected:

  FModel_1x1 *Mdl;
};


#endif
