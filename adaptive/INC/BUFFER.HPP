#ifdef Uses_Buffer

class Buffer
{
 public:

 virtual ~Buffer(void) { return; }

 virtual void reset(void) =0;

 virtual int setSize(unsigned long s) =0;
 virtual int lockSize(void) { return RC_DONE; }
 virtual int unlockSize(void) { return RC_DONE; }
 virtual unsigned long getSize(void) =0;

 virtual void _HUGE putNext(float val) =0;
 virtual float _HUGE getNext(void) =0;
 virtual float _HUGE get(signed long idx) =0;
 virtual float _HUGE operator[](unsigned long idx) =0;

 virtual int setMargin(unsigned int size) =0;
};

#endif

#ifdef Uses_MemBuf

class MemBuf: public Buffer
{
 public:
   MemBuf(void);
   virtual ~MemBuf(void);

   virtual void reset(void);

   virtual int setSize(unsigned long s);
   virtual int lockSize(void);
   virtual int unlockSize(void);
   virtual unsigned long getSize(void);

   virtual void _HUGE putNext(float val);
   virtual float _HUGE getNext(void);
   virtual float _HUGE get(signed long idx);
   virtual float _HUGE operator[](unsigned long idx);

   virtual int setMargin(unsigned int size);

 protected:

   unsigned int begm,curm;
   unsigned long len,cur,cnt;
   float _HUGE *buf;
   int lock;
};

#endif

#ifdef Uses_StaticMemBuf

class StaticMemBuf:public MemBuf
{
  public:
    StaticMemBuf(unsigned long size,float *mem);
    virtual ~StaticMemBuf(void);

  virtual int setSize(unsigned long s);

 private:
  unsigned long siz;
};

#endif

#ifdef Uses_NilBuffer

class NilBuffer:public Buffer
{
  public:

 virtual void reset(void) {;}

 virtual int setSize(unsigned long s) { return 0;}
 virtual unsigned long getSize(void) { return 0;}

 virtual void _HUGE putNext(float val) {;}
 virtual float _HUGE getNext(void)  { return 0;}
 virtual float _HUGE get(signed long idx) { return 0;}
 virtual float _HUGE operator[](unsigned long idx) { return 0;}

 virtual int setMargin(unsigned int size) { return 0;}
};

#endif


#ifdef Uses_MemBufPSinus


class gen_PSinus;

class MemBufPSinus:public MemBuf
{
  public:
    MemBufPSinus(unsigned long nr,float fdsk,float *a,float *f,float *fi,float ad);
    virtual ~MemBufPSinus(void);

   virtual void reset(void);

   virtual int setSize(unsigned long s);
   virtual unsigned long getSize(void);

   virtual void _HUGE putNext(float val);
   virtual float _HUGE getNext(void);

 private:

  int mode,cnt;
  unsigned int nr;
  float A[3],F[3],Fi[3],add;
  gen_PSinus *gen;
};

#endif


