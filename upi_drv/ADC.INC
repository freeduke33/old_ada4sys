receive_adc: {------}

        MR0=0x0400;             { выключение сериального порта          }
        MR1=0x0C00;             { включение сериального порта           }
        I0=^adc_buf;    M0=1; L0=0;
        I1=^adc_target; M1=1; L1=NUM_ADC;

        CNTR=NUM_ADC;
        do adc1 until CE;
           StartAd;
           call waitConv;
           AR=DM(ADC_PORT);
           AY1=2048;
           AR=AR+AY1;
           DM(I0,M0)=AR;
adc1:      NOP;
        rts;

waitConv: CNTR=33;
           do Conv until CE;
Conv:      NOP;
          rts;



waitChl:  AR=1;
          DM(Sys_Ctrl_Reg)=MR1;
wait1:
          IDLE;
          AF=PASS AR; AR=AF-1;
          IF EQ JUMP wait1;
          RTS;

sport: AR=0;
          DM(Sys_Ctrl_Reg)=MR0;   { выключим SPORT                        }
          RTI;





