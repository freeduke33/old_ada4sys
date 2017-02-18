#ifndef __NILCLASS_HPP__
#define __NILCLASS_HPP__

#include <iostream.h>

#ifdef __386__
#define _HUGE
#else
#define _HUGE huge
#endif

class NilClass
{
 public:
    // чтение/сохранение объекта из/в поток(а)
   virtual void Save(ostream& );
   virtual void Load(istream& );

   // работа с параметрами
   virtual int GetParam(void *);
   virtual int SetParam(void *);
   virtual int ParamSize(void);

    friend istream& operator>>(istream& ,NilClass* );
    friend ostream& operator<<(ostream& ,NilClass* );

   int ErrObj; // ошибка в состоянии объекта

   static const char NikName[];
};


inline istream& operator>>(istream& is,NilClass* c)
{
 c->Load(is);
 return is;
}

inline ostream& operator<<(ostream& os,NilClass* c)
{
 c->Save(os);
 return os;
}

#endif
