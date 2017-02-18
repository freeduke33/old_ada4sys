#include <NilClass.hpp>

void NilClass::Load(istream& is)
{
 char *buf;
 int len;

 len=ParamSize();
 if(len)
   {
   buf=new char[len];
   if(buf)
     {
     is. read(buf,len);
     SetParam(buf);
     delete buf;
     }
   else ErrObj=1;
   }
 return;
}


