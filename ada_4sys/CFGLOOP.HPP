#ifndef __CFGLOOP_HPP__
#define __CFGLOOP_HPP__

class LoopSC;
class Fmodel_1x1;
class Defence_1x1;
class Mistake_1x1;
class Model_1x1;
class Anatomy;
class Buffer;

class CfgLoop
{
 public:
   CfgLoop(CfgT *,LoopSC *);
   virtual ~CfgLoop(void);

   virtual int PrepareLoop(void);
   virtual int WriteResult(int PID);

 protected:

  virtual int PrepareModels(void) =0;
  virtual int PrepareIdeal(void);
  virtual int PrepareInform(void);
  virtual int PrepareBuffers(void);

 CfgT *Cfg;
 LoopSC *Ctrl;

 FModel_1x1 _HUGE **m[MAX_CHANNEL];
 Defence_1x1 *d[MAX_CHANNEL],*dd[MAX_CHANNEL];
 Mistake_1x1 *e[MAX_CHANNEL];
 Model_1x1 *dt[MAX_CHANNEL];
 Anatomy *a[MAX_infoCHANNEL];

 Buffer *xx[MAX_CHANNEL],*yy[MAX_CHANNEL],*yyt[MAX_CHANNEL];
};

#endif

