#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float *X[16],*Y[16],*Yt[16];

int UpdUpr(int mode,char *upr,char *tem)
{
 FILE *f,*ff,*of;
 int i,nki,nko;
 float freq,x,y,yt;
 char tmp[100],tmp1[100],tmp2[100];
 long nr,l,nr1;

 f=fopen(upr,"r+t");
 if(!f) { printf(" Файл <%s> не найден\n",upr); return 2; }
 ff=fopen(tem,"r+t");
 if(!ff) { printf(" Файл <%s> не найден\n",tem); return 2; }
 of=fopen("$$temp.$$$","w+t");
 if(!of) { printf(" Файл <%s> не найден\n","$$temp.$$$"); return 2; }

 fscanf(f,"%s %s %s",tmp,tmp1,tmp2); fprintf(of,"%s %s %s\n",tmp,tmp1,tmp2);
   if(strcmp(tmp2,"\"ПРОТОКОЛ_УПРАВЛЕНИЯ_СТЕНДОМ\""))
     {
     printf("Неверный исходный UPR-файл.\n");
     return 2;
     }
 fscanf(f,"%s %s %ld",tmp,tmp1,&nr); fprintf(of,"%s %s %ld\n",tmp,tmp1,nr);
 fscanf(f,"%s %s %f",tmp,tmp1,&freq);fprintf(of,"%s %s %f\n",tmp,tmp1,freq);
 fscanf(f,"%s %s %d",tmp,tmp1,&nki);fprintf(of,"%s %s %d\n",tmp,tmp1,nki);
 nko=nki;

 for(i=0;i<nki;i++)
   {
   X[i]=calloc(sizeof(float),nr);
   if(!X[i])
      {
      printf("\nData file too long.\n");
      return 1;
      }
   for(l=0;l<nr;l++) X[i][l]=0;
   }

 printf("\nRead ...    ");
 fscanf(ff,"%d %s %lu\n",&nko,tmp1,&nr1); nr1= nr>nr1 ? nr1:nr;
 for(l=0;l<nr1;l++) for(i=0;i<nko;i++)
   {
   fscanf(ff,"%f",&x);
   if(i<nki) X[i][l]=x;
   }
 fclose(ff);

 printf("\nWrite ...     ");
 for(l=0;l<nr;l++)
   {
   fscanf(f,"%s",tmp);
   fprintf(of,"\n%s",tmp);
   for(i=0;i<nki;i++)
   {
   fscanf(f,"%f %f %f",&x,&y,&yt);
        switch(mode)
          {
          case 6: fprintf(of,"\t%f\t%f\t%f",X[i][l],y,yt); break;
          case 7: fprintf(of,"\t%f\t%f\t%f",x,X[i][l],yt); break;
          case 8: fprintf(of,"\t%f\t%f\t%f",x,y,X[i][l]);  break;
          }
        }
     }
 fclose(f);
 fclose(of);

 unlink("oldfile.upr");
 rename(upr,"oldfile.upr");
 rename("$$temp.$$$",upr);
 printf("\nDone.\n");

 return 0;
}


int Cvt2Tem(int mode,char *upr,char *tem)
{
 FILE *f,*ff;
 int i,nki,nko;
 float freq=0;
 char tmp[100],tmp1[100],tmp2[100];
 long nr,l;

 freq+=0;
 f=fopen(upr,"r+t");
 if(!f) { printf("Файл <%s> не найден\n",upr); return 2; }
 ff=fopen(tem,"w+t");
 if(!ff) { printf("Файл <%s> не найден\n",tem); return 2; }

   fscanf(f,"%s %s %s",tmp,tmp1,tmp2);
   if(strcmp(tmp2,"\"ПРОТОКОЛ_УПРАВЛЕНИЯ_СТЕНДОМ\""))
     {
     printf("Неверный исходный файл.\n");
     return 2;
     }
   fscanf(f,"%s %s %ld",tmp,tmp1,&nr);
   fscanf(f,"%s %s %f",tmp,tmp1,&freq);
   fscanf(f,"%s %s %d",tmp,tmp1,&nki);
   nko=nki;

   for(i=0;i<nki;i++)
     {
     X[i]=calloc(sizeof(float),nr);
     Y[i]=calloc(sizeof(float),nr);
     Yt[i]=calloc(sizeof(float),nr);

     if(!X[i] || !Y[i] || !Yt[i])
        {
        printf("\nData file too long.\n");
        return 1;
        }
     }

   printf("\nRead ...     ");
   for(l=0;l<nr;l++)
     {
     fscanf(f,"%s",tmp);
     for(i=0;i<nki;i++)
        fscanf(f,"%f %f %f",&X[i][l],&Y[i][l],&Yt[i][l]);
     }
 fclose(f);

 printf("\nWrite ...    ");

 if(mode==0) i=3;
 else if(mode<=2) i=1;
 else if(mode<=5) i=2;

 fprintf(ff,"%d %g %lu\n",i*nki,freq,nr);
 for(l=0;l<nr;l++)
   {
   for(i=0;i<nki;i++)
    switch(mode)
      {
      case 0: fprintf(ff,"%f\t%f\t%f\t",X[i][l],Y[i][l],Yt[i][l]); break;
      case 1: fprintf(ff,"%f\t",X[i][l]); break;
      case 2: fprintf(ff,"%f\t",Y[i][l]); break;
      case 3: fprintf(ff,"%f\t%f\t",X[i][l],Y[i][l]); break;
      case 4: fprintf(ff,"%f\t%f\t",X[i][l],Yt[i][l]); break;
      case 5: fprintf(ff,"%f\t%f\t",Y[i][l],Yt[i][l]); break;
      }
   fprintf(ff,"\n");
   }
 printf("\nDone.\n");

 return 0;
}

int UpgradeUpr(int mode,char *ifile,char *ofile)
{
 FILE *f,*fo;
 unsigned i,l,nk,nr,nko;
 char tmp1[40],tmp2[40],tmp3[40];

 switch(mode)
   {
   case 9: /* upgrade UPR v1.0 (first) --> oldest */
      f=fopen(ifile,"r+t");
      fo=fopen(ofile,"w+t");
      if(f && fo)
	{
	printf("\nLook ...");
	fscanf(f,"%s %s %s",tmp1,tmp2,tmp3); fprintf(fo,"%s %s %s\n",tmp1,tmp2,tmp3);
	fscanf(f,"%s %s %d",tmp1,tmp2,&nr);  fprintf(fo,"%s %s %d\n",tmp1,tmp2,tmp3);
	fscanf(f,"%s %s %s",tmp1,tmp2,tmp3); fprintf(fo,"%s %s %s\n",tmp1,tmp2,tmp3);
	fscanf(f,"%s %s %d %s %d",tmp1,tmp2,&nk,tmp3,&nko);
	fprintf(fo,"%s %s %d\n",tmp1,tmp2,nk);
	for(i=0;i<nk;i++)
	  {
	  X[i]=calloc(sizeof(float),nr);
	  Y[i]=calloc(sizeof(float),nr);
	  Yt[i]=calloc(sizeof(float),nr);
	  if(!X[i] || !Y[i] || !Yt[i]) { printf("Data file too long.\n"); goto done; }
	  }
	printf("\nRead ...");
	for(i=0;i<nk;i++)
	  {
	  fscanf(f,"%s %s %s",tmp1,tmp2,tmp3);
	  for(l=0;l<nr;l++) fscanf(f,"%s %f %f %f",tmp1,&X[i][l],&Y[i][l],&Yt[i][l]);
	  }
	printf("\nWrite ...");
	for(l=0;l<nr;l++)
	  {
	  fprintf(fo,"\n");
	  for(i=0;i<nk;i++) fprintf(fo,"%f\t%f\t%f\t",X[i][l],Y[i][l],Yt[i][l]);
	  }
	fclose(fo);
	fclose(f);
	printf("\n");
	}
      else { printf("\nInvalid files: <%s> or <%s>",ifile,ofile); }
     break;
   }
done:
 return 0;
}

int main(int argc,char *argv[])
{
 int mode;

 printf("Модуль преобразования файлов данных v1.00 (C) \"Системы Триал\" 1997.\n");
 if(argc!=4)
   {
   printf("Использование:\n"
          " > %s <mode> <file1> <file2>\n"
          "\t<mode> - режим работы\n"
          "\t\t------ Преобразование из UPR (file1) в TEM (file2)\n"
          "\t\t 0 - перенести колонки X,Y,Yt\n"
          "\t\t 1 - перенести колонку X\n"
          "\t\t 2 - перенести колонку Y\n"
          "\t\t 3 - перенести колонки X,Y\n"
          "\t\t 4 - перенести колонки X,Yt\n"
          "\t\t 5 - перенести колонки Y,Yt\n"
          "\t\t------ Замещение UPR (file1) из TEM (file2)\n"
          "\t\t 6 - заместить X\n"
          "\t\t 7 - заместить Y\n"
          "\t\t 8 - заместить Yt\n"
          "\t\t------ Преобразование UPR (file1) в новый формат (file2)\n"
          "\t\t 9 - из формата v1.0 (каналы записаны последовательно)\n"
          "\n",argv[0]);
   return 3;
   }

 mode=atoi(argv[1]);
 printf("\nРежим работы : %d, <%s> <%s>",mode,argv[2],argv[3]);

 if(mode<6 && mode>=0) Cvt2Tem(mode,argv[2],argv[3]); else
 if(mode<9 && mode>=6) UpdUpr(mode,argv[2],argv[3]);  else
 if(mode==9) UpgradeUpr(mode,argv[2],argv[3]); else
 printf(" ? ----- Не знаю что делать.\n");

 return 0;
}

