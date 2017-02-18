#include "Look_GRF.h"
#include "stdio.h"
#include "conio.h"
#include "bios.h"
#include "string.h"

extern struct _internal_GRF _windAppendicsGRF;


static struct _scr_params FullScr=  {1,1,999,999,1,2,3,2,1,0,1};
static int (far *ClrFun)(int mode)=NULL;
static int (far *MarkerFunc)(triadGRF *p,unsigned long num,int ch)=NULL;



int far SetLook_Clr(int (far *Fun)(int mode))
{
 ClrFun=Fun;
 return(okGRF);
}

int far SetLook_Mark(int (far *Fun)(triadGRF *p,unsigned long num,int ch))
{
 MarkerFunc=Fun;
 return(okGRF);
}



static void triadLine(triadGRF *p,float xx,int mode)
{
 windowGRF w;
 float x,y;
 int col;
 struct __grf_style set;

 w=getWindowGRF(p->H);
#ifdef __TURBOC__
 setwritemode(1);
#else
 _setplotaction(_GXOR);
#endif
 col=w->col.graph;
 set=w->set.graph;

 w->col.graph=p->scr.grf_back^p->scr.marker;
 w->set.graph=w->set.mark;
 if(mode)
 {
  w->set.graph.style=USERBIT_LINE;
  w->set.graph.pattern=0x5555;
 }
 x=w->x;
 y=w->y;
 w->state|=grfBREAK;
 pointxGRF(w,xx,w->Ymin);
 pointxGRF(w,xx,w->Ymax);
 w->x=x;
 w->y=y;
#ifdef __TURBOC__
 setwritemode(0);
#else
 _setplotaction(_GPSET);
#endif
 w->col.graph=col;
 w->set.graph=set;
 if(MarkerFunc) (*MarkerFunc)(p,p->pos,0);
 return;
}

static int __Show_param;

/*
static void strtrim(char *str)
{
   int i,k;

   for(i=0;str[i] && str[i]==' ';i++);
   for(k=0;str[i];i++,k++) str[k]=str[i];
   i--;
   for(;str[i]==' ';i--);
   str[i]=0;

 return;
}
*/

static void MessageGRF(triadGRF *p,int type,int act,unsigned long point)
{
 char str[130];
 float y,x,y1[10];
 int i;

 if(type&TITLE_MESSAGE)
 {
  if(act) ActiveGRF(p->H);
  else    NactiveGRF(p->H);
  if(p->argument && !(p->open_mode&grfNoWBOX))
          BTextGRF(p->H,p->argument,(p->scr.X2-p->scr.X1)/2,
          p->scr.msg,p->scr.box_back,10,20,grfAxisX,grfCENTER|grfBOTTOM);
 }
 if(type&PARAMS_MESSAGE)
  {
  p->GetPoint(&p->Xpos,&y,p,point);
  if(__Show_param&&p->StrParam1 && !(p->open_mode&grfNoWBOX))
    {
    for(i=1;i<10; i++) y1[i]=0;
    for(i=1;i<10 && p[i-1].ctrl&grfDOUBLE; i++)
          p[i].GetPoint(&p[i].Xpos,&y1[i],&p[i],point);

    sprintf(str,p->StrParam1,p->Xpos,y,y1[1],y1[2],y1[3],y1[4],y1[5],y1[6],y1[7],y1[8],y1[9]);
    BTextGRF(p->H,str,p->scr.X2-p->scr.X1-5,p->scr.msg,p->scr.box_back,
           12,20,grfAxisX,grfEND|grfTOP);
    }
  }
 if(type&MARKER_MESSAGE&&__Show_param&&p->StrParam2  && !(p->open_mode&grfNoWBOX))
  {
  p->GetPoint(&x,&y,p,point);

  for(i=1;i<10; i++) y1[i]=0;
  for(i=1;i<10 && p[i-1].ctrl&grfDOUBLE; i++)
          p[i].GetPoint(&p[i].Xpos,&y1[i],&p[i],point);

  if(p->StrParam2) sprintf(str,p->StrParam2,x,y,y1[1],y1[2],y1[3],y1[4],y1[5],y1[6],y1[7],y1[8],y1[9]);
  else str[0]=0;
//  strtrim(str);
  BTextGRF(p->H,str,5,p->scr.msg,p->scr.box_back,
           12,20,grfAxisX,grfBEGIN|grfTOP);
  }
 return;
}


#ifdef __WATCOMC__
#define bioskey _bios_keybrd
#endif


int far LookGRF(int num,triadGRF p[])
{
 int select=0,i,key,Exit=0,single=0,_size,max_size=0;
 triadGRF *act=NULL,*pp=NULL;
 float Xmin,Xmax;
 float y,x;
 unsigned long beg,end;

 CheckGRF();
 to_GRF();

 select=BackgroundGRF;
 for(i=0;i<num;i++)
 {
  BackgroundGRF=p[i].scr.grf_back;
  if(p[i].MaxMin && p[i].ctrl&grfSCALE_FIL) p[i].MaxMin(&p[i]);
  p[i].H=OpenGRF(p[i].scr.X1,p[i].scr.Y1,p[i].scr.X2,p[i].scr.Y2,
                 p[i].Xmin,p[i].Ymin,p[i].Xmax,p[i].Ymax,
                 p[i].title,grfHIDDEN|p[i].open_mode);
  ColorGRF(p[i].H,p[i].scr.grf_fore,p[i].scr.box_grf,p[i].scr.box_wind,
                p[i].scr.axis,p[i].scr.title_fore,p[i].scr.title_back,
                p[i].scr.act_fore,p[i].scr.act_back,p[i].scr.num,
                p[i].scr.grf_back,p[i].scr.box_back);
  _size=1;
  p[i].Win=getWindowGRF(p[i].H);
  key=i;
  while((p[i].ctrl&grfDOUBLE)&&(i!=(num-1)))
  {
   i++;_size++;
   if(p[i].MaxMin && p[key].ctrl&grfSCALE_FIL)
     {
     p[i].MaxMin(&p[i]);
     p[key].Xmax=MaxGRF(p[key].Xmax,p[i].Xmax);
     p[key].Ymax=MaxGRF(p[key].Ymax,p[i].Ymax);
     p[key].Xmin=MinGRF(p[key].Xmin,p[i].Xmin);
     p[key].Ymin=MinGRF(p[key].Ymin,p[i].Ymin);
     }
   p[i].H=AppendGRF(p[key].H,p[i].scr.grf_fore,grfHIDDEN|p[key].open_mode);
   p[i].Win=NULL;
   p[i].App=getAppendGRF(p[i].H);
  }
  for(select=key+1;select<=i;select++)
    {
    RangeGRF(p[select].H,p[key].Xmin,p[key].Ymin,
                         p[key].Xmax,p[key].Ymax);
    }
  if(_size>max_size) max_size=_size;
 }
 BackgroundGRF=select;
 select=0;
 pp=allocGRF(max_size*sizeof(triadGRF));
 if(pp==NULL) goto NoMEM;
 max_size--;

 pp[0].H=OpenGRF(FullScr.X1,FullScr.Y1,FullScr.X2,FullScr.Y2,
           0,0,1,1,"!",grfHIDDEN);
 pp[0].Win=getWindowGRF(pp[0].H);
 for(_size=1;_size<=max_size;_size++)
   {
   pp[_size].H=AppendGRF(pp[0].H,1,grfHIDDEN);
   pp[_size].Win=NULL;
   pp[_size].App=getAppendGRF(pp[_size].H);
   }

 key=255;
 while(!Exit)
 {
  if(key==255||bioskey(1))
  {
   if(key!=255) key=bioskey(0);
   switch(key)
   {
    case 0x3920:  /* SPACE: set step ratio */
     if(act->step==1) act->step=MaxGRF((act->end-act->beg)/20,1);
     else act->step=1;
     break;
    case 0x0E08:  /* BACK: return from first to all windows */
      if(!single) break;
      __Show_param-=10;
      for(i=0;i<=max_size;i++) ShowGRF_off(pp[i].H);
      single=0;
      act=&p[select];
    case 255:  /* define all open windows */
      key=0;
      for(i=0;i<num;i++)
      {
       ShowGRF_on(p[i].H);
       BreakGRF(p[i].H);
       p[i].pos=p[i].cur=p[i].beg;
       p[i].step=MaxGRF((p[i].end-p[i].beg)/20,1);
       p[i].mark=p[i].first=0;
      }
      act=&p[select];
#ifdef __TURBOC__
      if(ClrFun==NULL || ClrFun(grfCLR_FULL)!=okGRF) cleardevice();
#else
      if(ClrFun==NULL || ClrFun(grfCLR_FULL)!=okGRF) _clearscreen(_GCLEARSCREEN);
#endif
      RedrawGRF();
      for(i=0;i<num;i++)
      {
       if(!(p[i].ctrl&grfPARAM_ALL)) __Show_param=0;
       else __Show_param=1;
       MessageGRF(&p[i],ALL_MESSAGE,(i==select),i);
       while(p[i].ctrl&grfDOUBLE) i++;
      }
    break;
    case 0x0F09:   /* TAB: select next window */
      i=select;
      if(single) break;
      do
        {
        while(p[select].ctrl&grfDOUBLE) select++;
        select++;
        if(select>=num) select=0;
        }
      while(p[i].ctrl&grfUNSELECT);

      if(i!=select)
      {
       MessageGRF(&p[i],TITLE_MESSAGE,0,0);
       MessageGRF(&p[select],TITLE_MESSAGE,1,0);
       act=&p[select];
       if(!(p[i].ctrl&grfPARAM_ALL)) __Show_param=0;
       else __Show_param=1;
      }
    break;
    case 0x1C0D:  /* ENTER: set marker */
      if(act->ctrl&grfUNMARK)
         {
         if(act==pp) break;
         act->mark=1; act->use=act->beg; act->pos=act->end;
         }
      if(!act->mark||(act->use==act->pos&&act->mark))
       {
        act->use=act->pos;
        act->mark=!act->mark;
        MessageGRF(act,MARKER_MESSAGE,select,act->pos);
        triadLine(act,act->Xpos,1);
       }
       else
       {
        if(act->use==act->pos) break;
        __Show_param+=10;
        for(i=0;i<num;i++) ShowGRF_off(p[i].H);

        single=pp[0].H;memcpy(&pp[0],act,sizeof(pp[0]));pp[0].H=single;
        pp[0].Win=getWindowGRF(pp[0].H);
        act=&pp[0];
        pp[0].scr.X1=FullScr.X1;
        pp[0].scr.Y1=FullScr.Y1;
        pp[0].scr.X2=FullScr.X2;
        pp[0].scr.Y2=FullScr.Y2;

        i=-1;
        beg=MinGRF(pp[0].pos,pp[0].use);
        end=MaxGRF(pp[0].pos,pp[0].use);
        do{
          i++;
          if(i)
             {
             single=pp[i].H;
             memcpy(&pp[i],&p[select+i],sizeof(pp[i]));
             pp[i].H=single;
             pp[i].Win=NULL;
             pp[i].App=getAppendGRF(pp[i].H);
             }

          pp[i].mark=pp[0].first=0;
          pp[i].GetPoint(&Xmin,&y,&pp[i],beg);
          pp[i].GetPoint(&Xmax,&y,&pp[i],end);
          pp[i].beg=beg;
          pp[i].end=end;
          pp[i].Xmin=Xmin;
          pp[i].Xmax=Xmax;

          if(pp[i].MaxMin && pp[0].ctrl&grfSCALE_FRM)
            {
            pp[i].MaxMin(&pp[i]);
            pp[0].Xmax=MaxGRF(pp[0].Xmax,pp[i].Xmax);
            pp[0].Ymax=MaxGRF(pp[0].Ymax,pp[i].Ymax);
            pp[0].Xmin=MinGRF(pp[0].Xmin,pp[i].Xmin);
            pp[0].Ymin=MinGRF(pp[0].Ymin,pp[i].Ymin);
            }
        }while(pp[i].ctrl&grfDOUBLE&&i<max_size);

        pp[0].step=MaxGRF((pp[0].end-pp[0].beg)/20,1);
        pp[0].pos=pp[0].cur=pp[0].beg;
        i=-1;
        do{
          i++;
          RangeGRF(pp[i].H,pp[0].Xmin,pp[0].Ymin,pp[0].Xmax,pp[0].Ymax);
          ModeGRF(pp[i].H,pp[i].open_mode);
          ColorGRF(pp[i].H,pp[i].scr.grf_fore,pp[0].scr.box_grf,pp[0].scr.box_wind,
                pp[0].scr.axis,pp[0].scr.title_fore,pp[0].scr.title_back,
                pp[0].scr.act_fore,pp[0].scr.act_back,pp[0].scr.num,
                pp[0].scr.grf_back,pp[0].scr.box_back);
          BreakGRF(pp[i].H);
          ShowGRF_on(pp[i].H);
                }while(pp[i].ctrl&grfDOUBLE&&i<max_size);

        if(pp[0].StrParamF) pp[0].StrParam1=pp[0].StrParamF;
        TitleGRF(pp[0].H,pp[0].title);
#ifdef __TURBOC__
        if(ClrFun==NULL || ClrFun(grfCLR_FRM)!=okGRF) cleardevice();
#else
        if(ClrFun==NULL || ClrFun(grfCLR_FRM)!=okGRF) _clearscreen(_GCLEARSCREEN);
#endif
        RedrawGRF();
        single=1;

        MessageGRF(act,ALL_MESSAGE,1,act->pos);
       }
    break;
    case -1:    /* Ctrl-Break or ESC : exit */
    case 0x011B: Exit=1; break;
    case 0x4700:
       if(act->first) triadLine(act,act->Xpos,0); act->first=1;
       act->pos=act->beg;
       MessageGRF(act,PARAMS_MESSAGE,0,act->pos);
       triadLine(act,act->Xpos,0);
       break;
    case 0x4F00:
       if(act->first) triadLine(act,act->Xpos,0); act->first=1;
       act->pos=act->end;
       MessageGRF(act,PARAMS_MESSAGE,0,act->pos);
       triadLine(act,act->Xpos,0);
       break;
    case 0x4D00:
       if(act->first) triadLine(act,act->Xpos,0); act->first=1;
       if(act->pos!=act->end)
       {
        if(act->pos+act->step>act->end) act->pos=act->end;
        else act->pos+=act->step;
        MessageGRF(act,PARAMS_MESSAGE,0,act->pos);
       }
       triadLine(act,act->Xpos,0);
      break;
    case 0x4B00:
       if(act->first) triadLine(act,act->Xpos,0); act->first=1;
       if(act->pos!=act->beg)
       {
        if(act->pos<act->beg+act->step) act->pos=act->beg;
        else act->pos-=act->step;
        MessageGRF(act,PARAMS_MESSAGE,0,act->pos);
       }
        triadLine(act,act->Xpos,0);
      break;
    default: if(MarkerFunc) (*MarkerFunc)(p,p->pos,key);
   }
  } /* end key if */
  if(single)
  {
   if(act->cur<=act->end)
   {
    i=0;
    while(pp[i].ctrl&grfDOUBLE&&i<max_size) i++;
    for(;i>=0;i--)
      {
      pp[i].GetPoint(&x,&y,&pp[i],act->cur);
      if(pp[i].Win) pointxGRF(pp[i].Win,x,y);
      else pointaGRF(pp[i].App,x,y);
      }
    act->cur++;
   }
  }
  else
  {
   int m,mm;
   for(i=0;i<num;i++)
   {
    m=i;
    while(p[i].ctrl&grfDOUBLE&&(i-m)<max_size) i++;
    for(mm=i;mm>=m;mm--) if(p[mm].cur<=p[mm].end)
      {
      p[mm].GetPoint(&x,&y,&p[mm],p[mm].cur++);
      if(p[mm].Win) pointxGRF(p[mm].Win,x,y);
      else pointaGRF(p[mm].App,x,y);
      }
   }
  }
 }
 for(i=0;i<=max_size;i++) CloseGRF(pp[i].H);
 freeGRF(pp);

 NoMEM:
 for(i=0;i<num;i++) CloseGRF(p[i].H);
 ClrFun=NULL;
 GRF_to();
 return(0);
}



