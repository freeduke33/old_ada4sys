#include "tabIdx.hpp"

unsigned long bTreeBlk::operator[](unsigned idx) { return Item[idx]; }

void bTreeBlk::Erase(void)
{
 if(lPtr) { lPtr->Erase(); free(lPtr); }
 if(rPtr) { rPtr->Erase(); free(rPtr); }
}





bTreeIdx::bTreeIdx(unsigned long blk):tabIndex(blk) { Root=Tree=NULL; }

bTreeIdx::~bTreeIdx(void)
{
 if(Root)
   {
   Root->Erase();
   free(Root);
   }
 return;
}

unsigned long bTreeIdx::operator[](unsigned long idx)
{
 if(Locate(idx)) return Tree[Idx];
 else return 0;
}

int bTreeIdx::put(unsigned long pos,unsigned long seekPtr)
{
 int ret;

 if( (ret=Locate(pos,1)) ) Tree[Idx]=seekPtr;

 return ret;
}

int bTreeIdx::expand(unsigned long idx,int isCreate)
{
 bTreeBlk *t;
 unsigned long avail;

 if(!isCreate) return 0;
 if( !Root && (Tree=Root=newBlk(0,0))==NULL ) return 0;

 return 0;
}

bTreeBlk *bTreeIdx::newBlk(unsigned idx,unsigned lvl)
{
 bTreeBlk *p;
 p=calloc(1,sizeof(bTreeBlk)+sizeof(unsigned long)*(blkSize-1));
 if(p)
   {
   p->lPtr=p->rPtr=NULL;
   p->Idx=idx;
   p->Level=lvl;
   for(unsigned i=0;i<blkSize;i++) p->Item[i]=0;
   }
 return p;
}

int bTreeIdx::Locate(unsigned long idx,int isCreate)
{
 if(!Root) return expand(idx,isCreate);

 return 0;
}
