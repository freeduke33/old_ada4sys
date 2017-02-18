#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "emsbuf.hpp"


#define size 100000L

test()
{
 EMSMemBuf Buf(size,0),Buf1(size,1);
 unsigned long i;
 float val;


 printf("\nWriting ...\n");
 for(i=1;i<=size;i++)
   {
   if(i%10==0) printf("\r%lu",i);
   if(i==4096)
        i=i;
   Buf.putNext(i);
   Buf1.putNext(i*10);
   }

 printf("\nReading ...\n");
 Buf.reset();
 Buf1.reset();
 for(i=2;i<=size;i++)
  {
  if(i%10==0) printf("\r%lu",i);
  val=Buf.getNext();
  if(fabs(val-i)>1e-7) { printf("\nError read Idx=%lu\n",i); break; }
  val=Buf1.getNext();
  if(fabs(val-i*10)>1e-7) { printf("\nError read Idx=%lu\n",i); break; }
  }
 printf("\nDone.\n");
}



main()
{
 int i;
 TcxlInit();


 for(i=0;i<100;i++) test();
}
