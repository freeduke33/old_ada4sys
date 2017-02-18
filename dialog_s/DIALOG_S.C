//Программа для ведения главного диалога
//в многоканальной адаптивной системе
//                <DIALOG_S.C>
//==============================================================
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <dir.h>
#include <stdlib.h>
#include <process.h>

#include "dac_adc.h"
#include "dialog_s.h"
#include "dbc.h"
#include "dbcdef.h"
#include "config.h"
// #include "swap.h"
#include "reports.h"
#include "spectr.h"
#include "database.h"

#include <userint.h>
#include <ui_attr.h>
#include <formatio.h>
#include <lwsystem.h>

unsigned int _stklen=16000;

/* DEFINES */
#define  TRUE   1
#define  FALSE  0
#define  OFF    0
#define  NMAX 1005
#define  PMAX 15
#define  QQMAX 17

float xk[8],yk[8];

//int Typ_kan[8],Typ_out[8];
int Dac_slot[]={2,2,2,2,2,2,2,2},Dac_kan[]={0,1,2,3,4,5,6,7,8};
int Adc_slot[]={1,1,1,1,1,1,1,1},Adc_kan[]={0,1,2,3,4,5,6,7,8};

int Inf_slot[8]={2,3,3,3,3,3,3,3},Inf_kan[8]={15,6,0,1,2,3,4,5};

//int Upr_slot[8]={2,2,2,2,2,2,2,2};
//int Upr_kan_p[8]={6,7,8,9,10,11,12,13},Upr_kan_s[8]={0,1,2,3,4,5,6,7};
//int Upr_cap_slot[8]={0,0,0,0,0,0,0,0},Upr_cap_kan[8]={0,1,2,3,4,5,6,7};
int num_kan,tek_num_kan,blokirov=0,n_inf_kan=0,t_inf_kan=0;
int id,menu_bar,val,panel1,panel2,panel3;
int panel4,panel5,panel6,panel7,panel8,panel9,panel10,handle;
double pr1;

int ChannelMode=8,Chl_ID[MAX_CHANNEL],Chl_SW;

double *S1=NULL,*S2=NULL,*S=NULL; // спектры
int nq;
int Spektr(int);
UnitUPI prizn_kan;


void IndikacijaKnopok(void);
void IndikacijaParam(void);
void ZapisParam(void);
int ParPerProc(void);
int StatPerProc(void);
int DrvEnable(float);
int DrvDisable();
int DrvWait();



void setHardware(void)
{
 int i;

 for(i=0;i<ChannelMode;i++)
   {
   LoadHardwareMap(Cfg.Def[i].usrDAC.T,i,&Dac_slot[i],&Dac_kan[i],NULL);
   LoadHardwareMap(Cfg.Def[i].usrADC.T,i,&Adc_slot[i],&Adc_kan[i],NULL);
   }
 return;
}


void selectPanel(int pnl,int num)
{
 tek_num_kan=num_kan-1;
 ZapisParam();
 IndikacijaKnopok();
 IndikacijaParam();
 HidePanel(panel1);
 HidePanel(panel2);
 HidePanel(panel5);
 HidePanel(panel6);
 HidePanel(panel7);
 HidePanel(panel8);
 HidePanel(panel9);

 if(num!=6)
   {
   DisplayPanel(panel3);
   DisplayPanel(panel4);
   }
 else
   {
   HidePanel(panel3);
   HidePanel(panel4);
   }

 DisplayPanel(pnl);
 Cfg.LastMode=num;
}



/* MAIN PROGRAM */
main(int argc,char *argv[])
{
FILE *fl,*wl,*fk1,*fk2,*fk3,*fk4;
int chan1,chan2,chan3;
char msg[60],ex[6];
char filename[256],x_fiele[31],f_ini[31];
char katal[55];
static float fdisk;
int nkan,typ_reg=1;
char str1[50];
char str2[50];
char str3[50];
int qq,q,p,n1,nkfl,q_opt,p_opt,nkan_tem;
long nr;
int i,j,k,v,m,ik;
double pr2,pr3,pr4;
float prfl[13];
int isLoop=1;
char grfMode[]=" ";
float sp_fmin,sp_fmax,p_filtr;
int sp_nkan,n_spt;
if(argc>1) ChannelMode= atoi(argv[1]);
DrvInitialize();

while (TRUE)
{
//-----------------------------------------------------------------
if(loadConfig("system.ini")==0)
  {
  strcpy(msg,"system.ini");
  fnsplit(msg,str1,str2,str3,ex);
  strcat(str3,ex);
  SetCtrlVal(panel4,UPRKANAL_FILE_CONFIG,str3);
  }
else
  MessagePopup("Не существует конфигурационный файл <system.ini>");

//if(DrvInitialize()!=DRV_OK) return;

//Cfg.Def[0].PassiveVol=4.0;
//Cfg.Def[1].PassiveVol=4.1;
//Cfg.Def[2].PassiveVol=4.2;
menu_bar = LoadMenuBar ("dialog_s.uir", GLMENU);
if (menu_bar < 0)
  {
  MessagePopup("Невозможно открыть панель GLMENU");
  return;
  }
panel1 = LoadPanel ("dialog_s.uir",KANAL);
if (panel1 < 0)
  {
  MessagePopup("Невозможно открыть панель KANAL");
  return;
  }
panel2 = LoadPanel ("dialog_s.uir", STA);
if (panel2 < 0)
  {
  MessagePopup("Невозможно открыть панель STA");
  return;
  }
switch(ChannelMode)
  {
  case 4:  panel3 = LoadPanel ("dialog_s.uir", WSEKAN4);
           Chl_ID[0]=WSEKAN4_K1_IN;
           Chl_ID[1]=WSEKAN4_K2_IN;
           Chl_ID[2]=WSEKAN4_K3_IN;
           Chl_ID[3]=WSEKAN4_K4_IN;
           Chl_SW=WSEKAN4_WYBOR_KANAL;
           break;
  case 8:  panel3 = LoadPanel ("dialog_s.uir", WSEKAN8);
           Chl_ID[0]=WSEKAN8_K1_IN;
           Chl_ID[1]=WSEKAN8_K2_IN;
           Chl_ID[2]=WSEKAN8_K3_IN;
           Chl_ID[3]=WSEKAN8_K4_IN;
           Chl_ID[4]=WSEKAN8_K5_IN;
           Chl_ID[5]=WSEKAN8_K6_IN;
           Chl_ID[6]=WSEKAN8_K7_IN;
           Chl_ID[7]=WSEKAN8_K8_IN;
           Chl_SW=WSEKAN8_WYBOR_KANAL;
           break;
  default: panel3 = LoadPanel ("dialog_s.uir", WSEKANALY);
           Chl_ID[0]=WSEKANALY_K1_IN;
           Chl_ID[1]=WSEKANALY_K2_IN;
           Chl_ID[2]=WSEKANALY_K3_IN;
           Chl_ID[3]=WSEKANALY_K4_IN;
           Chl_ID[4]=WSEKANALY_K5_IN;
           Chl_ID[5]=WSEKANALY_K6_IN;
           Chl_SW=WSEKANALY_WYBOR_KANAL;
           break;
  }
if (panel3 < 0)
  {
  MessagePopup("Невозможно открыть панель каналов");
  return;
  }
panel4 = LoadPanel ("dialog_s.uir", UPRKANAL);
if (panel4 < 0)
  {
  MessagePopup("Невозможно открыть панель UPRKANAL");
  return;
  }
panel5 = LoadPanel ("dialog_s.uir", SYSHARAKT);
if (panel5 < 0)
  {
  MessagePopup("Невозможно открыть панель SYSHARAKT");
  return;
  }
panel6 = LoadPanel ("dialog_s.uir", PARAMEXP);
if (panel6 < 0)
  {
  MessagePopup("Невозможно открыть панель PARAMEXP");
  return;
  }
panel7 = LoadPanel ("dialog_s.uir", PROCACH);
if (panel7 < 0)
  {
  MessagePopup("Невозможно открыть панель PROCACH");
  return;
  }
panel8 = LoadPanel ("dialog_s.uir", ACH);
if (panel8 < 0)
  {
  MessagePopup("Невозможно открыть панель ACH");
  return;
  }
panel9 = LoadPanel ("dialog_s.uir", MODELOFF);
if (panel9 < 0)
  {
  MessagePopup("Невозможно открыть панель MODELOFF");
  return;
  }
panel10 = LoadPanel ("dialog_s.uir", CONTRPAR);
if (panel10 < 0)
  {
  MessagePopup("Невозможно открыть панель CONTRPAR");
  return;
  }

if(loadConfig("SYSTEM.INI")==0)
  {
  strcpy(msg,"SYSTEM.INI");
  fnsplit(msg,str1,str2,str3,ex);
  strcat(str3,ex);
  SetCtrlVal(panel4,UPRKANAL_FILE_CONFIG,str3);
  }
else
  MessagePopup("Не существует конфигурационный файл <SYSTEM.INI>");

//DisplayPanel (panel10);
//getch();

setHardware();
num_kan=Cfg.LastField>Cfg.nk ? 1 : Cfg.LastField+1;
tek_num_kan=num_kan-1;

if(Cfg.LastMode!=6)
  {
  DisplayPanel (panel3);
  DisplayPanel (panel4);
  }

switch(Cfg.LastMode)
  {
  case 1:  DisplayPanel (panel1); break;
  case 2:  DisplayPanel (panel2); break;
//  case 3:  DisplayPanel (panel3); break;
//  case 4:  DisplayPanel (panel4); break;
  case 5:  DisplayPanel (panel5); break;
  case 6:  DisplayPanel (panel6); break;
  case 7:  DisplayPanel (panel7); break;
  case 8:  DisplayPanel (panel8); break;
  case 9:  DisplayPanel (panel9); break;
  case 10: DisplayPanel (panel10); break;

  default: DisplayPanel (panel1); break;
  }

IndikacijaKnopok();
IndikacijaParam();



while (isLoop)
  {
  GetUserEvent (TRUE, &handle, &id);
  if(handle == menu_bar)
    {
    switch (id)
      {
      case GLMENU_FILE_LOAD_TEM :
        tek_num_kan=num_kan-1;
        ZapisParam();
        IndikacijaKnopok();
        IndikacijaParam();

        if (FileSelectPopup ("","*.tem","Выберите имя файла",0,1,1,filename) == 1)
          {
          fl=fopen(filename,"r");
          fscanf(fl,"%d %f %ld",&nkan_tem,&fdisk,&nr);
          if(ChannelMode<nkan_tem)
            {
            MessagePopup("Число каналов в файле *.tem превышает допустимое");
//          break;
            }
//        strcpy(msg,filename);
          fnsplit(filename,str1,str2,str3,ex);
          strcat(str3,ex);
          strcpy(Cfg.ifile_name,str3);
          strcpy(Cfg.Def[num_kan-1].file_name,str3);
          Cfg.Freq=fdisk; Cfg.nr=nr; Cfg.nk=ChannelMode+1;
          SetCtrlVal(panel5,SYSHARAKT_NK_UPR,Cfg.nk);
          SetCtrlVal(panel5,SYSHARAKT_IM_INP_FILE,Cfg.ifile_name);
          SetCtrlVal(panel5,SYSHARAKT_F_DISK,Cfg.Freq);
          SetCtrlVal(panel5,SYSHARAKT_DL_NR,Cfg.nr);
//        SetCtrlVal(panel5,SYSHARAKT_NK_UPR,Cfg.nk);
          SetCtrlVal(panel9,MODELOFF_ETALON_FILE,str3);
//        SetCtrlVal(panel9,MODELOFF_N_KAN,Cfg.nk);
          SetCtrlVal(panel9,MODELOFF_NR_ET,Cfg.nr);
          for(i=nkan_tem;i<ChannelMode;i++)
                        Cfg.Def[i].isActive=0;
          IndikacijaKnopok();
          for(i=0;i<MAX_CHANNEL;i++)
            strcpy(Cfg.Def[i].file_name,Cfg.ifile_name);
          fclose(fl);
          }
        else
          {
          MessagePopup("Файл не найден");
          break;
          }
      break;
      case GLMENU_FILE_SAVE_TEM :
        tek_num_kan=num_kan-1;
        ZapisParam();
        IndikacijaKnopok();
        IndikacijaParam();
        FileSelectPopup ("","*.tem","Выберите имя файла",0,1,1,filename);
//        strcpy(msg,filename);
        fnsplit(filename,str1,str2,str3,ex);
        strcat(str3,ex);
        strcpy(Cfg.ufile_name,str3);
        SetMouseCursor(MOUSE_HOUR_GLASS);
        write_tem();
        SetMouseCursor(MOUSE_ARROW);
//        SetCtrlVal(panel5,SYSHARAKT_IM_OUT_FILE,Cfg.ufile_name);
        SetCtrlVal(panel5,GLMENU_FILE_SAVE_UPR,Cfg.ufile_name);
      break;
      case GLMENU_FILE_SAVE_UPR :
        tek_num_kan=num_kan-1;
        ZapisParam();
        IndikacijaKnopok();
        IndikacijaParam();
        FileSelectPopup ("","*.upr","Выберите имя файла",0,1,1,filename);
        strcpy(msg,filename);
        fnsplit(msg,str1,str2,str3,ex);
        strcat(str3,ex);
        strcpy(Cfg.ufile_name,str3);
        SetMouseCursor(MOUSE_HOUR_GLASS);
        write_upr();
        SetMouseCursor(MOUSE_ARROW);
//        SetCtrlVal(panel5,SYSHARAKT_IM_OUT_FILE,Cfg.ufile_name);
        SetCtrlVal(panel5,GLMENU_FILE_SAVE_UPR,Cfg.ufile_name);
      break;
      case GLMENU_FILE_SAVE_STH :
        tek_num_kan=num_kan-1;
        ZapisParam();
        IndikacijaKnopok();
        IndikacijaParam();
        FileSelectPopup ("","*.sth","Выберите имя файла",0,1,1,filename);
        strcpy(msg,filename);
        fnsplit(msg,str1,str2,str3,ex);
        strcat(str3,ex);
        strcpy(Cfg.sfile_name,str3);
        SetMouseCursor(MOUSE_HOUR_GLASS);
        write_sth();
        SetMouseCursor(MOUSE_ARROW);
      break;
      case GLMENU_FILE_SAVE_ACH :
        tek_num_kan=num_kan-1;
        ZapisParam();
        IndikacijaKnopok();
        IndikacijaParam();
        FileSelectPopup ("","*.ach","Выберите имя файла",0,1,1,filename);
        strcpy(msg,filename);
        fnsplit(msg,str1,str2,str3,ex);
        strcat(str3,ex);
        strcpy(Cfg.afile_name,str3);
        SetMouseCursor(MOUSE_HOUR_GLASS);
        write_achh();
        SetMouseCursor(MOUSE_ARROW);
      break;
      case GLMENU_OBSYS_OSN_PARSYS :
        selectPanel(panel5,5);
      break;
      case GLMENU_OBSYS_PAR_K_UPR :
        selectPanel(panel1,1);
      break;
      case GLMENU_OBSYS_HARAK_SYS :
        selectPanel(panel2,2);
      break;
      case GLMENU_OBSYS_ACH :
        selectPanel(panel8,8);
      break;
      case GLMENU_OBSYS_SPEKTR:
        selectPanel(panel6,6);
      break;
      case GLMENU_OBSYS_RASCH_SIGN :
        selectPanel(panel9,9);
      break;
      case GLMENU_KONTROL :
        tek_num_kan=num_kan-1;
        ZapisParam();
        IndikacijaKnopok();
        IndikacijaParam();
        HidePanel(panel6);
        HidePanel(panel2);
        DisplayPanel (panel3);
        DisplayPanel (panel4);
      break;

      case GLMENU_KONTROL_GR_OUT_ET:
        isLoop=0;
        grfMode[0]='2';
      break;
      case GLMENU_KONTROL_GR_IN_OUT:
        isLoop=0;
        grfMode[0]='3';
      break;
      case GLMENU_KONTROL_GR_CHAST:
        isLoop=0;
        grfMode[0]='6';
      break;
      case GLMENU_KONTROL_GR_STAT:
        isLoop=0;
        grfMode[0]='5';
      break;
      case GLMENU_START :
       switch(Cfg.LastMode)
        {
        case 1: /*=====================================================*/
        case 2: /*=====================================================*/
        tek_num_kan=num_kan-1;
        ZapisParam();
        IndikacijaKnopok();
        IndikacijaParam();
// ----------- ЗАПУСК ПРОГРАММЫ НА РАБОТУ <*.exe> ------------
        SetMouseCursor(MOUSE_HIDDEN);
        saveConfig("system.ini");                // PID Mode CfgFile
//      if(!swap_ison()) swap_on();
//      spawnl(P_WAIT,"ada_loop.exe","ada_loop.exe","123","1","system.ini",NULL);
        DrvDeinitialize();
        exit(1);
        SetMouseCursor(MOUSE_ARROW);
        isLoop=0;
        grfMode[0]='2'; // 3- вход/выход 2-эталон/выход

      OpenInterfaceManager();

      menu_bar = LoadMenuBar ("dialog_s.uir", GLMENU);
        HidePanel(panel6);
        HidePanel(panel1);
        HidePanel(panel2);
        DisplayPanel (panel5);
        DisplayPanel (panel4);
        DisplayPanel (panel3);
        break;
        case 8: /*=====================================================*/
      //Запуск программы на снятие АЧХ хар-тик
        tek_num_kan=num_kan-1;
        ZapisParam();
        DisplayPanel (panel7);
        SetMouseCursor(MOUSE_HIDDEN);
        saveConfig("system.ini");                // PID Mode CfgFile
//      if(!swap_ison()) swap_on();
//      spawnl(P_WAIT,"ada_loop.ovl","ada_loop.ovl","123","2","system.ini",NULL);
        DrvDeinitialize();
        exit(2);
        SetMouseCursor(MOUSE_ARROW);
        isLoop=0;
        grfMode[0]='6'; // 6 -АЧХ
        HidePanel(panel6);
        HidePanel(panel1);
        HidePanel(panel7);
        HidePanel(panel2);
        DisplayPanel (panel8);
        DisplayPanel (panel4);
        DisplayPanel (panel3);
        break;
        default:
          MessagePopup("Из этого режима нельзя запустить испытания");
        break;
        }
      break;
      case GLMENU_QUIT :
        tek_num_kan=num_kan-1;
        ZapisParam();
        CloseInterfaceManager();
        saveConfig("system.ini");
        return;
      }
    }

  else //Параметры каждого канала
  if(handle == panel1)
    {
    switch (id)
      {
      case KANAL_TYP_WYH :
        GetCtrlVal(panel1,KANAL_TYP_WYH,&Cfg.Def[num_kan-1].begCod);
        ParPerProc();
      break;
      case KANAL_TYP_MODEL :
        GetCtrlVal(panel1,KANAL_TYP_MODEL,&Cfg.Def[num_kan-1].mdlCod);
      break;
      case KANAL_TYP_UPR :
        GetCtrlVal(panel1,KANAL_TYP_UPR,&Cfg.Def[num_kan-1].mdlType);
      break;
      case KANAL_WID_TREND :
        GetCtrlVal(panel1,KANAL_WID_TREND,&Cfg.Def[num_kan-1].trCod);
      break;
      case KANAL_WID_ETAL_SIG :
        GetCtrlVal(panel1,KANAL_WID_ETAL_SIG,&Cfg.Def[num_kan-1].tplCod);
      break;
      }
    }
//Управление от <panel2> - Характеристики каналов
  else
  if(handle == panel2)
    {
    switch (id)
      {
//Для статических характеристик
      case STA_TYP_UPR :
        GetCtrlVal(panel2,STA_TYP_UPR,&Cfg.Sth[num_kan-1].Mode);
        StatPerProc();
      break;
/*      case STA_ZAP_STAT :
      //Запуск программы на снятие статических хар-тик
        tek_num_kan=num_kan-1;
        ZapisParam();
        DisplayPanel (panel7);
        SetMouseCursor(MOUSE_HIDDEN);
        saveConfig("system.ini");                // PID Mode CfgFile
//      if(!swap_ison()) swap_on();
//      spawnl(P_WAIT,"ada_loop.ovl","ada_loop.ovl","123","3","system.ini",NULL);
        DrvDeinitialize();
        exit(3);
        SetMouseCursor(MOUSE_ARROW);
        isLoop=0;
        grfMode[0]='5'; // 6 -АЧХ
        HidePanel(panel6);
        HidePanel(panel1);
        HidePanel(panel7);
        DisplayPanel (panel2);
        DisplayPanel (panel4);
        DisplayPanel (panel3);
      break;              */
      }
    }
//Управление от <panel8> - АЧХ
  else
  if(handle == panel8)
    {
    switch (id)
      {
//Для частотных характеристик
      case ACH_TYP_UPR :
        GetCtrlVal(panel2,ACH_TYP_UPR,&Cfg.Ach[num_kan-1].Mode);
        StatPerProc();
      break;
/*      case ACH_ZAP_ACH :
      break;*/
      }
    }
  else  //Управление от панели <panel3> - Панель с кнопками всех каналов
  if(handle == panel3)
    {
    tek_num_kan=num_kan-1;
    ZapisParam();
//    IndikacijaKnopok();
    IndikacijaParam();
    for(i=0;i<ChannelMode;i++)
         if(id==Chl_ID[i]) GetCtrlVal(panel3,Chl_ID[i],&Cfg.Def[i].isActive);
      if(id==Chl_SW)
        {
        tek_num_kan=num_kan-1;
        ZapisParam();//Запись параметров для предыдущего положения перекл.
        GetCtrlVal(panel3,Chl_SW,&num_kan);
        Cfg.LastField=num_kan-1;
        IndikacijaParam();
        }
    }
  //Управление от панели <panel4> - Управление каналами
  else
  if(handle == panel4)
    {
    switch (id)
      {
      case UPRKANAL_BLOKIR :
        GetCtrlVal(panel4,UPRKANAL_BLOKIR,&blokirov);
        if(blokirov==0)
          {
          SetInputMode(panel1,-1,TRUE);
          SetInputMode(panel3,-1,TRUE);
          }
        else
          {
          SetInputMode(panel1,-1,FALSE);
          SetInputMode(panel3,-1,FALSE);
          }
      break;
      case UPRKANAL_SAV_PARAM :
        tek_num_kan=num_kan-1;
        ZapisParam();
        FileSelectPopup ("","*.ini","Запись конфигурации в файл",0,1,1,f_ini);
        saveConfig(f_ini);
      break;
      case UPRKANAL_LOAD_PARAM :
        tek_num_kan=num_kan-1;
        ZapisParam();
        IndikacijaKnopok();
        IndikacijaParam();
        FileSelectPopup ("","*.ini","Чтение конфигурац. файла",0,1,1,f_ini);
        if(loadConfig(f_ini)==0)
          {
          strcpy(msg,f_ini);
          fnsplit(msg,str1,str2,str3,ex);
          strcat(str3,ex);
          SetCtrlVal(panel4,UPRKANAL_FILE_CONFIG,str3);
          setHardware();
          num_kan=Cfg.LastField>Cfg.nk ? 1 : Cfg.LastField+1;
          tek_num_kan=num_kan-1;
          IndikacijaKnopok();
          IndikacijaParam();
          }
        else
          MessagePopup("Имя несуществующего конфигурационного файла");
      break;
      }
    }
  //Управление от панели <panel5> - Системные параметры
  else
  if(handle == panel5)
    {
    switch (id)
      {
      case SYSHARAKT_TYP_DATCH :
        GetCtrlVal(panel5,SYSHARAKT_TYP_DATCH,&Cfg.Def[num_kan-1].usrDAC.T);
        LoadHardwareMap(Cfg.Def[num_kan-1].usrDAC.T,num_kan-1,
                        &Dac_slot[num_kan-1],&Dac_kan[num_kan-1],NULL);
        IndikacijaParam();
      break;
      case SYSHARAKT_TYP_D_ACP :
        GetCtrlVal(panel5,SYSHARAKT_TYP_D_ACP,&Cfg.Def[num_kan-1].usrADC.T);
        LoadHardwareMap(Cfg.Def[num_kan-1].usrADC.T,num_kan-1,
                        &Adc_slot[num_kan-1],&Adc_kan[num_kan-1],NULL);
        IndikacijaParam();
      break;
      case SYSHARAKT_KN_ANAL_SIG :
        GetCtrlVal(panel5,SYSHARAKT_KN_ANAL_SIG,&Cfg.Inform[n_inf_kan].isActive);
      break;
      case SYSHARAKT_TYP_AN_PARAM :
        t_inf_kan=n_inf_kan;
        SetCtrlVal(panel5,SYSHARAKT_KN_ANAL_SIG,Cfg.Inform[t_inf_kan].isActive);
//      SetCtrlVal(panel5,SYSHARAKT_INF_N_DATCH,(n_inf_kan+1));
        SetCtrlVal(panel5,SYSHARAKT_INF_N_SLOT,Inf_slot[t_inf_kan]);
        SetCtrlVal(panel5,SYSHARAKT_INF_N_KANAL,Inf_kan[t_inf_kan]);
        GetCtrlVal(panel5,SYSHARAKT_TYP_AN_PARAM,&n_inf_kan);
        Cfg.Inform[n_inf_kan].Chl[0]=n_inf_kan+6;
        Cfg.Inform[n_inf_kan].Examine.exmCod=1; //n_inf_kan;
        SetCtrlVal(panel5,SYSHARAKT_KN_ANAL_SIG,Cfg.Inform[n_inf_kan].isActive);
        SetCtrlVal(panel5,SYSHARAKT_INF_N_DATCH,(n_inf_kan+1));
        SetCtrlVal(panel5,SYSHARAKT_INF_N_SLOT,Inf_slot[n_inf_kan]);
        SetCtrlVal(panel5,SYSHARAKT_INF_N_KANAL,Inf_kan[n_inf_kan]);
      break;
      }
    }
  else
  if(handle == panel6)
    {
    switch (id)
      {
      case PARAMEXP_LOAD_FI :
        if (FileSelectPopup ("","*.tem","Выберите имя файла",0,1,1,filename) == 1)
          {
          fl=fopen(filename,"r");
          fscanf(fl,"%d %f %ld",&nkan,&fdisk,&nr);
          fnsplit(filename,str1,str2,str3,ex);
          strcat(str3,ex);
          SetCtrlVal(panel6,PARAMEXP_IM_INP_FILE,str3);
          SetCtrlVal(panel6,PARAMEXP_F_DISK,fdisk);
          SetCtrlVal(panel6,PARAMEXP_DL_NR,nr);
          SetCtrlVal(panel6,PARAMEXP_N_KAN,nkan);
          fclose(fl);
          }
        else
          {
          MessagePopup("Файл не найден");
          break;
          }
      break;
      case PARAMEXP_LOAD_UPR :
        if (FileSelectPopup ("","*.upr","Выберите имя файла",0,1,1,filename) == 1)
          {
          fl=fopen(filename,"r");
          fscanf(fl,"%s %s %s",msg,msg,msg);
          fscanf(fl,"%s %s %ld",msg,msg,&nr);
          fscanf(fl,"%s %s %f",msg,msg,&fdisk);
          fscanf(fl,"%s %s %d",msg,msg,&nkan);
          fnsplit(filename,str1,str2,str3,ex);
          strcat(str3,ex);
          SetCtrlVal(panel6,PARAMEXP_IM_INP_FILE,str3);
          SetCtrlVal(panel6,PARAMEXP_F_DISK,fdisk);
          SetCtrlVal(panel6,PARAMEXP_DL_NR,nr);
          SetCtrlVal(panel6,PARAMEXP_N_KAN,nkan);
          fclose(fl);
          }
        else
          {
          MessagePopup("Файл не найден");
          break;
          }
      break;
      case PARAMEXP_PUSK_SP :
        {
        int nq;
        tek_num_kan=num_kan-1;
        ZapisParam();
        IndikacijaKnopok();
        IndikacijaParam();
        DisplayPanel(panel7);
        //--- Запуск программы спектра
        nq=Spektr(panel6);
        HidePanel(panel7);
        DisplayPanel(panel6);
        //--- Вывод на графику
        DeletePlots(panel6,PARAMEXP_GGRAPH_SP);
        if(S1) PlotXY(panel6,PARAMEXP_GGRAPH_SP,S,S1,nq,4,4,0,0,1,0);
        if(S2) PlotXY(panel6,PARAMEXP_GGRAPH_SP,S,S2,nq,4,4,0,0,1,15);
        }
      break;
      }
    }
  else
  if(handle == panel9)
    {
    switch (id)
      {
      case MODELOFF_ZAP_GENER : //Запуск генератора
        tek_num_kan=num_kan-1;
        ZapisParam();
        saveConfig("system.ini");                // PID Mode CfgFile
        DrvDeinitialize();
        exit(6);

      break;
      case MODELOFF_RASCHET : //Запуск основного расчета
        tek_num_kan=num_kan-1;
        ZapisParam();
        saveConfig("system.ini");                // PID Mode CfgFile
        DrvDeinitialize();
        exit(5);
      break;
      }
    }
    //Конец if(handle == menu_bar)
  } //Конец <while (isLoop)>
 isLoop=1;
 // графики
// CloseInterfaceManager();
// if(!swap_ison()) swap_on();
// spawnl(P_WAIT,"ada_graf.ovl","ada_graf.ovl","123",grfMode,NULL);
 {
 int n=atoi(grfMode)+50;
 DrvDeinitialize();
 exit(n);
 }

 } //Конец <while (TRUE)>
} //Конец main()

//--------------- Дополнительные функции -----------

//Функция обновления канальных параметров структуры
void ZapisParam()
{
int i;
GetCtrlVal(panel1,KANAL_PASS_UROWEN,&pr1);
Cfg.Def[tek_num_kan].PassiveVol=pr1;
GetCtrlVal(panel1,KANAL_RAB_UROWEN,&pr1);
Cfg.Def[tek_num_kan].StartVol=pr1;
GetCtrlVal(panel1,KANAL_F_SIN_P_PR,&pr1);
Cfg.Def[tek_num_kan].begFreq=pr1;
GetCtrlVal(panel1,KANAL_A_SIN_P_PR,&pr1);
Cfg.Def[tek_num_kan].begAmpl=pr1;
GetCtrlVal(panel1,KANAL_KOEF_ETAL,&pr1);
Cfg.Def[tek_num_kan].begZoom=pr1;
GetCtrlVal(panel1,KANAL_ZA_STAT_MIN,&pr1);
Cfg.Def[tek_num_kan].MinStatic=pr1;
GetCtrlVal(panel1,KANAL_ZA_STAT_MAX,&pr1);
Cfg.Def[tek_num_kan].MaxStatic=pr1;
GetCtrlVal(panel1,KANAL_Z_PR_SKOR,&pr1);
Cfg.Def[tek_num_kan].MaxSpeed=pr1;
GetCtrlVal(panel1,KANAL_Z_PR_USKOR,&pr1);
Cfg.Def[tek_num_kan].MaxAcceleration=pr1;
GetCtrlVal(panel1,KANAL_WID_POGR,&Cfg.Def[tek_num_kan].mstCod);
GetCtrlVal(panel1,KANAL_MAX_POGR,&pr1);
Cfg.Def[tek_num_kan].mstMax=pr1;
GetCtrlVal(panel1,KANAL_WR_AW_RAB,&pr1);
Cfg.Def[tek_num_kan].mstTime=pr1;
GetCtrlVal(panel1,KANAL_T_ZAD_POGR,&pr1);
Cfg.Def[tek_num_kan].mstMargin=pr1;
for(i=0;i<MAX_CHANNEL;i++)
  strcpy(Cfg.Def[i].file_name,Cfg.ifile_name);
GetCtrlVal(panel1,KANAL_STAT_ETALON,&pr1);
Cfg.Def[tek_num_kan].file_st=pr1;
GetCtrlVal(panel1,KANAL_KPER_ETALON,&pr1);
Cfg.Def[tek_num_kan].file_k=pr1;
//SetCtrlVal(panel1,KANAL_TYP_MODEL,Cfg.Def[num_kan-1].mdlCod);
//SetCtrlVal(panel1,KANAL_TYP_UPR,Cfg.Def[num_kan-1].mdlType);
GetCtrlVal(panel1,KANAL_PORJADOK,&Cfg.Def[tek_num_kan].p);
GetCtrlVal(panel1,KANAL_ZAPAZD,&Cfg.Def[tek_num_kan].q);
GetCtrlVal(panel1,KANAL_REGULARIZ,&Cfg.Def[tek_num_kan].Q);
//GetCtrlVal(panel1,KANAL_WID_TREND,Cfg.Def[num_kan-1].trCod);
GetCtrlVal(panel1,KANAL_POR_TREND,&Cfg.Def[tek_num_kan].trP);
GetCtrlVal(panel1,KANAL_POR_TR_IZ,&Cfg.Def[tek_num_kan].delTr);
GetCtrlVal(panel1,KANAL_DL_TR_IZ,&Cfg.Def[tek_num_kan].delTr_size);
GetCtrlVal(panel1,KANAL_POS_TR_IZ,&pr1);
Cfg.Def[tek_num_kan].delTr_vol=pr1;
GetCtrlVal(panel1,KANAL_SKOR_TR_IZ,&pr1);
Cfg.Def[tek_num_kan].delTr_speed=pr1;
//GetCtrlVal(panel1,KANAL_WID_ET_SIG,Cfg.Def[num_kan-1].tplCod);
GetCtrlVal(panel1,KANAL_GAR_AMP1,&pr1);
Cfg.Def[tek_num_kan].a[0]=pr1;
GetCtrlVal(panel1,KANAL_GAR_AMP2,&pr1);
Cfg.Def[tek_num_kan].a[1]=pr1;
GetCtrlVal(panel1,KANAL_GAR_AMP3,&pr1);
Cfg.Def[tek_num_kan].a[2]=pr1;
GetCtrlVal(panel1,KANAL_GAR_CH1,&pr1);
Cfg.Def[tek_num_kan].f[0]=pr1;
GetCtrlVal(panel1,KANAL_GAR_CH2,&pr1);
Cfg.Def[tek_num_kan].f[1]=pr1;
GetCtrlVal(panel1,KANAL_GAR_CH3,&pr1);
Cfg.Def[tek_num_kan].f[2]=pr1;
GetCtrlVal(panel1,KANAL_GAR_FA1,&pr1);
Cfg.Def[tek_num_kan].fa[0]=pr1;
GetCtrlVal(panel1,KANAL_GAR_FA2,&pr1);
Cfg.Def[tek_num_kan].fa[1]=pr1;
GetCtrlVal(panel1,KANAL_GAR_FA3,&pr1);
Cfg.Def[tek_num_kan].fa[2]=pr1;
GetCtrlVal(panel1,KANAL_STAT_GARM,&pr1);
Cfg.Def[tek_num_kan].u=pr1;

//Для панели характеристик каналов
//Статические
GetCtrlVal(panel2,STA_N_STAT,&Cfg.Sth[tek_num_kan].Nq);
GetCtrlVal(panel2,STA_T_PERPR,&pr1);
Cfg.Sth[tek_num_kan].Wait=pr1;
GetCtrlVal(panel2,STA_MIN_STATIC,&pr1);
Cfg.Sth[tek_num_kan].Lo=pr1;
GetCtrlVal(panel2,STA_MAX_STATIC,&pr1);
Cfg.Sth[tek_num_kan].Hi=pr1;
GetCtrlVal(panel2,STA_MAX_SKOR_STAT,&pr1);
Cfg.Sth[tek_num_kan].HiSpeed=pr1;
GetCtrlVal(panel2,STA_MAX_USKOR_STAT,&pr1);
Cfg.Sth[tek_num_kan].HiAcceleration=pr1;
//Частотные
GetCtrlVal(panel8,ACH_N_ACH,&Cfg.Ach[tek_num_kan].Nq);
GetCtrlVal(panel8,ACH_MIN_FRE,&pr1);
Cfg.Ach[tek_num_kan].FreqMin=pr1;
GetCtrlVal(panel8,ACH_MAX_FRE,&pr1);
Cfg.Ach[tek_num_kan].FreqMax=pr1;
GetCtrlVal(panel8,ACH_N_PERIOD,&Cfg.Ach[tek_num_kan].Periods);
GetCtrlVal(panel8,ACH_AMP_ACH,&pr1);
Cfg.Ach[tek_num_kan].Ampl=pr1;
GetCtrlVal(panel8,ACH_STAT_ACH,&pr1);
Cfg.Ach[tek_num_kan].Const=pr1;

//Для панели системных параметров
GetCtrlVal(panel5,SYSHARAKT_F_DISK,&pr1);
Cfg.Freq=pr1;
GetCtrlVal(panel5,SYSHARAKT_DL_NR,&Cfg.nr);
GetCtrlVal(panel5,SYSHARAKT_VIEW_DL_NR,&Cfg.nr_view);
GetCtrlVal(panel5,SYSHARAKT_N_CIKL,&Cfg.cycle);
GetCtrlVal(panel5,SYSHARAKT_T_WYH_N_T,&pr1);
Cfg.iniTime=pr1;
GetCtrlVal(panel5,SYSHARAKT_NK_UPR,&Cfg.nk);
//if(Cfg.nk>ChannelMode)
Cfg.nk=ChannelMode;
GetCtrlVal(panel5,SYSHARAKT_NK_INF,&Cfg.nkInfo);
//GetCtrlVal(panel5,SYSHARAKT_N_DI_IN,&Cfg.nkIN);
//GetCtrlVal(panel5,SYSHARAKT_N_DI_OUT,&Cfg.nkOUT);
GetCtrlVal(panel5,SYSHARAKT_N_SLOT,&Dac_slot[tek_num_kan]);
GetCtrlVal(panel5,SYSHARAKT_N_KONT,&Dac_kan[tek_num_kan]);
GetCtrlVal(panel5,SYSHARAKT_ACP_SLOT,&Adc_slot[tek_num_kan]);
GetCtrlVal(panel5,SYSHARAKT_ACP_KONT,&Adc_kan[tek_num_kan]);
//Для панели без модели
GetCtrlVal(panel9,MODELOFF_APP_Q,&Cfg.NMDL.Def[num_kan-1].q);
GetCtrlVal(panel9,MODELOFF_GAM_DOP,&pr1);
Cfg.NMDL.Def[num_kan-1].Mistake=pr1;
GetCtrlVal(panel9,MODELOFF_N_ITERAC,&Cfg.NMDL.Cycles);
GetCtrlVal(panel9,MODELOFF_PAKET_REG,&Cfg.NMDL.isBatch);
GetCtrlVal(panel9,MODELOFF_RAZM_OKNA,&Cfg.NMDL.Def[tek_num_kan].nr);
GetCtrlVal(panel9,MODELOFF_NACH_OTCH,&Cfg.NMDL.Def[tek_num_kan].Margin);
GetCtrlVal(panel9,MODELOFF_N_RASCH_T,&Cfg.NMDL.Def[tek_num_kan].nq);

//Установка отображения логического канала на физический
prizn_kan.Unit=0; prizn_kan.Slot=Dac_slot[num_kan-1];
prizn_kan.Channel=Dac_kan[num_kan-1];
if(DrvIsPresent()) DrvSetupDAC(num_kan-1,&prizn_kan);
prizn_kan.Unit=0; prizn_kan.Slot=Adc_slot[num_kan-1];
prizn_kan.Channel=Adc_kan[num_kan-1];
if(DrvIsPresent()) DrvSetupADC(num_kan-1,&prizn_kan);

}


Spektr(int panel) //Функция для вычисления спектра
{
 char fname[80]="test.tem",tmp[40],tmp1[40];
 double freq,fn,fw,lam;
 float *buf1=NULL,*buf2=NULL,val,val1,val2;
 int nq=0,act,nr,nk,l,i;
 FILE *f;

 GetCtrlVal(panel,PARAMEXP_IM_INP_FILE,&fname);
 GetCtrlVal(panel,PARAMEXP_F_DISK,&freq);
 GetCtrlVal(panel,PARAMEXP_N_SP,&nq);
 GetCtrlVal(panel,PARAMEXP_FMIN,&fn);
 GetCtrlVal(panel,PARAMEXP_FMAX,&fw);
 GetCtrlVal(panel,PARAMEXP_PAR_FILTR,&lam);
 GetCtrlVal(panel,PARAMEXP_NUM_KAN,&act);

 if(S)  free(S);  S=NULL;
 if(S1) free(S1); S1=NULL;
 if(S2) free(S2); S2=NULL;

 f=fopen(fname,"r+t");
 if(!f) { MessagePopup("Неверный файл исходных данных"); goto done; }
 fscanf(f,"%s",tmp);
 if(!strcmp(tmp,"+++")) // ------- *.upr
   {
   fscanf(f,"%s %s",tmp,tmp1);
   fscanf(f,"%s %s %d",tmp,tmp1,&nr);
   fscanf(f,"%s %s %lf",tmp,tmp1,&freq);
   fscanf(f,"%s %s %d",tmp,tmp1,&nk);
   buf1=calloc(nr,sizeof(float));
   buf2=calloc(nr,sizeof(float));
   S=calloc(nq,sizeof(double));  if(S) for(i=0;i<nq;i++)  S[i]=i;
   S1=calloc(nq,sizeof(double)); if(S1) for(i=0;i<nq;i++) S1[i]=0;
   S2=calloc(nq,sizeof(double)); if(S2) for(i=0;i<nq;i++) S2[i]=0;
   if(!buf1 || !buf2 || !S1 || !S2 || !S)
        { MessagePopup("Нет памяти для исходных данных"); return 1; }
   for(l=0;l<nr;l++)
     {
     fscanf(f,"%s",tmp);
     for(i=1;i<=nk;i++)
       {
       fscanf(f,"%f %f %f",&val,&val1,&val2);
       if(i==act) { buf1[l]=val2; buf2[l]=val1; }
       }
     }
   }
 else // ---------- *.tem
   {
   nk=atoi(tmp);
   fscanf(f,"%f %d",&freq,&nr);
   buf1=calloc(nr,sizeof(float));
   S=calloc(nq,sizeof(double));  if(S)  for(i=0;i<nq;i++) S[i]=i;
   S1=calloc(nq,sizeof(double)); if(S1) for(i=0;i<nq;i++) S1[i]=0;
   if(!buf1 || !S1 || !S)
       { MessagePopup("Нет памяти для исходных данных"); goto done; }
   for(l=0;l<nr;l++)
       for(i=1;i<=nk;i++) { fscanf(f,"%f",&val); if(i==act) buf1[l]=val; }
   }
 fclose(f);

 CalcSpectr(fn,fw,nq,S1,S,freq,lam,nr,buf1);
 if(buf2) CalcSpectr(fn,fw,nq,S2,S,freq,lam,nr,buf2);

done:
 if(buf1) free(buf1);
 if(buf2) free(buf2);
 return nq;
}

//-----------Конец файла--------------

