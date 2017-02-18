#include <stdlib.h>
#include "tabidx.hpp"
#include "workarea.hpp"

workArea::workArea(unsigned int size,tabFile *fl)
{
 Size=0;
 Data=(tabItem **)calloc(size,sizeof(tabItem *));
 if(Data)
   {
   Size=size;
   for(unsigned int i=0;i<size;i++) Data[i]=NULL;
   File=fl;
   }
 else dispose();
 return;
}

workArea::~workArea(void) { if(Size) dispose(); }

void workArea::dispose(void)
{
 if(Data)
   {
   for(unsigned int i=0;i<Size;i++) if(Data[i]) delete Data[i];
   free(Data); Data=NULL;
   }
 return;
}

int workArea::get(unsigned long idx,tabItem **t)
{
 unsigned int i;
 int ret=0;

 for(i=0;i<Size;i++)
     if(Data[i] && Data[i]->Index()==idx) { ret=1; break; }

 if(!ret && getEmpty(&i)) ret=File->Read(idx,Data[i]);

 if(ret)
     {
     *t=Data[i];
     UpdateTIMESTAMP(&Data[i]->TIMESTAMP);
     }
 return ret;
}

int workArea::getEmpty(unsigned int *t)
{
 unsigned int i;
 unsigned long m;
 int ret=0;

 for(i=0;i<Size;i++) if(!Data[i])
   {
   *t=i;
   return File->CreateBlank(&Data[i]);
   }

 UpdateTIMESTAMP(&m,1);
 for(i=0;i<Size;i++)
    if(Data[i]->isEmpty() && Data[i]->TIMESTAMP<m)
      {
      *t=i;
      m=Data[i]->TIMESTAMP;
      ret=1;
      }

 return ret;
}


int workArea::put(tabItem *t)
{
 unsigned int i=0;

 if(getEmpty(&i))
   {
   Data[i]=t;
   UpdateTIMESTAMP(&Data[i]->TIMESTAMP);
   File->Write(Data[i]);
   }

 return i;
}

