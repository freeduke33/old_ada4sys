#ifndef __OKMODEL_HPP__
#define __OKMODEL_HPP__

class CfgLoopAda:public CfgLoop
{
 public:

  CfgLoopAda(CfgT *,LoopSC *l);
 ~CfgLoopAda(void);

 protected:

  virtual int PrepareModels(void);

  ExtTrendARSS_1x1 *tr[MAX_CHANNEL];
};

#endif


