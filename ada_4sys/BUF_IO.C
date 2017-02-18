#include <string.h>
#include <io.h>

#define BUF_SIZE 4096
static char buffer[BUF_SIZE];
static int pos=0,len=0;

int BufInit(void)
{
 return pos=len=0;
}


int BufRead(int f,void *buf,int size)
{

 if(pos+size>len)
   {
   if(len) memmove(buffer,&buffer[pos],len-pos);
   len-=pos;
   pos=0;
   len+=read(f,&buffer[len],BUF_SIZE-len);
   }
 memcpy(buf,&buffer[pos],size);
 pos+=size;
 return size;
}

int BufWrite(int f,void *buf,int size)
{
 if(pos+size>BUF_SIZE)
   {
   write(f,buffer,pos);
   pos=0;
   }
 memcpy(&buffer[pos],buf,size);
 pos+=size;
 return size;
}

int BufFlush(int f)
{
 write(f,buffer,pos);
 return pos=0;
}





