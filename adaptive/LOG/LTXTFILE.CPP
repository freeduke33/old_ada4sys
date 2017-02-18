#include <dos.h>
#include <io.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#include <log.hpp>


Log2TxtFile::Log2TxtFile(const char *name)
{
 f=open(name,O_BINARY|O_CREAT|O_RDWR,S_IREAD|S_IWRITE);
 if(f>0) lseek(f,0,SEEK_END);
}

Log2TxtFile::~Log2TxtFile(void)
{
 if(f>0) close(f);
}

void Log2TxtFile::writeLog(const char *buf)
{
 if(f>0) write(f,buf,strlen(buf));
 return;
}

void Log2TxtFile::Pack(void)
{
 const char PackID[]=" ==[ Packed ]== \r\n";
 unsigned long p1,p2,pp;
 unsigned int i,k,l;

 if(isNeedPack())
   {
   printf("Packed ...\n");
   pp=lseek(f,0,SEEK_END);
   p1=lseek(f,pp-Ini,SEEK_SET);
   l= (Ini<sizeof(buf)) ? Ini : sizeof(buf);
   p1+=read(f,buf,l);
   for(i=0;i<l;i++) if(buf[i]=='\n') break;


   p2=lseek(f,0,SEEK_SET);
   p2=write(f,PackID,sizeof(PackID));

   k=l-i;
   if(k>0)
     {
     p2+=write(f,&buf[i],k);
     }

   while(p1<pp)
     {
     lseek(f,p1,SEEK_SET);
     k=read(f,buf,sizeof(buf));
     p1+=k;
     lseek(f,p2,SEEK_SET);
     p2+=write(f,buf,k);
     }
   _dos_write(f,buf,0,&l);
   pp=filelength(f);
   }
 return;
};

int Log2TxtFile::isNeedPack(void)
{
 unsigned long p=0;

 if(Repack && f>0) p=lseek(f,0,SEEK_END);

 return (p>Repack);
}
