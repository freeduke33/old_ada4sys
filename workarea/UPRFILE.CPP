#include "adafile.hpp"


UprFile::UprFile(char *fn,tabIndex *idx):TemFile(fn,idx) {;}

void UprFile::loadHeader(void)
{
 char sign[30];
 char tmp[50];
 char title[50];
 unsigned int col;

 if(isOk())
   {
   fscanf(f,"%s %s %s",sign,tmp,title);
   fscanf(f,"%s %s %lu",sign,tmp,&rows);
   fscanf(f,"%s %s %f",sign,tmp,&freq);
   fscanf(f,"%s %s %u",sign,tmp,&col); col*=3;
   HdrSize=ftell(f);
   }
 return;
}

int UprFile::read(tabItem *Itm)
{
 float *val;
 float tmp;
 unsigned int i,k,j;
 unsigned long n;
 int ret=0,r=!EOF;

 if(f)
   {
   if(Itm) val=(float *)Itm->Data();

   for(i=j=0;i<RowsPerItem && r!=EOF;i++)
     {
     r=fscanf(f,"%lu",&n); // skip row number
     for(k=0;k<Columns() && r!=EOF;k++,j++)
       {
       if(Itm) { r=fscanf(f,"%f",&val[j]); if(r==EOF) val[j]=0; }
       else { r=fscanf(f,"%f",&tmp); }
       }
     }
   ret=(r!=EOF) || i>1; // at least 1 item readed successful
   }

 return ret;
}


UprArray::UprArray(char *fname,unsigned cacheSize,unsigned idxBlk):TemArray(fname,cacheSize,idxBlk)
{;}

TemFile *UprArray::mkFile(char *fname) { return new UprFile(fname,o); }

