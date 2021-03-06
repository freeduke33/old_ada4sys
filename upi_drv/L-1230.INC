; L1230_TIMED     =       1

; ����� ०����
; ----- ��������� ������
TestCmd          equ    0       ; ��� ������⢨� �㦭��� ���� �����
EnableCmd        equ    1       ; ������� ������� <-- freq_hi,freq_lo
DisableCmd       equ    2       ; �⪫���� �������
InfoCmd          equ    3       ; �᫮ ������� --> dac,adc,freq(hi,lo)
SetChannelCmd    equ    4       ; �᫮ ������� <-- dac,adc
GetSkippedCmd    equ    5       ; �ய�饭� ����⮢ --> tik(hi,lo)
ClrCmd           equ    6       ; ��� �ய�饭���
LoopBackCmd      equ    7       ; ��⮢� ०��
StopCmd          equ    8       ; �宥 ���ﭨ�
; ----- ��஢� ������
GetDigital       equ    9       ;
SetDigital       equ    10      ;




; ��������� ������
L1230_FREQ            EQU     10000000     ; 10 ���
L1230_NUM_DAC         EQU     16
L1230_NUM_ADC         EQU     16
L1230_DAC_BIT         EQU     4096
L1230_ADC_BIT         EQU     4096

; ��஢� ������
L1230_NUM_IN          EQU     32
L1230_NUM_OUT         EQU     32


L1230_ENABLE macro RateHi,RateLo,Vector

        mov     dx, cs:CtrlR
        mov     ax, 8
        out     dx, ax

      ; �������� ����� ���� ���뢠���
        in      al, 0A1H
        mov     byte ptr cs:OldMask, al

      ; �������� ����  ����� ���뢠���
        mov     ah, 35h
        mov     al, cs:IrqN
        int     21h
        mov     word ptr cs:OldHand[0], bx
        push    es
        pop     ax
        mov     word ptr cs:OldHand[2], ax

      ; ��⠭���� ����� ���뢠���
        mov     ah, 25h
        mov     al, cs:IrqN
        lea     dx, [Vector]
        int     21h

      ; �ந��樠�����㥬 ����஫��� ���뢠���
;        out     020h, al
;        out     0a0h, al          ; ��⠭���� ����஫���

        in      al, 0a1h
        mov     al, 094h ; cs:IrqMaskBit
;        and     al, bl
        out     0a1h, al          ; ��⠭���� ���� ���뢠���
        mov     al, 20h

        out     020h, al
        out     0a0h, al          ; ��⠭���� ����஫���

        mov     dx, cs:CtrlR
        mov     ax, 9
        out     dx, ax
        mov     dx, cs:IrqResetR
        in      ax, dx

        mov     ax, EnableCmd
        call    SendCommand

        mov     ax, RateLo
        call    PutData
        mov     ax, RateHi                ; ��⠭���� �����
        call    PutData

        call    TimeWaitData              ; ���� ����᪠
        jc      @@done
        putc    '!'
        mov     ax,0
@@done:
endm

L1230_DISABLE macro

;        cli
      ; ����⠭���� ����஫��� ���뢠���
;        mov     al,byte ptr cs:OldMask
;        out     0A1h, al
;        mov     al, 20h
;        out     020h, al
;        out     0a0h, al          ; ��⠭���� ����஫���
;        sti

      ; ����⠭���� ����  ����� ���뢠���
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
;*********** �����쭠� �㭪�� ��।�� ������ �� �����
PutData proc
        push    dx
        mov     dx, cs:DataR
        out     dx, ax

        L1230_WaitBoard 'w',2
        pop     dx
        ret
endp

;*********** �����쭠� �㭪�� �⥭�� ������ �� �����
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
;*                  ������� : void PLATA_TEST()                    *
;*  �����頥� �㫥��� ���祭�� � ��砥 �ᯥ譮�� ���஢����    *
;*  ������⢨� ����� � ����㦥��� �ࠩ��஬ LBIOS   � ���㫥���  *
;*  ���祭�� � ��⨢��� ��砥                                    *
;*  ���㫥��� ���祭�� ����砥�, �� ���� ���� ��������� �      *
;*  ��������, ���� �� �� ����㦥� �ࠩ��� LBIOS.                *
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
;* �����頥� ��� ����� � ᫮� ModuleNumber � �३� CraitNumber  *
;* AX = ModuleNumber & CraitNumber
;********************************************************************
GET_SLOT_CODE PROC
        push    ax

        mov     ax, GetLsmNameCmd       ; ������� �⥭�� ���� �����
        call    SendCommand

;        mov     bx, KrateNumber
;        mov     ax, ModuleNumber
;        shl     ax, 4
;        shl     bx, 10
;        or      ax, bx
        pop     ax

        call    PutData
        call    GetData                 ; ��⠥� ��� �����
        ret
ENDP

;********************************************************************
;*                  ������� ; void CALIBRATION()                    *
;*              ��१�ࢨ஢����� �㭪��                           *
;********************************************************************
CALIBRATION PROC
        mov     ax, CalibrCmd
        call    SendCommand
        call    GetData
        ret
ENDP

;********************************************************************
;*                  ������� ; void SET_UVX(int key);                *
;*              ��१�ࢨ஢����� �㭪��                           *
;********************************************************************
SET_UVX PROC C FAR UvxFlag : WORD
        mov     ax, SetUvxCmd
        call    SendCommand
        mov     ax, UvxFlag
        call    PutData
        ret
ENDP

;************************************************************************
;*                  ������� ; void SET_ADSP_SPEED(int type);            *
;* ᮮ�頥� �ࠩ���� � ᪮��� ��⠭��������� ������ ADSP-2105     *
;*   ax = 0  -> ᪮���� 100 �� �� 横� (��⠭���������� �� 㬮�砭��) *
;*   ax = 1  -> ᪮���� 50  �� �� 横�                                *
;************************************************************************
SET_ADSP_SPEED PROC
        push    ax
        mov     ax, SetDspSpeedCmd
        call    SendCommand
        pop     ax
        call    PutData
        call    GetData                 ; �������� ���� �������
        ret
ENDP

;************************************************************************
;*                  ������� ; void SET_WAIT_STATE(int wait_state);      *
;*  ��⠭�������� ����প� �� �ᯮ��㥬�� �� ���� ������.             *
;*  �� 㬮�砭�� �� ������ ��⠭���������� �㫥��� Wait State,          *
;*  �����⠭���� Wait State ����室��� ⮫쪮 �� �ᯮ�짮�����        *
;*  128 �������� ����� �� ���� � �᪮७�� ⨯�� ������ ADSP (   *
;*  � ᪮����� 横�� 50 ��). � �⮬ ��砥 ����室��� ��⠭�����      *
;*  ������� Wait State.                                               *
;* AX=WaitState
;************************************************************************
SET_WAIT_STATE PROC
        push    ax
        mov     ax, SetWaitStateCmd
        call    SendCommand
        pop     ax
        call    PutData
        call    GetData                 ; �������� ���� �������
        ret
ENDP

;********************************************************************
;*                  ������� ; int SET_TIMER_SCALE(int scale);       *
;*  ��⠭�������� �����樥�� ����⠡�஢���� ⠩��� (�� 㬮�砭�� *
;*  �� ��⠭����� ࠢ�� 9, �� ᮮ⢥����� ������� ���ࢠ���    *
;*  ����� � ����ᥪ㭤��)                                          *
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
