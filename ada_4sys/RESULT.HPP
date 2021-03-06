#ifndef __RESULT_HPP__
#define __RESULT_HPP__

#include "config.h"

class Buffer;

// ����� �� ������ ������ �� १���⠬ �ࠢ�����
typedef struct
{
 short int isPresent;
 short int is[3];
 unsigned long nr;
 float BegTime,Freq;
} ResultInfo;

// ����� �� ������ ������ �⠫���� ᨣ���
typedef struct
{
 short isPresent;
 unsigned long nr;
} EtalonInfo;

//  ����� �� ������ ������ ����᪠� �ࠪ���⨪�
typedef struct
{
 short isPresent;
 unsigned long nr;
 float MaxVol,MinVol;
} StatInfo;

//  ����� �� ������ ������ ������㤭�-���⭠� �ࠪ���⨪�
typedef struct
{
 short isPresent;
 unsigned long nr;
 float MaxFreq,MinFreq;
} AchInfo;

int StoreResults(int PID,Buffer **x,Buffer **y,Buffer **yt,CfgT *,LoopStat *);
int LoadResultBanner(int PID,int MaxChannels,int *fHandle,ResultInfo *);
int LoadResult(int fHandle,ResultInfo *,Buffer **x,Buffer **y,Buffer **yt);

int StoreEtalon(int PID,CfgT *,Buffer **yt);
int LoadEtalonBanner(int PID,int MaxChannels,int *fHandle,EtalonInfo *);
int LoadEtalon(int fHandle,EtalonInfo *,Buffer **yt);


#endif
