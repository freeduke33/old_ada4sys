Emul:   CALL Emulate;
        JUMP Exchg;

do_timer:{ENA    SEC_REG;}

        AR=DM(LoopBack);
        AF=PASS AR; AR=AF-1;
        IF EQ JUMP Emul;

        call receive_adc;

Exchg:  { -----------> ���뢠�� IBM ��� ������ ����묨 }

        DM(IRQ_PORT)=AR;

        call getData;
        AF=PASS AR; AR=AR OR AF;
        IF EQ  JUMP None;

        { -----------> ��।��� �� IBM }
         I0=^adc_buf; M0=1; L0=0;
         CNTR=NUM_ADC;
         do send_loop until CE;
           AR=DM(I0,M0);
           call putData;
send_loop: NOP;
        { -----------> �ਭ����� �� IBM }
        I0=^dac_buf; M0=1; L0=0;
        CNTR=NUM_DAC;
        do receive_loop until CE;
             call getData;
             DM(I0,M0)=AR;
receive_loop:NOP;

        call send_dac;

NONE:   {DIS SEC_REG;}
        RTI;

