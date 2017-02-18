

Exchange:{ DM(Sys_Ctrl_Reg)=MR0;   }      { выключим Sport                      }
        DM(CHL)=AR;

        AR=DM(LoopBack);
        AF=PASS AR;
        AR=AF-1;
        IF EQ CALL Emulate;

        { -----------> прерываем IBM для обмена данными }

        DM(IRQ_PORT)=AR;

        call getData;
        AF=PASS AR;
        AR=AR OR AF;
        IF EQ JUMP None;

        { -----------> передаем на IBM }
         I0=^adc_buf;
         M0=1;
         CNTR=NUM_ADC;
         do send_loop until CE;
           AR=DM(I0,M0);
send_loop: call putData;

        { -----------> принимаем из IBM }
         I0=^dac_buf;
         M0=1;
         CNTR=NUM_DAC;
         do receive_loop until CE;
             call getData;
receive_loop:DM(I0,M0)=AR;

None:    rts;

