#ifdef Uses_Mistake_1x1

class Mistake_1x1
{
 public:
   Mistake_1x1();
   // ���
   virtual void Reset(void);
   // ⥪���� ����譮��� �ࠢ�����
   virtual float Value(void) =0;
   // ���᫥��� ����譮�� �ࠢ�����
   virtual void Use(float x,float y,float yt) =0;

   inline void SetMargin(unsigned long m) { Margin=m; }
   inline void On(void)  { isE=1; }
   inline void Off(void) { isE=0; }
   inline int  isOn(void)  { return isE; }

 protected:
   int isE;
   // ����প� ����᪠ ���� ����譮��, N �����
   unsigned long Margin,Tik;

   void Wait(void);
};

#endif

#ifdef Uses_mst_Square

class mst_Square:public Mistake_1x1
{
 public:
   mst_Square(void);

   virtual void Reset(void);
   virtual float Value(void);
   virtual void Use(float x,float y,float yt);

 protected:
   double val,val1;
};

#endif

#ifdef Uses_mst_MathWait

class mst_MathWait:public Mistake_1x1
{
 public:
   mst_MathWait(void);

   virtual void Reset(void);
   virtual float Value(void);
   virtual void Use(float x,float y,float yt);

 protected:
   double val;
   DWord cnt;
};

#endif
