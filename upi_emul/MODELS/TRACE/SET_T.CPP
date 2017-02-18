#include <trace.hpp>

int TestBuildModel::SetParam(void *P)
{
 int i;
 struct _TestBuildModel *T=(struct _TestBuildModel *)P;

 p=T->p;
 E=T->E;

 for(i=0;i<N;i++)
   {
   C[i]=T->C[i];
   Lam[i]=T->Lam[i];
   }
 
 return 0;
}

int TestBuildModel::ParamSize(void) { return sizeof(struct _TestBuildModel); }
