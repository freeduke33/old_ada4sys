
emulate: { ==========> LoopBack mode }

        I0=^dac_buf; M0=1; L0=0;
        I1=^adc_buf; M1=1; L1=0;

        CNTR=NUM_ADC;
        do move_loop until CE;
           AR=DM(I0,M0);
           DM(I1,M1)=AR;
move_loop: NOP;
        rts;
