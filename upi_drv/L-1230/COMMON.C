#include <dos.h>
#include <conio.h>
#include "lcard.h"




lcRet putCmd(int base,unsigned cmd)
{
 int v=0;

 outport(COMMAND+base,cmd);
 inport(DATA+base);
 while(!(inport(CONTROL+base) & 0x02) && v++<10000) cputs("c\b");

 return lc_NONE;
}

lcRet putData(int base,unsigned val)
{
 int r,v=0;
 outport(DATA+base,val);
 while(!((r=inport(CONTROL+base)) & 0x02) && v++<10000) cputs("w\b");

 return lc_NONE;
}


lcRet getData(int base,unsigned *val)
{
 int v=0;

 while(!(inport(CONTROL+base) & 0x01) && v++<10000) cputs("r\b");
 *val=inport(DATA+base);

 return lc_NONE;
}

