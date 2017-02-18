#ifdef __386__
#  define _HUGE
#else
#  define _HUGE huge
#endif

#include <stdlib.h>

// =======================================================================

#ifdef Uses_ant_mdl020296
# define Uses_mdl_Anatomy
# define INC_020296
#endif
#ifdef Uses_ant_stat020296
# define Uses_mdl_Anatomy
# define INC_020296
#endif
#ifdef Uses_ant_dyn020296
# define Uses_mdl_Anatomy
# define INC_020296
#endif

// =======================================================================
#ifdef Uses_gss_qTS270392
#  define Uses_ss_qTS270392
#endif
#ifdef Uses_gss_q1TS270392
#  define Uses_ss_q1TS270392
#endif
#ifdef Uses_ss_1TS270392
#  define Uses_ss_qTS270392
#endif
#ifdef Uses_ss_q1TS270392
#  define Uses_ss_qTS270392
#endif
#ifdef Uses_ss_qTS270392
#  define Uses_arss_qTS270392
#endif


#ifdef Uses_garss_qTS270392
#  define Uses_arss_qTS270392
#endif
#ifdef Uses_garss_q1TS270392
#  define Uses_arss_q1TS270392
#endif
#ifdef Uses_arss_1TS270392
#  define Uses_arss_qTS270392
#endif
#ifdef Uses_arss_q1TS270392
#  define Uses_arss_qTS270392
#endif
#ifdef Uses_arss_qTS270392
#  define INC_270392
#  define Uses_TemplateARSS_1x1
#endif

// =======================================================================

#ifdef Uses_mdl_1TR031095
#  define Uses_mdl_0TR031095
#endif
#ifdef Uses_mdl_0TR031095
#  define INC_031095
#  define Uses_TemplateARSS_1x1
#endif

// =======================================================================

#ifdef Uses_mdl_arss211196
#  define INC_211196
#  define Uses_TemplateARSS_1x1
#endif

// =======================================================================

#ifdef Uses_gen_TR270395
#  define Uses_mdl_TR270395
#endif
#ifdef Uses_mdl_TR270395
#  define Uses_mdl_270395
#endif
#ifdef Uses_gen_270395
#  define Uses_mdl_270395
#endif
#ifdef Uses_mdl_270395
#  define INC_270395
#  define Uses_TemplateARSS_1x1
#endif

#ifdef Uses_delTr0_270395
#  define Uses_delTrend_270395
#endif
#ifdef Uses_delTr1_270395
#  define Uses_delTrend_270395
#endif
#ifdef Uses_delTrend_270395
#  define INC_270395
#  define Uses_Defence
#endif

// =======================================================================

#ifdef Uses_mdl_SS
#  define Uses_mdl_ARSS
#endif
#ifdef Uses_mdl_ARSS
#  define INC_OLDMODEL
#  define Uses_TemplateARSS_1x1
#endif

// =======================================================================

#ifdef Uses_TemplateARSS_1x1
#  define Uses_FModel_1x1
#  define Uses_ExtTrendARSS_1x1
#endif

#ifdef Uses_mdlTest
#  define Uses_FModel_1x1
#endif

// =======================================================================

#ifdef Uses_gen_StaticBuf
#  define Uses_FModel_1x1
#  define INC_GENERATE
#endif
#ifdef Uses_gen_Sinus
#  define Uses_FModel_1x1
#  define INC_GENERATE
#endif
#ifdef Uses_gen_PSinus
#  define Uses_FModel_1x1
#  define INC_GENERATE
#endif
#ifdef Uses_gen_Const
#  define Uses_FModel_1x1
#  define INC_GENERATE
#endif
#ifdef Uses_gen_DatFile
#  define Uses_gen_vBuffer
#endif
#ifdef Uses_gen_PrgFile
#  define Uses_gen_vBuffer
#endif
#ifdef Uses_gen_vBuffer
#  define Uses_FModel_1x1
#  define INC_GENERATE
#endif
#ifdef Uses_gen_Template
#  define Uses_FModel_1x1
#  define INC_GENERATE
#endif

// =======================================================================
#ifdef Uses_mst_Square
#define Uses_Mistake_1x1
#endif
#ifdef Uses_mst_MathWait
#define Uses_Mistake_1x1
#endif

// =======================================================================

#ifdef Uses_mdl_Anatomy
#define Uses_Container_1x1
#define INC_ANATOMY
#endif

// =======================================================================

#ifdef Uses_exm_StatAccelerate_1x1
#define Uses_exm_StatSpeed_1x1
#endif
#ifdef Uses_exm_StatSpeed_1x1
#define Uses_exm_Static_1x1
#endif
#ifdef Uses_exm_Static_1x1
#define Uses_FModel_1x1
#define INC_EXAMINE
#endif

#ifdef Uses_exm_Achh
#define Uses_FModel_1x1
#define INC_EXAMINE
#endif

// =======================================================================

#ifdef Uses_dfnSpeed
#  define Uses_Defence
#endif
#ifdef Uses_dfnAcceleration
#  define Uses_Defence
#endif
#ifdef Uses_dfnFull
#  define Uses_Defence
#endif
// =======================================================================

#ifdef Uses_etr_PL141296
# define Uses_ExtTrendARSS_1x1
# define INC_141296
#endif
#ifdef Uses_etr_EQ141296
# define Uses_ExtTrendARSS_1x1
# define INC_141296
#endif

// =======================================================================

#ifdef Uses_AsyncLoop
#  define Uses_SyncLoop
#endif
#ifdef Uses_SyncLoop
#  define Uses_LoopSC
#endif
// =======================================================================

#ifdef Uses_MemBufPSinus
#  define Uses_MemBuf
#  define Uses_gen_PSinus
#endif
#ifdef Uses_EMSMemBuf
#  define Uses_MemBuf
#endif
#ifdef Uses_StaticMemBuf
#  define Uses_MemBuf
#endif
#ifdef Uses_MemBuf
#  define Uses_Buffer
#endif
#ifdef Uses_NilBuffer
#  define Uses_Buffer
#endif

//
// ========================================================================
//

#ifdef Uses_ExtTrendARSS_1x1
#define INC_EXTTREND
#endif


#ifdef Uses_Container_1x1
#define Uses_FModel_1x1
#endif

#ifdef Uses_FModel_1x1
#  define Uses_Model
#endif

#ifdef Uses_Defence
#  define INC_DEFENCE
#endif

#ifdef Uses_Mistake_1x1
#  define INC_MISTAKE
#endif

#ifdef Uses_Buffer
#  define INC_BUFFER
#endif

#ifdef Uses_LoopSC
#  define INC_LOOPCTRL
#endif

#ifdef Uses_Model
#  define INC_MODEL
#endif

// ==== // ==== // ==== // ==== // ==== // ==== // ==== // ==== // ==== //
#include <errors.hpp>
#include <typedefs.hpp>
#include <gauges.hpp>

#ifdef Uses_FPError

#include <setjmp.h>
extern "C" {
 void GlobalFPE(int sig);
 void MessageFPE(int sig);
 extern jmp_buf FPE_ERR;
}
#endif

#ifdef INC_EXTTREND
# include <exttrend.hpp>
#endif
#ifdef INC_MODEL
# include <model.hpp>
#endif
#ifdef INC_DEFENCE
# include <defence.hpp>
#endif
#ifdef INC_BUFFER
# include <buffer.hpp>
#endif
#ifdef INC_LOOPCTRL
# include <loop.hpp>
#endif
#ifdef INC_MISTAKE
# include <mistake.hpp>
#endif
#ifdef INC_ANATOMY
# include <anatomy.hpp>
#endif
#ifdef INC_EXAMINE
# include <examine.hpp>
#endif

//
// ========================================================================
//
#ifdef INC_OLDMODEL
# include <oldmodel.hpp>
#endif
#ifdef INC_GENERATE
# include <generate.hpp>
#endif


#ifdef INC_270392
# include <i270392.hpp>   //===[ 27 марта 1992
#endif
#ifdef INC_270395
# include <i270395.hpp>   //===[ 24 - 27 марта 1995
#endif
#ifdef INC_031095
# include <i031095.hpp>   //===[ 2 - 3 октября 1995
#endif

#ifdef INC_020296
# include <i020296.hpp>   //===[ 31 января - 2 февраля 1996
#endif
#ifdef INC_211196
# include <i211196.hpp>   //===[ 20 ноября - 21 ноября 1996
#endif
#ifdef INC_141296
# include <i141296.hpp>   //===[ 13 декабря - 14 декабря 1996
#endif


#ifdef DEBUG
#include <log.hpp>
#endif
