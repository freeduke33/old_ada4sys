#define Uses_TApplication
#define Uses_TMenuBar
#define Uses_TMenuItem
#define Uses_TSubMenu
#define Uses_TStatusLine
#define Uses_TStatusItem
#define Uses_TStatusDef
#define Uses_TKeys
#define Uses_TRect

#define Uses_TVision
#define Uses_MyApp
#define Uses_Const

#include "emul_ds.hpp"

#include <string.h>


MyApp::MyApp():
    TProgInit( &MyApp::initStatusLine,
               &MyApp::initMenuBar,
               &MyApp::initDeskTop
             )
{
 strcpy(NikName,"*.EMU");
 Emul=NULL;
}



TStatusLine *MyApp::initStatusLine(TRect r)
{
 r.a.y=r.b.y-1;
 return new TStatusLine(r,
            *new TStatusDef(0,0xFFFF) +
            *new TStatusItem("~F1~ ������", kbF1, cmHelp)+
            *new TStatusItem("~F10~ ����", kbF10, cmMenu)+
            *new TStatusItem("~Alt-X~ ��室", kbAltX, cmQuit)
            );
}

TMenuBar *MyApp::initMenuBar(TRect r)
{
 r.b.y=r.a.y+1;

 return new TMenuBar(r,
            *new TSubMenu("~�~����", kbAltF)+
              *new TMenuItem("~�~���㧨��", cmLoadEmul, kbF3, hcLoadEmul, "F3")+
              *new TMenuItem("~�~��࠭���", cmSaveEmul, kbF2, hcSaveEmul, "F2")+
              *new TMenuItem("�~�~�࠭��� ��� ...", cmSaveEmulAs, 0, hcSaveEmul,"")+
              newLine()+
              *new TMenuItem("~�~�室", cmQuit, kbAltX, hcNoContext,"Alt+X")+

            *new TSubMenu("~�~�����_��ꥪ�_�ࠢ�����", kbAltM)+
              *new TMenuItem("~�~�⠢��� �����", cmInsChannel,0, hcInsChannel,"Ctrl+Ins")+
              *new TMenuItem("~�~������ �����", cmDelChannel,0, hcDelChannel,"Ctrl+Del")+
              *new TMenuItem("��~�~������ﭨ�", cmXrefChannel, kbCtrlV, hcXrefChannel,"Ctrl+V")+
              newLine()+
              *new TMenuItem("��⠢��� ~�~����", cmInsZveno, kbIns, hcInsZveno,"Ins")+
              *new TMenuItem("�~�~����� �����",cmDelZveno, kbDel, hcDelZveno, "Del")+
              *new TMenuItem("~�~�ࠬ���� �����", cmDefZveno, kbEnter, hcDefZveno, "Enter")+

            *new TSubMenu("~�~���_������", kbAltT)+
              *new TMenuItem("~�~�����㤭�-���⭠� �ࠪ���⨪�", cmACX, 0, hcACX, "")+
              *new TMenuItem("~�~����᭠� �ࠪ���⨪�", cmPulse, 0, hcPulse, "")+
              newLine()+
              *new TMenuItem("~�~�ࠢ����� ������ମ���. ᨣ�����", cmSinus,0, hcSinus, "")+

            *new TSubMenu("~�~���", kbAltO)+
              *new TMenuItem("~�~���冷���", cmCascade, kbAltY, hcCascade, "Alt+Y")+
              *new TMenuItem("~�~��᮪ ����", cmWinList, kbAlt0, hcWindow ,"Alt+0")
            );
}

void MyApp::handleEvent(TEvent& event)
{
 TApplication::handleEvent(event);
 switch(event.what)
   {
   case evCommand:
   switch(event.message.command)
     {
     case cmLoadEmul:   LoadEmul();      break;
     case cmSaveEmul:   SaveEmul(False); break;
     case cmSaveEmulAs: SaveEmul(True);  break;

     case cmEditChannel: EdtChannel(event.message.infoPtr);   break;
     case cmInsChannel: InsChannel();    break;
     case cmDelChannel: DelChannel();    break;
     case cmXrefChannel:XrefChannel();   break;

     case cmInsZveno:   InsZveno();      break;
     case cmDelZveno:   DelZveno();      break;
     case cmDefZveno:   DefZveno();      break;
     default:  return;
     }
   case evBroadcast:
   switch(event.message.command)
     {
     case cmOpenEmul: break;
     case cmCloseEmul:  Emul=NULL; break;
     default: return;
     }
   default: return;
   }
 clearEvent(event);
 return;
}

main()
{
 MyApp App;

 App.run();
 return 0;
}

