.model tiny
.code
.286

include common.mac
include nmpipe.inc

org 100h

start:
        jmp do
; ======================================================

NUM_DAC         equ     PIPE_NUM_DAC
NUM_ADC         equ     PIPE_NUM_ADC

use             db      0
flag            db      0
SkipTik         dd      0
Scale           dw      1
ScaleVal        dw      1
adc_word        dw      10 dup('*|')
nl              db      '$'

inp_string      db      10
dac_word        dw      10 dup('--')

LoopFun         dd      0

startmsg        db      'Session started ...',0dh,0ah,'$'
stopmsg         db      'Session stoped.',0dh,0ah,'$'

; ======================================================

do:
        PIPE_ENABLE 1,1,RunProc
        jc  @@done

        push       cs
        pop        ds
        mov        ah,09h
        lea        dx,startmsg
        int        21h

        mov     cx,100
l:
        push    cx
        putc    '<'
        call    RunProc
        call    Console
        putc    '>'
        putc    0Ah
        putc    0Dh
        pop     cx
        loop    l

        PIPE_DISABLE

        push       cs
        pop        ds
        mov        ah,09
        lea        dx,stopmsg
        int        21h

@@done:
        jmp     do

        mov     ax,4C00h
        int     21h
; =======================================================================

RunProc proc
        cmp     [cs:use],0
        je      @@cont
        inc     word ptr [cs:SkipTik+0]
        adc     word ptr [cs:SkipTik+2],0
@@err:
        ret
@@cont:
        dec     [cs:Scale]
        jnz     @@err
        inc     [cs:use]
        pusha
        push    ds
        push    es
        mov     ax,cs
        mov     ds,ax
        mov     es,ax

        mov     ax,[ScaleVal]
        mov     [Scale],ax

        PIPE_GetADC adc_word, NUM_ADC ;,ADC_channel
        PIPE_SetDAC dac_word, NUM_DAC ;DAC_channel,List_DAC


;        cmp     word ptr cs:[LoopFunc],0
;        je      @@skipfunc
;        cli
;        mov     cs:[_ss],ss
;        mov     cs:[_sp],sp
;        mov     ax,cs
;        mov     ss,ax
;        mov     sp,offset stackbuf
;        sti
;        pushf
;        call    dword ptr cs:[LoopFunc]
;        cli
;        mov     ax,cs:[_ss]
;        mov     ss,ax
;        mov     sp,cs:[_sp]
;@@skipfunc:

        mov     [flag],1
        dec     [use]

        pop     es
        pop     ds
        popa
        ret
endp RunProc

Console proc
        push    cs
        pop     ds
        mov     ah,09h
        lea     dx,adc_word
        int     21h
        mov     ah,0Ah
        lea     dx,inp_string
;        int     21h
        mov     ah,09h
        lea     dx,nl
        int     21h
        ret
endp Console

PIPE_Declare



end start