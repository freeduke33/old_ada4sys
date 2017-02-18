#include <trace.hpp>

TestBuildModel::TestBuildModel()
{
 int i;

 p=0;
 for(i=0;i<N;i++) C[i]=Lam[i]=0;

 for(i=0;i<NMAX;i++) x[i]=y[i]=0;
 return;
}