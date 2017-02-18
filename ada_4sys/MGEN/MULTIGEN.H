#ifndef __MULTIGEN_H__
#define __MULTIGEN_H__

#define CHANNEL 10

/*

#define ID_String "MultiGen config v1.0\n\r"

typedef enum {None=-300, Const=0, Sin=1, PSin=2, DatFile=3, PrgFile=4} SigType;
typedef struct { float Freq,Ampl,Faza; } _sin;
typedef struct { float Freq[3],Ampl[3],Faza[3]; } _psin;
typedef struct { char Name[65]; unsigned int MaxVol; } _file;

struct channel_dat
   {
   SigType Type;
   struct
     {
     _file DatFile;
     _file PrgFile;
     _sin Sin;
     _psin PSin;
     float ConstVal;
     } Dat;
};


typedef struct
{
 char ID[25];

 unsigned int ActChannel,Beg;
 float Freq,ErrTime;
 unsigned long Time,Cycle;
 unsigned int StartVol; // startup volume in channel 0 ADC
 unsigned int StopVol;  // stop volume    in channel 0 ADC
 struct channel_dat Channel[CHANNEL];

} Config;

typedef struct
{
 unsigned long count,length,spline;
 SigType Type;
 struct
   {
   _file DatFile;
   _sin Sin;
   _sin PSin[3];
   float ConstVal;
   } Dat;
} PrgDefine;

*/

#ifndef __TURBOC__
#define M_PI 3.1416
#endif

//#define ToDAC(a,MaxDAC)    ( (int)(( (a)<-1 ? -1: ((a)>1?1:(a)) *MaxDAC)+MaxDAC+0.5) )
//#define FromADC(a,MaxADC)  ( ((float)(((int)(a))-(int)MaxADC)) /(int)MaxADC)

#define ToDAC(a,MaxDAC)     ( (int) ((a)*(float)MaxDAC+MaxDAC)               )
#define VerifyDAC(a,MaxDAC) ( (a<0) ? 0 : (a)>=MaxDAC*2 ? MaxDAC*2-1 : (a)   )
#define FromDAC(a,MaxDAC)   ( ((float)(((int)(a))-(int)MaxDAC)) /(int)MaxDAC )
#define FromADC(a,MaxADC)   ( ((float)(((int)(a))-(int)MaxADC)) /(int)MaxADC )


#ifdef __cplusplus
extern "C" {
#endif
char *elapsTime(unsigned long sec);
void xprintf(const char *fmt, ...);
void xwait(void);
#ifdef __cplusplus
  }
#endif


#endif



