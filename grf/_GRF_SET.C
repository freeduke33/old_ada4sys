#include <stddef.h>

#ifdef TURBOC
  #include <values.h>
#else
  #include <float.h>
  #define MINFLOAT FLT_MIN
#endif

#include "_grf.h"


int far RangeGRF(int wind,float Xmin,float Ymin,float Xmax,float Ymax)
{
 windowGRF w;
 float tmp;
 CheckGRF();
 w=getWindowGRF(wind);if(w==NULL) return(errWindGRF);

 tmp=MaxGRF(Xmax,Xmin);Xmin=MinGRF(Xmax,Xmin);Xmax=tmp;
 tmp=MaxGRF(Ymax,Ymin);Ymin=MinGRF(Ymax,Ymin);Ymax=tmp;

 if(Xmax==Xmin) {Xmax+=MINFLOAT;Xmin-=MINFLOAT;}
 if(Ymax==Ymin) {Ymax+=MINFLOAT;Ymin-=MINFLOAT;}

 w->Xmax=Xmax;
 w->Ymax=Ymax;
 w->Xmin=Xmin;
 w->Ymin=Ymin;
 w->state|=grfCHANGE;
 return(okGRF);
}

int far ColorGRF(int wind,int graph,int box,int wbox,int axis,int title_fore,
	 int title_back,int act_fore,int act_back,int num,int bkg,int wbkg)
{
 windowGRF w;
 int col=0;

 CheckGRF();
 w=getWindowGRF(wind);if(w==NULL) return(errWindGRF);

 if(bkg   !=-1)
   {
   verifyCol(&col,&bkg);
   w->col.bkg   =bkg;
   }
 else bkg=w->col.bkg;
 if(wbkg  !=-1)
   {
   verifyCol(&col,&wbkg);
   w->col.wbkg  =wbkg;
   }
 else wbkg=w->col.wbkg;


 if(graph !=-1)
   {
   verifyCol(&graph,&bkg);
   w->col.graph =graph;
   }
 if(box   !=-1)
   {
   verifyCol(&box,&bkg);
   w->col.box   =box;
   }
 if(wbox  !=-1)
   {
   verifyCol(&wbox,&col);
   w->col.wbox  =wbox;
   }
 if(axis  !=-1)
   {
   verifyCol(&axis,&bkg);
   w->col.axis  =axis;
   }

 if(title_back !=-1)
   {
   verifyCol(&col,&title_back);
   w->col.title_back=title_back;
   }
 else title_back=w->col.title_back;
 if(title_fore !=-1)
   {
   verifyCol(&title_fore,&title_back);
   w->col.title_fore =title_back;
   }

 if(act_back !=-1)
   {
   verifyCol(&col,&act_back);
   w->col.act_back=act_back;
   }
 else act_back=w->col.act_back;
 if(title_fore !=-1)
   {
   verifyCol(&act_fore,&act_back);
   w->col.act_fore =act_back;
   }

 if(num   !=-1)
   {
   verifyCol(&num,&wbkg);
   w->col.msg   =num;
   }
 return(okGRF);
}


int far GSetGRF(int wind,int Mode,int width,int pattern)
{
 windowGRF w;

 CheckGRF();
 w=getWindowGRF(wind);if(w==NULL) return(errWindGRF);

 w->set.graph.style=Mode;
 w->set.graph.width=width;
 w->set.graph.pattern=pattern;
 return(okGRF);
}

int far BSetGRF(int wind,int Mode,int width,int pattern)
{
 windowGRF w;

 CheckGRF();
 w=getWindowGRF(wind);if(w==NULL) return(errWindGRF);
 w->set.box.style=Mode;
 w->set.box.width=width;
 w->set.box.pattern=pattern;
 return(okGRF);
}

int far WSetGRF(int wind,int Mode,int width,int pattern)
{
 windowGRF w;

 CheckGRF();
 w=getWindowGRF(wind);if(w==NULL) return(errWindGRF);
 w->set.wbox.style=Mode;
 w->set.wbox.width=width;
 w->set.wbox.pattern=pattern;
 return(okGRF);
}

int far ASetGRF(int wind,int Mode,int width,int pattern)
{
 windowGRF w;

 CheckGRF();
 w=getWindowGRF(wind);if(w==NULL) return(errWindGRF);
 w->set.axis.style=Mode;
 w->set.axis.width=width;
 w->set.axis.pattern=pattern;
 return(okGRF);
}

int far TitleGRF(int wind,char *title)
{
 windowGRF w;
 int i;

 CheckGRF();
 w=getWindowGRF(wind);if(w==NULL) return(errWindGRF);
 for(i=0;i<29;i++) w->Title[i]=title[i]; w->Title[i]=0;
 return(okGRF);
}



int far MSetGRF(int wind,int Font)
{
 windowGRF w;

 CheckGRF();
 w=getWindowGRF(wind);if(w==NULL) return(errWindGRF);
 w->set.msg_font=Font;
 return(okGRF);
}

int far TsetGRF(int wind,int Font)
{
 windowGRF w;

 CheckGRF();
 w=getWindowGRF(wind);if(w==NULL) return(errWindGRF);
 w->set.title_font=Font;
 return(okGRF);
}

int far NsetGRF(int wind,int Font)
{
 windowGRF w;

 CheckGRF();
 w=getWindowGRF(wind);if(w==NULL) return(errWindGRF);
 w->set.num_font=Font;
 return(okGRF);
}

int far GfillGRF(int wind,int mode,char far *pattern)
{
 windowGRF w;
 int i;

 CheckGRF();
 w=getWindowGRF(wind);if(w==NULL) return(errWindGRF);
 w->set.grf.fill_mode=mode;
 for(i=0;i<8;i++) w->set.grf.pattern[i]=pattern[i];
 return(okGRF);
}

int far BfillGRF(int wind,int mode,char far *pattern)
{
 windowGRF w;
 int i;

 CheckGRF();
 w=getWindowGRF(wind);if(w==NULL) return(errWindGRF);
 w->set.wind.fill_mode=mode;
 for(i=0;i<8;i++) w->set.wind.pattern[i]=pattern[i];
 return(okGRF);
}


int far ModeGRF(int wind,int Control)
{
 windowGRF w;

 CheckGRF();
 w=getWindowGRF(wind);if(w==NULL) return(errWindGRF);
 w->state=(Control&(0xff0))|(w->state&0x0f);
 SetWindGRF(w);
 return(okGRF);
}
