#define INCL_DOSNMPIPES
#include <os2.h>
#include <stdio.h>



char ibuf[100]="",obuf[100]="1234567890",tmp[100];
HPIPE pipe_handle;

int Convert(char *ibuf,char *obuf)
{
 memcpy(obuf,ibuf,10);
 return 0;
}

int doEmulate(void)
{
 PIPEINFO pipe_info;
 AVAILDATA av;
 ULONG cnt,cnt1,state;
 int ret;
 char ch=0;

 do {
    printf("Wait connection ...\n");
    do {
       ret=DosConnectNPipe(pipe_handle);
       ret=DosPeekNPipe(pipe_handle,tmp,0,&cnt,&av,&state);
//       printf("(%lx)\n\r",state);
       } while(state==NP_CLOSING || state==NP_LISTENING);

    printf("Process ...\n");
    while(state==NP_CONNECTED && ret==0)
       {
       ret=DosWrite(pipe_handle,&obuf,10,&cnt);
       ret=DosRead(pipe_handle,&ibuf,10,&cnt);
       DosPeekNPipe(pipe_handle,tmp,0,&cnt1,&av,&state);
       Convert(ibuf,obuf);
       printf("(%lx) <%s>[%ld] --> <%s>\n",state,ibuf,cnt,obuf);
       ch=getch();
       }
    ret=DosDisConnectNPipe(pipe_handle);
    } while(ch!=27);

 printf("Done\n");

 return ret;
}

int main()
{
 char pipe_name[]="\\PIPE\\UPI_Emulator";
 int ret;

#define OPEN NP_ACCESS_DUPLEX | NP_NOINHERIT
#define PIPE NP_TYPE_BYTE | NP_READMODE_BYTE | NP_WAIT | 1

 ret=DosCreateNPipe(pipe_name,&pipe_handle,OPEN,PIPE,100,100,NP_DEFAULT_WAIT);
 switch(ret)
   {
   case 0:
      switch(ret)
        {
        case 0:
           doEmulate();
           break;
        default:
           printf("Error DosConnectNPipe code: %d\n",ret);
           break;
        }
   default:
      printf("Error DosCreateNPipe code: %d\n",ret);
      break;
   }

 if(pipe_handle) DosClose(pipe_handle);
 return ret;
}