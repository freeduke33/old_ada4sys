#include <conio.h>
#ifdef __WATCOMC__
# include <graph.h>
# include <i86.h>
#endif
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <dos.h>

#include "common.h"


void bios_put(int x,int y,unsigned char ch,unsigned char attr);


#ifdef __TURBOC__
static int row,col;
#else
static struct rccoord old;
#endif



void tpreserv(void)
{
#ifdef __TURBOC__
 col=wherex();
 row=wherey();
#else
 old=_gettextposition();
#endif
 return;
}

void trestore(void)
{
#ifdef __TURBOC__
  gotoxy(col,row);
#else
  _settextposition(old.row,old.col);
#endif
 return;
}

void tprintf(unsigned x,unsigned y,unsigned len,unsigned col,char *fmt, ...)
{
 unsigned i,Col=col;
 va_list vparm;
 static char sbuf[256];


 va_start(vparm,fmt);
 vsprintf(sbuf,fmt,vparm); sbuf[len<256 ? len:256]=0;
 va_end(vparm);
 len+=x;

 for(i=0;sbuf[i];i++)
   switch(sbuf[i])
     {
     case '\027': if(sbuf[++i]==0xFF) Col=col; else Col=sbuf[i]; break;
     default: bios_put(x++,y,sbuf[i],Col); break;
     }
 for(;x<len;i++) bios_put(x++,y,' ',col);
 return;
}


void tprintDump(unsigned width,unsigned height,dumpType buf[])
{
 unsigned x,y,i=0;

  for(y=0;y<height;y++) for(x=0;x<width;x++)
    {
    bios_put(x,y,buf[i].Symbol,buf[i].Attr);
    i++;
    }

 return;
}


char *txtClock(unsigned long tik)
{
 static char buf[50];
 unsigned int ss,mm,hh;
 unsigned long dd;

 tik/=CLK_TCK;
 ss=(int)(tik%60); tik/=60;
 mm=(int)(tik%60); tik/=60;
 hh=(int)(tik%24); tik/=24;
 dd=tik;


 if(dd) sprintf(buf,"%lu, %02u:%02u:%02u",dd,hh,mm,ss);
 else
 if(hh) sprintf(buf,"%02u:%02u:%02u",hh,mm,ss);
 else   sprintf(buf,"%02u:%02u",mm,ss);

 return buf;
}

char *txtTime(char *s)
{
 static char buf[50];
 char w[5],m[5],d[5],t[10],y[10];


 sscanf(s,"%s %s %s %s %s",w,m,d,t,y);
 sprintf(buf,"%s-%s-%s %s",d,m,y,t);
 return buf;
}

static void bios_put(int x,int y,unsigned char ch,unsigned char attr)
{
#ifdef __WATCOMC__
 union REGPACK r;
 r.h.ah=0x2;
 r.h.bh=0;
 r.h.dl=x; r.h.dh=y;
#else
 struct REGPACK r;
 r.r_ax=0x200;
 r.r_bx=0;
 r.r_dx=x; r.r_dx+=y<<8;
#endif


 intr(0x10,&r);

#ifdef __WATCOMC__
 r.h.ah=0x9; r.h.al=ch;
 r.h.bh=0;   r.h.bl=attr;
 r.x.cx=1;
#else
 r.r_ax=ch;
 r.r_ax+=0x900U;
 r.r_bx=attr;
 r.r_cx=1;
#endif

 intr(0x10,&r);

 return;
}


unsigned long DosMemoryAvail(void)
{
#ifdef __WATCOMC__
 union REGPACK r;
 r.h.ah=0x48;
 r.x.bx=0xFFFF;
#else
 struct REGPACK r;
 r.r_ax=0x4800;
 r.r_bx=0xffff;
#endif


 intr(0x21,&r);

#ifdef __WATCOMC__
 return ((unsigned long)r.x.bx)*0x10;
#else
 return ((unsigned long)r.r_bx)*0x10;
#endif
}
