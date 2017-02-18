
f_putDIGIT:
            call getData;  { select channel }
            I0 = AR;
            call getData;  { data }
            SR0 = AR;
            AR = I0;
            CALL SetDAC_TTL;
            RTI;

f_getDIGIT: call getData;    { select channel }
            DM(COMM_PORT)=AR;

            I5=TTL_DSP_IN;   M5=0; L5=0;
            I6=TTL_DSP_OUT;  M6=0; L6=0;
            si = 0x0C8;
            ax0 = 6;
            mr0 = 4;
            sr0 = 0;
            dm(I6,M6) = ax0; { clock & write = 1 }
            call digWAIT;
            dm(I6,M6) = mr0; { clock = 0 }
            call digWAIT;
            dm(I6,M6) = ax0; { clock & write = 1 }
            call digWAIT;
            dm(I6,M6) = sr0; { clock & write = 0 }


            sr0 = 0;
            ay0 = 0x0004;

            CNTR = 0x0010;
            do f_digitLoop until ce;
                 cntr = 0x0003;
                 do getDigWait until ce;
                   NOP;
                   NOP;
getDigWait:        NOP;
                 ar = dm(i5,m5), sr = lshift sr0 (lo);
                 NOP;
                 dm(i6,m6) = ax1, ar = ar and ay0;
                 sr = sr or lshift ar by -3 (hi);
                 NOP;
                 NOP;
f_digitLoop:     DM(I6,M6)=ay1;

             AR=sr0;
             AR=33;
             call putData;

             RTI;

DigWAIT: cntr = si;
         do DigWait1 until ce;
DigWait1:  nop;
         rts;
