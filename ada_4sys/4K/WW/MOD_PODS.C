

/*
 *   Generated by  UI v2.00
 *        March 2, 1997
 */


/* Module: MOD_PODS.C ===================== */

#include <string.h>
#include <stdlib.h>

#include <tcxlcur.h>
#include <tcxlnls.h>
#include <tcxlwin.h>
#include <tcxlent.h>
#include <tcxlmnu.h>
#include <tcxlcod.h>
#include <tcxlinp.h>
#include <tcxlsel.h>
#include <tcxlstr.h>
#include <tcxlvid.h>
#include <tcxlhlp.h>
#include <tcxladd.h>

#define HLP_NONE 1


#ifndef __Struct_WDEF_
#define __Struct_WDEF_

       struct _WDEF
         {
         IntT BegC,BegR,EndC,EndR;   /* ��������� ����    */
         IntT MniC,MniR,MaxC,MaxR;   /* �������� ࠧ���� */
         BytT FrmT;                  /* ⨯ �����       */
         AtrT FrmA;                  /* ��ਡ��� �����  */
         AtrT Dat;                   /* ��ਡ��� ⥪��   */
         BytT Shadow;                /* ⥭�              */
         AtrT Sat;                   /* 梥� ⥭�         */
         };
#endif

#ifdef __cplusplus
  extern "C" {
#endif
VOID CTYP MnuDsp(MbldP mnu);
KeyT CTYP EntSelect(NOARG);
GBL  IntT EntSelAct;
LCL  IntT EntResult;
#ifdef __cplusplus
   }
#endif
#include "pro_achh.h"

LCL int chl=0,mdl=0;

LCL void refresh(void)
{
 _disp_model0_WRITE();
 _disp_model1_WRITE();
 _disp_model2_WRITE();
 _disp_model3_WRITE();
}

LCL VOID CTYP Do(NOARG)
{
 KquePut(Key_C_Ent);
}




KeyT  CTYP _channel_MENU(NOARG);
VOID  CTYP _disp_model0_WRITE(NOARG);
VOID  CTYP _disp_model1_WRITE(NOARG);
VOID  CTYP _disp_model2_WRITE(NOARG);
VOID  CTYP _disp_model3_WRITE(NOARG);
VOID  CTYP _ref_EXEC(NOARG);
VOID  CTYP _mod_menu_SELECT(NOARG);
KeyT  CTYP _type_menu_MENU(NOARG);
VOID  CTYP _mod_menu_SELECT(NOARG);
KeyT  CTYP _type_menu_MENU(NOARG);
VOID  CTYP _mod_menu_SELECT(NOARG);
KeyT  CTYP _type_menu_MENU(NOARG);
VOID  CTYP _mod_menu_SELECT(NOARG);
KeyT  CTYP _type_menu_MENU(NOARG);
VOID  CTYP _Trend_ENTRY(NOARG);
VOID  CTYP _Trend_ENTRY(NOARG);
VOID  CTYP _Trend_ENTRY(NOARG);
VOID CTYP _selTrend_2STR(NOARG);
VOID CTYP _selTrend_MSG(NOARG);
VOID CTYP _main_2STR(NOARG);
VOID CTYP _main_OPEN(NOARG);
VOID CTYP _channel_2STR(NOARG);
VOID CTYP _channel_MSG(NOARG);
VOID CTYP _disp_model0_2STR(NOARG);
VOID CTYP _disp_model1_2STR(NOARG);
VOID CTYP _disp_model2_2STR(NOARG);
VOID CTYP _disp_model3_2STR(NOARG);
VOID CTYP _mod_menu_2STR(NOARG);
VOID CTYP _mod_menu_MSG(NOARG);
VOID CTYP _type_menu_2STR(NOARG);
VOID CTYP _type_menu_MSG(NOARG);
VOID CTYP _Trend_2STR(NOARG);
VOID CTYP _Trend_OPEN(NOARG);


LCL IbldT _selTrend_Itm[]={
{"  ���஥���  ",'\0',ITM_NOR,1,1,  1,HLP_NONE,
   0,2,NULL,NULL,NULL,
   NULL,NULL},
{"  �������-�  ",'\0',ITM_NOR,2,1,  3,HLP_NONE,
   0,4,NULL,NULL,NULL,
   NULL,NULL},
{"  ��������  ",'\0',ITM_NOR,3,1,  5,HLP_NONE,
   0,6,NULL,NULL,NULL,
   NULL,NULL},
};


LCL VOID CTYP _selTrend_MSG(NOARG);
LCL MbldT _selTrend_Mnu={
   _selTrend_Itm,3,6,35,12,52,  MNU_OMN|MNU_SAV|MNU_DSP|MNU_NQS,BOX_SNG,
   _LGREY|BROWN,_BLUE|INTENSE|LGREY,_BLUE|INTENSE|LGREY,
   _BLUE|BLACK,_BLUE|BLACK,_CYAN|BLACK,
   0,_selTrend_MSG,7, 0,0,0, 0xff,0xff,0xff};

LCL VOID CTYP _selTrend_MSG(NOARG)
{

 CurHide();
  Wmessage("��� �७��",BRD_TOP,4,_LGREY|BROWN);
  {
  MnuDsp(&_selTrend_Mnu);
  }
  return;
}
LCL KeyT CTYP _selTrend_MENU(NOARG)
{
 IntT ret;
 KbndP tcxl_old_kb,tcxl_kb=NULL;
 tcxl_old_kb=KbndChg(tcxl_kb);


 MnuBuild(&_selTrend_Mnu);
 ret=MnuGet();
 KbndFre();
 KbndChg(tcxl_old_kb);
 return ret;
}

LCL ChrT main_var5[3];
LCL ChrT main_var6[3];
LCL ChrT main_var7[3];
LCL ChrT main_var8[13];
LCL IntT CTYP _main_var8_POPUP(ChrP str)
{
 extern MbldT _selTrend_Mnu;
 KeyT CTYP _selTrend_MENU(NOARG);

   EntPopUpMenu(_selTrend_MENU,&_selTrend_Mnu);
   return 0;
}
LCL ChrT main_var9[3];
LCL ChrT main_var10[7];
LCL ChrT main_var11[3];
LCL ChrT main_var12[3];
LCL ChrT main_var13[3];
LCL ChrT main_var14[13];
LCL IntT CTYP _main_var14_POPUP(ChrP str)
{
 extern MbldT _selTrend_Mnu;
 KeyT CTYP _selTrend_MENU(NOARG);

   EntPopUpMenu(_selTrend_MENU,&_selTrend_Mnu);
   return 0;
}
LCL ChrT main_var15[3];
LCL ChrT main_var16[7];
LCL ChrT main_var24[3];
LCL ChrT main_var26[3];
LCL ChrT main_var27[3];
LCL ChrT main_var28[13];
LCL IntT CTYP _main_var28_POPUP(ChrP str)
{
 extern MbldT _selTrend_Mnu;
 KeyT CTYP _selTrend_MENU(NOARG);

   EntPopUpMenu(_selTrend_MENU,&_selTrend_Mnu);
   return 0;
}
LCL ChrT main_var29[3];
LCL ChrT main_var30[7];
LCL ChrT main_var31[3];
LCL ChrT main_var32[3];
LCL ChrT main_var33[3];
LCL ChrT main_var35[13];
LCL IntT CTYP _main_var35_POPUP(ChrP str)
{
 extern MbldT _selTrend_Mnu;
 KeyT CTYP _selTrend_MENU(NOARG);

   EntPopUpMenu(_selTrend_MENU,&_selTrend_Mnu);
   return 0;
}
LCL ChrT main_var36[3];
LCL ChrT main_var37[7];
LCL FbldT _main_Fld[]={
 {"",0,0, main_var5,2,22,
  "99",NULL,NULL,NULL,
  0,8,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var6,3,22,
  "99",NULL,NULL,NULL,
  0,9,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var7,4,22,
  "99",NULL,NULL,NULL,
  0,10,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var8,5,12,
  "< >***********",NULL,_main_var8_POPUP,NULL,
  0,11,HLP_NONE,
  FLD_NUL|FLD_TRM,2,FLD_CHG},
 {"",0,0, main_var9,6,22,
  "99",NULL,NULL,NULL,
  0,12,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var10,7,18,
  "99.999",NULL,NULL,NULL,
  0,13,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var11,2,49,
  "99",NULL,NULL,NULL,
  0,14,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var12,3,49,
  "99",NULL,NULL,NULL,
  0,15,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var13,4,49,
  "99",NULL,NULL,NULL,
  0,16,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var14,5,39,
  "< >***********",NULL,_main_var14_POPUP,NULL,
  0,17,HLP_NONE,
  FLD_NUL|FLD_TRM,2,FLD_CHG},
 {"",0,0, main_var15,6,49,
  "99",NULL,NULL,NULL,
  0,18,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var16,7,45,
  "99.999",NULL,NULL,NULL,
  0,19,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var24,11,22,
  "99",NULL,NULL,NULL,
  0,20,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var26,12,22,
  "99",NULL,NULL,NULL,
  0,21,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var27,13,22,
  "99",NULL,NULL,NULL,
  0,22,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var28,14,12,
  "< >***********",NULL,_main_var28_POPUP,NULL,
  0,23,HLP_NONE,
  FLD_NUL|FLD_TRM,2,FLD_CHG},
 {"",0,0, main_var29,15,22,
  "99",NULL,NULL,NULL,
  0,24,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var30,16,18,
  "99.999",NULL,NULL,NULL,
  0,25,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var31,11,49,
  "99",NULL,NULL,NULL,
  0,26,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var32,12,49,
  "99",NULL,NULL,NULL,
  0,27,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var33,13,49,
  "99",NULL,NULL,NULL,
  0,28,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var35,14,39,
  "< >***********",NULL,_main_var35_POPUP,NULL,
  0,29,HLP_NONE,
  FLD_NUL|FLD_TRM,2,FLD_CHG},
 {"",0,0, main_var36,15,49,
  "99",NULL,NULL,NULL,
  0,30,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var37,16,45,
  "99.999",NULL,NULL,NULL,
  0,31,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
};

LCL EbldT _main_Frm={
 _main_Fld,24,EntSelect,NULL,32,0|ENT_WRP,
 0,_CYAN|INTENSE|LGREY,_BLUE|INTENSE|GREEN,_BLUE|INTENSE|GREEN
};
LCL VOID CTYP _main_OPEN(NOARG)
{

 Wopen(4,16,22,70,BOX_SNG,
       _MAGENTA|INTENSE|GREEN,_CYAN|INTENSE|LGREY);
 Wshadow(_BLACK|DGREY);
 CurHide();
  Wmessage("�1,5�",BRD_TOP,4,_MAGENTA|INTENSE|GREEN);
  Wbprintc(BRD_TOP,27,_MAGENTA|INTENSE|GREEN,'�');
  Wmessage("�2,6�",BRD_TOP,31,_MAGENTA|INTENSE|GREEN);
  Wmessage("�                                              �",BRD_BOT,3,_MAGENTA|INTENSE|GREEN);
  Wbprintc(BRD_LFT,9,_MAGENTA|INTENSE|GREEN,'�');
  Wbprintc(BRD_RGT,9,_MAGENTA|INTENSE|GREEN,'�');
  Wprts(0,1,_CYAN|BLACK,"������ :");
  Wprts(0,26,_MAGENTA|INTENSE|GREEN,"�");
  Wprts(0,28,_CYAN|BLACK,"������ :");
  Wprts(1,0,_CYAN|BLACK," ��� ��:");
  Wprts(1,26,_MAGENTA|INTENSE|GREEN,"�");
  Wprts(1,28,_CYAN|BLACK,"��� ��:");
  Wprts(2,0,_CYAN|BLACK," ���冷� ������");
  Wprts(2,26,_MAGENTA|INTENSE|GREEN,"�");
  Wprts(2,28,_CYAN|BLACK,"���冷� ������");
  Wprts(3,0,_CYAN|BLACK," �����ਧ��� ");
  Wprts(3,26,_MAGENTA|INTENSE|GREEN,"�");
  Wprts(3,28,_CYAN|BLACK,"�����ਧ��� ");
  Wprts(4,0,_CYAN|BLACK," ������뢠���  ");
  Wprts(4,26,_MAGENTA|INTENSE|GREEN,"�");
  Wprts(4,28,_CYAN|BLACK,"������뢠���   ");
  Wprts(5,0,_CYAN|BLACK," ��� �७�� ");
  Wprts(5,26,_MAGENTA|INTENSE|GREEN,"�");
  Wprts(5,28,_CYAN|BLACK,"��� �७�� ");
  Wprts(6,0,_CYAN|BLACK," ���冷� �७��");
  Wprts(6,26,_MAGENTA|INTENSE|GREEN,"�");
  Wprts(6,28,_CYAN|BLACK,"���冷� �७�� ");
  Wprts(7,0,_CYAN|BLACK," ���. ����譮���");
  Wprts(7,24,_CYAN|BLACK," ");
  Wprts(7,26,_MAGENTA|INTENSE|GREEN,"�");
  Wprts(7,28,_CYAN|BLACK,"���. ����譮���");
  Wprts(7,51,_CYAN|BLACK," ");
  Wprts(8,0,_MAGENTA|INTENSE|GREEN,"��Ĵ3,7����������������������Ĵ4,8�������������������");
  Wprts(9,0,_CYAN|BLACK," ������ :");
  Wprts(9,26,_MAGENTA|INTENSE|GREEN,"�");
  Wprts(9,28,_CYAN|BLACK,"������ :");
  Wprts(10,0,_CYAN|BLACK," ��� ��:");
  Wprts(10,26,_MAGENTA|INTENSE|GREEN,"�");
  Wprts(10,28,_CYAN|BLACK,"��� ��:");
  Wprts(11,1,_CYAN|BLACK,"���冷� ������ ");
  Wprts(11,26,_MAGENTA|INTENSE|GREEN,"�");
  Wprts(11,28,_CYAN|BLACK,"���冷� ������ ");
  Wprts(12,0,_CYAN|BLACK," �����ਧ��� ");
  Wprts(12,26,_MAGENTA|INTENSE|GREEN,"�");
  Wprts(12,28,_CYAN|BLACK,"�����ਧ���  ");
  Wprts(13,0,_CYAN|BLACK," ������뢠���  ");
  Wprts(13,26,_MAGENTA|INTENSE|GREEN,"�");
  Wprts(13,28,_CYAN|BLACK,"������뢠���  ");
  Wprts(14,1,_CYAN|BLACK,"��� �७��  ");
  Wprts(14,26,_MAGENTA|INTENSE|GREEN,"�");
  Wprts(14,28,_CYAN|BLACK,"��� �७��  ");
  Wprts(15,1,_CYAN|BLACK,"���冷� �७��      ");
  Wprts(15,26,_MAGENTA|INTENSE|GREEN,"�");
  Wprts(15,28,_CYAN|BLACK,"���冷� �७��");
  Wprts(16,1,_CYAN|BLACK,"���. ����譮���");
  Wprts(16,24,_CYAN|BLACK," ");
  Wprts(16,26,_MAGENTA|INTENSE|GREEN,"�");
  Wprts(16,28,_CYAN|BLACK,"���. ����譮���");
  Wprts(16,51,_CYAN|BLACK," ");

  _main_2STR();
  {
  EdfP e=EctlCur;
  if(e) EntPush();
  EntBuild(&_main_Frm);
  EntDsp();
  EntFree();
  if(e) EntPop();
  }

  _disp_model0_WRITE();
  _disp_model1_WRITE();
  _disp_model2_WRITE();
  _disp_model3_WRITE();
  return;
}
LCL VOID CTYP _main_2STR(NOARG)
{
  cvtic(main_var5,Cfg.Def[0].p,2);
  cvtic(main_var6,Cfg.Def[0].Q,2);
  cvtic(main_var7,Cfg.Def[0].q,2);
  {
   int i;
  for(i=0;i<MbldCnt(&_selTrend_Mnu);i++)
       IbldStr(&_selTrend_Itm[i])[0]=' ';
  strncpy(main_var8,IbldStr(&_selTrend_Itm[Cfg.Def[0].trCod]),12);
  main_var8[12]=0;
  IbldStr(&_selTrend_Itm[Cfg.Def[0].trCod])[0]='\xFB';
  MbldTag(&_selTrend_Mnu)=IbldTag(&_selTrend_Itm[Cfg.Def[0].trCod]);
  }
  cvtic(main_var9,Cfg.Def[0].trP,2);
  cvtfc(main_var10,Cfg.Def[0].mstMax,6,3);
  cvtic(main_var11,Cfg.Def[1].p,2);
  cvtic(main_var12,Cfg.Def[1].Q,2);
  cvtic(main_var13,Cfg.Def[1].q,2);
  {
   int i;
  for(i=0;i<MbldCnt(&_selTrend_Mnu);i++)
       IbldStr(&_selTrend_Itm[i])[0]=' ';
  strncpy(main_var14,IbldStr(&_selTrend_Itm[Cfg.Def[1].trCod]),12);
  main_var14[12]=0;
  IbldStr(&_selTrend_Itm[Cfg.Def[1].trCod])[0]='\xFB';
  MbldTag(&_selTrend_Mnu)=IbldTag(&_selTrend_Itm[Cfg.Def[1].trCod]);
  }
  cvtic(main_var15,Cfg.Def[1].trP,2);
  cvtfc(main_var16,Cfg.Def[1].mstMax,6,3);
  cvtic(main_var24,Cfg.Def[2].p,2);
  cvtic(main_var26,Cfg.Def[2].Q,2);
  cvtic(main_var27,Cfg.Def[2].q,2);
  {
   int i;
  for(i=0;i<MbldCnt(&_selTrend_Mnu);i++)
       IbldStr(&_selTrend_Itm[i])[0]=' ';
  strncpy(main_var28,IbldStr(&_selTrend_Itm[Cfg.Def[2].trCod]),12);
  main_var28[12]=0;
  IbldStr(&_selTrend_Itm[Cfg.Def[2].trCod])[0]='\xFB';
  MbldTag(&_selTrend_Mnu)=IbldTag(&_selTrend_Itm[Cfg.Def[2].trCod]);
  }
  cvtic(main_var29,Cfg.Def[2].trP,2);
  cvtfc(main_var30,Cfg.Def[2].mstMax,6,3);
  cvtic(main_var31,Cfg.Def[3].p,2);
  cvtic(main_var32,Cfg.Def[3].Q,2);
  cvtic(main_var33,Cfg.Def[3].q,2);
  {
   int i;
  for(i=0;i<MbldCnt(&_selTrend_Mnu);i++)
       IbldStr(&_selTrend_Itm[i])[0]=' ';
  strncpy(main_var35,IbldStr(&_selTrend_Itm[Cfg.Def[3].trCod]),12);
  main_var35[12]=0;
  IbldStr(&_selTrend_Itm[Cfg.Def[3].trCod])[0]='\xFB';
  MbldTag(&_selTrend_Mnu)=IbldTag(&_selTrend_Itm[Cfg.Def[3].trCod]);
  }
  cvtic(main_var36,Cfg.Def[3].trP,2);
  cvtfc(main_var37,Cfg.Def[3].mstMax,6,3);
 return;
}
LCL VOID CTYP _main_2VAL(NOARG)
{
    Cfg.Def[0].p=cvtci(main_var5);
    Cfg.Def[0].Q=cvtci(main_var6);
    Cfg.Def[0].q=cvtci(main_var7);
  {
  int i;
  main_var8[0]='\xFB';
  for(i=0;i<MbldCnt(&_selTrend_Mnu);i++)
    if(!strncmp(IbldStr(&_selTrend_Itm[i]),main_var8,12))
         {
         Cfg.Def[0].trCod=i;
         break;
         }
  }
    Cfg.Def[0].trP=cvtci(main_var9);
    Cfg.Def[0].mstMax=cvtcf(main_var10);
    Cfg.Def[1].p=cvtci(main_var11);
    Cfg.Def[1].Q=cvtci(main_var12);
    Cfg.Def[1].q=cvtci(main_var13);
  {
  int i;
  main_var14[0]='\xFB';
  for(i=0;i<MbldCnt(&_selTrend_Mnu);i++)
    if(!strncmp(IbldStr(&_selTrend_Itm[i]),main_var14,12))
         {
         Cfg.Def[1].trCod=i;
         break;
         }
  }
    Cfg.Def[1].trP=cvtci(main_var15);
    Cfg.Def[1].mstMax=cvtcf(main_var16);
    Cfg.Def[2].p=cvtci(main_var24);
    Cfg.Def[2].Q=cvtci(main_var26);
    Cfg.Def[2].q=cvtci(main_var27);
  {
  int i;
  main_var28[0]='\xFB';
  for(i=0;i<MbldCnt(&_selTrend_Mnu);i++)
    if(!strncmp(IbldStr(&_selTrend_Itm[i]),main_var28,12))
         {
         Cfg.Def[2].trCod=i;
         break;
         }
  }
    Cfg.Def[2].trP=cvtci(main_var29);
    Cfg.Def[2].mstMax=cvtcf(main_var30);
    Cfg.Def[3].p=cvtci(main_var31);
    Cfg.Def[3].Q=cvtci(main_var32);
    Cfg.Def[3].q=cvtci(main_var33);
  {
  int i;
  main_var35[0]='\xFB';
  for(i=0;i<MbldCnt(&_selTrend_Mnu);i++)
    if(!strncmp(IbldStr(&_selTrend_Itm[i]),main_var35,12))
         {
         Cfg.Def[3].trCod=i;
         break;
         }
  }
    Cfg.Def[3].trP=cvtci(main_var36);
    Cfg.Def[3].mstMax=cvtcf(main_var37);
 return;
}


LCL VOID CTYP _opt1_ACT(NOARG)
{
chl=0;
_mod_menu_SELECT();
return;
}

LCL VOID CTYP _opt2_ACT(NOARG)
{
chl=0;
_type_menu_MENU();
return;
}

LCL VOID CTYP _opt5_ACT(NOARG)
{
chl=1;
_mod_menu_SELECT();
return;
}

LCL VOID CTYP _opt7_ACT(NOARG)
{
chl=1;
_type_menu_MENU();
return;
}

LCL VOID CTYP _opt8_ACT(NOARG)
{
chl=2;
_mod_menu_SELECT();
return;
}

LCL VOID CTYP _opt9_ACT(NOARG)
{
chl=2;
_type_menu_MENU();
return;
}

LCL VOID CTYP _opt10_ACT(NOARG)
{
chl=3;
_mod_menu_SELECT();
return;
}

LCL VOID CTYP _opt12_ACT(NOARG)
{
chl=3;
_type_menu_MENU();
return;
}

LCL IbldT _channel_Itm[]={
{" ��� ������     ",'\0',ITM_NOR|ITM_AFT,1,1,  33,HLP_MODEL,
   0,34,NULL,_opt1_ACT,NULL,
   NULL,NULL},
{" ��� �ࠢ����� ",'\0',ITM_NOR|ITM_AFT,2,1,  35,HLP_MCTRL,
   0,36,NULL,_opt2_ACT,NULL,
   NULL,NULL},
{" ��� ������     ",'\0',ITM_NOR|ITM_AFT,4,1,  37,HLP_MODEL,
   0,38,NULL,_opt5_ACT,NULL,
   NULL,NULL},
{" ��� �ࠢ����� ",'\0',ITM_NOR|ITM_AFT,5,1,  39,HLP_MCTRL,
   0,40,NULL,_opt7_ACT,NULL,
   NULL,NULL},
{" ��� ������     ",'\0',ITM_NOR|ITM_AFT,7,1,  41,HLP_MODEL,
   0,42,NULL,_opt8_ACT,NULL,
   NULL,NULL},
{" ��� �ࠢ����� ",'\0',ITM_NOR|ITM_AFT,8,1,  43,HLP_MCTRL,
   0,44,NULL,_opt9_ACT,NULL,
   NULL,NULL},
{" ��� ������     ",'\0',ITM_NOR|ITM_AFT,10,1,  45,HLP_MODEL,
   0,46,NULL,_opt10_ACT,NULL,
   NULL,NULL},
{" ��� �ࠢ����� ",'\0',ITM_NOR|ITM_AFT,11,1,  47,HLP_MCTRL,
   0,48,NULL,_opt12_ACT,NULL,
   NULL,NULL},
};


LCL VOID CTYP _channel_MSG(NOARG);
LCL MbldT _channel_Mnu={
   _channel_Itm,8,5,17,19,36,  MNU_OMN|MNU_SAV|MNU_DSP|MNU_NQS,BOX_SNG,
   _LGREY|BROWN,_BLUE|LGREY,_BLUE|LGREY,
   _BLUE|BLACK,_BLUE|BLACK,_CYAN|BLACK,
   0,_channel_MSG,49, 0,0,0, 0xff,0xff,0xff};

LCL VOID CTYP _channel_MSG(NOARG)
{

 Wshadow(_BLACK|DGREY);
 CurHide();
  Wmessage(" �롥�� ०�� ",BRD_TOP,2,_LGREY|BROWN);
  Wprts(0,1,_BLUE|INTENSE|LGREY,"-- ����� N1");
  Wprts(3,1,_BLUE|INTENSE|LGREY,"-- ����� N2 ");
  Wprts(6,0,_BLUE|INTENSE|LGREY," -- ����� N3");
  Wprts(9,1,_BLUE|INTENSE|LGREY,"-- ����� N4 ");
  {
  MnuDsp(&_channel_Mnu);
  }
  return;
}
LCL KeyT CTYP _channel_MENU(NOARG)
{
 IntT ret;
 KbndP tcxl_old_kb,tcxl_kb=NULL;
 tcxl_old_kb=KbndChg(tcxl_kb);


 MnuBuild(&_channel_Mnu);
 ret=MnuGet();
 KbndFre();
 KbndChg(tcxl_old_kb);
  _ref_EXEC();
 return ret;
}

LCL ChrT disp_model0_var1[18];
LCL ChrT disp_model0_var3[15];
LCL FbldT _disp_model0_Fld[]={
 {"",0,0, disp_model0_var1,0,0,
  "*****************",NULL,NULL,NULL,
  0,50,HLP_NONE,
  FLD_NUL|FLD_TRM,0,FLD_CHG},
 {"",0,0, disp_model0_var3,1,0,
  "**************",NULL,NULL,NULL,
  0,51,HLP_NONE,
  FLD_NUL|FLD_TRM,0,FLD_CHG},
};

LCL EbldT _disp_model0_Frm={
 _disp_model0_Fld,2,NULL,NULL,52,0,
 0,_CYAN|INTENSE|LGREY,_BLUE|INTENSE|GREEN,_BLUE|INTENSE|GREEN
};
LCL VOID CTYP _disp_model0_2STR(NOARG)
{
  strcpy(disp_model0_var1,nam_modeli[Cfg.Def[0].mdlCod]); strsetsz(disp_model0_var1,17);
  strcpy(disp_model0_var3,wid_modeli[Cfg.Def[0].mdlType]); strsetsz(disp_model0_var3,14);
 return;
}
LCL VOID CTYP _disp_model0_WRITE(NOARG)
{
 Wopen(5,26,6,42,BOX_SPA,
       _CYAN|INTENSE|LGREY,_CYAN|INTENSE|LGREY);
 CurHide();

  _disp_model0_2STR();
  {
  EdfP e=EctlCur;
  if(e) EntPush();
  EntBuild(&_disp_model0_Frm);
  EntDsp();
  EntFree();
  if(e) EntPop();
  }

 Wunlink(Whandle());
 return;
}

LCL ChrT disp_model1_var2[18];
LCL ChrT disp_model1_var4[15];
LCL FbldT _disp_model1_Fld[]={
 {"",0,0, disp_model1_var2,0,0,
  "*****************",NULL,NULL,NULL,
  0,53,HLP_NONE,
  FLD_NUL|FLD_TRM,0,FLD_CHG},
 {"",0,0, disp_model1_var4,1,0,
  "**************",NULL,NULL,NULL,
  0,54,HLP_NONE,
  FLD_NUL|FLD_TRM,0,FLD_CHG},
};

LCL EbldT _disp_model1_Frm={
 _disp_model1_Fld,2,NULL,NULL,55,0,
 0,_CYAN|INTENSE|LGREY,_BLUE|INTENSE|GREEN,_BLUE|INTENSE|GREEN
};
LCL VOID CTYP _disp_model1_2STR(NOARG)
{
  strcpy(disp_model1_var2,nam_modeli[Cfg.Def[1].mdlCod]); strsetsz(disp_model1_var2,17);
  strcpy(disp_model1_var4,wid_modeli[Cfg.Def[1].mdlType]); strsetsz(disp_model1_var4,14);
 return;
}
LCL VOID CTYP _disp_model1_WRITE(NOARG)
{
 Wopen(5,53,6,69,BOX_SPA,
       _CYAN|INTENSE|LGREY,_CYAN|INTENSE|LGREY);
 CurHide();

  _disp_model1_2STR();
  {
  EdfP e=EctlCur;
  if(e) EntPush();
  EntBuild(&_disp_model1_Frm);
  EntDsp();
  EntFree();
  if(e) EntPop();
  }

 Wunlink(Whandle());
 return;
}

LCL ChrT disp_model2_var18[18];
LCL ChrT disp_model2_var21[15];
LCL FbldT _disp_model2_Fld[]={
 {"",0,0, disp_model2_var18,0,0,
  "*****************",NULL,NULL,NULL,
  0,56,HLP_NONE,
  FLD_NUL|FLD_TRM,0,FLD_CHG},
 {"",0,0, disp_model2_var21,1,0,
  "**************",NULL,NULL,NULL,
  0,57,HLP_NONE,
  FLD_NUL|FLD_TRM,0,FLD_CHG},
};

LCL EbldT _disp_model2_Frm={
 _disp_model2_Fld,2,NULL,NULL,58,0,
 0,_CYAN|INTENSE|LGREY,_BLUE|INTENSE|GREEN,_BLUE|INTENSE|GREEN
};
LCL VOID CTYP _disp_model2_2STR(NOARG)
{
  strcpy(disp_model2_var18,nam_modeli[Cfg.Def[2].mdlCod]); strsetsz(disp_model2_var18,17);
  strcpy(disp_model2_var21,wid_modeli[Cfg.Def[2].mdlType]); strsetsz(disp_model2_var21,14);
 return;
}
LCL VOID CTYP _disp_model2_WRITE(NOARG)
{
 Wopen(14,26,15,42,BOX_SPA,
       _CYAN|INTENSE|LGREY,_CYAN|INTENSE|LGREY);
 CurHide();

  _disp_model2_2STR();
  {
  EdfP e=EctlCur;
  if(e) EntPush();
  EntBuild(&_disp_model2_Frm);
  EntDsp();
  EntFree();
  if(e) EntPop();
  }

 Wunlink(Whandle());
 return;
}

LCL ChrT disp_model3_var19[18];
LCL ChrT disp_model3_var22[15];
LCL FbldT _disp_model3_Fld[]={
 {"",0,0, disp_model3_var19,0,0,
  "*****************",NULL,NULL,NULL,
  0,59,HLP_NONE,
  FLD_NUL|FLD_TRM,0,FLD_CHG},
 {"",0,0, disp_model3_var22,1,0,
  "**************",NULL,NULL,NULL,
  0,60,HLP_NONE,
  FLD_NUL|FLD_TRM,0,FLD_CHG},
};

LCL EbldT _disp_model3_Frm={
 _disp_model3_Fld,2,NULL,NULL,61,0,
 0,_CYAN|INTENSE|LGREY,_BLUE|INTENSE|GREEN,_BLUE|INTENSE|GREEN
};
LCL VOID CTYP _disp_model3_2STR(NOARG)
{
  strcpy(disp_model3_var19,nam_modeli[Cfg.Def[3].mdlCod]); strsetsz(disp_model3_var19,17);
  strcpy(disp_model3_var22,wid_modeli[Cfg.Def[3].mdlType]); strsetsz(disp_model3_var22,14);
 return;
}
LCL VOID CTYP _disp_model3_WRITE(NOARG)
{
 Wopen(14,53,15,69,BOX_SPA,
       _CYAN|INTENSE|LGREY,_CYAN|INTENSE|LGREY);
 CurHide();

  _disp_model3_2STR();
  {
  EdfP e=EctlCur;
  if(e) EntPush();
  EntBuild(&_disp_model3_Frm);
  EntDsp();
  EntFree();
  if(e) EntPop();
  }

 Wunlink(Whandle());
 return;
}
LCL VOID CTYP _ref_EXEC(NOARG)
{
refresh();  
            
  return;
}
LCL VOID CTYP _mod_menu_MSG(NOARG)
{

 Wshadow(_BLACK|DGREY);
 CurHide();
  Wmessage(" ��� ������ ",BRD_TOP,5,_GREEN|INTENSE|LGREY);
  return;
}
LCL VOID CTYP _mod_menu_SELECT(NOARG)
{
 IntT tmp=EntResult;
 IntT mod_menu_val;
 KbndP tcxl_old_kb,tcxl_kb=NULL;
 tcxl_old_kb=KbndChg(tcxl_kb);

  EntResult=0;
  mod_menu_val=SelStr(12,24,20,46,BOX_SNG,
             _GREEN|INTENSE|LGREY,_GREEN|INTENSE|LGREY,_BLUE|INTENSE|GREEN,
             NamesPtr,0,_mod_menu_MSG);
  if(mod_menu_val!=ERR_ARG)
     {
     Cfg.Def[chl].mdlCod=mod_menu_val;
     EntResult=1;
     }

 KbndFre();
 KbndChg(tcxl_old_kb);
 EntResult=tmp;
}

LCL VOID CTYP _opt11_ACT(NOARG)
{
Cfg.Def[chl].mdlType=0;
_Trend_ENTRY();
return;
}

LCL VOID CTYP _opt13_ACT(NOARG)
{
Cfg.Def[chl].mdlType=1;
_Trend_ENTRY();
return;
}

LCL VOID CTYP _opt14_ACT(NOARG)
{
Cfg.Def[chl].mdlType=2;
_Trend_ENTRY();
return;
}

LCL IbldT _type_menu_Itm[]={
{" ��६�饭��  ",'\0',ITM_NOR|ITM_AFT,1,1,  62,HLP_NONE,
   0,63,NULL,_opt11_ACT,NULL,
   NULL,NULL},
{" �������     ",'\0',ITM_NOR|ITM_AFT,2,1,  64,HLP_NONE,
   0,65,NULL,_opt13_ACT,NULL,
   NULL,NULL},
{" �᪮७��    ",'\0',ITM_NOR|ITM_AFT,3,1,  66,HLP_NONE,
   0,67,NULL,_opt14_ACT,NULL,
   NULL,NULL},
};


LCL VOID CTYP _type_menu_MSG(NOARG);
LCL MbldT _type_menu_Mnu={
   _type_menu_Itm,3,14,22,20,39,  MNU_OMN|MNU_SAV|MNU_DSP|MNU_NQS,BOX_SNG,
   _LGREY|BROWN,_BLUE|LGREY,_BLUE|LGREY,
   _BLUE|BLACK,_BLUE|BLACK,_CYAN|BLACK,
   0,_type_menu_MSG,68, 0,0,0, 0xff,0xff,0xff};

LCL VOID CTYP _type_menu_MSG(NOARG)
{

 Wshadow(_BLACK|DGREY);
 CurHide();
  Wmessage("��� �ࠢ�����",BRD_TOP,2,_LGREY|BROWN);
  {
  MnuDsp(&_type_menu_Mnu);
  }
  return;
}
LCL KeyT CTYP _type_menu_MENU(NOARG)
{
 IntT ret;
 KbndP tcxl_old_kb,tcxl_kb=NULL;
 tcxl_old_kb=KbndChg(tcxl_kb);


 MnuBuild(&_type_menu_Mnu);
 ret=MnuGet();
 KbndFre();
 KbndChg(tcxl_old_kb);
 return ret;
}

LCL ChrT Trend_var17[3];
LCL ChrT Trend_var20[5];
LCL ChrT Trend_var23[5];
LCL ChrT Trend_var25[5];
LCL FbldT _Trend_Fld[]={
 {"",0,0, Trend_var17,1,21,
  "99",NULL,NULL,NULL,
  0,69,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, Trend_var20,2,19,
  "9999",NULL,NULL,NULL,
  0,70,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, Trend_var23,3,19,
  "9.99",NULL,NULL,NULL,
  0,71,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, Trend_var25,4,19,
  "99.9",NULL,NULL,NULL,
  0,72,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
};

LCL EbldT _Trend_Frm={
 _Trend_Fld,4,NULL,NULL,73,0,
 0,_CYAN|INTENSE|LGREY,_BLUE|INTENSE|GREEN,_BLUE|INTENSE|GREEN
};
LCL VOID CTYP _Trend_OPEN(NOARG)
{

 Wopen(11,34,18,63,BOX_SNG,
       _MAGENTA|INTENSE|GREEN,_CYAN|INTENSE|LGREY);
 Wshadow(_BLACK|DGREY);
 CurHide();
  Wmessage("��������� �७�� ᨣ�����",BRD_TOP,2,_MAGENTA|INTENSE|GREEN);
  Wprts(1,1,_CYAN|BLACK,"���冷� �७�� ");
  Wprts(2,1,_CYAN|BLACK,"������ �鸞    ");
  Wprts(2,24,_CYAN|BLACK,"��");
  Wprts(3,1,_CYAN|BLACK,"����.��⠢����� ");
  Wprts(3,23,_CYAN|BLACK," B");
  Wprts(4,1,_CYAN|BLACK,"������� ���ࠢ�� ");
  Wprts(4,23,_CYAN|BLACK," B/c");

  _Trend_2STR();
  {
  EdfP e=EctlCur;
  if(e) EntPush();
  EntBuild(&_Trend_Frm);
  EntDsp();
  EntFree();
  if(e) EntPop();
  }

  return;
}
LCL VOID CTYP _Trend_2STR(NOARG)
{
  cvtic(Trend_var17,Cfg.Def[chl].delTr,2);
  cvtic(Trend_var20,Cfg.Def[chl].delTr_size,4);
  cvtfc(Trend_var23,Cfg.Def[chl].delTr_vol,4,2);
  cvtfc(Trend_var25,Cfg.Def[chl].delTr_speed,4,1);
 return;
}
LCL VOID CTYP _Trend_2VAL(NOARG)
{
    Cfg.Def[chl].delTr=cvtci(Trend_var17);
    Cfg.Def[chl].delTr_size=cvtci(Trend_var20);
    Cfg.Def[chl].delTr_vol=cvtcf(Trend_var23);
    Cfg.Def[chl].delTr_speed=cvtcf(Trend_var25);
 return;
}

LCL VOID CTYP _Trend_ENTRY(NOARG)
{
 EdfP e=EctlCur;
 IntT val,tmp=EntResult;
 KbndP tcxl_old_kb,tcxl_kb=NULL;
 tcxl_old_kb=KbndChg(tcxl_kb);

 _Trend_OPEN();
 _Trend_2STR();
 if(e) EntPush();
 EntBuild(&_Trend_Frm);
 CurShow();
 val=EntGet(_Trend_Fld[0].ftag);
 EntResult=0;
 if(val!=ERR_ARG)
    {
    _Trend_2VAL();
    EntResult=1;
    }
 CurHide();
 if(e) EntPop();
 KbndFre();
 KbndChg(tcxl_old_kb);
 Wclose();
 EntResult=tmp;
}
VOID CTYP MOD_PODS(NOARG)
{
{
 EdfP e=EctlCur;
 IntT val,tmp=EntResult;
 KbndP tcxl_old_kb,tcxl_kb=NULL;
 tcxl_old_kb=KbndChg(tcxl_kb);

 _main_OPEN();
 KbndSet(Key_F3,(VfvCP)_channel_MENU,0);
 KbndMou(Key_M_ClkL,(VfvCP)_channel_MENU,22,20,23);
  Vputn(22,20,"",_MAGENTA|INTENSE|GREEN,23);
  Vprints(22,20,_MAGENTA|INTENSE|LGREY," F3");
  Vprints(22,23,_MAGENTA|INTENSE|GREEN,"-");
  Vprints(22,25,_MAGENTA|INTENSE|GREEN,"������");
  Vprints(22,32,_MAGENTA|INTENSE|GREEN,"�����ன��,");
 KbndMou(Key_M_ClkL,(VfvCP)Do,22,43,23);
  Vputn(22,43,"",_MAGENTA|INTENSE|GREEN,23);
  Vprints(22,44,_MAGENTA|INTENSE|LGREY,"Ctrl+Enter");
  Vprints(22,54,_MAGENTA|INTENSE|GREEN,"-");
  Vprints(22,56,_MAGENTA|INTENSE|GREEN,"���������");
 _main_2STR();
 if(e) EntPush();
 EntBuild(&_main_Frm);
 CurShow();
 val=EntGet(_main_Fld[0].ftag);
 EntResult=0;
 if(val!=ERR_ARG)
    {
    _main_2VAL();
    EntResult=1;
    }
 CurHide();
 if(e) EntPop();
 KbndFre();
 KbndChg(tcxl_old_kb);
 Wclose();
 EntResult=tmp;
}
  return;
}

/*** END OF FILE ***/
