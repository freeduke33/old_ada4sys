; 
.model TINY
; MASM51
LOCALS
P286N

DAC_CHANNEL     =       00h
ADC_CHANNEL     =       00h
DAC_BIT         =       4096
ADC_BIT         =       4096

; DT_ACTIVE       =       1
VERSIONtbl         =       2

include common.mac



 CODESEG
        org 100h
entry:
        jmp     startup

IFDEF DT_ACTIVE
DRVname         equ     'DT&PCL-718'
ELSE
DRVname         equ     'PCL-718'
ENDIF

TIMER_FREQ              dd      989680h
COPYRIGHTstring         db      '(C) D.Kokorev Samara RU',0,EOT
NAMEstring              db      'Driver for ',DRVname,0,EOT
NAMElen                 equ     $-NAMEstring-1

ErrMsgOK        db      0,EOT
IFDEF DT_ACTIVE
ErrMsgTimeOut   db      'Timeout in Data Translation',0,EOT
ErrMsgTest      db      'Data Translation test failed',0,EOT
ENDIF
ErrMsgUNIPOLAR  db      'PCL-718 in unipolar mode',0,EOT
ErrMsgNOPCL     db      'PCL-718 can`t installed',0,EOT



BASE            =       300h
ADC_Lo          =       BASE+0
ADC_Hi          =       BASE+1

MUX             =       BASE+2          ; beg - end ADC channel

DAC_Lo          =       BASE+4          ; 0 DAC channel
DAC_Hi          =       BASE+5

STATUS          =       BASE+8
_EOC            =       10000000b       ; end of convertion
_UNI            =       01000000b       ; unipolar mode
_MUX            =       00100000b       ; differential mode indicator
_INT            =       00010000b       ; interrupt status
_CHANNEL        =       00001111b       ; channel mask
CONTROL         =       BASE+9
_INTE           =       10000000b
_DMAE           =       00000100b
_ST_soft        =       00000000b
_ST_extern      =       00000010b
_ST_pacer       =       00000011b
PACER           =       BASE+10

TIMER_0         =       BASE+12
TIMER_1         =       BASE+13
TIMER_2         =       BASE+14
TIMER_SET       =       BASE+15

DT_STATUS       =       225h
_IBF            =       00000010b
_F0             =       00000100b




Active          db      0
savSeg          dw      0
savOfs          dw      0
SkipTik         dd      0
dac_word        dw      0
BufDAC          equ     dac_word
adc_word        dw      0
BufADC          equ     adc_word
flag            db      0
use             db      0
overflow        db      0
LoopFunc        dd      0
EmulFunc        dd      0

_ss                     dw      0
_sp                     dw      0
                        dw      4096 dup('Ss')
stackbuf                dw      0

; ================[ Инициализация платы ]===============
ENABLE proc pascal far
arg BufTim:word,DivCount:word
        mov     ax,DRV_REINIT
        cmp     [cs:Active],0
        je      @@ok
        jmp     @@exit

; ==========================================
@@ok:
        mov     dx,CONTROL
        mov     al,70h
        out     dx,al
        call    near ptr WaitProc
        in      al,dx
        cmp     al,70h
        je      @@active
        mov     ax,DRV_FAILED
        mov     [cs:ErrMsg],offset ErrMsgNOPCL
        jmp     @@exit
@@active:
        mov     dx,STATUS
        in      al,dx
        test    al,_UNI
        jz      @@bipolar
        mov     ax,DRV_FAILED
        mov     [cs:ErrMsg],offset ErrMsgUNIPOLAR
        jmp     @@exit
@@bipolar:
        mov     dx,MUX
        mov     al,0h
        out     dx,al

        mov     dx,CONTROL
        xor     al,al
        out     dx,al
        call    near ptr WaitProc

        mov     dx,TIMER_1
        mov     ax,-1
        out     dx,al
        mov     al,ah
        out     dx,al

        mov     dx,TIMER_SET
        mov     al,10111100b
        out     dx,al

        mov     dx,TIMER_2
        mov     ax,-1
        out     dx,al
        mov     al,ah
        out     dx,al

        mov     dx,PACER
        mov     al,0
        out     dx,al
        call    near ptr WaitProc

        mov     dx,STATUS
        out     dx,al
        call    near ptr WaitProc

; --------------------[ активизировать плату PCL ]----------------------
        push    ds
        push    es

        mov     ax,350Fh
        int     21h
        mov     [cs:savSeg],es
        mov     [cs:savOfs],bx

        mov     ax,cs
        mov     ds,ax
        lea     dx,[RunProc]
        mov     ax,250Fh
        int     21h

        pop     es
        pop     ds


        mov     dx,TIMER_SET
        mov     al,01111100b
;                         -  binary
;                      ---   divide by N count
;                    --      Lo first, then Hi counter bytes
;                  --        counter
        out     dx,al

        mov     dx,TIMER_1
        mov     ax,[DivCount]
        out     dx,al
        mov     al,ah
        out     dx,al

        mov     dx,TIMER_SET
        mov     al,10111100b
        out     dx,al

        mov     dx,TIMER_2
        mov     ax,[BufTim]
        out     dx,al
        mov     al,ah
        out     dx,al

        mov     dx,CONTROL
        mov     al,_INTE+_ST_pacer+ 70h
        out     dx,al
        call    near ptr WaitProc

@@wait_act:
        mov     dx,STATUS
        in      al,dx
        test    al,_INT
        jz      @@wait_act

        mov     dx,PACER
        mov     al,11b
        out     dx,al

        cli
        in      al,21h
        jmp     $+2
        and     al,07Fh
        out     21h,al
        mov     al,67h
        jmp     $+2
        out     20h,al
        sti

        mov     dx,STATUS
        mov     al,1
        out     dx,al

        mov     [cs:Active],1
        mov     [cs:overflow],0
        mov     ax,DRV_OK
@@exit:
        ret
endp ENABLE


; ================[ Деинициализация платы ]==============
DISABLE proc pascal far
        push    ds
        cmp     [cs:Active],0
        je      @@exit
        cli
        mov     dx,CONTROL
        mov     al,0
        out     dx,al

        in      al,21h
        jmp     $+2
        or      al,80h
        out     21h,al
        sti
        mov     ds,[cs:savSeg]
        mov     dx,[cs:savOfs]
        mov     ax,250Fh
        int     21h
        mov     [cs:Active],0
@@exit:
        pop     ds
        mov     ax,DRV_OK
        ret
endp DISABLE

; ==============================[]===============================
GETVOLADC proc pascal  far
arg ADCbuf:dword
        push     ds
        lds      bx,[ADCbuf]
        mov      ax,cs:[BufADC]
        mov      [bx],ax
        pop      ds
        mov     ax,DRV_OK
        ret
endp GETVOLADC

; ==============================[]===============================
SETVOLDAC proc pascal far
arg DACbuf:dword
        push     ds
        lds      bx,[DACbuf]
        mov      ax,[bx]
        mov      cs:[BufDAC],ax
        mov      cs:[overflow],0
        pop      ds
        mov      ax,DRV_OK
        ret
endp SETVOLDAC

; ==============================[]===============================
SETLOOPFUN proc pascal far
arg  func:dword

        cli
        push    [func]
        pop     [cs:LoopFunc]
        sti
        ret
endp SETLOOPFUN

; ==============================[]===============================
CLRLOOPFUN proc far

        cli
        push    cs
        push    offset None
        pop     [cs:LoopFunc]
        sti
        ret
endp CLRLOOPFUN

; ================[ Обмен данными по флагу ]================
EXCHANGE proc pascal far
arg dac:dword,adc:dword
        cmp     [cs:Active],1
        jne     @@exit
        push    ds
        lds     bx,[dac]
        mov     ax,[bx]
        clc
        sti
        mov     [cs:dac_word],ax

        mov     [cs:flag],0
;        mov     cx,-1
@@loop:
;        dec     cx
;        jz      @@exit
        cmp     [cs:flag],1
        jne     @@loop
        lds     bx,[adc]
        mov     ax,[cs:adc_word]
        mov     [bx],ax
@@exit:
        ret
endp EXCHANGE



; ==============================[]===============================
GETBUF proc pascal far
arg DACbuf:dword,ADCbuf:dword
        push    ds
        push    cs
        push    offset BufDAC
        lds     bx,[DACbuf]
        pop     dword ptr [bx]
        push    cs
        push    offset BufADC
        lds     bx,[ADCbuf]
        pop     dword ptr [bx]
        mov     ax,DRV_FAILED
        pop     ds
        ret
endp GETBUF
; ==============================[]===============================
WAITDAC_ADC proc pascal far
        mov     ax,DRV_ERRARG
        cmp     cs:[Active],1
        jne     @@exit
        mov     cs:[flag],0
@@loop:
        cmp     cs:[flag],0
        je      @@loop
        mov     ax,DRV_OK
@@exit:
        ret
endp WAITDAC_ADC

; ==============================[]===============================
SETCHANNEL proc pascal far
arg DACnum:word,ADCnum:word
        mov     ax,DRV_FAILED
        cmp     DACnum,1
        jne     @@error
        cmp     ADCnum,1
        jne     @@error
        mov     ax,DRV_OK
@@error:
        ret
endp SETCHANNEL

; ==============================[]===============================
CLRSKIPPED proc pascal far
        xor     ax,ax
        push    ax
        push    ax
        pop     cs:[SkipTik]
        mov     ax,DRV_OK
        ret
endp CLRSKIPPED
; ==============================[]===============================
GETSKIPPED proc pascal far
arg skip:dword
        push    ds
        push    cs:[SkipTik]
        lds     bx,[skip]
        pop     dword ptr [bx]
        pop     ds
        mov     ax,DRV_OK
        ret
endp GETSKIPPED

; ==============================[]===============================
GETMAXFREQ proc pascal far
arg freq:dword
        push    ds
        lds     bx,[freq]
        push    cs:[TIMER_FREQ]
        pop     dword ptr [bx]
        pop     ds
        mov     ax,DRV_OK
        ret
endp GETMAXFREQ
; ==============================[]===============================
GETMAXCHANNEL proc pascal far
arg DACnum:dword,ADCnum:dword
        push    ds
        mov     ax,1
        lds     bx,[DACnum]
        mov     [bx],ax
        lds     bx,[ADCnum]
        mov     [bx],ax
        pop     ds
        mov     ax,DRV_OK
        ret
endp GETMAXCHANNEL

; ==============================[]===============================
GETDACINFO proc pascal far
arg num:dword
        push    ds
        mov     ax,DAC_BIT
        lds     bx,[num]
        mov     [bx],ax
        pop     ds
        mov     ax,DRV_OK
        ret
endp GETDACINFO
; ==============================[]===============================
GETADCINFO proc pascal far
arg num:dword
        push    ds
        mov     ax,ADC_BIT
        lds     bx,[num]
        mov     [bx],ax
        pop     ds
        mov     ax,DRV_OK
        ret
endp GETADCINFO


;           #####################################
;     ##################                ################
;  ########################## TABLE ########################
; ###########################################################

ErrMsg  dw      ErrMsgOK,0

FuncSKIP        equ     2

FuncTable:
        dw      VERSIONtbl

        dw      IDstring,0
        dw      COPYRIGHTstring,0
        dw      NAMEstring,0


        dw      offset ENABLE,0
        dw      offset DISABLE,0

        dw      offset GETVOLADC,0
        dw      offset SETVOLDAC,0

        dw      offset SETLOOPFUN,0
        dw      offset CLRLOOPFUN,0
        dw      offset EXCHANGE,0
        dw      offset GETBUF,0
        dw      offset WAITDAC_ADC,0

        dw      offset SETCHANNEL,0

        dw      offset CLRSKIPPED,0
        dw      offset GETSKIPPED,0

        dw      offset GETMAXFREQ,0
        dw      offset GETMAXCHANNEL,0
        dw      offset GETDACINFO,0
        dw      offset GETADCINFO,0
        dd      0



; ========================================================= ;
; ================[ Внутренние процедуры ]================= ;
; ========================================================= ;

None proc
        iret
endp None


RunProc proc
        cmp     [cs:use],1
        jne     @@cont
        inc     word ptr [cs:SkipTik+0]
        adc     word ptr [cs:SkipTik+2],0
        iret
@@cont:
        inc     [cs:use]
        pusha
        push    cs
        pop     ax
        mov     ds,ax
        mov     es,ax

        mov     al,[overflow]
        cbw
        add     word ptr [SkipTik+0],ax
        adc     word ptr [SkipTik+2],0
        mov     [overflow],1
        cli
; ========================== Data Translation ==============
IFDEF DT_ACTIVE

        mov     dx,DT_STATUS
@@wait:
        in      al,dx
        test    al,_IBF
        jnz     @@wait

        mov     ax,[dac_word]
        shl     ax,4
        or      al,DAC_CHANNEL*2                ; !!!!!!!!!!!!! DAC channel
        dec     dx
        out     dx,al

        inc     dx
@@wait1:
        in      al,dx
        test    al,_IBF
        jnz     @@wait1

        dec     dx
        mov     al,ah
        out     dx,al
ELSE
; ========================== PCL 718 ==============
        mov     ax,[dac_word]
        shl     ax,4
        and     al,DAC_CHANNEL
        mov     dx,DAC_Lo
        out     dx,al
        inc     dx
        mov     al,ah
        out     dx,al
ENDIF

        mov     dx,ADC_Hi
        in      al,dx
        mov     ah,al
        dec     dx
        in      al,dx
        shr     ax,4
        mov     [adc_word],ax

        cmp     word ptr [LoopFunc],0
        je      @@skipfunc
        cli
        mov     cs:[_ss],ss
        mov     cs:[_sp],sp
        mov     ax,cs
        mov     ss,ax
        mov     sp,offset stackbuf
        sti
        pushf
        call    dword ptr cs:[LoopFunc]
        cli
        mov     ax,cs:[_ss]
        mov     ss,ax
        mov     sp,cs:[_sp]
@@skipfunc:

        mov     [cs:flag],1
        dec     [cs:use]

        mov     dx,STATUS
        mov     al,1
        out     dx,al
        mov     al,20h
        out     20h,al
        popa
        iret
endp RunProc

WaitProc proc near
        nop
        ret
endp WaitProc

init proc
        call    CLRLOOPFUN
        push    di
        push    si
        push    ds
        push    es

        mov     dx,cs
        mov     es,dx
        mov     ds,dx
        mov     si,offset FuncTable+FuncSKIP
@@loop:
        lodsw
        mov     bx,ax
        mov     di,si
        lodsw
        cmp     ax,bx
        je      @@ok
        mov     ax,dx
        stosw
        jmp     @@loop

IFDEF DT_ACTIVE

@@ok:
; ==================[ DT2815 ]==============
        mov     bx,4
        mov     dx,DT_STATUS
@@beg_init:
        mov     cx,-1
@@ini:
        call    near ptr WaitProc
        in      al,dx
        test    al,_F0  ; ready to read control word
        jz      @@ini_loop

        and     al,11100001b    ; get result of test
        jz      @@init
        dec     bx
        jnz     @@reset

        mov     ax,DRV_FAILED
        mov     [ErrMsg],offset ErrMsgTest
        jmp     @@exit
@@ini_loop:
        loop    @@ini

        dec     bx
        jz      @@timeout
@@reset:
        out     dx,al         ; reset
        call    near ptr WaitProc
        jmp     @@beg_init

@@timeout:
        mov     ax,DRV_FAILED
        mov     [ErrMsg],offset ErrMsgTimeOut
        jmp     @@exit

@@init:
        dec     dx
        mov     al,00001000b
;                       ---    last channel
;                     --       output mode
;                  ---         reserved
        out     dx,al         ; init
        xor     ax,ax
@@exit:
ELSE
        xor     ax,ax
@@ok:
ENDIF
        pop     es
        pop     ds
        pop     si
        pop     di
        ret
endp

deinit proc far
        call    CLRLOOPFUN
        call    DISABLE
        xor     AX,AX
        ret
endp



include startup.inc


        end entry
