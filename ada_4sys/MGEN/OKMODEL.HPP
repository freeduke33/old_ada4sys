#ifndef __OKMODEL_HPP__
#define __OKMODEL_HPP__

class CfgLoopGen:public CfgLoop
{
 public:
   // sMem = ᪮�쪮 娯� �� �ண���
   CfgLoopGen(CfgT *,LoopSC *l,unsigned sMem);
   virtual ~CfgLoopGen();

 protected:


  virtual int PrepareModels(void);
  virtual int PrepareIdeal(void);
//  virtual int PrepareBuffers(void);

  int Template2Bin(char *fname,int k,unsigned DAC);
  int AllocBuffers(void);

  char fn[MAX_CHANNEL][20];
  unsigned long SafeMem;
};

#endif


