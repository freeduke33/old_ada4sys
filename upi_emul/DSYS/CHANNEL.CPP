
#include "channel.hpp"
#include <zveno.hpp>
#include <link.hpp>


Channel::Channel(void)
{

};

int Channel::Build(void)
{
 int ret=1;
 NilZveno *P=Ptr;

 while(P && ret)
   {
   ret=P->Build();
   P=P->next;
   }
 return ret;
}

float Channel::Calc(float val)
{
 NilZveno *P=Ptr;

 while(P)
   {
   val=P->Calc(val);
   P=P->next;
   }
 return val;
}

void Channel::Load(istream& is)
{
 char ch=',',Nik[NikLength];
 NilZveno *Z;
 int ok=1;


 while(ch==',' && ok)
   {
   is >> Nik; // читаем идентификатор звена
   Z=(NilZveno *)RegList::GetObject(Nik);
   if(Z) { Z->next=Ptr; Z->Load(is); Ptr=Z; }
   else ok=0;
   is >> ch;  // ? разделитель звеньев или конец канала
   }
 return;
}

void Channel::Save(ostream& os)
{
 char Nik[NikLength];
 NilZveno *Z=Ptr;
 int ok=1;


 while(Z && ok)
   {
   os << Z->NikName; // пишем идентификатор звена
   Z->Save(os); Z=Z->next;
   if(Z) os << ','; // будет продолжение
   else  os << '.'; // конец канала
   }
 return;
}

