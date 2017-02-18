#include "ada_graf.h"
#include "common.h"

static float maxy[CHANNEL],miny[CHANNEL];
static float maxx[CHANNEL],minx[CHANNEL];

static float *x[CHANNEL];
static float *yB[CHANNEL][CHANNEL],*yF[CHANNEL][CHANNEL];

//*********************************************
static void GetStatF(float *xx,float *yy,triadGRF *p,unsigned long num)
{
 int k=(int)p->Array1,m=(int)p->Array2;

 *xx= x[k]     ? 5.0*x[k][num]     : 0;
 *yy= yF[k][m] ? 5.0*yF[k][m][num] : 0;

 return;
}

static void GetStatB(float *xx,float *yy,triadGRF *p,unsigned long num)
{
 int k=(int)p->Array1,m=(int)p->Array2;

 *xx= x[k]     ? 5.0*x[k][num]     : 0;
 *yy= yB[k][m] ? 5.0*yB[k][m][num] : 0;

 return;
}
//*********************************************
static triadGRF Stat1Def[]={
{BOX_1_4_T,  -1,-1,1,1,   GetStatF,NULL,    0,0, 1,0,    0,100,
 "Активен канал 0",NULL,"%+6.3f/%+6.3f",NULL,grfPARAM_ALL|grfDOUBLE|grfUNMARK,grfRANGE|grfLeftTTL},
{ADD_1,    -1,-1,1,1,     GetStatB,NULL,    0,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    0,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    0,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    0,2, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    0,2, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    0,3, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    0,3, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    0,4, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    0,4, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    0,5, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    0,5, 1,0,    0,200},

{BOX_2_4_T,  -1,-1,1,1,     GetStatF,NULL,    1,1, 1,0,    0,100,
 "Активен канал 1",NULL,"%+6.3f/%+6.3f",NULL,grfPARAM_ALL|grfDOUBLE|grfUNMARK,grfRANGE|grfLeftTTL},
{ADD_1,    -1,-1,1,1,     GetStatB,NULL,    1,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    1,2, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    1,2, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    1,3, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    1,3, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    1,4, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    1,4, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    1,5, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    1,5, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    1,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    1,0, 1,0,    0,200},

{BOX_3_4_T,  -1,-1,1,1,     GetStatF,NULL,    2,2, 1,0,    0,100,
 "Активен канал 2",NULL,"%+6.3f/%+6.3f",NULL,grfPARAM_ALL|grfDOUBLE|grfUNMARK,grfRANGE|grfLeftTTL},
{ADD_1,    -1,-1,1,1,     GetStatB,NULL,    2,2, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    2,3, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    2,3, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    2,4, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    2,4, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    2,5, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    2,5, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    2,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    2,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    2,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    2,1, 1,0,    0,200,},

{BOX_4_4_T,  -1,-1,1,1,     GetStatF,NULL,    3,3, 1,0,    0,100,
 "Активен канал 3",NULL,"%+6.3f/%+6.3f",NULL,grfPARAM_ALL|grfDOUBLE|grfUNMARK,grfRANGE|grfLeftTTL},
{ADD_1,    -1,-1,1,1,     GetStatB,NULL,    3,3, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    3,4, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    3,4, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    3,5, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    3,5, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    3,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    3,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    3,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    3,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    3,2, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    3,2, 1,0,    0,200, },

{BOX_4_4_T,  -1,-1,1,1,     GetStatF,NULL,    4,4, 1,0,    0,100,
 "Активен канал 4",NULL,"%+6.3f/%+6.3f",NULL,grfPARAM_ALL|grfDOUBLE|grfUNMARK,grfRANGE|grfLeftTTL},
{ADD_1,    -1,-1,1,1,     GetStatB,NULL,    4,4, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    4,5, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    4,5, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    4,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    4,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    4,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    4,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    4,2, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    4,2, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    4,3, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    4,3, 1,0,    0,200, },

{BOX_4_4_T,  -1,-1,1,1,     GetStatF,NULL,    5,5, 1,0,    0,100,
 "Активен канал 5",NULL,"%+6.3f/%+6.3f",NULL,grfPARAM_ALL|grfDOUBLE|grfUNMARK,grfRANGE|grfLeftTTL},
{ADD_1,    -1,-1,1,1,     GetStatB,NULL,    5,5, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    5,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    5,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    5,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    5,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    5,2, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    5,2, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    5,3, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    5,3, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    5,4, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    5,4, 1,0,    0,200, },


{BOX_4_4_T,  -1,-1,1,1,     GetStatF,NULL,    5,5, 1,0,    0,100,
 "Активен канал 6",NULL,"%+6.3f/%+6.3f",NULL,grfPARAM_ALL|grfDOUBLE|grfUNMARK,grfRANGE|grfLeftTTL},
{ADD_1,    -1,-1,1,1,     GetStatB,NULL,    5,5, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    5,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    5,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    5,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    5,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    5,2, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    5,2, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    5,3, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    5,3, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    5,4, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    5,4, 1,0,    0,200, },

{BOX_4_4_T,  -1,-1,1,1,     GetStatF,NULL,    5,5, 1,0,    0,100,
 "Активен канал 7",NULL,"%+6.3f/%+6.3f",NULL,grfPARAM_ALL|grfDOUBLE|grfUNMARK,grfRANGE|grfLeftTTL},
{ADD_1,    -1,-1,1,1,     GetStatB,NULL,    5,5, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    5,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    5,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    5,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    5,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    5,2, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    5,2, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    5,3, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    5,3, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},

{ADD_2,    -1,-1,1,1,     GetStatF,NULL,    5,4, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStatB,NULL,    5,4, 1,0,    0,200, },

};


static int graf_stat1(int chl) // стаческая х-ка по перемещению
{
 int i,val;

 for(i=0;i<72;i++)
   {
   if((i%12)==0) memcpy(&Stat1Def[i].scr,&ChlScr[chl-1][i/12],sizeof(struct _scr_params));
//   Stat1Def[i].beg=0;
//   Stat1Def[i].end=nr-1;
   Stat1Def[i].Xmax=5.0;
   Stat1Def[i].Xmin=-5.0;
   Stat1Def[i].Ymax=5.0;
   Stat1Def[i].Ymin=-5.0;
   }

 ScrSave();
 val=InitGRF(grfGRAPHICS);
 if(val==okGRF)
   {
   SetLook_Clr(putTitle); strcpy(TitleStr,"Статическая характеристика");
   val=LookGRF(chl*12,Stat1Def);
   DeinitGRF(grfTEXT);
   }
 ScrRest();

 return val==okGRF ? 0 : 7;
}

/***********************************************************************/

static void GetStat(float *xx,float *yy,triadGRF *p,unsigned long num)
{
 int k=(int)p->Array1,m=(int)p->Array2;

 *xx= x[k]     ? 5.0*x[k][num]     : 0;
 *yy= yF[k][m] ? 5.0*yF[k][m][num] : 0;

 return;
}


static triadGRF Stat2Def[]={
{BOX_1_4_T,  -1,-1,1,1,   GetStat,NULL,    0,0, 1,0,    0,100,
 "Активен канал 0",NULL,"%+6.3f/%+6.3f",NULL,grfPARAM_ALL|grfDOUBLE|grfUNMARK|grfSCALE_ALL,grfRANGE|grfLeftTTL},

{ADD_2,    -1,-1,1,1,     GetStat,NULL,    0,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    0,2, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    0,3, 1,0,    0,200},

{BOX_2_4_T,  -1,-1,1,1,   GetStat,NULL,    1,1, 1,0,    0,100,
 "Активен канал 1",NULL,"%+6.3f/%+6.3f",NULL,grfPARAM_ALL|grfDOUBLE|grfUNMARK|grfSCALE_ALL,grfRANGE|grfLeftTTL},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    1,2, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    1,3, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    1,0, 1,0,    0,200},

{BOX_3_4_T,  -1,-1,1,1,   GetStat,NULL,    2,2, 1,0,    0,100,
 "Активен канал 2",NULL,"%+6.3f/%+6.3f",NULL,grfPARAM_ALL|grfDOUBLE|grfUNMARK|grfSCALE_ALL,grfRANGE|grfLeftTTL},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    2,3, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    2,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    2,1, 1,0,    0,200,},

{BOX_4_4_T,  -1,-1,1,1,   GetStat,NULL,    3,3, 1,0,    0,100,
 "Активен канал 3",NULL,"%+6.3f/%+6.3f",NULL,grfPARAM_ALL|grfDOUBLE|grfUNMARK|grfSCALE_ALL,grfRANGE|grfLeftTTL},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    3,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    3,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    3,2, 1,0,    0,200, },

{BOX_4_4_T,  -1,-1,1,1,   GetStat,NULL,    3,3, 1,0,    0,100,
 "Активен канал 4",NULL,"%+6.3f/%+6.3f",NULL,grfPARAM_ALL|grfDOUBLE|grfUNMARK|grfSCALE_ALL,grfRANGE|grfLeftTTL},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    3,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    3,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    3,2, 1,0,    0,200, },

{BOX_4_4_T,  -1,-1,1,1,   GetStat,NULL,    3,3, 1,0,    0,100,
 "Активен канал 5",NULL,"%+6.3f/%+6.3f",NULL,grfPARAM_ALL|grfDOUBLE|grfUNMARK|grfSCALE_ALL,grfRANGE|grfLeftTTL},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    3,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    3,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    3,2, 1,0,    0,200, },

{BOX_4_4_T,  -1,-1,1,1,   GetStat,NULL,    3,3, 1,0,    0,100,
 "Активен канал 6",NULL,"%+6.3f/%+6.3f",NULL,grfPARAM_ALL|grfDOUBLE|grfUNMARK|grfSCALE_ALL,grfRANGE|grfLeftTTL},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    3,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    3,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    3,2, 1,0,    0,200, },

{BOX_4_4_T,  -1,-1,1,1,   GetStat,NULL,    3,3, 1,0,    0,100,
 "Активен канал 7",NULL,"%+6.3f/%+6.3f",NULL,grfPARAM_ALL|grfDOUBLE|grfUNMARK|grfSCALE_ALL,grfRANGE|grfLeftTTL},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    3,0, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    3,1, 1,0,    0,200, NULL,NULL,NULL,NULL,grfDOUBLE},
{ADD_2,    -1,-1,1,1,     GetStat,NULL,    3,2, 1,0,    0,200, },

};



static int graf_stat2(int chl,int nr) // статическая хар-ка по ускорениюёскорости
{
 int i,val;

 for(i=0;i<16;i++)
   {
   Stat2Def[i].beg=0;
   Stat2Def[i].end=nr-1;
   Stat2Def[i].Xmax=5*maxx[i/4];
   Stat2Def[i].Xmin=5*minx[i/4];
   Stat2Def[i].Ymax=5*maxy[i/4];
   Stat2Def[i].Ymin=5*miny[i/4];
   }

 ScrSave();
 val=InitGRF(grfGRAPHICS);
 if(val==okGRF)
   {
   SetLook_Clr(putTitle); strcpy(TitleStr,"Статическая характеристика");
   val=LookGRF(16,Stat2Def);
   DeinitGRF(grfTEXT);
   }
 ScrRest();

 return val==okGRF ? 0 : 7;
}



int ViewSTH(int code,int OkPID)
{
 int f,PID,ret=0,k,j;
 unsigned int i;
 short int nk,nr;
 short int mode;

 for(i=0;i<CHANNEL;i++)
   {
   x[i]=NULL;
   for(k=0;k<CHANNEL;k++) yB[i][k]=yF[i][k]=NULL;
   }

 f=open("last_sth.dat",O_RDONLY|O_BINARY);
 if(f)
   {
   BufRead(f,&PID,sizeof(PID));
   if(PID==OkPID || OkPID==0)
     {
     BufRead(f,&nk,sizeof(nk));
     for(k=0;k<nk && ret==0;k++)
       {
       BufRead(f,&nr,sizeof(nr));
       BufRead(f,&mode,sizeof(mode));
       for(i=0;i<12;i++)
         {
         Stat1Def[k*12+i].beg=0;
         Stat1Def[k*12+i].end= nr ? nr-1 :0;
         }
       if(!nr) continue;


       maxy[k]=miny[k]=0;
       x[k]=calloc(sizeof(float),nr);
       yF[k][k]=calloc(sizeof(float),nr);
       yB[k][k]=calloc(sizeof(float),nr);
       if(!x[k] || !yF[k][k]|| !yB[k][k]) ret=Err_MEM;
       else
         {
         for(i=0;i<nr;i++)
             {
             BufRead(f,&x[k][i],sizeof(float));
             if(x[k][i]>maxx[k]) maxx[k]=x[k][i];
             if(x[k][i]<minx[k]) minx[k]=x[k][i];
             }
         for(i=0;i<nr;i++)
             {
             BufRead(f,&yB[k][k][i],sizeof(float));
             if(yB[k][k][i]>maxy[k]) maxy[k]=yB[k][k][i];
             if(yB[k][k][i]<miny[k]) miny[k]=yB[k][k][i];
             }
         for(i=0;i<nr;i++)
             {
             BufRead(f,&yF[k][k][nr-i-1],sizeof(float));
             if(yF[k][k][nr-i-1]>maxy[k]) maxy[k]=yF[k][k][nr-i-1];
             if(yF[k][k][nr-i-1]<miny[k]) miny[k]=yF[k][k][nr-i-1];
             }
         }
       }
     if(!ret)
       switch(mode)
           {
           case 0: ret=graf_stat1(nk); break;  // перемешение
           case 1:
           case 2: ret=graf_stat2(nk,nr); break;  // скорость\ускорение
           default: ret=Err_ARG;
           }
     }
   else ret=Err_PID;
   }
 else ret=Err_DatFile;

 return ret;
}

int ViewSTHReport(int code,char *name)
{
 FILE *f;
 char tmp[20],tmp1[20],tmp2[20];
 float freq,val;
 int k,i,m,chl;

 f=fopen(name,"r+t");
 if(f)
   {
   fscanf(f,"%s %s %s",tmp,tmp1,tmp2);
   fscanf(f,"%s %s %f",tmp,tmp1,&freq);
   fscanf(f,"%s %s %d %s %d",tmp,tmp1,&chl,tmp2,&nko);
   for(k=0;k<chl;k++)
     {
     fscanf(f,"%s %s %s",tmp,tmp1,tmp2);
     fscanf(f,"%s %s %d",tmp,tmp1,&nr);
     for(i=0;i<12;i++)
       {
       Stat1Def[k*12+i].beg=0;
       Stat1Def[k*12+i].end= nr ? nr-1 :0;
       }
     if(!nr) continue;

     x[k]=calloc(nr,sizeof(float));
     yF[k][k]=calloc(nr,sizeof(float));
     yB[k][k]=calloc(nr,sizeof(float));
     if( !x[k] || !yB[k][k] || !yF[k][k]) goto done;
     for(i=0;i<nr;i++)
       {
       fscanf(f,"%s %f",tmp,&val); x[k][i]=val/5;
       for(m=0;m<chl;m++)
          { fscanf(f,"%f",&val); if(m==k) yB[k][k][i]=val/5; }
       if(yB[k][k][i]>maxy[k]) maxy[k]=yB[k][k][i];
       if(yB[k][k][i]<miny[k]) miny[k]=yB[k][k][i];
       }
     for(i=0;i<nr;i++)
       {
       fscanf(f,"%s %s",tmp,tmp1);
       for(m=0;m<chl;m++)
          { fscanf(f,"%f",&val); if(m==k) yF[k][k][nr-i-1]=val/5; }
       if(yF[k][k][nr-i-1]>maxy[k]) maxy[k]=yF[k][k][nr-i-1];
       if(yF[k][k][nr-i-1]<miny[k]) miny[k]=yF[k][k][nr-i-1];
       }
     }
  graf_stat1(chl);
  }
done:
 return 0;
}

