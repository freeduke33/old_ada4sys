
send_dac:{===============> }
         I0=^dac_buf;     M0=1; L0=0;
         I1=TTL_DSP_OUT;  M1=0; L1=0;
         I2=^dac_target0; M2=1; L2=0; { �३� � ����� }
         I3=^dac_target1; M3=1; L3=0; { NN ������ }
         CNTR=NUM_DAC;
         AR=CNTR; AF=PASS AR;
         do dac_loop until CE;
           AR=DM(I0,M0); { �஢��� � ������ }
           SR=LSHIFT AR BY 4 (Lo);
           AR=DM(I3,M3); { NN ������ }
           AF=PASS AR;
           AR=SR0 or AF;
           SR0=AR;

           AY1=0x0; { ��� ��⠭���� }
           AR=CNTR; AY0=1;
           AR=AR-AY0;
           if EQ JUMP putDAC;
           AY1=0x80; { � ��⠭����� }

{===========================================================================}
putDAC:    {==========>}

           AR=DM(I2,M2);  { ���� + �३� }
           AR=AR or AY1;
{           AR=0x80; }
setDAC_TTL:
           DM(COMM_PORT)=AR;
{           SR0=0x0FF8;}

           I1=TTL_DSP_OUT;  M1=0; L1=0;
           AR=6; AY0=6; AY1=0x2; SE=1;

           CNTR=17;
           DO OutDac UNTIL CE;
              NOP;
              DM(I1,M1)=AR, AR=AR XOR AY1;
              DM(I1,M1)=AR, AR=AR XOR AY1;
              NOP;
              DM(I1,M1)=AR, SR=LSHIFT SR0 (LO);
OutDac:       AR=SR1 OR AY0;

        { ᣥ����㥬 ��஡ �����                                      }
        DM(I1,M1)=AY1;          { ᣥ����㥬 ��஡ �� ��� Write       }
        AR=6;                   { ��ᨬ ��஡                         }
        DM(I1,M1)=AR;           { ᣥ����㥬 ��஡ �� ��� Write       }

{===========================================================================}
dac_loop:  NOP;
         rts;



