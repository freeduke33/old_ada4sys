{-------------------------------------------------}
f_test:  AR=0x66;
         call putData;
         AR=0x99;
         call putData;
         jump back;
{-------------------------------------------------}
f_enable:{ ждем пока IBM запишет данные }
         DIS  TIMER;

         call getData;
         AF=PASS AR;
         AR=AF-1;
         DM(Tscale_Reg)=AR;

         call getData;
         AF=PASS AR;
         AR=AF-1;
         DM(Tperiod_Reg)=AR;
         DM(Tcount_Reg)=AR;

         AX0=0x0400;
         DM(sys_ctrl_reg)=AX0;

         call setup_DAC_ADC;
         DM(DoIt)=AR;
         AR=0;
         call putData;

         ENA TIMER;
         jump f_clr;
{-------------------------------------------------}
f_disable: DIS TIMER;
         AR=0;
         DM(DoIt)=AR;
         jump back;
{-------------------------------------------------}
f_info:  AR=0x080F; { 8 - ADC, 16 - DAC }
         call putData;
         AR=0xFFFF;          { Tperiod_Reg = 16 bit }
         call putData;
         AR=0x00FF;          { Tscale_Reg  = 8 bit }
         call putData;
         jump back;

{-------------------------------------------------}
f_set:   { ждем пока IBM запишет данные }
         call getData;
         DM(Act_DAC)=AR;
         I0=^dac_target0; M0=1; L0=0;
         CNTR=AR;
         DO f_setLoopDAC1 UNTIL CE;
               call getData;
               DM(I0,M0)=AR;
f_setLoopDAC1: NOP;

         I0=^dac_target1;
         CNTR=DM(Act_DAC);
         DO f_setLoopDAC2 UNTIL CE;
               call getData;
               DM(I0,M0)=AR;
f_setLoopDAC2: NOP;

         call getData;
         DM(Act_ADC)=AR;
         CNTR=AR;
         I0=^adc_target; M0=1; L0=0;
         DO f_setLoopADC UNTIL CE;
              call getData;
              DM(I0,M0)=AR;
f_setLoopADC: NOP;
         jump back;

{-------------------------------------------------}
f_skip:  AR=DM(skipTik);
         call putData;
         jump back;
{-------------------------------------------------}
f_clr:     AR=0;
           DM(skipTik)=AR;
           jump back;
f_begLoop: AR=1;
           DM(LoopBack)=AR;
           jump back;
f_endLoop: AR=0;
           DM(LoopBack)=AR;
           jump back;


