#include <stdlib.h>

// саморасширяющаяся таблица индексов
class tabIndex
{
 public:
  tabIndex(unsigned blk=1000) { blkSize=blk; }
 ~tabIndex(void) {;}

  virtual int put(unsigned long idx,unsigned long offset) =0;
  virtual int get(unsigned long idx, unsigned long *offset) =0;
  virtual int getlast(unsigned long *idx,unsigned long *offset) =0;

 protected:
  unsigned blkSize;
};


// таблица из блоков на базе двоичного дерева
typedef struct _bTreeBlk
{
 void Erase(void);
 unsigned long operator[](unsigned);

 struct _bTreeBlk *lPtr,*rPtr;
 unsigned long Idx,Level;

 unsigned long Item[1];
} bTreeBlk;

class bTreeIdx:public tabIndex
{
 public:
  bTreeIdx(unsigned long blkSize);
 ~bTreeIdx(void);

  virtual int put(unsigned long idx,unsigned long offset) =0;
  virtual int get(unsigned long idx, unsigned long *offset) =0;
  virtual int getlast(unsigned long *idx,unsigned long *offset) =0;

 private:
  bTreeBlk *Root,*Tree;
  unsigned Idx;

  int Locate(unsigned long,int isCreate=0);

  int expand(unsigned long,int isCreate);
  bTreeBlk *newBlk(unsigned idx,unsigned lvl);
};


typedef struct _lineBlk
{
 struct _lineBlk *nextPtr,*prevPtr;
 unsigned long Idx;

 unsigned long Item[1];
} lineBlk;

class lineIdx:public tabIndex
{
 public:
  lineIdx(unsigned long blkSize,unsigned long blkAdd=0);
 ~lineIdx();

  virtual int put(unsigned long idx,unsigned long offset);
  virtual int get(unsigned long idx, unsigned long *offset);
  virtual int getlast(unsigned long *idx,unsigned long *offset);

 protected:
 unsigned long blkSizeAdd;
 lineBlk *Cur,*Last;

 int append(unsigned long sz);
 int Expand(unsigned long pos);

 int locate(lineBlk *,unsigned long pos);
 int Locate(unsigned long pos);

 inline unsigned long sizeBlk(lineBlk *P) { return P->prevPtr ? blkSizeAdd : blkSize; }
};

