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
            *new TStatusItem("~F1~ Помощь", kbF1, cmHelp)+
            *new TStatusItem("~F10~ Меню", kbF10, cmMenu)+
            *new TStatusItem("~Alt-X~ Выход", kbAltX, cmQuit)
            );
}

TMenuBar *MyApp::initMenuBar(TRect r)
{
 r.b.y=r.a.y+1;

 return new TMenuBar(r,
            *new TSubMenu("~Ф~айлы", kbAltF)+
              *new TMenuItem("~З~агрузить", cmLoadEmul, kbF3, hcLoadEmul, "F3")+
              *new TMenuItem("~С~охранить", cmSaveEmul, kbF2, hcSaveEmul, "F2")+
              *new TMenuItem("С~о~хранить как ...", cmSaveEmulAs, 0, hcSaveEmul,"")+
              newLine()+
              *new TMenuItem("~В~ыход", cmQuit, kbAltX, hcNoContext,"Alt+X")+

            *new TSubMenu("~М~одель_объекта_управления", kbAltM)+
              *new TMenuItem("~В~ставить канал", cmInsChannel,0, hcInsChannel,"Ctrl+Ins")+
              *new TMenuItem("~У~далить канал", cmDelChannel,0, hcDelChannel,"Ctrl+Del")+
              *new TMenuItem("Вз~а~имовлияние", cmXrefChannel, kbCtrlV, hcXrefChannel,"Ctrl+V")+
              newLine()+
              *new TMenuItem("Вставить ~з~вено", cmInsZveno, kbIns, hcInsZveno,"Ins")+
              *new TMenuItem("У~д~алить звено",cmDelZveno, kbDel, hcDelZveno, "Del")+
              *new TMenuItem("~П~араметры звена", cmDefZveno, kbEnter, hcDefZveno, "Enter")+

            *new TSubMenu("~Т~ест_модели", kbAltT)+
              *new TMenuItem("~А~мплитудно-частотная характеристика", cmACX, 0, hcACX, "")+
              *new TMenuItem("~И~мпульсная характеристика", cmPulse, 0, hcPulse, "")+
              newLine()+
              *new TMenuItem("~У~правление полигармонич. сигналом", cmSinus,0, hcSinus, "")+

            *new TSubMenu("~О~кна", kbAltO)+
              *new TMenuItem("~У~порядочить", cmCascade, kbAltY, hcCascade, "Alt+Y")+
              *new TMenuItem("~С~писок окон", cmWinList, kbAlt0, hcWindow ,"Alt+0")
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

