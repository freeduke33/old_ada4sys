#include "workarea.hpp"

class NumberColFile: public tabFile
{
 public:
  NumberColFile(char *filename,unsigned col=1024,tabIndex *Offs=NULL);
  virtual ~NumberColFile(void);

  virtual int isOk(void) { return f!=NULL; }

 protected:
   unsigned int Columns;
   FILE *f;

   virtual unsigned int sizeData(void);
   virtual int read(tabItem *);
   virtual int seek(unsigned long);
   virtual unsigned long pos(void);
};


