#include <alloc.h>
#include <conio.h>

#define TBL_SIZE 1000

static void *table[TBL_SIZE];
static unsigned tbl=0;


void * operator new(size_t size)
{
 void *Ptr=NULL;

 if(tbl<TBL_SIZE) if((Ptr=calloc(size,1))!=0) table[tbl++]=Ptr;
 return Ptr;
}

void operator delete(void *Ptr)
{
 unsigned i;

 if(!Ptr) return;

 for(i=0;i<tbl;i++) if(table[i]==Ptr)
   {
   for(;i<tbl;i++) table[i]=table[i+1]; tbl--;
   free(Ptr);
   return;
   }
 gotoxy(1,24);
 cprintf("\007\007Invalid memory blok to free :%p",(void far *)Ptr);
 getch();
}
