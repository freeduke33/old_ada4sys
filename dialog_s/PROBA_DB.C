//Проверка библиотеки DBC.LIB
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "dbc.h"
#include "dbcdef.h"

/*-------------------*/
dBFIELD fldDefs[20];
RECNUM flds;

float getNumField(char *fl,long recno,char *fld)
{
 int i,ofs=0,r=0;
 char tmp[30];
 static char buf[1000];
 float val;

 dBgetr(fl,recno,&buf,&val);//Дать запись базы данных

 for(i=0;i<flds && r==0;i++)
   {
   if(!strcmp(fld,fldDefs[i].fieldnm)) r=1;
   else ofs+=fldDefs[i].width;
   }
 if(r)
   {
   i--;
   strncpy(tmp,&buf[ofs],fldDefs[i].width); tmp[fldDefs[i].width]=0;
//   for(i=0;tmp[i] && tmp[i]==' ';i++);
   sscanf(tmp,"%f",&val);
   }
 return val;
}
/*----------------------------*/

main()
{
char *fl;
int aa,bb,tmp;
char filename[31];
RECNUM ss;
float v;

clrscr();
ss=111;
aa=dBopen("datchik.dbf",&fl);
dBgetf(fl,&ss,&tmp,&tmp,&tmp,&flds,&fldDefs);
dBsize(fl,&ss);
  /**/
v=getNumField(fl,1,"A");
  /**/
printf("%d\n",aa);
bb=dBclose(fl);
printf("%d\n",bb);
printf("%ld\n",ss);
getchar();
}

