#ifdef __BORLANDC__
#define ENABLE_SWAP
#endif

#include <process.h>
#include <stdlib.h>
#include <dos.h>
#include <errno.h>

extern "C" {

#include <tcxlwin.h>
#include <tcxlcur.h>
#include <tcxlvid.h>

}

#include <pro_achh.h>
#include <ada_4sys.hpp>

#ifndef NO_TEMPLATE
# define Uses_Buffer
# define Uses_LoopSC
# include <adaptive.hpp>
# include "result.hpp"
#endif


#ifdef ENABLE_SWAP
#  include "swap.h"
#endif

#define graph_process "ada_graf.ovl"

extern "C" VOID CTYP Inf_Graf(NOARG);
extern int PID;

static VcelP sss=NULL;

extern "C" int ScrSave(void) { sss=Vsave(); return(sss!=NULL); }
extern "C" void ScrRest(void) { if(sss) Vrestore(sss);CurHide(); }



int ViewGraph(char *mode)
{
 int ret;
 char buf[20];

 ScrSave();
 if(sss)
   {
   Inf_Graf();
   Wunlink(Whandle());
   itoa(PID,buf,10);
#ifdef ENABLE_SWAP
   while(!swap_ison()) swap_on();
#endif
   ret=spawnl(P_WAIT,graph_process,graph_process,buf,mode,NULL);
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
 return ret;
}

extern "C" void graf_time(void)         { ViewGraph("3"); }
extern "C" void graf_2time(void)        { ViewGraph("2"); }

#ifndef NO_TEMPLATE

extern "C" void vi_et_graph(void)
{
 Buffer *yt[CHANNEL];
 int i,val=0;

 Inf_Graf();
 for(i=0;i<Cfg.nk;i++) yt[i]=NULL;
 for(i=0;i<Cfg.nk;i++)
   {
   val=MakeEtalon(i,&yt[i]);
   if(val) { Wperror("������ ��� �����室�騩 䠩� �⠫���"); goto done; }
   }
 StoreEtalon(PID,&Cfg,yt);
done:
 for(i=0;i<Cfg.nk;i++) if(yt[i]) delete yt[i];
 if(!val) ViewGraph("4");
 Wclose();
 return;
}

#endif
