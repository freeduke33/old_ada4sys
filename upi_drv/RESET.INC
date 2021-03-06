do_rst:
        DIS TIMER;

   { �⪮�䨣���㥬 ����প� �� ���饭�� � ���⠬     }
        AX0=0x1000;                     { 0000 0010  0000 0000 -> 0200H }
        DM(Dm_Wait_Reg)=AX0;            { ��⠭���� wait states         }
   { �⪮�䨣���㥬 ����஫��� ���뢠���              }
        ICNTL=0x17;                     { ���뢠��� �� �஢��          }
        IFC=0x23;                       { ����⨬ ॣ���� ���뢠���  }
        IMASK=0x23;                     { ࠧ�訬 ���뢠���           }

        AR=0; DM(LoopBack)=AR;


{============================================================> ADC }
        I0=^adc_buf; M0=1; L0=0;
        CNTR=NUM_ADC;
        AR=0;
        do l_adc until CE;
          DM(I0,M0)=AR;
l_adc:    NOP;

        I0=^adc_target; M0=1; L0=0;
        CNTR=NUM_ADC;
        AR =0xB01;
        AY1=0x010;
        do l_adc1 until CE;
          DM(I0,M0)=AR;
          AR=AR+AY1;
l_adc1:   NOP;

{============================================================> DAC}
        I0=^dac_buf; M0=1; L0=0;
        CNTR=NUM_DAC;
        AR=0;
        do l_dac until CE;
          DM(I0,M0)=AR;
l_dac:    NOP;

        I0=^dac_target0; M0=1; L0=0; { �३�+����� }
        AR=0;
        CNTR=NUM_DAC;
        do l_dac1 until CE;
          DM(I0,M0)=AR;
l_dac1:   NOP;

        I0=^dac_target1; M0=1; L0=0; { ����� }
        AR=8;
        CNTR=NUM_DAC;
        do l_dac2 until CE;
          DM(I0,M0)=AR;
          AF=PASS AR;
          AR=AF+1;
l_dac2:   NOP;

{============================================================}
        AX0=DM(DATA_PORT);              { ����⨬ ॣ���� �⥭�� �� PC }

   { �⪮�䨣���㥬 �ਠ��� ����                                      }
   { (����� �ᯮ������ ��� �ନ஢���� ����প� �� �६�             }
   {  �८�ࠧ������ ���)                                                 }
        { ����७���, ��������, ����ୠ⨢�� FRAME, ����� = 3 bits   }
        AX0=0x6F02;
        DM(Sport1_Ctrl_Reg)=AX0;

        AX0=0;                          { ���� 200 ��                }
        DM(Sport1_Sclkdiv)=AX0;         { 5 MHz (�������� ⠪⮢�� �����) }
        AX0=2;
        DM(Sport1_Rfsdiv)=AX0;          { ����প� FRAME                }

        AX0=0;
        DM(Sport1_Autobuf_Ctrl)=AX0;    { ��⮡��ਧ��� OFF           }


        AR=0;
        call putData;                  { ᮮ�騬, �� �� � ���浪� }

