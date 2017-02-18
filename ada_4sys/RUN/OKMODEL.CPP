// ===============================[ Модели прогноза
#define Uses_mdlTest

#define Uses_mdl_0TR031095
#define Uses_mdl_1TR031095

#define Uses_mdl_TR270395
#define Uses_mdl_270395
#define Uses_gen_TR270395
#define Uses_gen_270395

#define Uses_mdl_ARSS
#define Uses_mdl_SS

#define Uses_arss_qTS270392
#define Uses_arss_q1TS270392
#define Uses_ss_qTS270392
#define Uses_ss_q1TS270392
#define Uses_garss_qTS270392
#define Uses_garss_q1TS270392
#define Uses_gss_qTS270392
#define Uses_gss_q1TS270392

#define Uses_mdl_arss211196

#define Uses_gen_Volume
#define Uses_gen_Sinus
#define Uses_gen_Template

// ===============================[ Внешний тренд к АР-СС и СС моделям
#define Uses_etr_PL141296
#define Uses_etr_EQ141296

#include <adaptive.hpp>

#include "config.h"
#include "cfgloop.hpp"
#include "okmodel.hpp"
#include "common.h"

CfgLoopAda::CfgLoopAda(CfgT *Cfg,LoopSC *l) : CfgLoop(Cfg,l)
{
 typedef FModel_1x1 _HUGE *mdlP;

 for(int i=0;i<Cfg->nk;i++)
   {
   tr[i]=NULL;
   m[i]=new mdlP[3];
   if(!m[i]) goto done;
   for(int k=0;k<3;k++) m[i][k]=NULL;
   }

done:
 return;
}

CfgLoopAda::~CfgLoopAda(void)
{
 for(int i=0;i<Cfg->nk;i++) if(tr[i]) delete tr[i];
 return;
}

int CfgLoopAda::PrepareModels(void)
{
 int i,val=ERR_NOMEM,tmp,isARSS;

#define D Cfg->Def[i]

 for(i=0;i<Cfg->nk;i++)
   {
   if(!m[i]) goto done;

   tmp = D.isActive ? D.mdlCod : mc_PASSIVE;

   switch(tmp)
     {
     case mc_NONE:    m[i][0]= new mdlTest_1x1; break;
     case mc_PASSIVE: m[i][0]= new gen_Volume(D.PassiveVol/5.0); break;
     default:
        switch(D.begCod)
          {
          case 0: m[i][0]=new gen_Sinus(D.begAmpl/5.0,D.begFreq,0,1.0/Cfg->Freq,D.StartVol/5.0); break;
          case 1: m[i][0]=new gen_Template(D.begZoom); break;
          case 2: val=ERR_START; break;
          }
     }
   if(!m[i][0]) goto done;

   isARSS=1;

   switch(tmp)
     {
     case mc_NONE:          m[i][1]=new mdlTest_1x1; isARSS=0; break;
     case mc_PASSIVE:       m[i][1]=new gen_Volume(D.PassiveVol/5.0); isARSS=0; break;
     case mc_gen_270395:    m[i][1]=new gen_270395(D.mdlType,D.p,D.q,D.Q); break;
     case mc_gen_TR270395:  m[i][1]=new gen_TR270395(D.mdlType,D.p,D.q,D.Q,D.trP); break;
     case mc_mdl_270395:    m[i][1]=new mdl_270395(D.mdlType,D.p,D.q,D.Q); break;
     case mc_mdl_TR270395:  m[i][1]=new mdl_TR270395(D.mdlType,D.p,D.q,D.Q,D.trP);        break;
     case mc_mdl_TR031095:
              if(D.trP==0) m[i][1]=new mdl_0TR031095(D.mdlType,D.p,D.Q,D.q);
              else         m[i][1]=new mdl_1TR031095(D.mdlType,D.p,D.Q,D.q);        break;
     case mc_gen_ARSS_TS:
              if(D.trP==0) m[i][1]=new garss_qTS270392(D.p,D.Q,D.q,D.mdlType);
              else         m[i][1]=new garss_q1TS270392(D.p,D.Q,D.q,D.mdlType);        break;
     case mc_gen_SS_TS:
              if(D.trP==0) m[i][1]=new gss_qTS270392(D.p,D.Q,D.q,D.mdlType);
              else         m[i][1]=new gss_q1TS270392(D.p,D.Q,D.q,D.mdlType);        break;
     case mc_mdl_ARSS_TS:
              if(D.trP==0) m[i][1]=new arss_qTS270392(D.p,D.Q,D.q,D.mdlType);
              else         m[i][1]=new arss_q1TS270392(D.p,D.Q,D.q,D.mdlType);        break;
     case mc_mdl_SS_TS:
              if(D.trP==0) m[i][1]=new ss_qTS270392(D.p,D.Q,D.q,D.mdlType);
              else         m[i][1]=new ss_q1TS270392(D.p,D.Q,D.q,D.mdlType);        break;
     case mc_mdl_ARSS_1K:  m[i][1]=new mdl_ARSS(D.p,D.Q,D.q,D.trP);        break;
     case mc_mdl_SS_1K:    m[i][1]=new mdl_SS(D.p,D.Q,D.q,D.trP);        break;
     case mc_mdl_ARSS_2:   m[i][1]=new mdl_arss211196(D.mdlType,D.p,D.Q,D.q);  break;
     default: val=ERR_MODEL; goto done;
     }
  if(!m[i][1]) goto done;
  }

 val=0;
done:
 return val;
}


