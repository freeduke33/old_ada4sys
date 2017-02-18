class tabFile
{
 public:

 tabFile(char *filename,unsigned begCol,unsigned numCol);
 virtual ~tabFile(void);

 float *operator[](unsigned long idx);

 private:

 unsigned long tsize,*tPtr,tmod,curIdx,tabSize;

 float *readValue(void);

 virtual int initCache(unsigned int size)            { return 0; }
 virtual int freeCache(void)                         { return 0; }
 virtual int readCache(unsigned int idx,float **val) { return 0; }
 virtual int toCache(unsigned int idx,float *val)    { return 0; }

 virtual unsigned long calcTabSize(void) =0;
 virtual float *readFile() =0;
 virtual int openFile(char *, unsigned, unsigned) =0;
 virtual int closeFile(void) =0;
 virtual int rewindFile(void) =0;
 virtual int seekFile(unsigned long) =0;
 virtual unsigned long posFile(void) =0;
};

class tabTextFile: public tabFile
{
 public:
 tabTextFile(char *filename,unsigned begCol,unsigned numCol):tabFile(filename,begCol,numCol) {;}

 private:

 virtual unsigned long calcTabSize(void) =0;
 virtual float *readFile() =0;
 virtual int rewindFile(void) =0;

 virtual int openFile(char *);
 virtual int closeFile(void);
 virtual int seekFile(unsigned long);
 virtual unsigned long posFile(void);

 FILE *f;
};


