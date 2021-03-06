#ifdef __BORLANDC__
#  define ENABLE_SWAP
#endif

#include <process.h>
#include <stdlib.h>
#include <dos.h>
#include <errno.h>


extern "C" {
// #include "look_grf.h"
#include <tcxlwin.h>
#include <tcxlinp.h>
 }

#ifdef ENABLE_SWAP
# include "swap.h"
#endif
#include "pro_achh.h"

#define loop_process "ada_loop.ovl"

void PrepareConfig(void)
{
 int i;
#define MAX_DEFINED 4

 for(i=MAX_DEFINED;i<CHANNEL;i++) Cfg.Def[i]=Cfg.Def[i%MAX_DEFINED];
}

extern "C" void Loop(void)
{
 int ret;
 char buf[20];

 if(ScrSave())
   {
   Inf_Rab();

   PrepareConfig(); // ��७�� ���䨣��樨 �� ������ ������� � ���訥

   itoa(PID,buf,10);
   xSaveCfg("lastwork.ini");
#ifdef ENABLE_SWAP
   while(!swap_ison()) swap_on();
#endif
   ret=spawnl(P_WAIT,loop_process,loop_process,buf,"1","lastwork.ini",NULL);
   xLoadCfg("lastwork.ini");

   Wprtf(7,4,_RED|LGREY,"�६� ࠡ���       :");
   WprtCen(9,_CYAN|BLACK|_BLINK,"  ������ ���� �������  ");
   if(ret==0 || ret==12) KeyWait();
   Wclose();
   ScrRest();
   }
 else ret=1;

 switch(ret)
  {
  case  0: /*�� ��ଠ�쭮*/
  case 12: /*��ࢠ�� �����஬*/
     graf_2time();
   break;
  case -1:
   switch(errno)
     {
     case E2BIG:
     case EINVAL:
       Wperror("DOS: �஡���� �� ����᪥ ��ᯥਬ���");
       break;
     case ENOENT:
       Wperror("DOS: ��������� ����� �ࠢ����� ��ᯥਬ��⮬ \"" loop_process "\"");
       break;
     case ENOEXEC:
       Wperror("DOS: ����� �ࠢ����� ��ᯥਬ��⮬ \"" loop_process "\" ���०���");
       break;
     case ENOMEM:
       Wperror("DOS: �������筮 ᢮������ ����� ��� �஢������ ��ᯥਬ���");
       break;
     }
   break;
  case 1: Wperror("EXE: ����७��� �訡�� ��ਮ�� �믮�����"); break;
  case 2:
  case 3:
  case 4:
  case 5:
  case 13:
  case 15:
  case 16:
  case 17:
     Wperror("��ᮢ���⨬�� ����� ����� �஢������ ��ᯥਬ���"); break;
  case 6: Wperror("�訡�� �� �ନ஢���� �⠫���"); break;
  case 7: Wperror("��� 䠩�� �⠫���"); break;
  case 8: Wperror("�����室�饥 �᫮ ������� ��� ��࠭���� �⠫���"); break;
  case 9: Wperror("�����室��� ������ �鸞 ��� ��࠭���� �⠫���"); break;
  case 10:Wperror("�஡���� � ����� ����᪠ ��ᯥਬ���"); break;
  case 11:Wperror("�訡�� � ����� �஢������ ��ᯥਬ���"); break;
  case 14:Wperror("�訡�� �� ��।�� ��ࠬ��஢ � ����� �஢������ ��ᯥਬ���"); break;
  case 18: Wperror("�������筮 ᢮������ ����� ��� �஢������ ��ᯥਬ���"); break;
  default: Wperror("�஡���� � ����� �஢������ ��ᯥਬ���");
  }
 return;
}


// ======================= �ਥ� ᨣ����� � �⥭��
extern "C" void from_stend(void)
{

}

