
#ifdef Uses_gen_Sinus

class gen_Sinus: public FModel_1x1
{
 public:
   gen_Sinus(float ampl,float freq,float faza,float delt,float ad);

   virtual void Reset(void) { cnt=0; }
   virtual float _HUGE Calc(float val);

 protected:
   float freq,ampl,faza,delt,add;
   unsigned long cnt;

};

#endif

#ifdef Uses_gen_PSinus

class gen_PSinus: public FModel_1x1
{
 public:
   gen_PSinus(float ampl[3],float freq[3],float faza[3],float delt,float ad);

   virtual void Reset(void) { cnt=0; }
   virtual float _HUGE Calc(float val);

 protected:
   float freq[3],ampl[3],faza[3],delt,add;
   unsigned long cnt;

};

#endif

#ifdef Uses_gen_Const

class gen_Const: public FModel_1x1
{
 public:
   gen_Const(float v) { vol=v; }

   virtual float _HUGE Calc(float val) { return vol; }

 protected:
   float vol;

};

#endif

#ifdef Uses_gen_vBuffer

class gen_vBuffer: public FModel_1x1
{
 public:
   gen_vBuffer(Word MaxVol);
   virtual ~gen_vBuffer();

   virtual float _HUGE Calc(float val);

// update data buffer if needed
   virtual void Build(void);
// create memory buffers (if MemSize==0 - no limitation)
   virtual int CreateBuffer(DWord MemSize);
// calculate nedded memory for work
   virtual DWord NeedMemory();
// return allocated memory
   virtual DWord Allocated();
// verify state and build message if invalid
   virtual int isSuccessful(char *msg);

 protected:
         virtual DWord calcBufSize() =0;
         virtual void  fillBuf(int ) =0;

         int bufs,act,next;
         Word *buf[2];
         Word MaxVol;
         DWord need,EOB,Tik;
};

#endif


#ifdef Uses_gen_DatFile

class gen_DatFile: public gen_vBuffer
{
 public:
   gen_DatFile(DWord Time,char *file,Word MaxVol);
   virtual ~gen_DatFile();

   virtual int isSuccessful(char *msg);

 protected:
   virtual DWord calcBufSize();
   virtual void  fillBuf(int );

   int x_read(Word *buf,DWord size);

  int handle;
  DWord fsize,offset,Time;
  char Name[80];

};

#endif

#ifdef Uses_gen_StaticBuf

class gen_StaticBuf: public FModel_1x1
{
 public:
   gen_StaticBuf(long nr,float *buf);

   virtual void Reset(void) { cnt=len-1; }
   virtual float _HUGE Calc(float val);

 protected:
   unsigned long cnt,len;
   float *Buf;

};

#endif

#ifdef Uses_gen_Template

class gen_Template: public FModel_1x1
{
 public:
   gen_Template(float _k);

   virtual void Reset(void);
   virtual float _HUGE Calc(float val);

 protected:
   float k;

};

#endif

