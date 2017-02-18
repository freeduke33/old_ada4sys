#define DEBUG

#include <float.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <math.h>

#define Uses_FPError
#include <adaptive.hpp>

extern "C" {

jmp_buf FPE_ERR;



void MessageFPE(int sig)
{
 static char str[200];

// strcpy(str,"<ERR>");
/* switch(sig)
   {
   case FPE_INTOVFLOW:      strcat(str, "INT overflow" );               break;
   case FPE_INTDIV0:        strcat(str, "INT divide by 0" );            break;
   case FPE_INVALID:        strcat(str, "Domain" );                     break;
   case FPE_DENORMAL:       strcat(str, "Denormal" );                   break;
   case FPE_ZERODIVIDE:     strcat(str, "Divide by 0" );                break;
   case FPE_OVERFLOW:       strcat(str, "Overflow" );                   break;
   case FPE_UNDERFLOW:      strcat(str, "Underflow" );                  break;
   case FPE_INEXACT:        strcat(str, "Partial loss of precision" );  break;
   case FPE_STACKFAULT:     strcat(str, "Stack fault" );                break;
   default:                 strcat(str, "Unknown" );                    break;
   }*/
// Wperror(str);
#ifdef DEBUG
 if(LOG)  LOG->put('!',"\0x7F");
#endif
 _fpreset();
 return;
}

void GlobalFPE(int sig)
{
 signal(SIGFPE,GlobalFPE);
 _fpreset();
 MessageFPE(sig);
//   longjmp(FPE_ERR,1);
 return;
}

} /* extern "C" */
