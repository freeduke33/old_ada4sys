#include "adafile.hpp"


const unsigned TemFile::RowsPerItem=128;

TemFile::TemFile(char *fn,tabIndex *idx):NumberTxtFile(fn,1,idx)
{
 loadHeader();
}

void TemFile::loadHeader(void)
{
 unsigned col;
 unsigned long len;

 if(isOk())
   {
   fscanf(f,"%u %f %lu %n",&col,&freq,&len,&HdrSize);
   rows=len;
   Items=col*RowsPerItem;
   }
 return;
}

int TemFile::seek(unsigned long pos)
{
 if(pos==0) pos+=HdrSize;
 return NumberTxtFile::seek(pos);
}

TemArray::TemArray(char *fname,unsigned cacheSize,unsigned idxBlk)
{
 o=NULL; t=NULL; w=NULL;
 o=new lineIdx(idxBlk);
 if(o) t=mkFile(fname);
 if(t) w=new workArea(cacheSize,t);
 return;
}

TemFile *TemArray::mkFile(char *fname) { return new TemFile(fname,o); }

TemArray::~TemArray(void)
{
 if(w) delete w;
 if(t) delete t;
 if(o) delete o;

 return;
}

float TemArray::Val(unsigned long row,unsigned col)
{
 tabItem *i;
 float ret=0;
 float *val;

 if(w && (t->Rows()>row || t->Columns()>col) )
   {
   w->get(row/t->RowsPerItem,&i);
   val=(float *)i->Data();
   ret=val[t->Columns()*(row%t->RowsPerItem)+col];
   i->SetEmpty();
   }
 return ret;
}


