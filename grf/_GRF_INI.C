static char __GRF_VER__[]=     "GRF library v1.03 by D.Kokorev (C)";


#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "_grf.h"




/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                 Ограничения пакета                       */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


#define MININT -3000
#define MAXINT  3000
#define sizeofinternalGRF sizeof(struct _internal_GRF)
#define RANGEDISP 1000


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                 Глобальные переменные                    */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

void near _pointxGRF(windowGRF w,float x1,float y1,float x2,float y2,
                    int col,struct _grf_set *set);

#define Clip 1

int maxWindGRF;               /* число открытых окон */
windowGRF TopWindowGRF;       /* последнее открытое окно */
appendGRF TopAppendGRF;       /* последний открытый аппендикс */
appendGRF _SAVEappendGRF;     /* последний использованный аппендикс */
struct _internal_GRF _windAppendicsGRF;  /* используется при рисовании */
                              /* графиков в аппендиксе */
int GRFstate;
void (far *ShifthookGRF)(int wind,windowGRF w);
int  (far *TracehookGRF)(int wind,int num,bufferGRF *buf,char type)=NULL;
int far AdapterGRF;
unsigned int disp_height,disp_width;

struct viewporttype disp;
int far aPage=0,                   /* активная страница */
    far vPage=0;                   /* визуальная страница */
unsigned far Vseg,                  /* видео сегмент */
         far Voff=0,                /* смещение 0 страницы */
         far Voff1=0;               /* смещение 1 страницы */

int far BackgroundGRF=0;
char far StrLeftRangeGRF[20]="%-5.3G";
char far StrRightRangeGRF[20]="%5.3G";

/* область хранения текущего состояния системы */
struct viewporttype save_port;
#ifdef __TURBOC__
struct fillsettingstype save_fill;
struct linesettingstype save_line;
struct textsettingstype save_text;
#endif
int save_col,save_bkg,save_x,save_y;
char save_userfill[8];
struct _grf_col grf_col;
struct _grf_set grf_set;
int grf_box;

const struct _grf_col grf_default_col=
{CYAN,WHITE,WHITE,LIGHTGREEN,
 BLUE,LIGHTGRAY,
 LIGHTGRAY,BLACK,
 YELLOW,LIGHTBLUE,BLACK};

#ifdef __WATCOMC__
const char FillPattern[][8]={
       {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}, // SOLID_FILL
       {0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55}, // HATCH_FILL
       {0x88,0x22,0x88,0x22,0x88,0x22,0x88,0x22}  // DOT_FILL
       };
const int LinePattern[]={
       0xFFFF,  // SOLID_LINE
       0xF0F0   // DASHED_LINE
       };


char *FillPat(int M,char *pattern)
{
  if(M==USERBIT_FILL) return pattern;
  else return FillPattern[M];
}

int LinePat(int M,int pattern)
{
  if(M==USERBIT_LINE) return pattern;
  else return LinePattern[M];
}
#endif



const struct _grf_set grf_default_set=
{
 {SOLID_LINE,0,NORM_WIDTH},  /* graph    */
 {SOLID_LINE,0,NORM_WIDTH},  /* box      */
 {SOLID_LINE,0,NORM_WIDTH},  /* wbox     */
 {SOLID_LINE,0,NORM_WIDTH},  /* axis     */
 {SOLID_LINE,0,NORM_WIDTH},  /* mark     */
 {SOLID_FILL},               /* graphics */
 {HATCH_FILL},               /* window   */
 DEFAULT_FONT,DEFAULT_FONT,DEFAULT_FONT /* msg,title,num */
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                 Текст процедур                           */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define round(x) (int)(((x)<0) ? ((x)-0.5) : ((x)+0.5))

char far ActiveFlag=0;

int far InitGRF(char Control)
{
 int Mode=0;

 if(ActiveFlag) return(okGRF);

#ifdef __TURBOC__
 AdapterGRF=DETECT;

 GRFstate=Control;
 if(Control&grfGRAPHICS)
 {
  initgraph(&AdapterGRF,&Mode,"");
  if(graphresult()!=grOk) return(errAdapterGRF);
 }
 detectgraph(&AdapterGRF,&Mode);
 if(graphresult()!=grOk) return(errAdapterGRF);
 if(AdapterGRF==CGA&&GRFstate&grfCOLOR && getgraphmode()!=CGAHI)
   {
   setgraphmode(CGAC0);
   setviewport(0,0,319,199,1);
   }

   getviewsettings(&disp);
   disp_height = disp.bottom - disp.top;
   disp_width  = disp.right  - disp.left;
#else
 GRFstate=Control;
  {
    struct videoconfig vc;

    _getvideoconfig(&vc);
    switch(vc.adapter) 
       {
       case _VGA:   Mode=_VRES16COLOR; break;
       case _SVGA:  Mode=_SVRES16COLOR;break;
       case _EGA:   Mode=_ERESCOLOR;   break;
       default:     Mode=_MAXRESMODE;  break;
       }

    _setvideomode(Mode);
    _getvideoconfig(&vc);
    disp_height = vc.numypixels;
    disp_width  = vc.numxpixels;
    disp.left=0; disp.top=0;
    disp.right=disp_width;
    disp.bottom=disp_height;
    disp.maxcol=vc.numcolors;
  }
#endif

    grf_set = grf_default_set;
    grf_col = grf_default_col;
    verifyCol(&grf_col.graph,   &grf_col.bkg);
    verifyCol(&grf_col.box,     &grf_col.bkg);
    verifyCol(&grf_col.axis,    &grf_col.bkg);
    verifyCol(&grf_col.act_back,&grf_col.wbkg);
    verifyCol(&grf_col.msg,     &grf_col.wbkg);


 grf_box = 30;
 _SAVEappendGRF=NULL;
 maxWindGRF=0;

 ActiveFlag++;
 return(okGRF);
}

int far DeinitGRF(int ctrl)
{
 windowGRF w,w1;
 appendGRF a,a1;

 CheckGRF();
 ActiveFlag=0;

 w=TopWindowGRF;
 while(w!=NULL)
 {
  w1 = w->next;
  freeGRF(w);
  w = w1;
 }
 a=TopAppendGRF;
 while(a!=NULL)
 {
  a1 = a->next;
  freeGRF(a);
  a = a1;
 }

 _SAVEappendGRF = NULL;
#ifdef __TURBOC__
 if(ctrl&grfTEXT) closegraph();
#else
 if(ctrl&grfTEXT) _setvideomode(_DEFAULTMODE);
#endif
 TopWindowGRF=NULL;
 TopAppendGRF=NULL;
 return(okGRF);
}



int far OpenGRF(int xlo,int ylo,int xhi,int yhi,
            float Xmin,float Ymin,float Xmax,float Ymax,
            char *title,int ctrl)
{
 windowGRF w;
 int i;

 CheckGRF();
 w=allocGRF(sizeofinternalGRF);
 if(w==NULL) return(errNomemGRF);

 w->state=ctrl;
 w->xlo=xlo;
 w->ylo=ylo;
 w->xhi=xhi;
 w->yhi=yhi;
 SetWindGRF(w);

 if(w->x1>=w->x2||w->y1>=w->y2) {freeGRF(w);return(errPositionGRF);}
 w->Xmax=Xmax;
 w->Ymax=Ymax;
 w->Xmin=Xmin;
 w->Ymin=Ymin;
 w->next=TopWindowGRF;
 TopWindowGRF=w;
 w->col=grf_col;
 w->set=grf_set;
 for(i=0;i<29;i++)  w->Title[i]=title[i]; w->Title[i]=0;
 w->H=++maxWindGRF;
 w->state=ctrl|grfCHANGE;   /* рассчитать коэффициенты */
 RedrawGRF();
 w->state=ctrl|grfBREAK;
 return(w->H);
}

int far AppendGRF(int wind,int col,int state)
{
 windowGRF w;
 appendGRF a;
 int c;

 CheckGRF();
 w=getWindowGRF(wind);if(w==NULL) return(errWindGRF);
 if(_SAVEappendGRF!=NULL) /*  если присоединяю к апендиксу, */
    w=_SAVEappendGRF->top;/*  беру его родителя             */

 a=allocGRF(sizeof(*a));
 if(a==NULL) return(errNomemGRF);
 a->next=TopAppendGRF;
 TopAppendGRF=a;
 w->append++;
 a->top=w;
 a->state=state|grfBREAK;
 c=w->col.bkg;
 verifyCol(&col,&c);
 a->col=col;
 return(a->H=++maxWindGRF);
}

int far CloseGRF(int wind)
{
 windowGRF w, top=TopWindowGRF,*tmp;
 appendGRF   atop=TopAppendGRF,*atmp;

 CheckGRF();
 w=getWindowGRF(wind);if(w==NULL) return(errWindGRF);

 tmp=&TopWindowGRF;
 while(top)
 {
  if(top->H==wind)
  {
   *tmp=top->next;
   freeGRF(top);
   top=NULL;
  }
  else
  {
   tmp=&(top->next);
   top=top->next;
  }
 }
 atmp=&TopAppendGRF;
 while(atop)
 {
  if(atop->top==w&&atop->H==wind)
  {
   *atmp=atop->next;
   if(_SAVEappendGRF==atop) _SAVEappendGRF=NULL;
   freeGRF(atop);
   atop=NULL;
  }
  else
  {
   atmp=&(atop->next);
   atop=atop->next;
  }
 }
 return(okGRF);
}



int far RedrawGRF()
{
 windowGRF w=TopWindowGRF;

 CheckGRF();
 to_GRF();
 while(w!=NULL)
  {
   redrawxGRF(w);
   w=w->next;
  }
 GRF_to();
 return(okGRF);
}

int far PointGRF(int wind,float x,float y)
{
 windowGRF w;

 CheckGRF();
 w=getWindowGRF(wind);
 if(w!=NULL)
 {
  to_GRF();
  pointxGRF(w,x,y);
  GRF_to();
 }
 else return(errWindGRF);
 return(okGRF);
}

int far Fbuf2GRF(int wind,int num,float *bufX,float *bufY)
{
 int i;
 windowGRF w;

 CheckGRF();
 w=getWindowGRF(wind);
 to_GRF();
 if(w!=NULL)
 {
  for(i=0;i<num;i++) pointxGRF(w,bufX[i],bufY[i]);
  i=okGRF;
 }
 else i=errWindGRF;
 GRF_to();
 return i;
}

int far Dbuf2GRF(int wind,int num,double *bufX,double *bufY)
{
 int i;
 windowGRF w;

 CheckGRF();
 w=getWindowGRF(wind);
 to_GRF();
 if(w!=NULL)
 {
  for(i=0;i<num;i++) pointxGRF(w,bufX[i],bufY[i]);
  i=okGRF;
 }
 else i=errWindGRF;
 GRF_to();
 return i;
}


/*==================[internal functions]====================*/


void far to_GRF()
{
#ifdef __TURBOC__
 save_col=getcolor();
 save_bkg=getbkcolor();
 getfillsettings(&save_fill);
 getfillpattern(save_userfill);
 getlinesettings(&save_line);
 gettextsettings(&save_text);
 getviewsettings(&save_port);
 save_x=getx();
 save_y=gety();

 setviewport(disp.left,disp.top,disp.right,disp.bottom,disp.clip);
 setwritemode(0);
#else
 _setviewport(disp.left,disp.top,disp.right,disp.bottom);
 _setplotaction(_GPSET);
#endif
 return;
}

void far GRF_to()
{
#ifdef __TURBOC__
 setcolor(save_col);
 setbkcolor(save_bkg);
 setfillstyle(save_fill.pattern,save_fill.color);
 setfillpattern(save_userfill,save_fill.color);
 setlinestyle(save_line.linestyle,save_line.upattern,save_line.thickness);
 settextjustify(save_text.horiz,save_text.vert);
 settextstyle(save_text.font,save_text.direction,save_text.charsize);
 setviewport(save_port.left,save_port.top,
             save_port.right,save_port.bottom,save_port.clip);
 setwritemode(0);
#else
 _setcolor(save_col);
 _setbkcolor(save_bkg);
 _setviewport(save_port.left,save_port.top,
             save_port.right,save_port.bottom);
 _setplotaction(_GPSET);
#endif
}

int far getNumWindGRF(windowGRF w)
{
 int ret=maxWindGRF;
 windowGRF ww;
 ww=TopWindowGRF;
 while(ww!=w||ww==NULL)
 {
  ww=ww->next;
  ret--;
 }
 if(ret<1) return(errPtrGRF);
 return(ret);
}


appendGRF far getAppendGRF(int wind)
{
 appendGRF a;

  a=TopAppendGRF;
  while(a)
  {
   if(a->H==wind) break;
   else a=a->next;
  }
 return a;
}


windowGRF far getWindowGRF(int wind)
{
 windowGRF w;
 appendGRF a;

 if(_SAVEappendGRF)
   {
    _SAVEappendGRF->x    = _windAppendicsGRF.x;
    _SAVEappendGRF->y    = _windAppendicsGRF.y;
    _SAVEappendGRF->state= _windAppendicsGRF.state;
    _SAVEappendGRF->col  = _windAppendicsGRF.col.graph;
    _SAVEappendGRF->set  = _windAppendicsGRF.set;
    _SAVEappendGRF=NULL;
   }


 w=TopWindowGRF;
 while(w)
 {
  if(w->H==wind) break;
  else w=w->next;
 }
 if(!w)
 {
  a=getAppendGRF(wind);
  if(a)
  {
   w=&_windAppendicsGRF;
   *w=*a->top;
   w->x=a->x;
   w->y=a->y;
   w->state=a->state;
   w->col.graph=a->col;
   w->set=a->set;
   _SAVEappendGRF=a;
  }
 }
 return(w);
}


void *allocGRF(unsigned int size)
{
 return(calloc(1,size));
}

int far freeGRF(void *ptr)
{
 free(ptr);
 return(okGRF);
}

void near delend(float *x1,float *y1,float *x2,float *y2)
{
 float y;
 y=*y2;
 if( *y2>MAXINT) y = MAXINT;
 if( *y2<MININT) y = MININT;
 if(*y2!=y)
 {
  if(*y2-*y1!=0) *x2 = (*x2-*x1)*(y-*y1)/(*y2-*y1)+*x1;
  *y2 = y;
 }
 return;
}


int far todisp(int x,int type)
{
 register int max;
 if(type==grfAxisX) max=disp_width;
 else max=disp_height;
 return( round((x*(float)max)/RANGEDISP) );
}

void far verifyCol(int *fore,int *back)
{
 int max_col;

#ifdef __TURBOC__
 max_col=getmaxcolor()+1;
#else
 max_col=disp.maxcol;
#endif

 *fore%=max_col;
 *back%=max_col;
 if(*fore==*back) *fore=max_col-(*fore)-1;
 return;
}

void near redrawxGRF(windowGRF w)
{
 float x,y,_x,_y;
 int old,c;
 struct __grf_style set;
 register int delt2,xx,yy,k,delt1,delt;
 char str[40];

 /* сохранить состояние окна */
 old=w->state;
 x=w->x;
 y=w->y;
 c=w->col.graph;

 /* рассчитать коэффициенты */
 if(old&grfCHANGE)
 {
   _x=w->Xmax-w->Xmin; if(_x==0) _x=1;
   w->kx = (float)(w->x2-w->x1)/(_x);
   _y=w->Ymax-w->Ymin; if(_y==0) _y=1;
   w->ky = (float)(w->y2-w->y1)/(_y);
 }

 /*  вывести окно на экран, если разрешено */
 if(!(old&grfHIDDEN))
 {
#ifdef __TURBOC__
  setviewport(disp.left,disp.top,disp.right,disp.bottom,1);
#else
  _setviewport(disp.left,disp.top,disp.right,disp.bottom);
#endif

  if(!(old&grfNoBACK))
    {
    /* очистить окно */
#ifdef __TURBOC__
   setfillpattern(w->set.grf.pattern,w->col.bkg);
    setfillstyle(w->set.grf.fill_mode,w->col.bkg);
    bar(w->x1,disp_height-w->y1,w->x2,disp_height-w->y2);
#else
 _setfillmask(FillPat(w->set.grf.fill_mode,w->set.grf.pattern));
 _setcolor(w->col.bkg);
 _rectangle(_GFILLINTERIOR,w->x1,disp_height-w->y1,w->x2,disp_height-w->y2);
#endif
    }

  if(!(old&grfNoWBOX))
    {
#ifdef __TURBOC__
    setfillpattern(w->set.wind.pattern,w->col.wbkg);
    setfillstyle(w->set.wind.fill_mode,w->col.wbkg);
    bar(w->x1w+1,disp_height-w->y2w+1,w->x2w-1,disp_height-w->y2 -1);
    bar(w->x1w+1,disp_height-w->y1 +1,w->x2w-1,disp_height-w->y1w-1);
    bar(w->x2 +1,disp_height-w->y2w+1,w->x2w-1,disp_height-w->y1w-1);
    bar(w->x1w+1,disp_height-w->y2w+1,w->x1 -1,disp_height-w->y1w-1);
#else
  _setfillmask(FillPat(w->set.wind.fill_mode,w->set.wind.pattern));
  _setcolor(w->col.wbkg);
  _rectangle(_GFILLINTERIOR,w->x1w+1,disp_height-w->y2w+1,w->x2w-1,disp_height-w->y2 -1);
  _rectangle(_GFILLINTERIOR,w->x1w+1,disp_height-w->y1 +1,w->x2w-1,disp_height-w->y1w-1);
  _rectangle(_GFILLINTERIOR,w->x2 +1,disp_height-w->y2w+1,w->x2w-1,disp_height-w->y1w-1);
  _rectangle(_GFILLINTERIOR,w->x1w+1,disp_height-w->y2w+1,w->x1 -1,disp_height-w->y1w-1);


#endif

    /* вывести заголовок графика */
    _putTitleGRF(w);

#ifdef __TURBOC__
  setviewport(disp.left,disp.top,disp.right,disp.bottom,1);
#else
  _setviewport(disp.left,disp.top,disp.right,disp.bottom);
#endif

    /* вывести внешний бордюр окна */
#ifdef __TURBOC__
    setcolor(w->col.wbox);
    setlinestyle(w->set.wbox.style,w->set.wbox.pattern,w->set.wbox.width);
    rectangle(w->x1w,disp_height-w->y2w,w->x2w,disp_height-w->y1w);
#else
    _setcolor(w->col.wbox);
    _setlinestyle(LinePat(w->set.wbox.style,w->set.wbox.pattern));
    _rectangle(_GBORDER,w->x1w,disp_height-w->y2w,w->x2w,disp_height-w->y1w);
#endif
    }

  if(!(old&grfNoGBOX))
    {
    /* вывести внутренний бордюр окна */
    xx=w->x1-1;
    yy=disp_height-w->y1+1;
#ifdef __TURBOC__
    setcolor(w->col.box);
    setlinestyle(w->set.box.style,w->set.box.pattern,w->set.box.width);
    rectangle(xx,disp_height-w->y2-1,w->x2+1,yy);
#else
    _setcolor(w->col.box);
    _setlinestyle(LinePat(w->set.box.style,w->set.box.pattern));
    _rectangle(_GBORDER,xx,disp_height-w->y2-1,w->x2+1,yy);
#endif
    }

  if(!(old&grfNoAXIS))
    {
    /* вывести оси графика */
#ifdef __TURBOC__
    setcolor(w->col.axis);
    setlinestyle(w->set.axis.style,w->set.axis.pattern,w->set.axis.width);
#else
    _setcolor(w->col.axis);
    _setlinestyle(LinePat(w->set.axis.style,w->set.axis.pattern));
#endif
    w->col.graph=w->col.axis;
    set=w->set.graph;
    w->set.graph=w->set.axis;
    w->state|=grfBREAK;
    if(w->Ymax>0 && w->Ymin<0)
       {
       pointxGRF(w,w->Xmax,0);
       pointxGRF(w,w->Xmin,0);
       w->state|=grfBREAK;
       }
    if(w->Xmax>0 && w->Xmin<0)
       {
       pointxGRF(w,0,w->Ymax);
       pointxGRF(w,0,w->Ymin);
       w->state|=grfBREAK;
       }
    w->set.graph=set;
    }

  /* вывести границы графика */
  if(!(old&grfNoWBOX) && old&grfRANGE)
    {
    sprintf(str,StrRightRangeGRF,w->Xmax);
    BTextGRF(w->H,str,w->x2-w->x1w-2,w->col.msg,w->col.wbkg,
          todisp(20,grfAxisX),todisp(30,grfAxisY),
          grfAxisX,grfREAL|grfEND|grfBOTTOM);
    sprintf(str,StrLeftRangeGRF,w->Xmin);
    BTextGRF(w->H,str,w->x1-w->x1w+2,w->col.msg,w->col.wbkg,
          todisp(20,grfAxisX),todisp(30,grfAxisY),
          grfAxisX,grfREAL|grfBEGIN|grfBOTTOM);
    sprintf(str,StrRightRangeGRF,w->Ymax);
    BTextGRF(w->H,str,w->y2-w->y1w-2,w->col.msg,w->col.wbkg,
          todisp(30,grfAxisX),todisp(20,grfAxisY),
          grfAxisY,grfREAL|grfEND|grfTOP);
    sprintf(str,StrLeftRangeGRF,w->Ymin);
    BTextGRF(w->H,str,w->y1-w->y1w+2,w->col.msg,w->col.wbkg,
          todisp(30,grfAxisX),todisp(20,grfAxisY),
          grfAxisY,grfREAL|grfBEGIN|grfTOP);
    }
#ifdef __TURBOC__
  setviewport(disp.left,disp.top,disp.right,disp.bottom,1);
#else
  _setviewport(disp.left,disp.top,disp.right,disp.bottom);
#endif

  if(!(old&grfNoAXIS) && old&grfALABEL)
    {
    /* вывести насечку на осях графика */
    }


  if(!(old&grfNoGBOX) && old&grfBLABEL)
  {
   /* вывести насечку на внутреннем бордюре */
#ifdef __TURBOC__
   setcolor(w->col.box);
   setlinestyle(w->set.box.style,w->set.box.pattern,w->set.box.width);
#else
   _setcolor(w->col.box);
    _setlinestyle(LinePat(w->set.box.style,w->set.box.pattern));
#endif
   delt=MinGRF(round((w->y2-w->y1)/100.0),todisp(10,grfAxisY));
   delt1=w->x2-w->x1+2;

    for(k=1;k<19;k++)
    {
     delt2=round(k*delt1/20.0);
#ifdef __TURBOC__
     line(xx+delt2,yy,xx+delt2,yy-delt-delt);
#else
     _moveto(xx+delt2,yy);
     _lineto(xx+delt2,yy-delt-delt);
#endif

     k++;
     delt2=round(k*delt1/20.0);
#ifdef __TURBOC__
     line(xx+delt2,yy,xx+delt2,yy-delt);
#else
     _moveto(xx+delt2,yy);
     _lineto(xx+delt2,yy-delt);
#endif
    }

   delt=MinGRF(round((w->x2-w->x1)/100.0),todisp(10,grfAxisX));
   delt1=w->y2-w->y1+2;

    for(k=1;k<19;k++)
    {
     delt2=round(k*delt1/20.0);
#ifdef __TURBOC__
     line(xx,yy-delt2,xx+delt+delt,yy-delt2);
#else
     _moveto(xx,yy-delt2);
     _lineto(xx+delt+delt,yy-delt2);
#endif
     k++;
     delt2=round(k*delt1/20.0);
#ifdef __TURBOC__
     line(xx,yy-delt2,xx+delt,yy-delt2);
#else
     _moveto(xx,yy-delt2);
     _lineto(xx+delt,yy-delt2);
#endif
    }
  }
 }

 /*  восстановить состояние окна */
 w->x=x;
 w->y=y;
 w->state=old|grfBREAK;
 w->col.graph=c;
 return;
}


void far pointxGRF(windowGRF w,float x2,float y2)
{
 if(!(w->state&grfBREAK)) _pointxGRF(w,w->x,w->y,x2,y2,w->col.graph,&w->set);
 w->x = x2;
 w->y = y2;
 w->state&=~grfBREAK;
 return;
}

void far pointaGRF(appendGRF a,float x2,float y2)
{
 if(!(a->state&grfBREAK)) _pointxGRF(a->top,a->x,a->y,x2,y2,a->col,&a->set);
 a->x=x2;
 a->y=y2;
 a->state&=~grfBREAK;
 return;
}



void near _pointxGRF(windowGRF w,float x1,float y1,float x2,float y2,
            int col, struct _grf_set *set)
{
 register int xx1,xx2,yy1,yy2;

 if(!(w->state&grfHIDDEN))
  {
  x1 = (x1 - w->Xmin) * w->kx;
  y1 = (y1 - w->Ymin) * w->ky;
  x2 = (x2 - w->Xmin) * w->kx;
  y2 = (y2 - w->Ymin) * w->ky;
  delend(&x1,&y1,&x2,&y2);
  delend(&x2,&y2,&x1,&y1);
  delend(&y1,&x1,&y2,&x2);
  delend(&y2,&x2,&y1,&x1);
  xx1 = round(x1);
  yy1 = round(y1);
  xx2 = round(x2);
  yy2 = round(y2);

#ifdef __TURBOC__
  setviewport(disp.left+w->x1,disp.top+disp_height-w->y2,
              disp.left+w->x2,disp.top+disp_height-w->y1,Clip);
  setcolor(col);
  setlinestyle(set->graph.style,set->graph.pattern,set->graph.width);
  line(xx1,w->y2-yy1-w->y1,xx2,w->y2-yy2-w->y1);
#else
  _setviewport(disp.left+w->x1,disp.top+disp_height-w->y2,
              disp.left+w->x2,disp.top+disp_height-w->y1);
  _setcolor(col);
  _setlinestyle(LinePat(w->set.graph.style,w->set.graph.pattern));
  _moveto(xx1,w->y2-yy1-w->y1);
  _lineto(xx2,w->y2-yy2-w->y1);
#endif
 }
 return;
}

void far _putTitleGRF(windowGRF w)
{
 int back,fore;
 struct __grf_fill sav;

 if(w->state&grfNoWBOX) return;

  if(w->state&grfACTIVE)
    {
    back=w->col.act_back;
    fore=w->col.act_fore;
    }
  else
    {
    back=w->col.title_back;
    fore=w->col.title_fore;
    }


  sav=w->set.wind;
  w->set.wind.fill_mode=SOLID_FILL;

  if(w->state&grfRightTTL)
  BTextGRF(w->H,w->Title,w->x2-w->x1w,
          fore,back,10,30,grfAxisX,grfREAL|grfEND|grfTOP);
  else
  if(w->state&grfLeftTTL)
  BTextGRF(w->H,w->Title,w->x1-w->x1w,
          fore,back,10,30,grfAxisX,grfREAL|grfBEGIN|grfTOP);
  else
  BTextGRF(w->H,w->Title,(w->x2w-w->x1w)/2,
          fore,back,10,30,grfAxisX,grfREAL|grfCENTER|grfTOP);


  w->set.wind=sav;
  return;
}

void far SetWindGRF(windowGRF w)
{
 int _grf_box=RANGEDISP;

 if(!(w->state&grfNoWBOX))
   {
   _grf_box=MinGRF((w->xhi-w->xlo)/10,grf_box);
   _grf_box=MinGRF((w->yhi-w->ylo)/10,_grf_box);
   }
 else _grf_box=0;

 w->x1 =todisp(w->xlo+_grf_box,grfAxisX);
 w->y1 =todisp(w->ylo+_grf_box,grfAxisY);
 w->x2 =todisp(w->xhi-_grf_box,grfAxisX);
 w->y2 =todisp(w->yhi-_grf_box,grfAxisY);
 w->x1w=todisp(w->xlo,grfAxisX);
 w->y1w=todisp(w->ylo,grfAxisY);
 w->x2w=todisp(w->xhi,grfAxisX);
 w->y2w=todisp(w->yhi,grfAxisY);
 return;
}


