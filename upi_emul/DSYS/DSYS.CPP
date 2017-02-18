#include <fstream.h>
#include <fcntl.h>

#include <Function.hpp>
#include <Oscill.hpp>
#include "dsys.hpp"

#define MaxChannel 50


UPI_dsys::UPI_dsys(char *filename,int ctl): UPI(ctl,MaxChannel,MaxChannel)
{
 ifstream is(filename,O_RDONLY);

 Ptr=NULL;

 LoadConfig(is);
 return;
}

UPI_dsys::~UPI_dsys(void)
{
 ResetUPI();
 return;
}

void UPI_dsys::ShowConfig(void)
{
 cout << "\nÚŠ ­ «ÂÄÄ Ž¯¨á ­¨¥ ª ­ «  (á¯¨á®ª §¢¥­ì¥¢ + ¯®¬¥å ) ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n";
 cout <<   "ÀÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n";
 return;
}

void UPI_dsys::ResetUPI(void)
{
 Channel *P;
  while(Ptr)
    {
    P=Ptr->next;
    delete Ptr;
    Ptr=P;
    }
 return;
}

int UPI_dsys::LoadConfig(istream& is)
{
 Channel *P,*P1=NULL;
 char id[100];

 ResetUPI();
 return 0;

 is.read(id,100);


 while(!is.eof() && !is.bad())
   {
   P=new Channel;
   P->next=NULL;
   if(P1) P1->next=P; else P1=P; P1=P;
   if(!Ptr) Ptr=P;
   P->Load(is);
   }
 return 0;
}



void UPI_dsys::StartUPI(void)
{
 return;
}

void UPI_dsys::StopUPI(void)
{
 return;
}


int UPI_dsys::BuildUPI(void)
{
 int val=0;

 return val;
}

int UPI_dsys::CalcSignal(float *iBuf,float *oBuf)
{

 return 0;
}


main(int argc, char *argv[])
{
 UPI_dsys *Emul;

 cout << "\nEmulator for UPI&External Object v1.10 (C) by"
        " \"Samara-Trend\" 1994.\n";

 if(_8087==0)  cout << "\007\007I can`t found hardware FPU, aborted.\n";
 else if(argc>=3)
    {
    Emul=new UPI_dsys(argv[1],1);
    if(!Emul->DoLoop(&argv[2])) cout << "\nError: " << Emul->ErrMsg() << "\n";
    delete Emul;
    }
 else cout << "\n Usage: " << argv[0] << "<CfgFile> <ExecProgramFile> [list of parameters for <ExecProgramFile> ]\n";

 return 0;
}
