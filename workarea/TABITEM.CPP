#include "workarea.hpp"

tabItem::tabItem(void)  { Ptr=NULL; dispose(); }
tabItem::~tabItem(void) { dispose(); }

void tabItem::dispose(void)
{
 if(Ptr) { free(Ptr); Ptr=NULL; }
 okSize=ptrSize=0;
 empty=1;
 changed=0;
 return;
}

int tabItem::Init(unsigned long idx,unsigned long size)
{
 if(empty)
   {
   if(size>ptrSize)
     {
     dispose();
     if( (Ptr=calloc(1,size)) == NULL ) return 0;
     ptrSize=size;
     }
   okSize=size;
   Idx=idx;
   TIMESTAMP=0;
   UpdateTIMESTAMP(&TIMESTAMP);
   empty=0;
   }

 return !empty;
}


/*
main()
{
 tabItem A,B,C;
 void *p;
 unsigned int sz;


 p=A.Data();
 sz=A.Size();

 A.Init(1,100,p);

}

*/