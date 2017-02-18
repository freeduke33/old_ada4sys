#ifndef __LINK_HPP__
#define __LINK_HPP__

#include <string.h>
#include <NilClass.hpp>


#define NikLength=40;

class RegList
{
 public:
   static NilClass *GetObject(char *NikName);

   virtual NilClass *CreateObj(void) =0;
   virtual int isValidNikName(char *Str) =0;

 protected:
 static RegList *Ptr;
 RegList *next;
};


template <class T>
class Registrate:public RegList
{
 public:
   Registrate(const char Str[]) { Nik=Str; next=Ptr;if(!Ptr) Ptr=this; }
   virtual NilClass *CreateObj(void)
                                { return new T; }
   virtual int isValidNikName(char *Str)
                                { return !strcmp(Str,Nik); }
  protected:
   const char *Nik;
};

#define LINK_ZVENO(ClassName,Nik)   \
     const char ClassName::NikName[]=Nik; \
     static Registrate<ClassName> __force__##ClassName(ClassName::NikName);

#endif
