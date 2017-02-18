{***********************************************************************}
{ функция программирования LSM модулей, подключенных через крейт	}
ProgramModuleMain:
	CALL GetWord; 		{ считаем номер модуля			}
        DM(INPUT_CTRL)=AR;	{ установим номер модуля		}

        CALL GetWord;		{ считаем число бит управляющего регистра }
        CNTR=AR;		{ сохраним в регистре			}

        CALL GetWord;		{ считаем младшее слово			}
        SR0=AR;			{ сохраним в регистре			}

	CALL InitLmProg;
        AR=SR0 AND AY0, DM(I6,M7)=AX0;{ выделим первый бит данных	}

        { цикл по числу записываемых бит в управляющий регистр модуля	}
        DO WriteLsmModeRegister UNTIL CE;
           AY0=6;				{ биты Clock и Write в 1}
           AR=AR OR AY0;			{ наложим на бит данных	}
           DM(I6,M7)=AR, AR=AR XOR AY1;		{ выведем бит данных	}
           DM(I6,M7)=AR, AR=AR XOR AY1;		{ выведем строб		}
           NOP;
           DM(I6,M7)=AR, SR=LSHIFT SR0 (LO);	{ сбросим строб 	}
           AF=AF-1;				{ проверим переход	}
           IF NE JUMP WriteLsmModeRegister1;	{ если нет то дальше	}
           SR0=SI;				{ возьмем второе слово	}
         WriteLsmModeRegister1:
           AY0=1;		{ маска на бит данных			}
        WriteLsmModeRegister:  AR=SR0 AND AY0;	{ выделим бит данных    }

        { сгенерируем строб записи					}
        CALL StrobeWr;
	JUMP ProgramModuleMain;	{ уйдем на начало			}


{***********************************************************************}
{ функция чтения Кода Модуля  						}
GetLsmName:
	SI=100;
        CALL InitReadTtl;
        CALL StrobeTtlIn;
        CNTR=5;			{ размер кода - пять бит		}
        DO GetNameLoop UNTIL CE;
	   CALL Delay;
           AR=PASS AF;		{ на сколько сдвигать			}
	   SE=AR;		{ установим счетчик сдвига		}
           AR=DM(TTL_DSP_IN);	{ считаем бит данных			}
           AR=AR AND AY0;	{ наложим маску на бит данных		}
           SR=SR OR LSHIFT AR (LO); { сдвинем и наложим на результат	}
           DM(TTL_DSP_OUT)=AX1;	{ сгенерируем строб на линии Clock	}
	   CALL Delay;
           DM(TTL_DSP_OUT)=AY1;
        GetNameLoop: AF=AF+1;   { прирастим счетчик сдвигов		}
        DM(TTL_DSP_OUT)=AX0;	{ установим Clock и Write в 1		}
        DM(PCI)=SR0;		{ передадим результат в PC		}
        JUMP GetLsmName;

Delay:
	CNTR=SI;
	DO DelayLoop UNTIL CE;
	DelayLoop: NOP;
	RTS;

DacLm:
	CALL InitLmProg;
        DM(INPUT_CTRL)=SI;	{ установим номер модуля		}
 DacSample:
	CALL GetWord;		{ портит AX0, AY0, AR			}
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
        CALL GetWord;		{ считаем старшее слово			}
        SI=AR;			{ сохраним в регистре			}
        AY1=2;			{ подготовим маску для бита строба	}
        SE=-1;			{ подготовим индекс для левого сдвига	}
        AR=16;			{ счетчик для перехода через слово	}
        AF=PASS AR;		{ запишем его в регистр AF		}
        CALL InitIMRegisters;	{ проинициализируем индексные регистры	}
        I6=TTL_DSP_OUT;		{ подготовим адрес для записи в цифровой порт}
        AX0=6;			{ запишем 1 в биты Clock и Write	}
        AY0=1;			{ маска на бит данных			}
        RTS;

StrobeWr:
        DM(I6,M7)=AY1;		{ сгенерируем строб на бите Write	}
        AR=6;			{ сбросим строб				}
        DM(I6,M7)=AR;		{ сгенерируем строб на бите Write	}

        DM(PCI)=AR;		{ сообщим о конце программирования	}
        RTS;

TtlInLmMain:
	SI=200;
        CALL InitIMRegisters;	{ проинициализируем индексные регистры	}
        I6=TTL_DSP_OUT;		{ подготовим адрес для записи в цифровой порт}
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
	CALL GetWord; 		{ считаем номер модуля			}
        DM(INPUT_CTRL)=AR;	{ установим номер модуля		}
        AX0=6;			{ перенесем константу в регистр		}
        AX1=0;			{ перенесем константу в регистр		}
        AY0=8;			{ перенесем константу в регистр		}
        AY1=2;			{ перенесем константу в регистр		}
        SR0=0;			{ в SR0 будет помещаться код		}
        MR0=4;
        AR=-3;			{ на сколько сдвигать текущий бит	}
        AF=PASS AR;             { запомним в регистре AF		}
        RTS;

StrobeTtlIn:
        DM(TTL_DSP_OUT)=AX0;	{ установим Clock и Write в 1		}
	CALL Delay;
        DM(TTL_DSP_OUT)=MR0;	{ установим Clock в 0			}
	CALL Delay;
        DM(TTL_DSP_OUT)=AX0;	{ установим Clock и Write в 1		}
	CALL Delay;
        DM(TTL_DSP_OUT)=SR0;	{ установим Clock и Write в 0		}
        RTS;
