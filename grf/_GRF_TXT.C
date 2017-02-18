#include "_grf.h"
#include "stddef.h"
#include "limits.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#ifdef __WATCOMC__
#include "math.h"
#endif


#define RANGEDISP 1000
extern struct viewporttype disp;
#ifdef __TURBOC__
extern void far pascal GRF8x8(void);
void (far *_fontGRF)(void)=(void far *)GRF8x8;
#endif

int fontGRF=0;
void near parseRANGE(int  *_mul,int *_div,long num);
extern int grf_box;



/* ========================================================== */

int far BTextGRF(int wind,char far *str,int ofs,int col,int bkg,
             int dx,int dy,int axis,int mode)
{
 windowGRF w;
 int X,Y;

 CheckGRF();
 w=getWindowGRF(wind);
 if(w==NULL) return(errWindGRF);
 if(w->state&grfHIDDEN) return(errHiddenGRF);

 if(!(mode&grfREAL))
    {
    dx=todisp(dx,grfAxisX);
    dy=todisp(dy,grfAxisY);
    }

 if(axis==grfAxisX)
   {
   if(!(mode&grfREAL))    ofs=todisp(ofs,grfAxisX);
   dx=MinGRF((w->x2-w->x1)/strlen(str)-4,dx);
   dy=MinGRF(w->y2w-w->y2-4,dy);
   dy=MaxGRF(dy,0);
   X=w->x1w+ofs;
   Y=((float)(w->y2w-w->y2)/2.0-0.5);
   if(mode&grfTOP) Y+=w->y2;
   else Y+=w->y1w;
   }
 else
   {
   if(!(mode&grfREAL))    ofs=todisp(ofs,grfAxisY);
   dx=MinGRF(w->x2w-w->x2-2,dx);              dx=MaxGRF(dx,0);
   dy=MinGRF((w->y2-w->y1)/strlen(str),dy);   dy=MaxGRF(dy,0);
   Y=w->y1w+ofs;
   X=(w->x2w-w->x2)/2;
   if(mode&grfTOP) X+=w->x1w;
   else X+=w->x2;
   }
#ifdef __TURBOC__
 setfillpattern(w->set.wind.pattern,0);
#else
#endif
 TextGRF(str,X,disp_height-Y,col,bkg,dx,dy,axis,
         w->set.wind.fill_mode,mode|grfREAL);
 return(okGRF);
}


int far WTextGRF(int wind,char *str,float x,float y,int col,int bkg,
             int dx,int dy,int axis,int mode)
{
 windowGRF w;

 CheckGRF();
 w=getWindowGRF(wind);
 if(w==NULL) return(errWindGRF);
 if(w->state&grfHIDDEN) return(errHiddenGRF);
 x= ((x - w->Xmin) * w->kx + w->x1) * RANGEDISP /disp_width;
 y= ((y - w->Ymin) * w->ky + w->y1) * RANGEDISP /disp_height;

#ifdef __TURBOC__
 setfillpattern(w->set.grf.pattern,0);
#else
 _setfillmask(FillPat(w->set.wind.fill_mode,w->set.grf.pattern));
#endif
 TextGRF(str,(int)x,(int)y,col,bkg,dx,dy,axis,w->set.grf.fill_mode,mode);
 return(okGRF);
}

void near parseRANGE(int  *_mul,int *_div,long num)
{
 if(num>INT_MAX/10)
 {
  *_mul=INT_MAX/10;
  *_div=(int)(((long)(*_mul)*RANGEDISP)/(num*8));
 }else
 {
  *_div=RANGEDISP;
  *_mul=(int)num;
 }
}

int far TextGRF(char *str,int x,int y,int col,int bkg,
            int dx,int dy,int axis,int pattern,int mode)
{
 int h1,h2,l1,l2,swp,ll,hh;
#ifndef __TURBOC__
 char buf[30];
#endif

 CheckGRF();
 to_GRF();
 verifyCol(&col,&bkg);

 if(!(mode&grfREAL))
 {
  x = todisp(x,grfAxisX);
  y = disp_height-todisp(y,grfAxisY);
  dx = todisp(dx,grfAxisX);
  dy = todisp(dy,grfAxisY);
 }

 if(axis==grfAxisY) {swp=dx;dx=dy;dy=swp;}

#ifdef __TURBOC__
 if(!fontGRF) fontGRF=registerbgifont(_fontGRF);
 settextstyle(fontGRF,axis,USER_CHAR_SIZE);
 setusercharsize(dx,8,dy,8);
#else
 _setviewport(disp.left,disp.top,disp.right,disp.bottom);
 if(!fontGRF)
    {
    fontGRF=_registerfonts("terminal.fon");
    }

 sprintf(buf,"n%d",fontGRF-1);
 ll=_setfont(buf);
// printf("%d <%s>",ll,buf);


 if(axis==grfAxisY)
   { _settextorient(0,1);_setcharsize(dy,dx); }
 else
   { _settextorient(1,0);_setcharsize(dx,dy); }
// _setcharsize(dy,dx);
#endif

#ifdef __TURBOC__
  h1=textheight(str);
  h2=h1*1/3;
  hh=h1/2-h2;
  h1=h1*2/3;
  ll=textwidth("X");
  l1=textwidth(str)-ll;
#else
  {
   struct _fontinfo fi;
   struct textsettings ts;
   _getfontinfo(&fi);
   _gettextsettings(&ts);

  if(fi.type==0)
     {
//     l1=fi.pixwidth*floor((float)dx/fi.pixwidth);
//     ll=strlen(str)*l1;
//     hh=fi.pixheight*floor((float)dy/fi.pixheight);
     ll=strlen(str)*dx;
     hh=dy;
     }
  else
     {
     hh=ts.height;
     ll=_getgtextextent(str);
     }
  h1=h2=hh/2;
  }
#endif

 if(mode&grfREVERSE)
 {
#ifdef __TURBOC__
  setcolor(bkg);
  setfillstyle(pattern,col);
#else
  _setfillmask(FillPat(pattern,"\0\0\0\0\0\0\0\0"));
#endif
 }
 else
 {
#ifdef __TURBOC__
  setcolor(col);
  setfillstyle(pattern,bkg);
#else
  _setfillmask(FillPat(/*pattern*/ 0,"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"));
#endif
 }



 if(mode&grfCENTER)
 {
#ifdef __TURBOC__
  settextjustify(CENTER_TEXT,CENTER_TEXT);
  l2=l1/2+(ll*3/5);
  l1=l1/2+(ll*2/5);
#else
  _settextalign(_CENTER,_HALF);
  l1=l2=ll/2;
#endif
 }
 if(mode&grfBEGIN)
 {
#ifdef __TURBOC__
  if(axis==grfAxisX) settextjustify(LEFT_TEXT,CENTER_TEXT);
  else settextjustify(CENTER_TEXT,BOTTOM_TEXT);
  l2=ll*3/5-ll/2;
  l1+=ll/2+ll*2/5;
#else
  _settextalign(_LEFT,_HALF);
  l1=ll;
  l2=0;
#endif
 }
 if(mode&grfEND)
 {
#ifdef __TURBOC__
  if(axis==grfAxisX) settextjustify(RIGHT_TEXT,CENTER_TEXT);
  else settextjustify(CENTER_TEXT,TOP_TEXT);
  l2=l1+ll/2+ll*3/5;
  l1=ll/2-ll*2/5;
#else
  _settextalign(_RIGHT,_HALF);
  l1=0;
  l2=ll;
#endif
 }

#ifdef __TURBOC__
 ll=ll/2-ll*2/5;
#endif


 if(axis==grfAxisY)
 {
     swp=h1;h1=l2;l2=h2;h2=l1;l1=swp;
     swp=hh;hh=ll;ll=-swp;
 }

#ifdef __TURBOC__
 x=x+ll;
 y=y-hh;
 if(!(mode&grfNOBKG)) bar(x-l2,y-h2,x+l1,y+h1);
 outtextxy(x,y,str);
#else
 if(mode&grfREVERSE) _setcolor(col);
                else _setcolor(bkg);
 if(!(mode&grfNOBKG)) _rectangle(_GFILLINTERIOR,x-l2,y-h2,x+l1,y+h1);

 if(mode&grfREVERSE) _setcolor(bkg);
                else _setcolor(col);

 _moveto(x,y); _outgtext(str);
#endif

 GRF_to();

 return(okGRF);
}


