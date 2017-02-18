#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "look_def.h"

#define CHANNEL 8


extern char TitleStr[];
extern float delt,*x[CHANNEL],*y[CHANNEL],*yt[CHANNEL],
             *xh[CHANNEL],*yh[CHANNEL][CHANNEL];
extern unsigned long nr;
extern int nki,nko,nk;
extern struct _scr_params ChlScr[][CHANNEL];

void ScrSave(void);
void ScrRest(void);
int putTitle(int mode);


#define Err_Mode        2
#define Err_PID         3
#define Err_MEM         4
#define Err_DatFile     5
#define Err_ARG         6


int ViewSTH(int code,int okPID);
int ViewACH(int code,int okPID);
int ViewWork(int code,int OkPID);
int ViewEtalon(int code,int OkPID);

int ViewSTHReport(int code,char *name);
int ViewACHReport(int code,char *name);
int ViewWorkReport(int code,char *name);
int ViewEtalonReport(int code,char *name);

#ifdef __cplusplus
 }
#endif