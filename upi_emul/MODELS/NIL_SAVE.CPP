#include <NilClass.hpp>

void NilClass::Save(ostream& os)
{
 char *buf;
 int len;

 len=ParamSize();
 if(len)
   {
   buf=new char[len];
   if(buf)
     {
     GetParam(buf);
     os.write(buf,len);
     delete buf;
     }
   else ErrObj=1;
   }
 return;
}


