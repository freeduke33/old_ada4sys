; L1230_TIMED     =       1

; номера режимов
; ----- аналоговые каналы
TestCmd          equ    0       ; тест присутствия нужного биоса платы
EnableCmd        equ    1       ; включить генерацию <-- freq_hi,freq_lo
DisableCmd       equ    2       ; отключить генерацию
InfoCmd          equ    3       ; число каналов --> dac,adc,freq(hi,lo)
SetChannelCmd    equ    4       ; число каналов <-- dac,adc
GetSkippedCmd    equ    5       ; пропущено отсчетов --> tik(hi,lo)
ClrCmd           equ    6       ; сброс пропущенных
LoopBackCmd      equ    7       ; тестовый режим
StopCmd          equ    8       ; тихое состояние
; ----- цифровые каналы
GetDigital       equ    9       ;
SetDigital       equ    10      ;




; аналоговые каналы
L1230_FREQ            EQU     10000000     ; 10 МГц
L1230_NUM_DAC         EQU     16
L1230_NUM_ADC         EQU     16
L1230_DAC_BIT         EQU     4096
L1230_ADC_BIT         EQU     4096

; цифровые каналы
L1230_NUM_IN          EQU     32
L1230_NUM_OUT         EQU     32


L1230_ENABLE macro RateHi,RateLo,Vector

        mov     dx, cs:CtrlR
        mov     ax, 8
        out     dx, ax

      ; запомним старую маску прерываний
        in      al, 0A1H
        mov     byte ptr cs:OldMask, al

      ; запомним старый  вектор прерывания
        mov     ah, 35h
        mov     al, cs:IrqN
        int     21h
        mov     word ptr cs:OldHand[0], bx
        push    es
        pop     ax
        mov     word ptr cs:OldHand[2], ax

      ; установим вектор прерывания
        mov     ah, 25h
        mov     al, cs:IrqN
        lea     dx, [Vector]
        int     21h

      ; проинициализируем контроллер прерываний
;        out     020h, al
;        out     0a0h, al          ; установим контроллер

        in      al, 0a1h
        mov     al, 094h ; cs:IrqMaskBit
;        and     al, bl
        out     0a1h, al          ; установим маску прерываний
        mov     al, 20h

        out     020h, al
        out     0a0h, al          ; установим контроллер

        mov     dx, cs:CtrlR
        mov     ax, 9
        out     dx, ax
        mov     dx, cs:IrqResetR
        in      ax, dx

        mov     ax, EnableCmd
        call    SendCommand

        mov     ax, RateLo
        call    PutData
        mov     ax, RateHi                ; установим частоту
        call    PutData

        call    TimeWaitData              ; ждем запуска
        jc      @@done
        putc    '!'
        mov     ax,0
@@done:
endm

L1230_DISABLE macro

;        cli
      ; восстановим контроллер прерываний
;        mov     al,byte ptr cs:OldMask
;        out     0A1h, al
;        mov     al, 20h
;        out     020h, al
;        out     0a0h, al          ; установим контроллер
;        sti

      ; восстановим старый  вектор прерывания
        mov     dx, word ptr cs:OldHand[0]
        mov     ax, word ptr cs:OldHand[2]
        push    ax
        pop     ds
        mov     ah, 25h
        mov     al, cs:IrqN
        int     21h

        mov     ax, DisableCmd
        call    SendCommand

endm



L1230_SetDAC macro dac_word,channels
local @@loop,@@set,@@done

;        jmp     @@done

        lea     si,dac_word
        mov     cx,channels
@@loop:
        lodsw
        call    PutData
        loop    @@loop
@@done:
endm

L1230_GetADC macro adc_word,channels
local @@l,@@done

;       jmp      @@done

       lea      di,adc_word
       mov      cx,channels
@@l:
       call     GetData
       stosw
       loop     @@l
@@done:
endm

L1230_IRQRESET macro reg,val
        mov     ax,val
        call    PutData
        mov     dx,reg
        in      ax, dx
        mov     al, 020h
        out     020h, al
        out     0a0h, al
endm


L1230_WaitBoard macro OutCh,TstBit

        mov     dx, cs:FlagR

ifdef L1230_TIMED
        push    cx
        mov     cx, 20000
@@Wait:
        loop    @@Do
        putc    OutCh
        jmp     short @@Abort
@@Do:

else

@@Wait:

endif

        in      ax, dx
        test    ax, TstBit
        jz      @@Wait
@@Abort:

ifdef L1230_TIMED
        pop     cx
endif
endm



L1230_Declare macro

SendCommand proc near
        push    dx
        mov     dx, cs:CmdR
        out     dx, ax
        L1230_WaitBoard 'c',2
        pop     dx
        clc
        ret
endp
;*********** локальная функция передачи данных на плату
PutData proc
        push    dx
        mov     dx, cs:DataR
        out     dx, ax

        L1230_WaitBoard 'w',2
        pop     dx
        ret
endp

;*********** локальная функция чтения данных от платы
GetData proc
        push    dx
        L1230_WaitBoard 'r',1

        mov     dx, cs:DataR
        in      ax, dx
        pop     dx
        clc
        ret
endp

TimeWaitData proc
        push    dx
        push    cx

        mov     cx, 30
        mov     dx, cs:FlagR
@@Wait:
        in      ax, dx
        test    ax, 1
        jnz      @@ok
        loop     @@Wait
        putc    't'
        pop     cx
        pop     dx
        stc
        ret
@@ok:
        mov     dx, cs:DataR
        in      ax, dx
        pop     cx
        pop     dx
        clc
        ret
endp


endm


Appendiks macro

;*******************************************************************
;*                  ФУНКЦИЯ : void PLATA_TEST()                    *
;*  возвращает нулевое значение в случае успешного тестирования    *
;*  присутствия платы с загруженным драйвером LBIOS   и ненулевое  *
;*  значение в противном случае                                    *
;*  Ненулевое значение означает, что либо плата отсутствует в      *
;*  компьютере, либо не был загружен драйвер LBIOS.                *
;*******************************************************************
Plata_Test PROC
        mov     ax, TestCmd
        mov     dx, cs:CmdR
        out     dx, ax
        mov     dx, cs:DataR
        in      ax, dx
        mov     dx, cs:DataR
        mov     cx, 30
        mov     dx, cs:FlagR
      pltst:
        in      ax, dx
        test    ax, 1
        jnz     outtst
        loop pltst
     outtst:
        mov     dx, cs:DataR
        in      ax, dx
        cmp     ax, 05566h
        mov     ax, 0
        jz      pltstlp1
        mov     ax, 1
     pltstlp1:
        ret
ENDP


;********************************************************************
;* Возвращает код модуля в слоте ModuleNumber в крейте CraitNumber  *
;* AX = ModuleNumber & CraitNumber
;********************************************************************
GET_SLOT_CODE PROC
        push    ax

        mov     ax, GetLsmNameCmd       ; команда чтения кода модуля
        call    SendCommand

;        mov     bx, KrateNumber
;        mov     ax, ModuleNumber
;        shl     ax, 4
;        shl     bx, 10
;        or      ax, bx
        pop     ax

        call    PutData
        call    GetData                 ; считаем код модуля
        ret
ENDP

;********************************************************************
;*                  ФУНКЦИЯ ; void CALIBRATION()                    *
;*              зарезервированная функция                           *
;********************************************************************
CALIBRATION PROC
        mov     ax, CalibrCmd
        call    SendCommand
        call    GetData
        ret
ENDP

;********************************************************************
;*                  ФУНКЦИЯ ; void SET_UVX(int key);                *
;*              зарезервированная функция                           *
;********************************************************************
SET_UVX PROC C FAR UvxFlag : WORD
        mov     ax, SetUvxCmd
        call    SendCommand
        mov     ax, UvxFlag
        call    PutData
        ret
ENDP

;************************************************************************
;*                  ФУНКЦИЯ ; void SET_ADSP_SPEED(int type);            *
;* сообщает драйверу о скорости установленного процессора ADSP-2105     *
;*   ax = 0  -> скорость 100 нс на цикл (устанавливается по умолчанию) *
;*   ax = 1  -> скорость 50  нс на цикл                                *
;************************************************************************
SET_ADSP_SPEED PROC
        push    ax
        mov     ax, SetDspSpeedCmd
        call    SendCommand
        pop     ax
        call    PutData
        call    GetData                 ; подождем конца команды
        ret
ENDP

;************************************************************************
;*                  ФУНКЦИЯ ; void SET_WAIT_STATE(int wait_state);      *
;*  Устанавливает задержку на используемую на плате память.             *
;*  По умолчанию на память устанавливается нулевой Wait State,          *
;*  переустановка Wait State необходима только при использовании        *
;*  128 кСловной памяти на плате с ускоренным типом процессора ADSP (   *
;*  со скоростью цикла 50 нс). В этом случае необходимо установить      *
;*  единичный Wait State.                                               *
;* AX=WaitState
;************************************************************************
SET_WAIT_STATE PROC
        push    ax
        mov     ax, SetWaitStateCmd
        call    SendCommand
        pop     ax
        call    PutData
        call    GetData                 ; подождем конца команды
        ret
ENDP

;********************************************************************
;*                  ФУНКЦИЯ ; int SET_TIMER_SCALE(int scale);       *
;*  устанавливает коэффициент масштабирования таймера (по умолчанию *
;*  он установлен равным 9, что соответствует заданию интервалов    *
;*  ввода в микросекундах)                                          *
;********************************************************************
SET_TIMER_SCALE PROC
        push    ax
        mov     ax, SetScaleCmd
        call    SendCommand
        pop     ax
        call    PutData
        ret
ENDP





endm
