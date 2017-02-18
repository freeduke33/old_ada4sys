// ограничение упр. сигнала

#ifdef Uses_Defence
// ограничение упр. сигнала и расчет погрешности управления

// для варианта 1:1
class Defence_1x1
{
 public:
   virtual ~Defence_1x1() {;}

   // ограничения для безопасности управления
   virtual float Verify(float x,float y,float yt) =0;

   inline void VerifyOn(void)  { isV=1;}
   inline void VerifyOff(void) { isV=0;}
   inline int  isVerify(void)  { return isV; }

 protected:
   int isV;

};

#endif


#ifdef Uses_dfnSpeed

class dfnSpeed:public Defence_1x1
{
 public:
   dfnSpeed(float Freq,float MaxS) { delt=1.0/Freq; speed=MaxS; cnt=0; }

   virtual float Verify(float x,float y,float yt);

 protected:
   float delt,speed,old;
   int cnt;
};

#endif

#ifdef Uses_dfnAcceleration

class dfnAcceleration:public Defence_1x1
{
 public:
   dfnAcceleration(float Freq,float MaxA) { delt=1.0/Freq; acc=MaxA; cnt=0; }

   virtual float Verify(float x,float y,float yt);
 protected:
   float delt,acc,old[2];
   int cnt;
};

#endif

#ifdef Uses_dfnFull

class dfnFull:public Defence_1x1
{
 public:
    dfnFull(float f,float ms,float ma)
         { delt=1.0/f; speed=ms; acc=ma; cnt=0; }

   virtual float Verify(float x,float y,float yt);
 protected:
   float delt,acc,speed,old[2];
   int cnt;
};

#endif
