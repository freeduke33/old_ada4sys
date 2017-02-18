#include "_grf.h"

extern struct viewporttype disp;

int far LineGRF(int x1,int y1,int x2,int y2,int col)
{
 CheckGRF();
 to_GRF();
 x1=todisp(x1,grfAxisX);
 y1=todisp(y1,grfAxisY);
 x2=todisp(x2,grfAxisX);
 y2=todisp(y2,grfAxisY);
#ifdef __WATCOMC__
 _setcolor(col);
 _moveto(x1,disp_height-y1);
 _lineto(x2,disp_height-y2);
#else
 setcolor(col);
 line(x1,disp_height-y1,x2,disp_height-y2);
#endif
 GRF_to();
 return okGRF;
}

