#include <stdio.h>
#include "Adafile.hpp"


int main()
{
 TemArray *A;
 unsigned long i;
 unsigned k;

#ifdef LOGGED
 LOGFILE=fopen("debug.log","w+t");
#endif

 A=new TemArray("noname.tem");

 if(A) for(i=A->Rows();(i+1)>0;i--)
   {
   printf("\n %lu\t",i+1);
   for(k=0;k<A->Columns();k++) printf("%g\t",A->Val(i,k));
   }


#ifdef LOGGED
 fclose(LOGFILE);
#endif
 return 0;
}
