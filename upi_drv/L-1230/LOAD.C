
/* ====[ ПРОЦЕДУРА ЗАГРУЗКИ УПРАВЛЯЮЩЕЙ ПРОГРАММЫ В ПЛАТУ LCard-12xx ]==== */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>

#ifndef __WATCOMC__
# include <alloc.h>
#endif

#include <conio.h>


#include "lcard.h"

static unsigned char PageLength;
static int GlobalN;


static int fill(unsigned char *LP, unsigned int n)
{
unsigned char *ptr, up, lo, mi;
int i, j;

if(n % 24) return 1;
PageLength=n/24-1;

ptr=malloc(n+10);
if(ptr == NULL) return 1;

// запишем длину
ptr[0]=PageLength;

// запишем программу с хвоста
for(i=n-3, j=1; i >= 0; i-=3, j+=3)
        {
        up=LP[i];
        mi=LP[i+1];
        lo=LP[i+2];

        ptr[j]=up;
        ptr[j+1]=lo;
        ptr[j+2]=mi;
        }
n++;

for(i=0; i < n; i++) LP[i]=ptr[i];
GlobalN=n;

free(ptr);
return 0;
}

static int reset(int Base,char *ptr)
{
int i, j;

outport(CONTROL+Base, 0);
outport(DATA+Base, ptr[0]);
outport(CONTROL+Base, 8);
for(i=1; i < GlobalN+1; i++) { outport(DATA+Base, ptr[i]); for(j=0; j < 10; j++); }

return 0;
}

int echo(unsigned char *ptr)
{
 FILE *f;
 int i;

 f=fopen("upi_bios.hex","w+t");
 fprintf(f,"bios_cnt dw\t%u\nbios_mem:\n",GlobalN);
 if(f) for(i=0;i<GlobalN;i++)
   {
   fprintf(f,"\tdb\t%03Xh\n",(unsigned)ptr[i]);
   }
 fclose(f);      

 return 0;
}


static int waitDSP(int Base)
{
 long l=0;
 while(!(inport(Base+CONTROL)&1) && l++<500000);
 return inport(Base+DATA);
}

static int test(int Base)
{
 int c1,c2;

 outport(COMMAND+Base,0);

 c1=waitDSP(Base);
 c2=waitDSP(Base);
 return (c1==0x66 && c2==0x99) ? lc_NONE : lc_NORESPONSE;
}


lcRet LoadBios(int base,char *filename)
{
 FILE *FilePtr;
 unsigned char ExeHeader[]={0x1b, 0x1b, 0x69, 0x0d, 0x0a, 0x40, 0x50, 0x41,
                            0x0d, 0x0a, 0x30, 0x30, 0x30, 0x30, 0x0d, 0x0a};
 int NHeader=16;                           
 static unsigned char Buffer[32000];
 static unsigned char LoadingProgram[3*1024];
 unsigned /*left, right,*/ i, j, tail, n/*, dsl_length, no_talk, n1*/;
 unsigned char StrBuf[10]/*, buffer[200]*/;
 unsigned long l;


 FilePtr=fopen(filename, "rb");
 if(FilePtr == NULL) return lc_NOBIOSFILE;
 l=fread(Buffer, 1, 32000, FilePtr);
 fclose(FilePtr);
 for(i=0; i < NHeader-6; i++) if(Buffer[i] != ExeHeader[i]) return lc_BADBIOSFILE;

 // удалим шапку EXE файла
 for(i=0; i < l-NHeader; i++) Buffer[i]=Buffer[i+NHeader];

 // найдем конец программной секции
 for(i=1, l-=NHeader; i < l; i++)
   if(  Buffer[i-1] == 0x0a && Buffer[i] == 0x23 && Buffer[i+1] == 0x31 &&
                Buffer[i+2] == 0x32 && Buffer[i+3] == 0x33) break;
 if((tail=i) == l) return lc_NOTAIL;


 // удалим переходы \r\n
 for(i=j=0, n=tail-tail/4; i < n; i++)
        {
        Buffer[i]=Buffer[j];
        if(!((i+1) % 6) && i) j+=3; else j++;
        }

 if(n/6 > 1000) return lc_BADBIOSFILE;

 // перекодируем в двоичный формат
 for(i=j=0; i < n && j < 6000; i+=2, j++)
        {
        sprintf(StrBuf, "0x%c%c", Buffer[i], Buffer[i+1]);
        LoadingProgram[j]=strtoul(StrBuf, NULL, 0);
        }
 n/=2;

 // добавим до кратности 24 нули
 for(i=n; i < (n+(24-n%24)); i++) LoadingProgram[i]=0;
 n=i;

 printf("Loading ...");
 fill(LoadingProgram, n);
 reset(base,LoadingProgram);

 sleep(1);
 waitDSP(base);
 echo(LoadingProgram);

 return test(base);
}

