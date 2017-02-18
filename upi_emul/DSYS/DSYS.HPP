#ifndef __UPI_DSYS_HPP__
#define __UPI_DSYS_HPP__

#include <iostream.h>

#include "upi.hpp"
#include "channel.hpp"


class UPI_dsys:public UPI
{
  public:
    UPI_dsys(char *filename,int ctrl);
    ~UPI_dsys(void);

    UPI::DoLoop;
    UPI::ErrMsg;

    virtual void huge StartUPI(void);
    virtual void huge StopUPI(void);

    virtual int huge BuildUPI(void);
    virtual int huge CalcSignal(float *iBuf,float *oBuf);


    inline Channel *GetChannels(void) { return Ptr; }

  protected:

    virtual void ShowConfig(void);
    virtual int LoadConfig(istream& is);
    virtual void ResetUPI(void);

    Channel *Ptr;
};



#endif
