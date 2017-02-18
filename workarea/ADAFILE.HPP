#ifndef __ADAFILE_HPP__
#define __ADAFILE_HPP__

#include "tabidx.hpp"
#include "workarea.hpp"
#include "numtxt.hpp"

class TemFile: public NumberTxtFile
{
 public:
  TemFile(char *fn,tabIndex *idx=NULL);

  inline unsigned Columns(void )  { return Items/RowsPerItem; }
  inline unsigned long Rows(void) { return rows; }
  inline float Freq(void) { return freq; }

  static const unsigned RowsPerItem;

 protected:
  unsigned HdrSize;
  unsigned long rows;
  float freq;

  virtual void loadHeader(void);
  virtual int seek(unsigned long);
};

class TemArray
{
 public:
   TemArray(char *fname,unsigned cacheSize=10,unsigned idxBlk=32);
  ~TemArray(void);

   virtual float Val(unsigned long row,unsigned col);
   inline unsigned long Rows(void) { return t ? t->Rows()    : 0; }
   inline unsigned Columns(void)   { return t ? t->Columns() : 0; }
   inline float Freq(void)   { return t ? t->Freq() : 0; }

 protected:
   lineIdx *o;
   TemFile *t;
   workArea *w;

   virtual TemFile *mkFile(char *);
};


class UprFile:public TemFile
{
 public:
  UprFile(char *fn,tabIndex *idx=NULL);

 protected:
  virtual void loadHeader(void);
  virtual int read(tabItem *);
};

class UprArray:public TemArray
{
 public:
   UprArray(char *fname,unsigned cacheSize=10,unsigned idxBlk=32);

 protected:
   virtual TemFile *mkFile(char *);
};


#endif
