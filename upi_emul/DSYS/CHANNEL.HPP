#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <iostream.h>
#include <NilClass.hpp>

class NilZveno;

class Channel:public NilClass
{
 public:
    Channel(void);

    virtual void Save(ostream& os);
    virtual void Load(istream& is);

    int huge Build(void);
    float huge Calc(float );

    Channel *next;

 protected:
    NilZveno *Ptr; // ���� � ᯨ᪥ ����쥢 �⮣� ������
};


#endif
