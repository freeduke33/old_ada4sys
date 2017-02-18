#include <trace.hpp>

int TestBuildModel::GetParam(void *P)
{
 int i;
 struct _TestBuildModel *T=(struct _TestBuildModel *)P;

 T->p=p;
 T->E=E;

 for(i=0;i<N;i++)
   {
   T->C[i]=C[i];
   T->Lam[i]=Lam[i];
   }
 
 return 0;
}
