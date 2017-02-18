.module upi_bios

{ адреса регистров, отображенных на память  процессора ADSP-21XX        }
.const  Sys_Ctrl_Reg=               0x3fff;
.const  Dm_Wait_Reg=                0x3ffe;
.const  Tperiod_Reg=                0x3ffd;
.const  Tcount_Reg=                 0x3ffc;
.const  Tscale_Reg=                 0x3ffb;
.const  Sport1_Ctrl_Reg=            0x3ff2;
.const  Sport1_Sclkdiv=             0x3ff1;
.const  Sport1_Rfsdiv=              0x3ff0;
.const  Sport1_Autobuf_Ctrl=        0x3fef;

.CONST NUM_DAC=16;
.CONST NUM_ADC=16;

.CONST DATA_PORT=  0x3400;
.CONST DAC_PORT=   0x3001;
.CONST START_PORT= 0x3401;
.CONST COMM_PORT=  0x3402;
.CONST ADC_PORT=   0x3402;
.CONST SEG_PORT=   0x3003;
.CONST FLAGS_PORT= 0x3403;
.CONST TTL_DSP_IN= 0x3403;
.CONST TTL_DSP_OUT=0x3004;
.CONST IRQ_PORT=   0x3005;
.CONST DAC1_PORT=  0x3006;
.CONST DAC2_PORT=  0x3007;

{=============================================================};
.VAR/SEG=dseg/DM/RAM  skipTik;
.VAR/SEG=dseg/DM/RAM  Act_DAC;
.VAR/SEG=dseg/DM/RAM  Act_ADC;
.VAR/SEG=dseg/DM/RAM  DoIt;
.VAR/SEG=dseg/DM/RAM  SetUpDAC;
.VAR/SEG=dseg/DM/RAM  CHL;
.VAR/SEG=dseg/DM/RAM  LoopBack;
.VAR/SEG=dseg/DM/RAM  dac_buf[NUM_DAC+1];
.VAR/SEG=dseg/DM/RAM  adc_buf[NUM_ADC+1];
.VAR/SEG=dseg/DM/RAM  adc_target[NUM_ADC+1];  { крейт + модуль + канал }
.VAR/SEG=dseg/DM/RAM  dac_target0[NUM_DAC+1]; { крейт + модуль }
.VAR/SEG=dseg/DM/RAM  dac_target1[NUM_DAC+1]; { канал }

.macro StartAd;
 AY1=DM(START_PORT);
.endmacro;


{============================= code section }
JUMP do_rst;   NOP; NOP; NOP; { RESET }
JUMP from_ibm; NOP; NOP; NOP; { IRQ2 }
RTI;           NOP; NOP; NOP; { SPORT0 TRANSMIT }
JUMP sport;    NOP; NOP; NOP; { SPORT0 RECEIVE }
RTI;           NOP; NOP; NOP; { SPORT1 TRANSMIT, IRQ1}
RTI;           NOP; NOP; NOP; { SPORT1 RECEIVE,  IRQ0}
JUMP do_timer; NOP; NOP; NOP; { TIMER }


{===================== RESET ========================}
.include <reset.inc>;
{ Вечный цикл  }
FIdle:
        IDLE;
        JUMP    FIdle;

{===================== RECEIVE FROM IBM ========================}
.include <command.inc>;
.include <function.inc>;

{===================== from ADC by SPORT IRQ ========================}
.include <adc.inc>;

{===================== TIMER ========================}
.include <timer.inc>;

{===================== SEND buffer to DAC ========================}
.include <dac.inc>;

{===================== Digital part ========================}
.include <digit.inc>;

{===================== LoopBack Emulator ========================}
.include <emulate.inc>;

{===================== SETUP all subsystems ========================}
setup_DAC_ADC:NOP;
         AR=0;
         DM(DoIt)=AR;

         AR=NUM_ADC;
         DM(Act_ADC)=AR;
         AR=NUM_DAC;
         DM(Act_DAC)=AR;

         AR=0x10;
         rts;


{@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@}
{@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@}
{                                                                       }
{                        Internal routines                              }
{                                                                       }
{@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@}
{@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@}


getData: AY0=2;
lb2:     AX0=DM(FLAGS_PORT);
         AR=AX0 AND AY0;
         if NE jump lb2;
         AR=DM(DATA_PORT);
         rts;

putData: AY0=1;
         DM(DATA_PORT)=AR;
lb1:     AX0=DM(FLAGS_PORT);
         AR=AX0 AND AY0;
         if NE jump lb1;
         rts;


InitIMRegisters:
        L0=0;
        L1=0;
        L2=0;
        L3=0;
        L5=0;
        L6=0;
        L7=0;

        M0=1;
        M1=1;
        M2=1;
        M3=1;
        M5=1;
        M6=1;
        M7=0;
        RTS;

.ENDMOD;
