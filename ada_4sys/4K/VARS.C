#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include "tcxlwin.h"
#include "tcxlmnu.h"
#include "pro_achh.h"


#ifndef __TURBOC__
#define M_PI 3.1415
#endif

void getFile(char *fn,char *ext,char *buf);

char HelpFileName[100]="Ada_4Sys.hlp";

char wid_modeli[][20]={"Перемещение","Скорость","Ускорение"};
char nam_modeli[][20]={"Отсутствует",
                       "Без учета помех",
                       "Ген. без помех",
                       "С учетом помех",
                       "Ген. с помехами",
                       "С учетом помех 2",
                       "Полная типa`Тc`",
                       "Ген. полная`Тc`",
                       "Неполная типa`Тc`",
                       "Ген. неполная`Тc`",
                       "Полная типа`1k`",
                       "Неполная типа`1k`",
                       "Полная с помехами"
                       };

char *NamesPtr[]={nam_modeli[0],nam_modeli[1],
                  nam_modeli[2],nam_modeli[3],
                  nam_modeli[4],nam_modeli[5],
                  nam_modeli[6],nam_modeli[7],
                  nam_modeli[8],nam_modeli[9],
                  nam_modeli[10],nam_modeli[11],
                  nam_modeli[12],NULL};

/*
char _isEMS_str[][20]={"Только 640Kb","EMS"};
char *isEMS_str[]={_isEMS_str[0],_isEMS_str[1],NULL};

char _ErrType_str[][20]={"Ср.квадрат.","Мат.ожидание"};
char *ErrType_str[]={_ErrType_str[0],_ErrType_str[1],NULL};

char _AftErr_str[][20]={"Останов",
                        "Ост.канала",
                        "Перезапуск",
                        "Перезап.кан.",
                        "Игнорировать"};
char *AftErr_str[]={_AftErr_str[0],_AftErr_str[1],_AftErr_str[2],
                    _AftErr_str[3],_AftErr_str[4],NULL};


float fdisk;
*/
int PID=0;

void about()
{
 return;
}


void fnamecpy(char *str1,char *str2)
{
 int i;
 for(i=strlen(str2);str2[i]!='\\' && i;i--); if(str2[i]=='\\') i++;
 strcpy(str1,&str2[i]);
 return;
}

void SetCheck(int tag)
{
 IdfP i=MdfTop(MctlCur);
 int j;

 for(j=0;j<3;j++)
  {
  IdfStr(i)[1]=' '; ItmDsp(IdfTag(i));
  i=ItmLoc(IdfTag(i)+2);

  }
 i=ItmLoc(tag);
 if(i) { IdfStr(i)[1]='√'; ItmDsp(tag); }

 return;
}

char *getHelpFile(void)
{

 getFile("ada_4sys","hlp",HelpFileName);
 return HelpFileName;
}

