#ifdef __cplusplus
 extern "C" {
#endif

#include <tcxlmem.h>

#ifdef __cplusplus
 }
#endif



#define Uses_MemBuf
#include <adaptive.hpp>

class EMSMemBuf:public MemBuf
{
  public:
    EMSMemBuf(unsigned long size,int Page,int _ndx,int _ndx_conut);
    virtual ~EMSMemBuf(void);

   virtual void reset(void);
   virtual int setSize(unsigned long size);
   virtual unsigned long getSize(void);

   virtual void huge putNext(float val);
   virtual float huge getNext(void);
   virtual float huge get(signed long idx);
   virtual float huge operator[](unsigned long idx);

 private:
  unsigned long Pages;
  _HdlT Hdl;
  int isEMS,HdwPage,ndx,ndx_count,isFirst;
  float *MapedBuf;
  unsigned long LclPage;

  static EMSMemBuf *Top;
  EMSMemBuf *List;

  void CheckNextPage(void);
  _HdlT SearchEMSHandle(void);
};

