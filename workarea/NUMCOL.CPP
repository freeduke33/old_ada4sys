#include "NumCol.hpp"

// ------------------------------------------------------------------------ //

NumberColFile::NumberColFile(char *filename,unsigned num,tabIndex *Offs) : tabFile(Offs)
{
 Columns=num;
 f=fopen(filename,"r+t");
 return;
}

NumberColFile::~NumberColFile(void) { if(f) fclose(f); }

int NumberColFile::seek(unsigned long t)
{
 if(f) fseek(f,t,SEEK_SET);
 return 1;
}

unsigned long NumberColFile::pos(void)
{
 if(f) return ftell(f);
 else return 0;
}

int NumberColFile::read(tabItem *Itm)
{
 float *val,tmp;
 unsigned int i;
 int ret=0,r=!EOF;

 if(f)
   {
   if(Itm) val=(float *)Itm->Data();
   for(i=0;i<Columns && r!=EOF;i++)
     {
     if(Itm) { val[i]=0; r=fscanf(f,"%f",&val[i]); }
     else { r=fscanf(f,"%f",&tmp); }
     }
   ret=(r!=EOF);
   }

 return ret;
}


unsigned int NumberColFile::sizeData(void)
{
 return Columns*sizeof(float);
}



