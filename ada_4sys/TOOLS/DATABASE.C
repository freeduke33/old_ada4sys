#include <string.h>
#include <stdio.h>

#include "database.h"
#include "dbc.h"
#include "dbcdef.h"

/*-------------------*/
static dBFIELD fldDefs[20];
static RECNUM flds;

float GetNumField(char *fl,long recno,char *fld)
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


int LoadHardwareMap(int chl,int type,int *slot,int *num,Tar *info)
{
 int ret,tmp,ok=0;
 char *fl;
 RECNUM nr,i;
 RECLEN nl;
 int Chl,Type;

 ret=dBopen("datchik.dbf",&fl);
 if(ret==SUCCESS)
   {
   dBgetf(fl,&nl,&tmp,&tmp,&tmp,&flds,&fldDefs);
   dBsize(fl,&nr);
   for(i=1;i<=nr;i++)
     {
     Chl=GetNumField(fl,i,"K_UPR");
     Type=GetNumField(fl,i,"T_DAT");
     if(Chl==chl && Type==type)
        {
        *slot=GetNumField(fl,i,"NAPR_USO");
        *num =GetNumField(fl,i,"KN_USO");

        info->Const  =GetNumField(fl,i,"E");
        info->Delt   =GetNumField(fl,i,"A");
        info->MaxVol =GetNumField(fl,i,"MAX");
        info->MinVol =GetNumField(fl,i,"MIN");
        info->ISO    = 0; /* нечто ? */
        ok=1;
        break;
        }
     }
   dBclose(fl);
   }


 return ok;
}


#ifdef DEBUG

main()
{
 int slot,chl;
 Tar t;
 LoadHardwareMap(1,0,&slot,&chl,&t);
 printf("Slot=%d Chl=%d (E=%f A=%f MAX=%f MIN=%f)",
         slot,chl,t.Const,t.Delt,t.MaxVol,t.MinVol);
 return;
}

#endif
