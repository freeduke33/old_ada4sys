#include <stdio.h>
#include <conio.h>

#include "ada_graf.h"


void *sss=NULL;
char TitleStr[100];
float delt,*x[CHANNEL],*y[CHANNEL],*yt[CHANNEL],*xh[CHANNEL],*yh[CHANNEL][CHANNEL];
unsigned long nr;
int nki,nko,nk;

struct _scr_params ChlScr[][8]={
   {FULL_SCR_T},
   {HOR_1_2_T,HOR_2_2_T},
   {BOX_1_4_T,BOX_2_4_T,BOX_3_4_T,BOX_4_4_T},
   {BOX_1_4_T,BOX_2_4_T,BOX_3_4_T,BOX_4_4_T},
   {BOX_1_6_T,BOX_2_6_T,BOX_3_6_T,BOX_4_6_T,BOX_5_6_T,BOX_6_6_T},
   {BOX_1_6_T,BOX_2_6_T,BOX_3_6_T,BOX_4_6_T,BOX_5_6_T,BOX_6_6_T},
   {BOX_1_8_T,BOX_2_8_T,BOX_3_8_T,BOX_4_8_T,BOX_5_8_T,BOX_6_8_T,BOX_7_8_T,BOX_8_8_T},
   {BOX_1_8_T,BOX_2_8_T,BOX_3_8_T,BOX_4_8_T,BOX_5_8_T,BOX_6_8_T,BOX_7_8_T,BOX_8_8_T}
   };


void ScrSave(void) { /*sss=Vsave();*/ }
void ScrRest(void) { /*Vrestore(sss);CurHide();*/ }



int putTitle(int mode)
{
#ifdef __TURBOC__
 cleardevice();
#else
 _clearscreen(_GCLEARSCREEN);
#endif
 if(mode==grfCLR_FULL)
   {
   TextGRF(TitleStr,500,950,WHITE,BLACK,20,50,grfAxisX,0,grfCENTER);
   }
 return okGRF;
}


main(int argc,char *argv[])
{
 int f,ret=0,code,PID;

 fprintf(stdout,"Модуль просмотра графиков v1.01 (C) \"Системы Триал\" 1996-97.\n");
 if(argc!=3)
   {
   fprintf(stdout,"Запуск:\n"
    " ?:\\> %s <PID>|<File> <Code>\n"
    "\t <PID>  - идентификатор сеанса работы\n"
    "\t <File> - файл паспорта\n"
    "\t <Code> - режим работы\n"
    "\t\tДанные берутся с использованием <PID>\n"
    "\t\t  1 - резерв\n"
    "\t\t  2 - результаты эксперимента (Вход/Выход)\n"
    "\t\t  3 - результаты эксперимента (Эталон/Выход)\n"
    "\t\t  4 - просмотр Эталона\n"
    "\t\t  5 - статическая характеристика\n"
    "\t\t  6 - АЧХ\n"
    "\t\tДанные берутся из <File>\n"
    "\t\t  101 - резерв\n"
    "\t\t  102 - результаты эксперимента (Вход/Выход)\n"
    "\t\t  103 - результаты эксперимента (Эталон/Выход)\n"
    "\t\t  104 - просмотр Эталона\n"
    "\t\t  105 - статическая характеристика\n"
    "\t\t  106 - АЧХ\n",argv[0]);
   return Err_ARG;
   }
 PID =atoi(argv[1]);
 code=atoi(argv[2]);

 switch(code)
   {
   /* ============== просмотр процесса управления */
   case 1:
   case 2:
   case 3:   ret=ViewWork(code,PID); break;
   case 101:
   case 102:
   case 103: ret=ViewWorkReport(code,argv[1]); break;

   /* ============== просмотр эталона */
   case 4:   ret=ViewEtalon(code,PID); break;
   case 104: ret=ViewEtalonReport(code,argv[1]); break;

   /* ============== просмотр статической характеристики */
   case 5:   ret=ViewSTH(code,PID); break;
   case 105: ret=ViewSTHReport(code,argv[1]); break;

   /* ============== просмотр ACH */
   case 6:   ret=ViewACH(code,PID); break;
   case 106: ret=ViewACHReport(code,argv[1]); break;

   default:
            ret=Err_Mode;
   }

 return ret;
}

