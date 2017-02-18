#define Uses_TApplication
#define Uses_TDialog
#define Uses_TEvent
#define Uses_TRect
#define Uses_TDeskTop

#define Uses_TVision
#define Uses_MyApp

#include "emul_ds.hpp"
#include <channel.hpp>

class DefChannel: public Channel,public TDialog
{
 public:
    DefChannel(void);

    virtual void handleEvent(TEvent &);

    int isOpened;

 protected:

    static char *CalcTitle(void);

};


void MyApp::InsChannel(void)
{
 return;
}

void MyApp::DelChannel(void)
{
 return;
}

void MyApp::XrefChannel(void)
{
 return;
}

void MyApp::EdtChannel(void *P)
{
 DefChannel *C=(DefChannel *)P;

 if(C->isOpened) C->setState(sfFocused,True);
 else deskTop->insert(C);
 return;
}


// ====================================================================


char *DefChannel::CalcTitle(void)
{
 char str[]="[ Канал ]";

 return str;
}

DefChannel::DefChannel(void) : TDialog( TRect(1,2,23,50),CalcTitle()),
                               TWindowInit( DefChannel::initFrame )
{
 flags|=wfMove|wfClose|wfGrow;
 return;
}

void DefChannel::handleEvent(TEvent &event)
{
 TDialog::handleEvent(event);
 return;
}


