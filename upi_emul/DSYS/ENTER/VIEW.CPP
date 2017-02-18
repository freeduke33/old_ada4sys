#define Uses_TApplication
#define Uses_TEvent
#define Uses_TDialog
#define Uses_TKeys
#define Uses_TListBox
#define Uses_TButton
#define Uses_TScrollBar
#define Uses_TRect
#define Uses_TDeskTop
#define Uses_MsgBox
#define Uses_TStringCollection

#define Uses_TVision
#define Uses_DefEmul
#define Uses_Const

#include "emul_ds.hpp"

#include <iostream.h>

#include <Channel.hpp>
#include <dsys.hpp>



//========================================================================

DefEmul::DefEmul(char *fname,char *name):
       TDialog(TRect(3, 1, 40, 18), name),
       TWindowInit(&TDialog::initFrame)
{
 TView *control;

 chgFlag=0;

 control = new TScrollBar(TRect(34, 2, 35, 11));
 insert(control);

 control = new TListBox(TRect(3, 2, 34, 11), 1, (TScrollBar*)control);
 control->options |= ofFramed;
 insert(control);

 control = new TButton(TRect(2, 12, 34, 14), "Взаимовлияние каналов", cmXrefChannel, bfNormal);
 insert(control);

 control = new TButton(TRect(2, 14, 21, 16), "Выбрать канал", cmOK, bfDefault);
 insert(control);

 control = new TButton(TRect(21, 14, 34, 16), "Закончить", cmClose, bfNormal);
 insert(control);


 selectNext(False);

 DSYS=new UPI_dsys(fname,1);


 D.CHL.collection=new TStringCollection(50,10);
 D.CHL.focused=1;

 setData(&D);

 return;
}

DefEmul::~DefEmul(void)
{
 if(D.CHL.collection) destroy(D.CHL.collection);
 if(DSYS) delete DSYS;
 return;
}



void DefEmul::handleEvent(TEvent& event)
{
 Channel *TT=DSYS->GetChannels();
 int i;
 TEvent e;

 if(event.what==evCommand)
   {
   switch(event.message.command)
     {
     case cmClose:
       e.what=evBroadcast;
       e.message.command=cmCloseEmul;
       e.message.infoPtr=this;
       putEvent(e);
       clearEvent(e);
       event.message.infoPtr=this;
       break;
     case cmOK:  // select channel
       getData(&D);
       for(i=0;i<D.CHL.focused && TT!=NULL;i++) TT=TT->next;
       if(TT) ; // ?????????
       else  messageBox("Попытка открыть несуществующий канал",mfError|mfOKButton);
       break;
     case cmXrefChannel:
       messageBox("Здесь взаимовлияние каналов",mfOKButton);
       break;
     }
   }
 TDialog::handleEvent(event);
 return;
}


void DefEmul::SaveCfg(ostream& os)
{
 return;
}


