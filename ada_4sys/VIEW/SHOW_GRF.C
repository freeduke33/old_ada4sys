#include <process.h>
#include <errno.h>

#include <tcxlwin.h>
#include <tcxlcur.h>
#include <tcxlvid.h>
#include <tcxlinp.h>
#include <tcxlcod.h>

#include "swap.h"
#include "view.h"

#define graph_process "ada_graf.exe"

int Sel;
char ach_filename[180];
char sth_filename[180];
char tem_filename[180];
char upr_filename[180];

static VcelP sss=NULL;

int ScrSave(void) { sss=Vsave(); return(sss!=NULL); }
void ScrRest(void) { if(sss) Vrestore(sss);CurHide(); }

int showGRF(char *fname,char *mode);


void ViewUPR_YYt(void) { showGRF(upr_filename,"102"); return; }
void ViewUPR_XY (void) { showGRF(upr_filename,"103"); return; }
void ViewTEM(void)     { showGRF(tem_filename,"104"); return; }
void ViewSTH(void)     { showGRF(sth_filename,"105"); return; }
void ViewACH(void)     { showGRF(ach_filename,"106"); return; }


int showGRF(char *fname,char *mode)
{
 int ret;

 if(!fname[0]) { Wperror("����୮� �������� 䠩�� ������"); return; }

 ScrSave();
 if(sss)
   {
   while(!swap_ison()) swap_on();
   CurPush(CUR_HID,-1,0,0);
   ret=spawnl(P_WAIT,graph_process,graph_process,fname,mode,NULL);
   CurPop();
   ScrRest();
   }
 else ret=4;

 switch(ret)
  {
  case  0: /*�� ��ଠ�쭮*/
   break;
  case -1:
   switch(errno)
     {
     case E2BIG:
     case EINVAL:
       Wperror("�஡���� �� ����᪥ ��ᬮ�� ��䨪��");
       break;
     case ENOENT:
       Wperror("��������� ����� ��ᬮ�� ��䨪�� \"" graph_process "\"");
       break;
     case ENOEXEC:
       Wperror("����� ��ᬮ�� ��䨪�� \"" graph_process "\" ���०���");
       break;
     case ENOMEM:
       Wperror("�������筮 ᢮������ ����� ��� ��ᬮ�� ��䨪��");
       break;
     }
   break;
  case 2:
  case 6:
   Wperror("����� ������ ०�� ࠡ��� ����� ��ᬮ�� ��䨪��");
   break;
  case 3:
  case 5:
   Wperror("��� ������ �� ��� ᥠ�� ࠡ���");
   break;
  case 4:
   Wperror("�������筮 ᢮������ ����� ��� ��ᬮ�� ��䨪��");
   break;
  case 7:
   Wperror("�訡�� ���樠����樨 ����� ��ᬮ�� ��䨪��");
   break;
  default:
   Wperror("�஡���� � ����� ��ᬮ�� ��䨪��");
  }
 KquePut(Key_Ent);
 return;
}




