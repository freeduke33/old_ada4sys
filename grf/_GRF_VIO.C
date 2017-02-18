#include "_grf.h"
#include "stddef.h"

int far ShowGRF_on(int wind)
{
 windowGRF w;

 CheckGRF();
 w=getWindowGRF(wind);
 if(w==NULL) return(errWindGRF);
 w->state&=~grfHIDDEN;
 return(okGRF);
}

int far ShowGRF_off(int wind)
{
 windowGRF w;

 CheckGRF();
 w=getWindowGRF(wind);
 if(w==NULL) return(errWindGRF);
 w->state|=grfHIDDEN;
 return(okGRF);
}

int far BreakGRF(int wind)
{
 windowGRF w;

 CheckGRF();
 w=getWindowGRF(wind);
 if(w==NULL) return(errWindGRF);
 w->state|=grfBREAK;
 return(okGRF);
}


int far VisualGRF_on()
{
 CheckGRF();
#ifdef __TURBOC__
 setvisualpage(aPage);
 vPage=aPage;
#endif
 return okGRF;
}

int far VisualGRF_off()
{
 unsigned tmp;

 CheckGRF();
#ifdef __TURBOC__
 if(aPage!=vPage) return okGRF;
 setactivepage((aPage=!aPage));
 tmp=Voff1;
 Voff1=Voff;
 Voff=tmp;
#endif
 return okGRF;
}


int far ActiveGRF(int wind)
{
 windowGRF w;

 CheckGRF();
 w=getWindowGRF(wind);
 if(w==NULL) return(errWindGRF);
 w->state|=grfACTIVE;
 _putTitleGRF(w);
 return(okGRF);
}

int far NactiveGRF(int wind)
{
 windowGRF w;

 CheckGRF();
 w=getWindowGRF(wind);
 if(w==NULL) return(errWindGRF);
 w->state&=~grfACTIVE;
 _putTitleGRF(w);
 return(okGRF);
}

