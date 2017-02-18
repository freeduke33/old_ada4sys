#include <string.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

#define Uses_gen_DatFile
#include "adaptive.hpp"

#define mdl gen_DatFile
#define inherit gen_vBuffer

#define min(a,b) ( (a)>(b) ? (b) : (a) )


mdl::mdl(DWord time,char *name,Word mv): inherit(mv)
{
 Time=time;
 offset=fsize=0;
 strncpy(Name,name,80); Name[80]=0;

 handle=open(Name,O_BINARY|O_RDONLY);
 if(handle>0) fsize=min(filelength(handle)/sizeof(Word),Time);

 return;
}

mdl::~mdl()
{
        if(handle>0) close(handle);
  return;
}

DWord mdl::calcBufSize() { return fsize; }

void mdl::fillBuf(int a)
{
 DWord size;

// printf("\n\r");
 lseek(handle,offset*sizeof(Word),SEEK_SET);
 size=min((fsize-offset),EOB);
 if(size) x_read(buf[a],size);
 if(size<EOB)
         {
         lseek(handle,0,SEEK_SET);
         x_read(&buf[a][size],(EOB-size));
         size=(EOB-size);
         offset=0;
         }

 offset+=size;
 return;
}

int mdl::x_read(Word *buf,DWord siz)
{
 const MAX_READ=0xFFF0L;
 DWord i,cnt=siz*sizeof(Word)/MAX_READ;
 int ret=0;
 unsigned int last=(unsigned int)((siz*sizeof(Word))%MAX_READ);

 for(i=0;i<cnt && !ret;i++)
   {
   ret=(read(handle,buf,MAX_READ)==MAX_READ);
   buf+=MAX_READ;
   }
 if(!ret) ret=(read(handle,buf,last)==last);

 return ret;
}


int mdl::isSuccessful(char *msg)
{
 int val=gen_vBuffer::isSuccessful(msg);

 if(!val && handle<=0)
   {
   sprintf(msg,"unable to open file <%s>",Name);
   val=-1;
   }

 return val;
}

