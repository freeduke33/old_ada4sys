

/*
 *   Generated by  UI v2.00
 *        January 26, 1996
 */


/* Module: SETSIN.C ===================== */

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
#include "mgen_def.h"




VOID CTYP _main_2STR(NOARG);
VOID CTYP _main_OPEN(NOARG);


LCL ChrT main_var1[11];
LCL ChrT main_var2[11];
LCL ChrT main_var3[11];
LCL FbldT _main_Fld[]={
 {"",0,0, main_var1,1,23,
  "9999999.99",NULL,NULL,NULL,
  0,1,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var2,2,23,
  "9999999.99",NULL,NULL,NULL,
  0,2,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
 {"",0,0, main_var3,3,23,
  "9999999.99",NULL,NULL,NULL,
  0,3,HLP_NONE,
  FLD_NUL|FLD_DEC,0,FLD_CHG},
};

LCL EbldT _main_Frm={
 _main_Fld,3,NULL,NULL,4,0,
 0,_CYAN|BLACK,_LGREY|BLACK,_LGREY|BLACK
};
LCL VOID CTYP _main_OPEN(NOARG)
{

 Wopen(7,25,13,64,BOX_SNG,
       _CYAN|INTENSE|LGREY,_CYAN|BLACK);
 Wshadow(_BLACK|DGREY);
 CurHide();
  Wmessage(" ����ᮨ����� ᨣ��� ",BRD_TOP,8,_CYAN|INTENSE|LGREY);
  Wprts(1,3,_CYAN|BLACK,"������㤠");
  Wprts(1,13,_CYAN|BLACK,"ᨣ����");
  Wprts(1,21,_CYAN|BLACK,":");
  Wprts(1,33,_BLUE|INTENSE|LGREY," ");
  Wprts(2,3,_CYAN|BLACK,"�����");
  Wprts(2,11,_CYAN|BLACK,"ᨣ����");
  Wprts(2,21,_CYAN|BLACK,":");
  Wprts(2,33,_BLUE|INTENSE|LGREY," ");
  Wprts(3,3,_CYAN|BLACK,"����");
  Wprts(3,8,_CYAN|BLACK,"ᨣ����");
  Wprts(3,21,_CYAN|BLACK,":");
  Wprts(3,33,_BLUE|INTENSE|LGREY," ");

  _main_2STR();
  EntPush();
  EntBuild(&_main_Frm);
  EntDsp();
  EntFree();
  EntPop();

  return;
}
LCL VOID CTYP _main_2STR(NOARG)
{
  cvtfc(main_var1,EntPtr->Dat.Sin.Ampl,10,2);
  cvtfc(main_var2,EntPtr->Dat.Sin.Freq,10,2);
  cvtfc(main_var3,EntPtr->Dat.Sin.Faza,10,2);
 return;
}
LCL VOID CTYP _main_2VAL(NOARG)
{
    EntPtr->Dat.Sin.Ampl=cvtcf(main_var1);
    EntPtr->Dat.Sin.Freq=cvtcf(main_var2);
    EntPtr->Dat.Sin.Faza=cvtcf(main_var3);
 return;
}

VOID CTYP SETSIN(NOARG)
{
{
 IntT val,tmp=EntResult;
 KbndP tcxl_old_kb,tcxl_kb=NULL;
 tcxl_old_kb=KbndChg(tcxl_kb);

 _main_OPEN();
 _main_2STR();
 EntPush();
 EntBuild(&_main_Frm);
 CurShow();
 EntResult=0;
 val=EntGet(_main_Fld[0].ftag);
 if(val!=ERR_ARG)
    {
    _main_2VAL();
    EntResult=1;
    }
 CurHide();
 EntPop();
 KbndFre();
 KbndChg(tcxl_old_kb);
 Wclose();
 EntResult=tmp;
}
  return;
}

/*** END OF FILE ***/
