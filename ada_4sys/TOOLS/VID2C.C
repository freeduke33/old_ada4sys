#include <io.h>
#include <fcntl.h>
#include <stdio.h>


int main(int argc,char *argv[])
{
 unsigned char buf[2];
 FILE *fo;
 int fi;

 printf("����� �८�ࠧ������ ����� ⥪�⮢��� ��࠭� � ���ᨢ �몠 `C`.\n");
 if(argc!=4)
   {
   printf("�����:\n ?:\> Vid2C <DumpFile> <TxtFile> <ArrayName>\n");
   return -1;
   }
 fi=open(argv[1],O_RDONLY|O_BINARY);
 fo=fopen(argv[2],"w+t");
 if(fi>0 && fo)
   {
   printf("������� ...\n");
   fprintf(fo,"\ndumpType %s[]={\n",argv[3]);
   while(!eof(fi))
      {
      read(fi,buf,2);
      fprintf(fo,"\t{0x%02X, 0x%02X},\n",(unsigned)buf[0],(unsigned)buf[1]);
      }
   fprintf(fo,"\t{0x00, 0x00}};\n\n");
   fclose(fo);
   close(fi);
   printf("��⮢�\n");
   }
 else printf("�訡�� ������/ᮧ����� 䠩��(��): <%s> <%s>.\n",argv[1],argv[2]);

 return 0;
}