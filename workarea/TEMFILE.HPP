#ifndef __TEMFILE_HPP__
#define __TEMFILE_HPP__

#include "tabidx.hpp"
#include "workarea.hpp"
#include "numtxt.hpp"

class TemplateFile: public NumberTxtFile
{
 public:
  TemplateFile(char *fn,tabIndex *idx=NULL);

  inline unsigned Columns(void )  { return Items/RowsPerItem; }
  inline unsigned long Rows(void) { return rows; }
  inline float Freq(void) { return freq; }

  static const unsigned RowsPerItem;

 protected:
  unsigned HdrSize;
  unsigned long rows;
  float freq;

  virtual int seek(unsigned long);
};

class TemplateArray
{
 public:
   TemplateArray(char *fname,unsigned cacheSize=10,unsigned idxBlk=32);
  ~TemplateArray(void);

   float Val(unsigned long row,unsigned col);
   inline unsigned long Rows(void) { return t ? t->Rows()    : 0; }
   inline unsigned Columns(void)   { return t ? t->Columns() : 0; }
   inline float Freq(void)   { return t ? t->Freq() : 0; }

 protected:
   lineIdx *o;
   TemplateFile *t;
   workArea *w;
};

#endif
