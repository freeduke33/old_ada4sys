#include "workarea.hpp"

class NumberTxtFile: public tabFile
{
 public:
  NumberTxtFile(char *filename,unsigned itm=1024,tabIndex *Offs=NULL);
  virtual ~NumberTxtFile(void);

  virtual int isOk(void) { return f!=NULL; }

 protected:
   unsigned int Items;
   FILE *f;

   virtual unsigned int sizeData(void);
   virtual int read(tabItem *);
   virtual int seek(unsigned long);
   virtual unsigned long pos(void);
};


