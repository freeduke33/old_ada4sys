

#ifdef Uses_LoopSC
// ᨬ������ ���易��� ������
// ������ ����� �ࠢ����� �����஬ M:M
class Buffer;
class FModel_1x1;

// ����ﭨ� ��⥬� �ࠢ����� ��᫥ �஢������ ��ᯥਬ���
typedef struct
{
 // ��� ���ﭨ� (������)
 int ResultCode;

 // �᫮ ��ࠡ�⠭��� 横��� � ����⮢ � ��᫥���� 横��
 int IsTiksExist;
 unsigned long OkCycle,OkTik;

 // �६� ࠡ��� ��⥬� �ࠢ�����
 int IsTimeExist;
 float WorkTime;

 // ����譮��� �ࠢ�����
 int IsMistakeExist;
 float MaxMistake,MinMistake,AveMistake;

 // �᫮ �ய�饭��� ����⮢
 unsigned long Skipped;

} LoopStat;


class LoopSC
{
 public:
    LoopSC(unsigned int chl);
    virtual ~LoopSC(void);

   // ���筨�� � �ਥ����� ������
   int SetBuffer(unsigned int chl,Buffer *x,Buffer *y,Buffer *yt);
   // ������ �ࠢ����� (�� 横��, 横�, ��᫥ 横��)
   int SetModel(unsigned int chl,FModel_1x1 *Bef,
                FModel_1x1 *Act,FModel_1x1 *Aft);

   // ���⥫쭮��� ��室� �� ���⮢�� ������
   int SetPassMargin(unsigned long len);

   // �஢��� ᨣ���� � ���ᨢ��� ���ﭨ�
   void SetPassiveVol(unsigned int chl,float vol);
   float GetPassiveVol(unsigned int chl);

   // ���⮢� �஢��� ᨣ���� ��। ����᪮�
   void SetStartVol(unsigned int chl,float vol);
   float GetStartVol(unsigned int chl);

   // ������� ���ଠ�� �� ��ᯥਬ���
   int GetStatictics(LoopStat *s);

   // �믮����� 横�� �ࠢ����� ��ꥪ⮬
   // � ������ ��室�� �� ��砫� � �� ����� ������� 横��
   virtual int PassLoop(float freq,unsigned long cycles,unsigned long time);

   // ����譮��� �ࠢ�����
   virtual float Mistake(void) =0;
   virtual void SetMistakeWait(unsigned int tiks) =0;

 protected:

   // ����祭��/�몫�祭�� ������ �ࠢ�����
   virtual int Enable();
   virtual int Disable();

   // ࠡ�� � �����஬ �ࠢ�����
   virtual int doStart(void) =0;
   virtual int doLoop(void) =0;
   virtual int doDone(void) =0;

   virtual void doRewind(float* valB,float* valE);

   // TRUE �᫨ �६� ࠡ��� �� �� ���稫���
   virtual int isWorkTime(void) =0;
   // TRUE �᫨ ����譮��� �ࠢ����� �� �ॢ�蠥� ��������
   virtual int isOkMistake(void) =0;

   // �������� ࠡ��� ������ �ࠢ�����
   virtual void showStart(void) =0;
   virtual int  showProgress(void) =0;
   virtual void showDone(void) =0;


   float *iBuf,*oBuf,*Buf; // ���ᨢ� ��� ࠡ��� � ���
   float *Buf1,*Buf2;

   unsigned int Chl;       // �᫮ ������� � ������ �ࠢ�����
   Buffer _HUGE **X,_HUGE **Y,_HUGE **Yt;    // 㪠��⥫� �� ���ᨢ� ������
   FModel_1x1 _HUGE **Model[3];  // 㪠��⥫� �� ������ �ࠢ�����

   // ����,�᫮ 横���,������ 1 横�� �ࠢ�����
   unsigned long Freq,Cycles,Tiks,Margin;

   // �஢��� ᨣ����� � ���⮢�� �窠�
   float *StartVol,*PassiveVol;

   LoopStat Info;
};

#endif


#ifdef Uses_SyncLoop


class SyncLoop: public LoopSC
{
 public:
    SyncLoop(unsigned int chl);
    virtual ~SyncLoop(void);

   virtual float Mistake(void);
   virtual void SetMistakeWait(unsigned int tiks);

 protected:
   // ࠡ�� � �����஬ �ࠢ�����
   virtual int doStart(void);
   virtual int doLoop(void);
   virtual int doDone(void)  { return 0;}
   virtual int isWorkTime(void);
   virtual int isOkMistake(void);

   int *num;

   // �६� ࠡ��� � �訡���, ���ᨬ.�६� ࠡ��� � �訡���
   unsigned int doErr,ErrTik;
};


#endif

#ifdef Uses_AsyncLoop

class AsyncLoop: public SyncLoop
{
 public:
   inline AsyncLoop(unsigned int chl):SyncLoop(chl) {;}

 protected:
   // ࠡ�� � �����஬ �ࠢ�����
   virtual int doStart(void);
   virtual int doLoop(void);

   virtual int Enable(void);
   virtual int Disable(void);

   friend void interrupt hook(void);

 private:
   int Do;
};

#endif


#ifdef Uses_LoopMC
// ᨬ������ �������易��� ������
// ������ ����� �ࠢ����� �����஬ M:M

class LoopMC: public LoopSC
{
 public:
   LoopMC(unsigned int chl);
  ~LoopMC(void);

   // ������ �ࠢ����� (�� 横��, 横�, ��᫥ 横��)
   int SetModel(FModel_NxN *Bef,FModel_NxN *Act,FModel_NxN *Aft);

   // �믮����� 横�� �ࠢ����� ��ꥪ⮬
   // � ������ ��室�� �� ��砫� � �� ����� ������� 横��
   virtual int PassLoop(float freq,unsigned long cycles,unsigned long time);

   // ����譮��� �ࠢ�����
   virtual float Mistake(void) =0;
   virtual void SetMistakeWait(unsigned int tiks) =0;

 protected:
   // ����祭��/�몫�祭�� ������ �ࠢ�����
   virtual int Enable();
   virtual int Disable();

   FModel_NxN *ModelN[3];
};

#endif
