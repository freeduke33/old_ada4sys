#include "tabfile.hpp"

tabFile::tabFile(char *filename,unsigned begCol,unsigned numCol)
{
 if(openFile(filename,begCol,numCol))
   {
   curIdx=0;
   tabSize=calcTabSize();
   if(tsize)
     {
     tsize=tabSize/100+1;
     if(tsize>MAXPTRSIZE) tsize=MAXTSIZE;
     tPtr=new unsigned long[tsize];
     if(tPtr)
       {
       for(unsigned long i=0;i<tsize;i++) tPtr[i]=0;
       tmod=tabSize/tsize;
       }
     initCache(tabSize);
     }
   else closeFile();
   }
 else tsize=0;
 return;
}

tabFile::~tabFile() {  freeCache(); if(tsize) closeFile(); }

float *tabFile::operator[](unsigned long idx)
{
 float *val;
 unsigned long i,k;

 if(idx>=tabSize) idx=tabSize-1;

 if(readCache(idx,&val)) return val;

 if(curIdx==idx) return readValue();

 if(tPtr)
   {
   i=idx/tmod;
   if(tPtr[i]==0)
     {
     for(k=i;tPtr[k]==0 && k;k--); // first known tPtr
     if(k==0) rewindFile();
     else     seekFile(tPtr[k]);
     curIdx=k*tmod;

     while(k<i)
       {
       readValue();
       if(curIdx/tmod>k) { k++; tPtr[k]=posFile(); }
       }
     }
   else
     {
     seekFile(tPtr[i]);
     curIdx=i*tmod;
     }
   }
 else // tPtr==NULL
   {
   if(curIdx>idx) { rewindFile(); curIdx=0; }
   }

 while(curIdx<=idx) val=readValue();

 return val;
}

float tabFile::readValue()
{
 float v=readFile();

 toCache(curIdx++,v);
 if(curIdx>tabSize) curIdx=tabSize;

 return v;
}


int tabTextFile::openFile(char filename)
{
 f=fopen(filename,"r+t");
 return f!=NULL;
}

int tabTextFile::closeFile()
{
 if(f) fclose(f);
 return 1;
}

int tabTextFile::seekFile(unsigned long t)
{
 if(f) fseek(f,t,SEEK_SET);
 return 1;
}

unsigned long tabTextFile::posFile()
{
 if(f) return ftell(f);
 else return 0;
}




