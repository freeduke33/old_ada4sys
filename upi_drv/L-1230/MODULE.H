{***********************************************************************}
{ �㭪�� �ணࠬ��஢���� LSM ���㫥�, ������祭��� �१ �३�	}
ProgramModuleMain:
	CALL GetWord; 		{ ��⠥� ����� �����			}
        DM(INPUT_CTRL)=AR;	{ ��⠭���� ����� �����		}

        CALL GetWord;		{ ��⠥� �᫮ ��� �ࠢ���饣� ॣ���� }
        CNTR=AR;		{ ��࠭�� � ॣ����			}

        CALL GetWord;		{ ��⠥� ����襥 ᫮��			}
        SR0=AR;			{ ��࠭�� � ॣ����			}

	CALL InitLmProg;
        AR=SR0 AND AY0, DM(I6,M7)=AX0;{ �뤥��� ���� ��� ������	}

        { 横� �� ��� �����뢠���� ��� � �ࠢ���騩 ॣ���� �����	}
        DO WriteLsmModeRegister UNTIL CE;
           AY0=6;				{ ���� Clock � Write � 1}
           AR=AR OR AY0;			{ ������� �� ��� ������	}
           DM(I6,M7)=AR, AR=AR XOR AY1;		{ �뢥��� ��� ������	}
           DM(I6,M7)=AR, AR=AR XOR AY1;		{ �뢥��� ��஡		}
           NOP;
           DM(I6,M7)=AR, SR=LSHIFT SR0 (LO);	{ ��ᨬ ��஡ 	}
           AF=AF-1;				{ �஢�ਬ ���室	}
           IF NE JUMP WriteLsmModeRegister1;	{ �᫨ ��� � �����	}
           SR0=SI;				{ ���쬥� ��஥ ᫮��	}
         WriteLsmModeRegister1:
           AY0=1;		{ ��᪠ �� ��� ������			}
        WriteLsmModeRegister:  AR=SR0 AND AY0;	{ �뤥��� ��� ������    }

        { ᣥ����㥬 ��஡ �����					}
        CALL StrobeWr;
	JUMP ProgramModuleMain;	{ 㩤�� �� ��砫�			}


{***********************************************************************}
{ �㭪�� �⥭�� ���� �����  						}
GetLsmName:
	SI=100;
        CALL InitReadTtl;
        CALL StrobeTtlIn;
        CNTR=5;			{ ࠧ��� ���� - ���� ���		}
        DO GetNameLoop UNTIL CE;
	   CALL Delay;
           AR=PASS AF;		{ �� ᪮�쪮 ᤢ�����			}
	   SE=AR;		{ ��⠭���� ���稪 ᤢ���		}
           AR=DM(TTL_DSP_IN);	{ ��⠥� ��� ������			}
           AR=AR AND AY0;	{ ������� ���� �� ��� ������		}
           SR=SR OR LSHIFT AR (LO); { ᤢ���� � ������� �� १����	}
           DM(TTL_DSP_OUT)=AX1;	{ ᣥ����㥬 ��஡ �� ����� Clock	}
	   CALL Delay;
           DM(TTL_DSP_OUT)=AY1;
        GetNameLoop: AF=AF+1;   { ����⨬ ���稪 ᤢ����		}
        DM(TTL_DSP_OUT)=AX0;	{ ��⠭���� Clock � Write � 1		}
        DM(PCI)=SR0;		{ ��।���� १���� � PC		}
        JUMP GetLsmName;

Delay:
	CNTR=SI;
	DO DelayLoop UNTIL CE;
	DelayLoop: NOP;
	RTS;

DacLm:
	CALL InitLmProg;
        DM(INPUT_CTRL)=SI;	{ ��⠭���� ����� �����		}
 DacSample:
	CALL GetWord;		{ ����� AX0, AY0, AR			}
        SR0=AR;
        AY0=6;
        AR=6;
        SE=1;

        CNTR=17;
        DO OutDac UNTIL CE;
        NOP;
        DM(I6,M7)=AR, AR=AR XOR AY1;
        DM(I6,M7)=AR, AR=AR XOR AY1;
        NOP;
	DM(I6,M7)=AR, SR=LSHIFT SR0 (LO);
        OutDac: AR=SR1 OR AY0;

        CALL StrobeWr;
	JUMP DacSample;

InitLmProg:
        CALL GetWord;		{ ��⠥� ���襥 ᫮��			}
        SI=AR;			{ ��࠭�� � ॣ����			}
        AY1=2;			{ �����⮢�� ���� ��� ��� ��஡�	}
        SE=-1;			{ �����⮢�� ������ ��� ������ ᤢ���	}
        AR=16;			{ ���稪 ��� ���室� �१ ᫮��	}
        AF=PASS AR;		{ ����襬 ��� � ॣ���� AF		}
        CALL InitIMRegisters;	{ �ந��樠�����㥬 ������� ॣ�����	}
        I6=TTL_DSP_OUT;		{ �����⮢�� ���� ��� ����� � ��஢�� ����}
        AX0=6;			{ ����襬 1 � ���� Clock � Write	}
        AY0=1;			{ ��᪠ �� ��� ������			}
        RTS;

StrobeWr:
        DM(I6,M7)=AY1;		{ ᣥ����㥬 ��஡ �� ��� Write	}
        AR=6;			{ ��ᨬ ��஡				}
        DM(I6,M7)=AR;		{ ᣥ����㥬 ��஡ �� ��� Write	}

        DM(PCI)=AR;		{ ᮮ�騬 � ���� �ணࠬ��஢����	}
        RTS;

TtlInLmMain:
	SI=200;
        CALL InitIMRegisters;	{ �ந��樠�����㥬 ������� ॣ�����	}
        I6=TTL_DSP_OUT;		{ �����⮢�� ���� ��� ����� � ��஢�� ����}
	I5=TTL_DSP_IN;
        SE=-1;
        SR0=0;
   TtlInLoop:
        CALL InitReadTtl;
        CALL StrobeTtlIn;
        CNTR=16;
        AY0=4;
        DO Loop16Ttl UNTIL CE;
        NOP;
        NOP;
        NOP; 
   	AR=DM(I5,M7), SR=LSHIFT SR0(LO);
        NOP;
        DM(I6,M7)=AX1, AR=AR AND AY0;
        SR=SR OR LSHIFT AR BY -3 (HI);
        NOP;
	NOP;
   Loop16Ttl: DM(I6,M7)=AY1;
        DM(PCI)=SR0;
	JUMP TtlInLoop;

InitReadTtl:
	CALL GetWord; 		{ ��⠥� ����� �����			}
        DM(INPUT_CTRL)=AR;	{ ��⠭���� ����� �����		}
        AX0=6;			{ ��७�ᥬ ����⠭�� � ॣ����		}
        AX1=0;			{ ��७�ᥬ ����⠭�� � ॣ����		}
        AY0=8;			{ ��७�ᥬ ����⠭�� � ॣ����		}
        AY1=2;			{ ��७�ᥬ ����⠭�� � ॣ����		}
        SR0=0;			{ � SR0 �㤥� ��������� ���		}
        MR0=4;
        AR=-3;			{ �� ᪮�쪮 ᤢ����� ⥪�騩 ���	}
        AF=PASS AR;             { �������� � ॣ���� AF		}
        RTS;

StrobeTtlIn:
        DM(TTL_DSP_OUT)=AX0;	{ ��⠭���� Clock � Write � 1		}
	CALL Delay;
        DM(TTL_DSP_OUT)=MR0;	{ ��⠭���� Clock � 0			}
	CALL Delay;
        DM(TTL_DSP_OUT)=AX0;	{ ��⠭���� Clock � Write � 1		}
	CALL Delay;
        DM(TTL_DSP_OUT)=SR0;	{ ��⠭���� Clock � Write � 0		}
        RTS;
