from_ibm: DIS SEC_REG;
             AR=DM(DATA_PORT); { command }
             AY0=1;
             AF=AR+AY0;
{ one from list below: }
{ 0 - test bios present ==> word sign_lo,sign_hi }
{ 1 - enable           <== word freq_lo,freq_hi) }
{ 2 - disable                                    }
{ 3 - get info          ==> byte num_adc,num_dac word freq_lo,freq_hi }
{ 4 - set channels     <== word num_dac,dac's_list,num_adc,adc's_list }
{ 5 - get skipped tik   ==> word skip_lo,skip_hi }
{ 6 - clear skipped                              }
{ 7 - begin loopback mode                        }
{ 8 - end loopback mode                          }
{------------------------------------------------}
{ 9  - get from digital    chl ==> data }
{ 10 - put to digital      <==  chl,data }


        AF= AF-1; I0=^f_test;
        IF EQ JUMP DONE;
        AF= AF-1; I0=^f_enable;
        IF EQ JUMP DONE;
        AF= AF-1; I0=^f_disable;
        IF EQ JUMP DONE;
        AF= AF-1; I0=^f_info;
        IF EQ JUMP DONE;
        AF= AF-1; I0=^f_set;
        IF EQ JUMP DONE;
        AF= AF-1; I0=^f_skip;
        IF EQ JUMP DONE;
        AF= AF-1; I0=^f_clr;
        IF EQ JUMP DONE;
        AF= AF-1; I0=^f_begLoop;
        IF EQ JUMP DONE;
        AF= AF-1; I0=^f_endLoop;
        IF EQ JUMP DONE;
        AF= AF-1; I0=^f_getDIGIT;
        IF EQ JUMP DONE;
        AF= AF-1; I0=^f_putDIGIT;
        IF EQ JUMP DONE;

        I0=^Fidle;

DONE:   NOP;
        { очистим стеки }
        POP STS, POP PC, POP CNTR, POP LOOP;
        POP STS, POP PC, POP CNTR, POP LOOP;
   SetNewLab2:
        POP STS, POP PC, POP CNTR, POP LOOP;
        AX1=SSTAT;
        AY0=0x55;
        AR=AX1 AND AY0;
        AX1=AR;
        AR=AX1 XOR AY0;
        IF NE JUMP SetNewLab2;  { если стеки не очистились то дальше    }

        DIS TIMER;              { выключим таймер                       }
        AR=I0;
        TOPPCSTACK=AR;         { занесем адрес основной процедуры      }
        IFC=0x23;               { сбросим прерывания                    }
        IMASK=0x23;             { разрешим прерывания                   }
        PUSH STS;               { запомним в стеке                      }
        RTI;

back:   JUMP FIdle;
        IFC=0x20;
        RTI;

