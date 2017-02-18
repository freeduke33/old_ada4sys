#ifndef __RUN_HPP__
#define __RUN_HPP__

class Buffer;
int MakeEtalon(int ,Buffer **);

int Loop(void);
int doExamine(int Mode);
int doNomodel(void);


extern "C" {
void  ShutDown(void);
void  StartUp(void);

void  RazgruzkaSTART(void);
void  RazgruzkaSTOP(void);
void  GotoNull(void);

void print(int x,int y,char *buf);


 }



#endif
