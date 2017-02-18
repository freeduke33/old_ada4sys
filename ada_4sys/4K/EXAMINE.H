#ifdef __cplusplus
extern "C" {
#endif

#define exmSTAT 0
#define exmWZST 1
#define exmACHH 2

#define CHANNEL 8

void fnamecpy(char *str1,char *str2);

void doExamine(int);
void graphExamine(int mode);
void tablExamine(int mode);
int ViewGraph(char *mode);

void write_st(void);
void write_achh(void);
void write_wzs(void);


void pascal Inf_Rab(void);
void pascal SYS_ACHH(void);
void pascal SYS_WZST(void);
void pascal SYS_STAT(void);


int loadCfg(void);
int saveCfg(void);

int xLoadCfg(char *);
int xSaveCfg(char *);

void StartUp(void);
void ShutDown(void);

int InitLog(void);
int DoneLog(void);


extern int PID;
extern char wid_modeli[][20];

#include "config.h"

#ifdef __cplusplus
}
#endif