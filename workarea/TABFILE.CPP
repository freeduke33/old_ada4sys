#include "tabIdx.hpp"
#include "workarea.hpp"

tabFile::tabFile(tabIndex *offs)
{
 Offset=offs;
 return;
}


int tabFile::Read(unsigned long idx,tabItem *Itm)
{
 unsigned long p;
 unsigned long last;
 int ret=1;

 if(Offset)
   {
   if(Offset->get(idx,&p)) seek(p);
   else
     {
     if(!Offset->getlast(&last,&p)) Offset->put(last=0,p=0);
     seek(p);

     for(;last<idx && ret;last++)
        {
        ret=read(NULL);
        if(ret) Offset->put(last+1,pos());
        }
     }

   if(ret)
     {
     Itm->Init(idx,sizeData());
     if(!Itm->isEmpty()) ret=read(Itm);
     if(ret) Offset->put(idx+1,pos());
     }
   }
 else ret=0;

 return ret;
}


int tabFile::Write(tabItem *)
{
 return 0;
}

int tabFile::CreateBlank(tabItem **Itm)
{
 *Itm= new tabItem();
 return *Itm!=NULL;
}
