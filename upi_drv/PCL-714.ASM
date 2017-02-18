; 
.MODEL TINY

; MASM51
LOCALS

include common.mac


DEBUG           equ     1   ; не использовать ЦАП/АЦП (замкнуть вх. на вых.)
; TIMER           equ     1   ; разрешение программировать таймер


DAC_BIT         =       4096
ADC_BIT         =       4096

base714         equ     200h
DRVname         equ     'PCL-714'
VERSIONtbl      equ     2


     .CODE
org 100h

entry:
        jmp  startup

; ====================[ COMMON DATA BLOCK ]============================
TIMER_FREQ              dd     1193180  ; тактовая частота системного таймера

COPYRIGHTstring         db      '(C) D.Kokorev Samara RU',0
NAMEstring              db      'Driver ',DRVname,0,EOT
NAMElen                 equ     $-offset NAMEstring-1

_FlagIO                 db      0
_BufTime                dw      0
_DivCount               dw      1
SkipTik                 dd      0

BufDAC                  dw      0
BufADC                  dw      0

old08                   dd      0
act_fl                  db      0

flag                    db      0
count                   dw      0
LoopFunc                dw      offset None,0

_ss                     dw      0
_sp                     dw      0
                        dw      4096 dup('Ss')
stackbuf                dw      0

; ==============================[]===============================
ENABLE proc pascal far
arg DivHi:word,DivLo:word

        cmp     cs:act_fl,0
        je      cont1
        mov     ax,DRV_REINIT
        ret
cont1:
        push    ds
        mov     ax,cs
        mov     ds,ax
        inc     act_fl

; Set divisor for timer

ifdef TIMER
       push    DivLo
       pop     _BufTime
       push    DivHi
       pop     _DivCount
endif

; Init Timer interrupt
        mov     dx,offset Time_int
        call    near ptr SetTimeInt

        mov     ax,DRV_OK
        pop     ds
        ret
endp  ENABLE

; ==============================[]===============================
DISABLE proc pascal  far
        push    ds
        push    cs
        pop     ds

        mov     ax,DRV_NONE
        cmp     act_fl,0
        je      exit1
        dec     act_fl

        cli
        mov     al, 0FFH
        out     040H, al
        out     040H, al
        sti

        lds     dx,dword ptr old08
        mov     ax,2508h
        int     21h

        mov     ax,DRV_OK
exit1:
        pop     ds
        ret
endp    DISABLE



; ==============================[]===============================
GETVOLADC proc pascal far
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
        pop      ds
        mov      ax,DRV_OK
        ret
endp SETVOLDAC

; ==============================[]===============================
SETLOOPFUN proc pascal far
arg fun:dword
        push    dword ptr [fun]
        pop     dword ptr cs:[LoopFunc]
        mov     ax,DRV_OK
        ret
SETLOOPFUN endp

; ==============================[]===============================
CLRLOOPFUN proc pascal far
        push    cs
        push    offset None
        pop     dword ptr cs:[LoopFunc]
        mov     ax,DRV_OK
        ret
CLRLOOPFUN endp


; ==============================[]===============================
EXCHANGE proc pascal far
arg DACbuf:dword,ADCbuf:dword

        push    es
        push    ds
        push    bx

        push    cs
        pop     ds
        cmp     act_fl,0
        je      exit
        les     bx,[DACbuf]
        mov     ax,es:[bx]
        mov     BufDAC,ax
        mov     _FlagIO,0
wait_ok:
        cmp     _FlagIO,0
        je      wait_ok
exit:
        mov     ax,BufADC
        les     bx,[ADCbuf]
        mov     es:[bx],ax
        pop     dx
        pop     ds
        pop     es
        mov     ax,DRV_OK
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
        mov     cs:[_FlagIO],0
wait_ok1:
        cmp     cs:[_FlagIO],0
        je      wait_ok1
        mov     ax,DRV_OK
        ret
endp WAITDAC_ADC

; ==============================[]===============================
SETCHANNEL proc pascal far
arg DACnum:word,ADCnum:word
        mov     ax,DRV_ERRARG
        cmp     DACnum,1
        je      @@error
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

ErrMsgOK        db      0
ErrMsg          dw      offset ErrMsgOK,0

FuncSKIP        equ     2

FuncTable:
                dw      VERSIONtbl

                dw      offset IDstring,0
                dw      offset COPYRIGHTstring,0
                dw      offset NAMEstring,0

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



; *************************************************************************
; *****                       РАБОТА С ТАЙМЕРОМ                       *****
; *************************************************************************

;
; cs:dx - timer procedure
;
SetTimeInt  PROC  NEAR
        mov     ax,3508h
        int     21h

        mov     word ptr old08,bx
        mov     word ptr [old08+2],es
        mov     ah,25h
        int     21h

; Изменение частоты таймера.
        cli
ifdef TIMER
        mov     ax, _BufTime
        out     040H, al
        mov     al, ah
        out     040H, al
endif
        mov     ax,_DivCount
        mov     count,ax
        sti

        ret
SetTimeInt  ENDP


None proc far
        iret
None endp

;********************************************************
;   П/пр-ма обработки прерывания от системного таймера
;********************************************************


Time_int  PROC
        dec     cs:count
        jnz     no_cont
        push    ax
        mov     ax,cs:_DivCount
        mov     cs:count,ax
        pop     ax
        cmp     cs:flag,0
        je      continue
        inc     word ptr cs:[SkipTik+0]
        adc     word ptr cs:[SkipTik+2],0
no_cont:
        jmp     dword ptr cs:[old08]
continue:
        inc     cs:flag
        pushf
        call    dword ptr cs:[old08]

        push    ax
        push    dx
        push    ds

        mov     ax,cs
        mov     ds,ax


;          ЦАП

;        mov     dx, base714 + 13
;        mov     al, 1           ;  DigOut 0 = 1  - трассировка
;        out     dx, al          ;

; Вывод числа на ЦАП

        mov     ax,BufDAC

ifndef DEBUG
        mov     dx, base714+4   ; мл. байт ->на 0-канал ЦАП
        out     dx, al
        mov     al, ah
        mov     dx, base714+5   ; ст. байт ->на 0-канал ЦАП
        out     dx, al

; Читаем с АЦП
        mov        al, 11
        mov        dx, base714+10   ; выбор канала АЦП
        out        dx, al

        mov        dx, base714+11
        mov        al, 1            ; старт с'ема АЦП
        out        dx, al

        mov        dx, base714+5
wait0:                             ; ждем конца преобразования
        in         al, dx
        cmp        al, 40H
        jge        wait0

        mov        ah, al          ; ст. байт АЦП

        mov        dx, base714+4
        in         al, dx               ; мл. байт АЦП
endif

        mov        BufADC,ax

;        mov        dx, base714 + 13
;        mov        al, 0            ;  DigOut 0 = 0  - трассировка
;        out        dx, al           ;


        mov        _FlagIO,1

        pop     ds
        pop     dx

        cmp     word ptr cs:[LoopFunc],0
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

        pop     ax
        dec     cs:flag
        iret
Time_int  ENDP


init    proc
        push    ds
        push    es
        push    si
        push    di
        mov     dx,cs
        mov     ds,dx
        mov     es,dx

        call    CLRLOOPFUN
        mov     si,offset FuncTable+FuncSKIP
@@loop:
        lodsw
        mov   bx,ax
        mov   di,si
        lodsw
        cmp   bx,ax
        je    @@end
        mov   ax,dx
        stosw
        jmp    @@loop
@@end:
        pop     di
        pop     si
        pop     es
        pop     ds
        xor     ax,ax
        ret
init    endp

deinit  proc far
        call    CLRLOOPFUN
        call    DISABLE
        xor     ax,ax
        ret
deinit  endp


include startup.inc

;  ----------- NonTSR part ------------

ifdef DEBUG
                db      0ah,0dh,7,'*** DEBUG version (whithout using UPI) ***',7
ifdef TIMER
                db      0ah,0dh,'*** TIMER programing is ENABLE ***'
endif
else
ifndef TIMER
                db      0ah,0dh,7,'*** Whithout programable TIMER ***',7
endif
endif

                db      0ah,0dh,24h

     END entry
