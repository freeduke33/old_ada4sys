#include <stdio.h>
#include <conio.h>
#include <math.h>


MakeSin(FILE *f,int act,int chl,float Freq,unsigned int nq,
        float Ampl,float minfreq,int Periods)
{
 long N,w,SN=0;
 float xk,freq=minfreq,df=0.025;
 long i,k;


 for(i=0;;i++)
   {
   N=(Periods*Freq)/freq;
   printf("Point %ld ...\r",SN);

   for(w=0;w<N;w++)
      {
      xk=Ampl*sin(2.0*M_PI*freq*w/Freq);
      for(k=0;k<chl;k++) fprintf(f,"%f\t",act==k ? xk:(float)k);
      fprintf(f,"\n");

      if(SN++==nq) goto done;
      }
   freq+=df;
   }

done:
 printf("\nActual size = %lu\n",SN);
}


main()
{
 FILE *f;
 int nk=4;
 float freq=100;
 long len=40000L;

 printf("Writing ...\n");
 f=fopen("psinus.tem","w+t");
 if(f)
   {
   fprintf(f,"%d %f %lu\n",nk,freq,len);
   MakeSin(f,0,nk,freq,len,1,2,10);
   fclose(f);
   }
 return 0;
}
