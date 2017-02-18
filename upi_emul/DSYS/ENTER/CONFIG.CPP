#define Uses_TApplication
#define Uses_TFileDialog
#define Uses_TDeskTop
#define Uses_MsgBox
#define Uses_TFileInputLine

#define Uses_TVision
#define Uses_MyApp
#define Uses_DefEmul

#include "emul_ds.hpp"

#include <fstream.h>
#include <string.h>

void MyApp::LoadEmul(void)
{
 ushort control;
 TFileDialog *fd=new TFileDialog(NikName,"Загрузить эмулятор","Файлы конфигурации :",fdOpenButton|fdHelpButton,10);

 control = deskTop->execView(fd);

 if(control!=cmCancel)
   {
   fd->getFileName(EmulName);
   strncpy(NikName,fd->fileName->data,10);
   NikName[10]=0;
   ViewEmul();
   }

 destroy(fd);
 return;
}

void MyApp::SaveEmul(Boolean isAsk)
{
 ushort control;
 Boolean Do=True;

 if(isAsk)
   {
   TFileDialog *fd=new TFileDialog(NikName,"Сохранить эмулятор","Файлы конфигурации :",fdOKButton|fdHelpButton,10);

   control=deskTop->execView(fd);
   if(control==cmCancel) Do=False;
   else fd->getFileName(EmulName);

   destroy(fd);
   }
 if(Do && Emul)
   {
   ofstream os(EmulName);
   Emul->SaveCfg(os);
   }
 return;
}


void MyApp::ViewEmul(void)
{
 if(Emul)
   {
//   deskTop->remove(Emul);
//   message(Emul,cmClose,evCommand,NULL);
   destroy(Emul);
   }

 Emul=new DefEmul(EmulName,NikName);

 if(lowMemory())
   {
   destroy(Emul); Emul=NULL;
   messageBox("Нет достаточно свободной памяти для выполнения операции",mfError);
   }
 else deskTop->insert(Emul);

 return;
}




