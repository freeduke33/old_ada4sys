#include "NumTxt.hpp"

// ------------------------------------------------------------------------ //

NumberTxtFile::NumberTxtFile(char *filename,unsigned num,tabIndex *Offs) : tabFile(Offs)
{
 Items=num;
 f=fopen(filename,"r+t");
 return;
}

NumberTxtFile::~NumberTxtFile(void)
{
 if(f) fclose(f);
 tabFile::~tabFile();
}

int NumberTxtFile::seek(unsigned long t)
{
 if(f) fseek(f,t,SEEK_SET);
 return 1;
}

unsigned long NumberTxtFile::pos(void)
{
 if(f) return ftell(f);
 else return 0;
}

int NumberTxtFile::read(tabItem *Itm)
{
 float *val;
 float tmp;
 unsigned int i;
 int ret=0,r=!EOF;

 if(f)
   {
   if(Itm) val=(float *)Itm->Data();
   for(i=0;i<Items && r!=EOF;i++)
     {
     if(Itm) { r=fscanf(f,"%f",&val[i]); if(r==EOF) val[i]=0; }
     else { r=fscanf(f,"%f",&tmp); }
     }
   ret=(r!=EOF) || i>1; // at least 1 item readed successful
   }

 return ret;
}


unsigned int NumberTxtFile::sizeData(void)
{
 return Items*sizeof(float);
}



