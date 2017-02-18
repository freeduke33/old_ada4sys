#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>


#include "immak.h"

#include "tcxlmnu.h"
#include "tcxlent.h"
#include "tcxlwin.h"
#include "tcxlcod.h"
#include "tcxlcur.h"
#include "tcxlinp.h"
#include "tcxlsel.h"

// #include "data_glb.h"
#include "color.h"
#include "string.h"

#include "upi_3w.h"


VOID CTYP WZK(NOARG);
VOID CTYP SinParm(NOARG);
VOID CTYP PSinParm(NOARG);
VOID CTYP RandParm(NOARG);
VOID CTYP TestParm(NOARG);
VOID CTYP ArssParm(NOARG);

int _USE_=0,Chl,Z_curr;


struct _param_3w def;

/*
┌──────── Имитационная модель ─────────────┐
│ Частота дискретизации    ХХХХХХ.ХХХХ  Гц │
├1────функциональное───────┬──вид помехи───┤
│ Вид нелин. КВАДРАТИЧЕСКАЯ│               │
│ К. передачи    ХХХ.ХХХХ  │ ПОЛИГАРМОНИКА │
├2────апериодическое───────┤               │
│ Порядок │ Fв  XXXXX.XXXX │ Ампл. XXXX.XX │
│  ХХХХ   │ Fн  XXXXX.XXXX │ Част. XXXX.XX │
├3────колебательное────────┤ Фаза   XXX.XX │
│ Степень усп.  ХХ.ХХХХХХХ ├────статика────┤
│ Рез. частота  ХХХХХ.ХХХХ │ U= XXXXX.XXXX │
└──────────────────────────┴───────────────┘

*/
LCL VposT DOS_cur_POS;

LCL VOID CTYP ActPIC(NOARG);
LCL VOID CTYP DeActPIC(NOARG);
LCL VOID CTYP ZvenoView(int num);


LCL char names[][40]={"отсутствует",
                      "функциональное",
                      "апериодическое",
                      "колебательное",
                      "колеб.(перемещение)",
                      "колеб.(скорость)",
                      "колеб.(ускорение)",
                      "СИНУСОИДА",
                      "ПОЛИГАРМОНИКА",
                      "П/БЕЛЫЙ ШУМ",
                      "ИНВЕРСИЯ",
                      "двухмассовое",
                      "тест модели",
                      "АР-СС модель"};
LCL char FuncNames[][20]={"ЛИНЕЙНОЕ",
                          "КВАДРАТИЧЕСКОЕ",
                          "КУБИЧЕСКОЕ",
                          "ГИСТЕРЕЗИС",
                          "СКОРОСТЬ",
                          "УСКОРЕНИЕ"};

LCL char _Zveno[10][40];


LCL char FldStr1[20],FldStr2[20],FldStr3[20],FldStr4[40];
char OptStr3[]="ИМИТАЦИОННАЯ МОДЕЛЬ";
LCL int FldType=0;

IntT CTYP _Wputl(ChrP str) { return(strlen(str)); }

int CTYP Error(char *str)
{
 int val;
 int sav=VidFlg;
 VidParm(VP_BIO);
 val=Wperror(str);
 VidFlg=sav;
 return(val);
}



VOID CTYP SubFunEnd(NOARG)
{
 KquePut(Key_Tab);
 KquePut(Key_S_Tab);
 return;
}

int CTYP SubFunBeg(NOARG)
{
 KeyT k;
 FdfP f;

 CurHide();
 MouPush(MOU_FULL);
 MouShow();
 k=KeyGetc();
 MouPop();
 CurShow();


 if(k==0x3920) return(0);
 else
 {
  if(!(k==Key_Up || k==Key_Dwn || k==Key_Esc || k==Key_Ent || k==Key_C_Ent
    || k==Key_Tab || k==Key_S_Tab || k==Key_C_Home || k==Key_C_End
    || (k&0xff00)==0xd400))
  {
   f = FldCur();
   if(FdfPrv(f) || FdfUp(f)) k = Key_Up;
   else k=Key_Dwn;
  }
  KquePut(k);
 }
 return(1);
}




VOID CTYP OpenW(NOARG)
{
 Wshadow(C_SHADOW);
 CurHide();
 WsetEsc(TRUE);
 return;
}


int CTYP MemAllocErr(int arg)
{
 static struct {int Freq,delay;} melody[]={
#include "alarm.inc"
          {0,-1}};
 int i=0,val;

 if(TcxlErr==W_ALLOCERR)
   {
   if(Error("Мало свободной памяти")==W_ALLOCERR && arg==0)
      {
      while(melody[i].delay!=-1 && !(KeyPeek()))
        {
        if(!melody[i].Freq) nosound();
        else sound(melody[i].Freq);
        delay(melody[i++].delay);
        }
      nosound();
      }
   if(arg)
      {
      WcloseAll();
      VcurPut(DOS_cur_POS);
      fprintf(stderr,"Internal: Memory allocations failure\n");
      exit(-1);
      }
   val=1;
   }
 else  val=0;
 return(val);
}


/*====================[ Func Mode ]=====================*/

LCL IbldT FuncItm[6]=
{{FuncNames[0],'\0',ITM_AFT, 0,0, 44},
 {FuncNames[1],'\0',ITM_AFT, 1,0, 45},
 {FuncNames[2],'\0',ITM_AFT, 2,0, 46},
 {FuncNames[3],'\0',ITM_AFT, 3,0, 47},
 {FuncNames[4],'\0',ITM_AFT, 4,0, 48},
 {FuncNames[5],'\0',ITM_AFT, 5,0, 49},
};

LCL  MbldT FuncMnu=
{&FuncItm[0],6, 7,20,14,36,  MNU_SAV|MNU_VER, BOX_SNG,
 C_WBOX,C_WTXT,C_WSEL, C_CHR,C_NSEL,C_WASEL,
   44, OpenW,  0, 10,  1, 0xFF, 0xFF, 0xFF
};

LCL VOID CTYP FuncRun(NOARG)
{
 int s;
 FdfP f;

 f=FldCur();
 if(SubFunBeg()) return;
 MbldTag(&FuncMnu)=FldType+44;
 if(MnuBuild(&FuncMnu)==ERR_MEM) MemAllocErr(0);
 else
   {
   s=MnuGet();
   if(s)
     {
     FldType=s-44;
     strcpy(FdfBfr(f), FuncNames[FldType]);
     FldChg();
     }
   }

 SubFunEnd();
 return;
}
/*====================[ Func ]=====================*/
LCL FbldT FuncFld[2]=
{{"Вид нелинейности"    ,0,0,  FldStr1, 0,18,"***************",
  FuncRun,NULL,NULL, 0,170,0, FLD_NUL,0,FLD_CHG},
 {"Коэффициент передачи",1,0,  FldStr2, 1,23,"999.9999",
  NULL,NULL,NULL, 0,171,0, FLD_DEC,0,FLD_CHG}
};

EbldT FuncFrm=
{FuncFld,2,NULL, NULL,0, ENT_SAV,
 C_WTXT,C_WSEL,C_WASEL,C_WNSEL};


LCL VOID CTYP FuncParm(NOARG)
{
 int val;

 Wopen(8,12,11,48,BOX_DBL,C_WBOX,C_WTXT);
 if(!MemAllocErr(0))
   {
   Wshadow(C_SHADOW);
   FldType=def.Fm[Chl][Z_curr].Type;
   strcpy(FldStr1,FuncNames[FldType]);
   cvtfc(FldStr2,def.Fm[Chl][Z_curr].k,8,4);
   EntPush();
   if(EntBuild(&FuncFrm)==ERR_MEM) MemAllocErr(0);
         {
     val=EntGet(FuncFld[0].ftag);
     EntFree();
         }
   EntPop();
   if(val!=ERR_ARG)
     {
     def.Fm[Chl][Z_curr].k=cvtcf(FldStr2);
     def.Fm[Chl][Z_curr].Type=FldType;
     }
   Wclose();
   }
 return;
}

/*======================[ Aperiod ]========================*/
LCL FbldT AperFld[3]=
{{"Верхняя граница"    ,0,0,  FldStr1, 0,20,"99999.9999",
  NULL,NULL,NULL, 0,170,0, FLD_DEC,0,FLD_CHG},
 {"Нижняя  граница"    ,1,0,  FldStr2, 1,20,"99999.9999",
  NULL,NULL,NULL, 0,170,0, FLD_DEC,0,FLD_CHG},
 {"Порядок фильтра"    ,2,0,  FldStr3, 2,26,"9999",
  NULL,NULL,NULL, 0,171,0, FLD_DEC,0,FLD_CHG}
};

EbldT AperFrm=
{AperFld,3,NULL, NULL,0, ENT_SAV,
 C_WTXT,C_WSEL,C_WASEL,C_WNSEL};

LCL VOID CTYP AperParm(NOARG)
{
 int val;

 Wopen(8,12,12,48,BOX_DBL,C_WBOX,C_WTXT);
 if(!MemAllocErr(0))
   {
   cvtfc(FldStr1,def.Am[Chl][Z_curr].FV,10,4);
   cvtfc(FldStr2,def.Am[Chl][Z_curr].FN,10,4);
   cvtic(FldStr3,def.Am[Chl][Z_curr].p,4);
   EntPush();
   if(EntBuild(&AperFrm)==ERR_MEM) MemAllocErr(0);
   else
         {
     val=EntGet(AperFld[0].ftag);
     EntFree();
         }
   EntPop();
   if(val!=ERR_ARG)
     {
     def.Am[Chl][Z_curr].FV=cvtcf(FldStr1);
     def.Am[Chl][Z_curr].FN=cvtcf(FldStr2);
     def.Am[Chl][Z_curr].p =cvtci(FldStr3);
     }
   Wclose();
   }
 return;
}

/*=====================[ Kolebatel ]=======================*/
LCL FbldT KolbFld[2]=
{{"Степень успокоения"    ,0,0,  FldStr1, 0,20,"99.9999999",
  NULL,NULL,NULL, 0,170,0, FLD_DEC,0,FLD_CHG},
 {"Резонансная частота"   ,1,0,  FldStr2, 1,20,"99999.9999",
  NULL,NULL,NULL, 0,171,0, FLD_DEC,0,FLD_CHG}
};

EbldT KolbFrm=
{KolbFld,2,NULL, NULL,0, ENT_SAV,
 C_WTXT,C_WSEL,C_WASEL,C_WNSEL};

LCL VOID CTYP KolbParm(NOARG)
{
 int val;

 Wopen(8,12,11,48,BOX_DBL,C_WBOX,C_WTXT);
 if(!MemAllocErr(0))
   {
   cvtfc(FldStr1,def.Km[Chl][Z_curr].Betta,10,7);
   cvtfc(FldStr2,def.Km[Chl][Z_curr].Freq ,10,4);
   EntPush();
   if(EntBuild(&KolbFrm)==ERR_MEM) MemAllocErr(0);
   else
         {
     val=EntGet(KolbFld[0].ftag);
     EntFree();
         }
   EntPop();
   if(val!=ERR_ARG)
     {
     def.Km[Chl][Z_curr].Betta=cvtcf(FldStr1);
     def.Km[Chl][Z_curr].Freq =cvtcf(FldStr2);
     }
   Wclose();
   }
 return;
}

/*=====================[ SecondMass ]=======================*/
LCL FbldT KolbFld1[4]=
{{"Степень успокоения"    ,1,0,  FldStr1, 1,20,"99.9999999",
  NULL,NULL,NULL, 0,170,0, FLD_DEC,0,FLD_CHG},
 {"Резонансная частота"   ,2,0,  FldStr2, 2,20,"99999.9999",
  NULL,NULL,NULL, 0,171,0, FLD_DEC,0,FLD_CHG},
 {"Степень успокоения"    ,4,0,  FldStr3, 4,20,"99.9999999",
  NULL,NULL,NULL, 0,170,0, FLD_DEC,0,FLD_CHG},
 {"Резонансная частота"   ,5,0,  FldStr4, 5,20,"99999.9999",
  NULL,NULL,NULL, 0,171,0, FLD_DEC,0,FLD_CHG}
};

EbldT KolbFrm1=
{KolbFld1,4,NULL, NULL,0, ENT_SAV,
 C_WTXT,C_WSEL,C_WASEL,C_WNSEL};

LCL VOID CTYP MassParm(NOARG)
{
 int val;

 Wopen(5,12,12,48,BOX_DBL,C_WBOX,C_WTXT);
 Wprts(0,3,C_WTXT,"--- Параметры массы N1 ---");
 Wprts(3,3,C_WTXT,"--- Параметры массы N2 ---");
 if(!MemAllocErr(0))
   {
   cvtfc(FldStr1,def.Mm[Chl][Z_curr].Betta[0],10,7);
   cvtfc(FldStr2,def.Mm[Chl][Z_curr].Freq[0] ,10,4);
   cvtfc(FldStr3,def.Mm[Chl][Z_curr].Betta[1],10,7);
   cvtfc(FldStr4,def.Mm[Chl][Z_curr].Freq[1] ,10,4);
   EntPush();
   if(EntBuild(&KolbFrm1)==ERR_MEM) MemAllocErr(0);
   else
         {
     val=EntGet(KolbFld1[0].ftag);
     EntFree();
         }
   EntPop();
   if(val!=ERR_ARG)
     {
     def.Mm[Chl][Z_curr].Betta[0]=cvtcf(FldStr1);
     def.Mm[Chl][Z_curr].Freq[0] =cvtcf(FldStr2);
     def.Mm[Chl][Z_curr].Betta[1]=cvtcf(FldStr3);
     def.Mm[Chl][Z_curr].Freq[1] =cvtcf(FldStr4);
     }
   Wclose();
   }
 return;
}
/*=========================================================================*/

LCL IbldT UnkItm[5]=
{{names[0],'\0',ITM_AFT, 0,0, 44},
 {names[7],'\0',ITM_AFT, 1,0, 51,0,0,0,NULL,SinParm},
 {names[8],'\0',ITM_AFT, 2,0, 52,0,0,0,NULL,PSinParm},
 {names[9],'\0',ITM_AFT, 3,0, 53,0,0,0,NULL,RandParm},
 {names[10],'\0',ITM_AFT, 4,0, 54},
};

LCL  MbldT UnkMnu=
{&UnkItm[0],5, 10,15,16,31,  MNU_SAV|MNU_VER, BOX_SNG,
 C_WBOX,C_WTXT,C_WSEL, C_CHR,C_NSEL,C_WASEL,
   44, OpenW,  0, 10,  1, 0xFF, 0xFF, 0xFF
};

/*==========================================================================*/

VOID CTYP OpenWW(NOARG)
{
 OpenW();
 ItmDis(46); /* DISABLE: Апериодическое звено */
}

LCL IbldT DinItm[10]=
{{names[0],'\0',ITM_AFT, 0,0, 44},
 {names[1],'\0',ITM_AFT, 1,0, 45,0,0,0,NULL,FuncParm},
 {names[2],'\0',ITM_AFT, 2,0, 46,0,0,0,NULL,AperParm},
 {names[3],'\0',ITM_AFT, 3,0, 47,0,0,0,NULL,KolbParm},
 {names[4],'\0',ITM_AFT, 4,0, 48,0,0,0,NULL,KolbParm},
 {names[5],'\0',ITM_AFT, 5,0, 49,0,0,0,NULL,KolbParm},
 {names[6],'\0',ITM_AFT, 6,0, 50,0,0,0,NULL,KolbParm},
 {names[11],'\0',ITM_AFT,7,0, 55,0,0,0,NULL,MassParm},
 {names[12],'\0',ITM_AFT,8,0, 56,0,0,0,NULL,TestParm},
 {names[13],'\0',ITM_AFT,9,0, 57,0,0,0,NULL,ArssParm},
};

LCL  MbldT DinMnu=
{&DinItm[0],10, 7,15,18,38,  MNU_SAV|MNU_VER, BOX_SNG,
 C_WBOX,C_WTXT,C_WSEL, C_CHR,C_WBOX,C_WASEL,
   44, OpenWW,  0, 10,  1, 0xFF, 0xFF, 0xFF
};

VOID CTYP DinTypeFun(NOARG)
{
 int s;
 FdfP f;


 ActPIC();
 s=SubFunBeg();
 if(!s)
   {
   f=FldCur();
   Z_curr=FdfTag(f)-51;
   if(FdfTag(f)!=54)
     {
     MbldTag(&DinMnu)=44+def.Z_Type[Chl][Z_curr];
     if(MnuBuild(&DinMnu)==ERR_MEM) MemAllocErr(0);
     else  s=MnuGet();
     }
   else
     {
     MbldTag(&UnkMnu)=44+def.Z_Type[Chl][Z_curr];
     if(MnuBuild(&UnkMnu)==ERR_MEM) MemAllocErr(0);
     else s=MnuGet();
     }
   if(s)
     {
     s=def.Z_Type[Chl][Z_curr]=s-44;
     ZvenoView(Z_curr);
     strncpy(FdfBfr(f), names[s],14); FdfBfr(f)[14]=0;
     FldChg();
     }

   SubFunEnd();
   }
 return;
}
/*==========================================================================*/


/*-----------------11-24-92 04:19pm-----------------*/


LCL char _ZvenoPIC[40]="!Aa!**************";

LCL VOID CTYP ActPIC(NOARG)
{
  _ZvenoPIC[2]=C_MASEL;
  _FldPut(FldCur(),FDF_MOV);
  return;
}


LCL VOID CTYP DeActPIC(NOARG)
{
  _ZvenoPIC[2]=C_MSEL;
  _FldPut(FldCur(),FDF_MOV);
  return;
}

LCL VOID CTYP SetPIC(NOARG)
{
        _ZvenoPIC[2]=C_MSEL;
}

LCL FbldT  DinFld[7]={
 {"Частота дискретизации",10,3, _Zveno[5],   10,28,"99999.999",
  NULL      ,NULL,NULL, 0,50,0, FLD_DEC,0,FLD_CHG},
 {"Величина задержки (такты)" ,11,3, _Zveno[6],   11,32,"99999",
  NULL      ,NULL,NULL, 0,50,0, FLD_DEC,0,FLD_CHG},
 {"1",          12,2,  _Zveno[0], 12,7 ,_ZvenoPIC,
  DinTypeFun,       NULL,DeActPIC, 0,51,0, FLD_SUB,0,FLD_CHG},
 {"2",          15,2,  _Zveno[1], 15,7 ,_ZvenoPIC,
  DinTypeFun,       NULL,DeActPIC, 0,52,0, FLD_SUB,0,FLD_CHG},
 {"3",          18,2,  _Zveno[2], 18,7 ,_ZvenoPIC,
  DinTypeFun,       NULL,DeActPIC, 0,53,0, FLD_SUB,0,FLD_CHG},
 {"вид помехи", 12,31, _Zveno[3], 14,30,_ZvenoPIC,
  DinTypeFun,       NULL,DeActPIC, 0,54,0, FLD_SUB,0,FLD_CHG},
 {"статика",    19,33, _Zveno[4], 20,33,"99999.9999",
  NULL,      NULL,NULL, 0,54,0, FLD_DEC,0,FLD_CHG}
};

EbldT DinFrm=
{DinFld,7,NULL, NULL,0, ENT_SAV,
 C_TXT,C_SEL,C_ASEL,C_NSEL};

VOID CTYP DinFun(NOARG)
{
 int val;
 struct _FuncModel Fm;
 struct _AperModel Am;
 struct _KolbModel Km;
 struct _MassModel Mm;
 int Z[4];

 // save old values
 memcpy(&Fm,&def.Fm[Chl],sizeof(Fm));
 memcpy(&Am,&def.Am[Chl],sizeof(Am));
 memcpy(&Km,&def.Km[Chl],sizeof(Km));
 memcpy(&Mm,&def.Mm[Chl],sizeof(Mm));
 memcpy(&Z,&def.Z_Type[Chl],sizeof(Z));


 SetPIC();
 cvtfc(_Zveno[4],def.imi_Const[Chl],10,4);
 cvtfc(_Zveno[5],def.fdds[Chl],9,3);
 cvtfc(_Zveno[6],def.wait[Chl],5,0);

 _USE_=0;
 if(EntBuild(&DinFrm)==ERR_MEM) MemAllocErr(0);
 else
   {
   val=EntGet(DinFld[1].ftag);
   if(val!=ERR_ARG)
         {
         _USE_=1;
         def.imi_Const[Chl]=cvtcf(_Zveno[4]);
         def.fdds[Chl]     =cvtcf(_Zveno[5]);
         def.wait[Chl]     =min(cvtcf(_Zveno[6]),MAX_WAIT);
         }
   else
         {
         // restore old values
         memcpy(&def.Fm[Chl],&Fm,sizeof(Fm));
         memcpy(&def.Am[Chl],&Am,sizeof(Am));
         memcpy(&def.Km[Chl],&Km,sizeof(Km));
         memcpy(&def.Mm[Chl],&Mm,sizeof(Mm));
         memcpy(&def.Z_Type[Chl],&Z,sizeof(Z));
         }
   EntFree();
   }
 return;
}

LCL VOID CTYP ZvenoView(int num)
{
 strcpy(_Zveno[num],names[def.Z_Type[Chl][num]]);
 if(num<3)
   {
   WfilBox(13+num*3,2,14+num*3,27,' ',C_TXT);
   switch(def.Z_Type[Chl][num])
      {
          case 0:  /* none */
                break;
          case 1:  /* функциональное */
                Wprts(13+num*3, 3,C_TXT,"Вид нелин.");
                Wprts(13+num*3,14,C_SEL,FuncNames[def.Fm[Chl][num].Type]);
                Wprts(14+num*3, 3,C_TXT,"Кф. перед.");
                Wprtf(14+num*3,18,C_SEL,"%7.4f",def.Fm[Chl][num].k);
                break;
          case 2:  /* апериодическое */
                Wprts(13+num*3, 3,C_TXT,"Порядок");
                Wprtf(14+num*3, 4,C_SEL,"%04d",def.Am[Chl][num].p);
                Wprtf(13+num*3,11,C_TXT,"├ Fв");
                Wprtf(13+num*3,17,C_SEL,"%10.4f",def.Am[Chl][num].FV);
                Wprtf(14+num*3,11,C_TXT,"├ Fн");
                Wprtf(14+num*3,17,C_SEL,"%10.4f",def.Am[Chl][num].FN);
                break;
          case 3:  /* колебательное */
          case 4:  /* колебательное */
          case 5:  /* колебательное */
          case 6:  /* колебательное */
                Wprtf(13+num*3, 3,C_TXT,"Степень усп.");
                Wprtf(13+num*3,17,C_SEL,"%10.7f",def.Km[Chl][num].Betta);
                Wprtf(14+num*3, 3,C_TXT,"Рез. частота");
                Wprtf(14+num*3,17,C_SEL,"%10.5f",def.Km[Chl][num].Freq);
                break;
          case 11: /* Двухмассовое звено */
                Wprtf(13+num*3, 3,C_TXT,"Успок.");
                Wprtf(13+num*3,10,C_SEL,"%8.5f",def.Mm[Chl][num].Betta[0]);
                Wprtf(13+num*3,19,C_SEL,"%8.5f",def.Mm[Chl][num].Betta[1]);
                Wprtf(14+num*3, 3,C_TXT,"Резон.");
                Wprtf(14+num*3,10,C_SEL,"%8.3f",def.Mm[Chl][num].Freq[0]);
                Wprtf(14+num*3,19,C_SEL,"%8.3f",def.Mm[Chl][num].Freq[1]);
                break;
      }
   }
 else
   {
   }
 return;
}

VOID CTYP DinView(NOARG)
{
 int i;

 cvtfc(_Zveno[4],def.imi_Const[Chl],10,4);
 cvtfc(_Zveno[5],def.fdds[Chl],9,3);
 cvtfc(_Zveno[6],def.wait[Chl],5,0);

 SetPIC();
 WdrwBox( 9, 1,21,44,BOX_DBL,C_BOX); /* имитац. модель */
 WdrwHor(12, 1,44,BOX_VER,C_BOX);
 WdrwVer(13,28, 9,BOX_DBL,C_BOX);
 WdrwVer(12,28, 2,BOX_VER,C_BOX);
 WdrwHor(15, 1,28,BOX_VER,C_BOX);
 WdrwHor(18, 1,28,BOX_VER,C_BOX);
 WdrwHor(19,28,16,BOX_VER,C_BOX);
 Wprtf(9, 38,C_BOX," %d ═",Chl+1);
 Wprtf(9, 12,C_MSEL," %s ",OptStr3);
 Wprts(10,38,C_TXT,"Гц.");
 Wprts(20,30,C_TXT,"U=");
 for(i=0;i<4;i++) ZvenoView(i);
 if(EntBuild(&DinFrm)==ERR_MEM) MemAllocErr(0);
 else
   {
   EntDsp();
   EntFree();
   }
 return;
}


static char RusTabl[0x100]="ЙйЦцУуКкЕеНнГгШшЩщЗзХхФфЫыВвАаПпРрОоЛлДдЖжЭэЯяЧчСсМмИиТтЬьБбЮю\00";

main(int argc,char *argv[])
{
 static int f,i,changed,k;
 struct _FuncModel xFuncModel[3]={{0,1.0},{0,1.0},{1,1.0}};
 struct _AperModel xAperModel[3]={{40,5,2},{40,5,2},{40,5,2}};
 struct _KolbModel xKolbModel[3]={{0.5,27.7},{0.5,27.7},{0.5,27.7}};
 struct _MassModel xMassModel[3]={{{0.5,0.5},{27.7,27.7}},{{0.5,0.5},{27.7,27.7}},{{0.5,0.5},{27.7,27.7}}};
 struct _TestModel xTestModel[3]={{0,0,{0.33,0.11},{0.17,0.7}},{0,0,{0.33,0.11},{0.17,0.7}},{0,0,{0.33,0.11},{0.17,0.7}}};
 struct _ArssModel xArssModel[3]={{4,0,{1,2.8,2.94,1.392,0.2401},{1,-3.2,3.84,-2.048,0.4096}},{0,0,{0.33,0.11},{0.17,0.7}},{0,0,{0.33,0.11},{0.17,0.7}}};

 int    xZ_Type[4]={1,0,0,0};
 float  _wzk[10]={1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
 float  _Ampl[3]={0.3,0.1,0.04};
 float  _Freq[3]={3.5,12.7,34.3};
 float  _Faza[3]={1.33,0.5,0.12};

 char *Mnu[]={"  Канал 1","  Канал 2","  Канал 3","  Канал 4","  Канал 5",
              "  Канал 6","  Канал 7","  Канал 8","  Канал 9","  Канал 10",
              "Взаимовлияние",
              " Сохранить ",NULL};

 printf("Setup UPI's emulator 'UPI_3w' " upiVERSION " (c) by D.Kokorev  " __DATE__ ".\n");
 if(argc!=2)
   {
   printf("\007Usage: >%s <cfg_filename>\n",argv[0]);
   exit(-1);
   }
 f=open(argv[1],O_RDWR|O_CREAT|O_BINARY,S_IREAD|S_IWRITE);
 if(f==-1)
   {
   printf("\007Error open or create file '%s'.\n",argv[1]);
   exit(-2);
   }
 if(read(f,&def,sizeof(def)) != sizeof(def))
   {
   printf("Error read parameters from '%s'; using default.\n",argv[1]);
   for(i=0;i<Channel;i++)
      {
      memcpy(&def.Fm[i],&xFuncModel,3*sizeof(struct _FuncModel));
      memcpy(&def.Am[i],&xAperModel,3*sizeof(struct _AperModel));
      memcpy(&def.Km[i],&xKolbModel,3*sizeof(struct _KolbModel));
      memcpy(&def.Mm[i],&xMassModel,3*sizeof(struct _MassModel));
      memcpy(&def.Tm[i],&xTestModel,3*sizeof(struct _TestModel));
      memcpy(&def.Sm[i],&xArssModel,3*sizeof(struct _ArssModel));
      memcpy(&def.Z_Type[i],xZ_Type,4*sizeof(int));
      def.imi_Const[i]=1.0/(i+2);
      def.fdds[i]=133.3;
      def.wait[i]=1;
      for(k=0;k<Channel;k++) def.wzk[i][k]=_wzk[(k-i+10)%10];
      for(k=0;k<3;k++)
         {
         def.xAmpl[i][k]=_Ampl[k];
         def.xFreq[i][k]=_Freq[k];
         def.xFaza[i][k]=_Faza[k];
         }
      }
   }

 TcxlKeyOff();
 TcxlMouOff();
 TcxlInit();
 if(!TcxlCty)
 {
  NlsUpl=RusTabl;
  TcxlCty=007;
 }
 VcurGet(DOS_cur_POS);

 WsetFil('░');
 Wopen(0,0,24,79,BOX_SPA,C_BOX,C_TXT);
 MemAllocErr(1);
 CurHide();

 WsetFil(' ');
 WfilBox(9,1,20,44,' ',C_TXT);

 Chl=0; changed=0;
 while(Chl>=0)
   {
   DinView();
   Chl=SelStr(2,50,15,70,BOX_DBL|BOX_EXP,C_WBOX,C_WTXT,C_WASEL,Mnu,Chl,OpenW);
   switch(Chl)
     {
     case -1:  _USE_=0; MemAllocErr(0); break;
     case 10:  WZK(); changed=1; Chl=9; break;
     case 11:  Chl=-1;  if(changed) _USE_=1; break;
     default:  DinView(); DinFun();
               if(_USE_) { Mnu[Chl][0]=0xFB; changed=1; } break;
     }
   }


 Wclose();
 VcurPut(DOS_cur_POS);
 if(_USE_)
   {
   printf("Save new parameters to '%s' ... ",argv[1]);
   lseek(f,0L,SEEK_SET);
   if(write(f,&def,sizeof(def)) == sizeof(def)) printf(" Ok.\n");
   else printf("ERROR\007, it's lost.\n");
   }
 close(f);

 return 0;
}
