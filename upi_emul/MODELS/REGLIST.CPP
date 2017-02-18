#include "link.hpp"

NilClass *RegList::GetObject(char *Nik)
{
 NilClass *P=NULL;
 RegList *tmp=Ptr;

 while(tmp && !P)
   {
   if(tmp->isValidNikName(Nik)) P=tmp->CreateObj();
   else tmp=tmp->next;
   }
 return P;
}