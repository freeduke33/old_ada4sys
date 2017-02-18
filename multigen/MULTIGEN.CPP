#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include <math.h>
#include <malloc.h>

#include "dac_adc.h"
#include "multigen.h"

#define Uses_gen_Const
#define Uses_gen_Sinus
#define Uses_gen_PSinus
#define Uses_gen_DatFile
#define Uses_gen_PrgFile
#define Uses_AsyncLoop
#define Uses_NilBuffer
#define Uses_MemBuf
#include "adaptive.hpp"


#define min(a,b) ((a)<(b) ? (a) : (b))

unsigned int __stklen=1024*20;

extern "C" { FILE *of,*inf,*disp; }

#include "mgloop.hpp"

Config Cfg;
volatile short Break,go,Act;
Word MaxDAC,MaxADC;
DWord Tik,Cycle;
FModel_1x1 *Gen[CHANNEL];





int LoadCfg(char *file)
{
 int f,val=1,i,k,l;
 unsigned long num,need;
 gen_vBuffer *Tmp[CHANNEL];

 xprintf("Чтение задания из файла <%s> ...",file);
 f=open(file,O_RDONLY|O_BINARY);
 if(f!=-1)
     {
     if(read(f,&Cfg,sizeof(Cfg))==sizeof(Cfg))
       {
       if(!strcmp(Cfg.ID,ID_String)) val=0;
       else xprintf("\nНеизвестный формат конфигурационного файла <%s>.\n",file);
       }
     else xprintf("\nОшибка при чтении файла <%s> или неверный файл.\n",file);
     }
 else xprintf("\nОшибка при открытии файла <%s>.\n",file);

 // ==================[ !!!!!!!!!!!!!!!!!!! ]=====================

 if(!val)
   {
   for(i=l=0;i<Cfg.ActChannel && !val;i++)
      {
      switch(Cfg.Channel[i].Type)
           {
           case Const:  Gen[i]=new gen_Const(Cfg.Channel[i].Dat.ConstVal); break;
           case Sin:    Gen[i]=new gen_Sinus(Cfg.Channel[i].Dat.Sin.Ampl,Cfg.Channel[i].Dat.Sin.Freq,Cfg.Channel[i].Dat.Sin.Faza,1.0/Cfg.Freq);  break;
           case PSin:   Gen[i]=new gen_PSinus(Cfg.Channel[i].Dat.PSin.Ampl,Cfg.Channel[i].Dat.PSin.Freq,Cfg.Channel[i].Dat.PSin.Faza,1.0/Cfg.Freq); break;
           case DatFile:Tmp[l]=new gen_DatFile(Cfg.Time,Cfg.Channel[i].Dat.DatFile.Name,Cfg.Channel[i].Dat.DatFile.MaxVol);
                        Gen[i]=Tmp[l++];
                        break;
//           case _PrgFile:Gen[i]=new gen_PrgFile(&Cfg.Channel[i].Dat.PrgFile); break;
           }
      if(!Gen[i]) val=2;
      }
   }

 if(!val)
  {
  for(i=need=0;i<l;i++) need+=Tmp[i]->NeedMemory();
  num=coreleft()-1024*10;
  fprintf(disp,"Ok\nСвободно памяти : %luKb,  "
         "необходимо для работы : %luKb\n",num/1024+1,need/1024+1);
  if(need<num) for(i=0;i<l;i++) Tmp[i]->CreateBuffer(0);
  else
     {
     for(i=k=0;i<l;i++) if(Tmp[i]->NeedMemory()<num/Cfg.ActChannel)
        {
        Tmp[i]->CreateBuffer(0); k++;
        }
     if(k!=Cfg.ActChannel)
       {
       num=coreleft()-1024*10;
       for(i=0;i<l;i++) Tmp[i]->CreateBuffer(num/(Cfg.ActChannel-k)-1);
       }
     }

  }

 if(val==2) xprintf("Слишком мало памяти для работы.\n");

 return !val;
}



int DoGenerate(void)
{
 aLoop Ctrl(Cfg.ActChannel);
 NilBuffer buf[CHANNEL];
 MemBuf mbufX[CHANNEL],mbufY[CHANNEL];
 int i,val=ERR_NONE;
 char msg[80];
 DWord Skip,siz;
 FILE *fl;



 for(i=0;i<Cfg.ActChannel && !Break;i++)
   {
   Break=!Gen[i]->isSuccessful(msg);
   Ctrl.SetBuffer(i,&mbufX[i],&mbufY[i],&buf[i]);
//   Ctrl.SetBuffer(i,&buf[i],&buf[i],&buf[i]);
   }
 if(Break)
   {
   fprintf(disp," *** Проблемы при настройке канала #%d : %s\n",i,msg);
   return(Break);
   }

 // время выхода на начальную точку генерации
 Ctrl.SetPassMargin(Cfg.Beg);
 // время работы с превышением погрешности
 Ctrl.SetMistakeWait(Cfg.ErrTime*Cfg.Freq);

 fprintf(disp,"Настройки УСО: %gHz, %dx1, elapsed time %s\n",
           Cfg.Freq,Cfg.ActChannel,elapsTime((unsigned long)(Cfg.Time*Cfg.Cycle/Cfg.Freq)));
 xprintf("Настройка каналов ... ");

 Tik=coreleft();
 Tik=0;

 for(i=0;i<Cfg.ActChannel;i++) Gen[i]->Build();
 for(i=0;i<Cfg.ActChannel;i++) Ctrl.SetModel(i,Gen[i],Gen[i],NULL);



 siz=(coreleft()-1024*(6/**Cfg.ActChannel*/))/(sizeof(float)*Cfg.ActChannel*2);
 siz=min(siz,Cfg.Time);
 siz=min(siz,64000L/sizeof(float));

 if(siz)
   {
   for(i=0;i<Cfg.ActChannel;i++)
      {
      mbufX[i].setSize(siz);      mbufX[i].lockSize();
      mbufY[i].setSize(siz);      mbufY[i].lockSize();
      }
   }


 xprintf("Осталось свободной памяти: %uKb\n",coreleft()/1024);
 if(coreleft()>1024*5) val=Ctrl.PassLoop(Cfg.Freq,Cfg.Cycle,Cfg.Time);
 else val=ERR_NOMEM;

 switch(val)
     {
     case ERR_NONE:  xprintf(" Ok\n"); break;
     case ERR_BUILD: xprintf(" *** Проблемы при запуске задания ***\n"); xwait();break;
     case ERR_NOMEM: xprintf(" *** Нужно больше свободной памяти ***\n"); xwait();break;
     case ERR_UPI:   xprintf(" *** Ошибка интерфейса УСО ***\n");     xwait();break;
     case ERR_USER:  xprintf(" *** Прервано пользователем ***\n");    xwait();break;
     }

 if(val==ERR_NONE || val==ERR_USER)
   {
   xprintf("Запись отчета ...");
   fl=fopen("out.upr","w+t");
   fprintf(fl,"+++ Заголовок_: \"ПРОТОКОЛ_УПРАВЛЕНИЯ_СТЕНДОМ\"\n");
   fprintf(fl,"+++ Длинна_ряда_: %lu\n",siz);
   fprintf(fl,"+++ Частота_дискретизации_: %f\n",Cfg.Freq);
   fprintf(fl,"+++ Число_каналов_: %d x %d\n",Cfg.ActChannel,Cfg.ActChannel);

   for(i=0;i<Cfg.ActChannel;i++)
     {
     fprintf(fl,"\n+++ Канал_N %d\n",i+1);
     mbufX[i].reset();
     mbufY[i].reset();
     for(siz=0;siz<mbufX[i].getSize();siz++)
       fprintf(fl,"%lu\t%f\t%f\t%f\n",siz,mbufX[i].getNext()*5,mbufY[i].getNext()*5,0);
/*     mbufX[i].unlockSize(); mbufX[i].setSize(0);
     mbufY[i].unlockSize(); mbufY[i].setSize(0); */
     if(Gen[i]) delete Gen[i];
     }

   fclose(fl);
   xprintf("Ok\n");

   DrvGetSkipped(&Skip);
   if(Skip)
      {
      xprintf("Пропущено %lu отсчетов.\n",Skip);
      xwait();
      }

   }

 return Break;
}


main(int argc,char *argv[])
{
 int ret;

 if(!DrvIsPresent() || DrvInitialize()!=DRV_OK)
   {
   printf("Драйвер УСО не найден.\n");
   return -1;
   }



/*  directvideo=0;
  textcolor(LIGHTGRAY);
  textbackground(BLACK); */

//  _heapgrow();

// of=fopen("output.txt","w+t"); if(!of) return -5;
// inf=fopen("input.txt","w+t"); if(!inf) return -5;

 if(argc>2 && !strcmp(strupr(argv[2]),"/SUBROUTINE")) disp=stdin;
 else disp=stdout;

 fprintf(disp,"Многоканальный генератор v1.00 (C) Samara-\"Trend\" 1995,96.\n");
 if(argc<2 || argc>3)
     {
     xprintf("Запуск: %s <CfgFile> [/SUBROUTINE]\n",argv[0]);
     xwait();
     exit(-1);
     }
 if(LoadCfg(argv[1]))
         ret=DoGenerate();
 else ret=-1;

// fclose(of);
// fclose(inf);

 DrvDeinitialize();

 return ret;
}
