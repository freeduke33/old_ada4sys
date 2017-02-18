/*Функции и переменные для программы <ACHH.C>  */
/*            <PRO_ACHH.H>                     */

#ifdef __cplusplus
extern "C" {
#endif

#define VIRTUAL
// #define SYS_TEST


#include <stdio.h>
#include <math.h>
#include <tcxldef.h>

#define DEFINE_CHANNEL 4

#define NMAX      300
#define WMAX      102
#define EMAX      6
// #define KAN_MAX   4
#define CHANNEL   8
#define ZAPAZ     4
#define PMAX      5
#define QMAX      4
#define PSMAX     50
#define QQMAX     50
#define NU        1E-30

#include "config.h"

extern int PID;
/*
#ifdef VIRTUAL
extern float huge *x[4],huge *y[4],huge *yt[4];
#else
extern float x[4][NMAX+1],y[4][NMAX+1],yt[4][NMAX+1];
#endif
*/

extern float xmin,ymin;
extern float xmax,ymax,fdisk;
extern float xk[CHANNEL+1],yk[CHANNEL+1];
extern float beta[CHANNEL][PMAX],gamma[CHANNEL];

#ifdef SYS_TEST

extern float w[WMAX+1];
extern float xh[WMAX+1],yh[KAN_MAX][KAN_MAX][WMAX+1];
extern float wxy[4][4];
extern float be[KAN_MAX+2][KAN_MAX+2][KAN_MAX+2][KAN_MAX+2];
extern float psx[CHANNEL][PSMAX],psy[CHANNEL][PSMAX];
extern float del_x_k[6][CHANNEL][PSMAX],del_y_k[6][CHANNEL][PSMAX];
extern float beta[CHANNEL][PMAX],gamma[CHANNEL];


void graf_achh(void);
void tabl_achh(void);
void   do_achh(void);

void  graf_stat(void);
void  tabl_stat(void);
void    do_stat(void);
void    wz_stat(void);

void CTYP sys_wzst(NOARG);
void CTYP sys_achh(NOARG);
void CTYP Inf_Stat(NOARG);
void CTYP Inf_WzSt(NOARG);
void CTYP Inf_Achh(NOARG);

void CTYP ACHH(NOARG);

void write_st(void);
void write_wzs(void);
void write_achh(void);

void       Out_y(int);
void Out_kan(int,int);
void     per_pr(void);
void        shag(int);
void   tab_y_wz(void);
void   tab_beta(void);

#endif

extern char wid_modeli[][20];
extern char nam_modeli[][20];
extern char *NamesPtr[];
extern char *isEMS_str[];
extern char *ErrType_str[];
extern char *AftErr_str[];



int ScrSave(void);
void ScrRest(void);

void Imit_Get(void);
void Imit_Driv(void);
int     Enable(void);
void   Disable(void);


void p_garm(void);
void m_p_garm(void);

void  graf4(float,float,float,float);

void CTYP poly_sin(NOARG);

void CTYP Inf_Rab(NOARG);
VOID CTYP Inf_Form(NOARG);
void CTYP Inf_File(NOARG);
void CTYP Inf_Graf(NOARG);


void CTYP MOD_PODS(NOARG);
void CTYP sys_har(NOARG);
void CTYP sys_test(NOARG);


void  tabl_time(void);
void  graf_time(void);
void  graf_2time(void);

void fnamecpy(char *,char *);


void  ShutDown(void);
void  StartUp(void);

void  RazgruzkaSTART(void);
void  RazgruzkaSTOP(void);
void  GotoNull(void);
int   AllocBuf(void);
void  FreeBuf(void);

float huge Sign_upr(short,unsigned long);
float huge Sign_per(short,unsigned long);
void  par_mod(short,unsigned long);
void  Loop(void);
void  Loop_Debug(void);
int  etalon(void);
int ViewGraph(char *mode);


void write_et(void);
void write_upr(void);

void vi_et_graph(void);
void vi_et_tabl(void);

void from_stend(void);

void SetCheck(int tag);

VOID CTYP _disp_model0_WRITE(NOARG);
VOID CTYP _disp_model1_WRITE(NOARG);
VOID CTYP _disp_model2_WRITE(NOARG);
VOID CTYP _disp_model3_WRITE(NOARG);


extern char HelpFileName[];

char *getHelpFile(void);

VOID CTYP SaveCfg(NOARG);
VOID CTYP LoadCfg(NOARG);

void xSaveCfg(char *);
void xLoadCfg(char *);




#include "help.h"

#ifdef __cplusplus
 }
#endif


