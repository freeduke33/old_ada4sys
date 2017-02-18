//Файл  <PARAM_F.C>
//Дополнительные функции обновления параметров
#include <userint.h>
#include <ui_attr.h>
#include <formatio.h>
#include <lwsystem.h>

#include "config.h"
#include "dialog_s.h"
#define  TRUE   1
#define  FALSE  0

extern int panel1,panel2,panel3,panel4,panel5;
extern int panel6,panel7,panel8,panel9,panel10;
extern int ChannelMode,Chl_ID[],Chl_SW;
extern int Dac_slot[],Dac_kan[],Adc_slot[],Adc_kan[],Typ_kan[];
extern int Inf_slot[],Inf_kan[];
extern int num_kan,tek_num_kan,blokirov,n_inf_kan,t_inf_kan;

//Функция индикации кнопок
void IndikacijaKnopok()
{
 int i=num_kan-1;

 SetCtrlVal(panel3,Chl_SW,i);
 for(i=0;i<ChannelMode;i++)
   SetCtrlVal(panel3,Chl_ID[i],Cfg.Def[i].isActive);
}

//------------Функция индикации канальных параметров---------
void IndikacijaParam()
{
//GetCtrlVal(panel4,WSEKANALY_WYBOR_KANAL,&num_kan);
GetCtrlVal(panel4,UPRKANAL_BLOKIR,&blokirov);
//Для панели параметров по каждому каналу
SetCtrlVal(panel1,KANAL_TYP_WYH,Cfg.Def[num_kan-1].begCod);
ParPerProc();
SetCtrlVal(panel1,KANAL_N_KAN,num_kan);
SetCtrlVal(panel1,KANAL_PASS_UROWEN,Cfg.Def[num_kan-1].PassiveVol);
SetCtrlVal(panel1,KANAL_RAB_UROWEN,Cfg.Def[num_kan-1].StartVol);
SetCtrlVal(panel1,KANAL_F_SIN_P_PR,Cfg.Def[num_kan-1].begFreq);
SetCtrlVal(panel1,KANAL_A_SIN_P_PR,Cfg.Def[num_kan-1].begAmpl);
SetCtrlVal(panel1,KANAL_KOEF_ETAL,Cfg.Def[num_kan-1].begZoom);
SetCtrlVal(panel1,KANAL_ZA_STAT_MIN,Cfg.Def[num_kan-1].MinStatic);
SetCtrlVal(panel1,KANAL_ZA_STAT_MAX,Cfg.Def[num_kan-1].MaxStatic);
SetCtrlVal(panel1,KANAL_Z_PR_SKOR,Cfg.Def[num_kan-1].MaxSpeed);
SetCtrlVal(panel1,KANAL_Z_PR_USKOR,Cfg.Def[num_kan-1].MaxAcceleration);
SetCtrlVal(panel1,KANAL_WID_POGR,Cfg.Def[num_kan-1].mstCod);
SetCtrlVal(panel1,KANAL_MAX_POGR,Cfg.Def[num_kan-1].mstMax);
SetCtrlVal(panel1,KANAL_WR_AW_RAB,Cfg.Def[num_kan-1].mstTime);
SetCtrlVal(panel1,KANAL_T_ZAD_POGR,Cfg.Def[num_kan-1].mstMargin);
SetCtrlVal(panel1,KANAL_FILE_ET,Cfg.Def[num_kan-1].file_name);
SetCtrlVal(panel1,KANAL_STAT_ETALON,Cfg.Def[num_kan-1].file_st);
SetCtrlVal(panel1,KANAL_KPER_ETALON,Cfg.Def[num_kan-1].file_k);
SetCtrlVal(panel1,KANAL_TYP_MODEL,Cfg.Def[num_kan-1].mdlCod);
SetCtrlVal(panel1,KANAL_TYP_UPR,Cfg.Def[num_kan-1].mdlType);
SetCtrlVal(panel1,KANAL_PORJADOK,Cfg.Def[num_kan-1].p);
SetCtrlVal(panel1,KANAL_ZAPAZD,Cfg.Def[num_kan-1].q);
SetCtrlVal(panel1,KANAL_REGULARIZ,Cfg.Def[num_kan-1].Q);
SetCtrlVal(panel1,KANAL_WID_TREND,Cfg.Def[num_kan-1].trCod);
SetCtrlVal(panel1,KANAL_POR_TREND,Cfg.Def[num_kan-1].trP);
SetCtrlVal(panel1,KANAL_POR_TR_IZ,Cfg.Def[num_kan-1].delTr);
SetCtrlVal(panel1,KANAL_DL_TR_IZ,Cfg.Def[num_kan-1].delTr_size);
SetCtrlVal(panel1,KANAL_POS_TR_IZ,Cfg.Def[num_kan-1].delTr_vol);
SetCtrlVal(panel1,KANAL_SKOR_TR_IZ,Cfg.Def[num_kan-1].delTr_speed);
SetCtrlVal(panel1,KANAL_WID_ETAL_SIG,Cfg.Def[num_kan-1].tplCod);
SetCtrlVal(panel1,KANAL_GAR_AMP1,Cfg.Def[num_kan-1].a[0]);
SetCtrlVal(panel1,KANAL_GAR_AMP2,Cfg.Def[num_kan-1].a[1]);
SetCtrlVal(panel1,KANAL_GAR_AMP3,Cfg.Def[num_kan-1].a[2]);
SetCtrlVal(panel1,KANAL_GAR_CH1,Cfg.Def[num_kan-1].f[0]);
SetCtrlVal(panel1,KANAL_GAR_CH2,Cfg.Def[num_kan-1].f[1]);
SetCtrlVal(panel1,KANAL_GAR_CH3,Cfg.Def[num_kan-1].f[2]);
SetCtrlVal(panel1,KANAL_GAR_FA1,Cfg.Def[num_kan-1].fa[0]);
SetCtrlVal(panel1,KANAL_GAR_FA2,Cfg.Def[num_kan-1].fa[1]);
SetCtrlVal(panel1,KANAL_GAR_FA3,Cfg.Def[num_kan-1].fa[2]);
SetCtrlVal(panel1,KANAL_STAT_GARM,Cfg.Def[num_kan-1].u);

//Для панели характеристик каналов
StatPerProc();
SetCtrlVal(panel2,STA_N_KAN,num_kan);
//Статические
SetCtrlVal(panel2,STA_TYP_UPR,Cfg.Sth[num_kan-1].Mode);
SetCtrlVal(panel2,STA_N_STAT,Cfg.Sth[num_kan-1].Nq);
SetCtrlVal(panel2,STA_T_PERPR,Cfg.Sth[num_kan-1].Wait);
SetCtrlVal(panel2,STA_MIN_STATIC,Cfg.Sth[num_kan-1].Lo);
SetCtrlVal(panel2,STA_MAX_STATIC,Cfg.Sth[num_kan-1].Hi);
SetCtrlVal(panel2,STA_MAX_SKOR_STAT,Cfg.Sth[num_kan-1].HiSpeed);
SetCtrlVal(panel2,STA_MAX_USKOR_STAT,Cfg.Sth[num_kan-1].HiAcceleration);
//Частотные
SetCtrlVal(panel8,ACH_N_KAN,num_kan);
SetCtrlVal(panel8,ACH_TYP_UPR,Cfg.Ach[num_kan-1].Mode);
SetCtrlVal(panel8,ACH_N_ACH,Cfg.Ach[num_kan-1].Nq);
SetCtrlVal(panel8,ACH_MIN_FRE,Cfg.Ach[num_kan-1].FreqMin);
SetCtrlVal(panel8,ACH_MAX_FRE,Cfg.Ach[num_kan-1].FreqMax);
SetCtrlVal(panel8,ACH_N_PERIOD,Cfg.Ach[num_kan-1].Periods);
SetCtrlVal(panel8,ACH_AMP_ACH,Cfg.Ach[num_kan-1].Ampl);
SetCtrlVal(panel8,ACH_STAT_ACH,Cfg.Ach[num_kan-1].Const);
//
//SetCtrlVal(panel4,UPRKANAL_FILE_CONFIG,);
//Для панели системных параметров
SetCtrlVal(panel5,SYSHARAKT_F_DISK,Cfg.Freq);
SetCtrlVal(panel5,SYSHARAKT_DL_NR,Cfg.nr);
SetCtrlVal(panel5,SYSHARAKT_VIEW_DL_NR,Cfg.nr_view);
SetCtrlVal(panel5,SYSHARAKT_N_CIKL,Cfg.cycle);
SetCtrlVal(panel5,SYSHARAKT_T_WYH_N_T,Cfg.iniTime);
SetCtrlVal(panel5,SYSHARAKT_IM_INP_FILE,Cfg.ifile_name);
//SetCtrlVal(panel5,SYSHARAKT_IM_OUT_FILE,Cfg.ufile_name);
//if(Cfg.nk>ChannelMode)
Cfg.nk=ChannelMode;

SetCtrlVal(panel5,SYSHARAKT_NK_UPR,Cfg.nk);
SetCtrlVal(panel5,SYSHARAKT_NK_INF,Cfg.nkInfo);
//SetCtrlVal(panel5,SYSHARAKT_N_DI_IN,Cfg.nkIN);
//SetCtrlVal(panel5,SYSHARAKT_N_DI_OUT,Cfg.nkOUT);
SetCtrlVal(panel5,SYSHARAKT_N_KAN,num_kan);
SetCtrlVal(panel5,SYSHARAKT_TYP_DATCH,Cfg.Def[num_kan-1].usrDAC.T);
SetCtrlVal(panel5,SYSHARAKT_N_SLOT,Dac_slot[num_kan-1]);
SetCtrlVal(panel5,SYSHARAKT_N_KONT,Dac_kan[num_kan-1]);
SetCtrlVal(panel5,SYSHARAKT_TYP_D_ACP,Cfg.Def[num_kan-1].usrADC.T);
SetCtrlVal(panel5,SYSHARAKT_ACP_SLOT,Adc_slot[num_kan-1]);
SetCtrlVal(panel5,SYSHARAKT_ACP_KONT,Adc_kan[num_kan-1]);
SetCtrlVal(panel5,SYSHARAKT_KN_ANAL_SIG,Cfg.Inform[n_inf_kan].isActive);
SetCtrlVal(panel5,SYSHARAKT_INF_N_DATCH,(n_inf_kan+1));
SetCtrlVal(panel5,SYSHARAKT_INF_N_SLOT,Inf_slot[n_inf_kan]);
SetCtrlVal(panel5,SYSHARAKT_INF_N_KANAL,Inf_kan[n_inf_kan]);
SetCtrlVal(panel5,SYSHARAKT_INF_N_KANAL,Inf_kan[n_inf_kan]);
//Для панели без модели
SetCtrlVal(panel9,MODELOFF_APP_Q,Cfg.NMDL.Def[num_kan-1].q);
SetCtrlVal(panel9,MODELOFF_GAM_DOP,Cfg.NMDL.Def[num_kan-1].Mistake);
SetCtrlVal(panel9,MODELOFF_RAZM_OKNA,Cfg.NMDL.Def[num_kan-1].nr);
SetCtrlVal(panel9,MODELOFF_NACH_OTCH,Cfg.NMDL.Def[num_kan-1].Margin);
SetCtrlVal(panel9,MODELOFF_N_RASCH_T,Cfg.NMDL.Def[num_kan-1].nq);
SetCtrlVal(panel9,MODELOFF_PAKET_REG,Cfg.NMDL.isBatch);
SetCtrlVal(panel9,MODELOFF_N_ITERAC,Cfg.NMDL.Cycles);

}

//--------Функция для параметров переходного процесса-------
ParPerProc()
{
switch (Cfg.Def[num_kan-1].begCod)
  {
  case 0:
    SetInputMode(panel1,KANAL_F_SIN_P_PR,TRUE);
    SetInputMode(panel1,KANAL_A_SIN_P_PR,TRUE);
    SetInputMode(panel1,KANAL_KOEF_ETAL,FALSE);
  break;
  case 1:
    SetInputMode(panel1,KANAL_F_SIN_P_PR,FALSE);
    SetInputMode(panel1,KANAL_A_SIN_P_PR,FALSE);
    SetInputMode(panel1,KANAL_KOEF_ETAL,TRUE);
  break;
  case 2:
    SetInputMode(panel1,KANAL_F_SIN_P_PR,FALSE);
    SetInputMode(panel1,KANAL_A_SIN_P_PR,FALSE);
    SetInputMode(panel1,KANAL_KOEF_ETAL,FALSE);
  break;
  }
 return 0;
}

StatPerProc(void)
{
switch (Cfg.Sth[num_kan-1].Mode)
  {
  case 0:
    SetInputMode(panel2,STA_MIN_STATIC,TRUE);
    SetInputMode(panel2,STA_MAX_STATIC,TRUE);
    SetInputMode(panel2,STA_MAX_SKOR_STAT,FALSE);
    SetInputMode(panel2,STA_MAX_USKOR_STAT,FALSE);
  break;
  case 1:
    SetInputMode(panel2,STA_MIN_STATIC,FALSE);
    SetInputMode(panel2,STA_MAX_STATIC,FALSE);
    SetInputMode(panel2,STA_MAX_SKOR_STAT,TRUE);
    SetInputMode(panel2,STA_MAX_USKOR_STAT,FALSE);
  break;
  case 2:
    SetInputMode(panel2,STA_MIN_STATIC,FALSE);
    SetInputMode(panel2,STA_MAX_STATIC,FALSE);
    SetInputMode(panel2,STA_MAX_SKOR_STAT,FALSE);
    SetInputMode(panel2,STA_MAX_USKOR_STAT,TRUE);
  break;
  }
 return 0;
}


