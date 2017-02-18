#ifndef __VIEW_H__
#define __VIEW_H__

#ifdef __cplusplus
extern "C" {
#endif

void ViewSTH(void);
void ViewTEM(void);
void ViewACH(void);
void ViewUPR_XY(void);
void ViewUPR_YYt(void);

extern char sth_filename[],tem_filename[],ach_filename[],upr_filename[];

extern int Sel;
#ifdef __cplusplus
 }
#endif


#endif
