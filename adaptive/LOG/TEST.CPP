#include "log.hpp"


main()
{
 int i,k;

 Log2TxtFile l("test.log");

 l.setPackSize(1024,1024*5);
// l.setLevel("@#$%^&*()");

 for(i=0;i<10;i++)
   for(k=0;k<10;k++)
     {
     l.put('!',"(%d) 1234567890\n",i*100+k);
     l.Pack();
     }
  return 0;
}
