; 
.model TINY
LOCALS
P286N

RESET           =1     ;
NeedCFG         =1

; Table2          =1
Table3          =1

include nmpipe.inc ; Макросы управления Named Pipes (OS/2 VDM & Lan Server)

NUM_DAC         =       PIPE_NUM_DAC
DAC_BIT         =       PIPE_DAC_BIT
NUM_ADC         =       PIPE_NUM_ADC
ADC_BIT         =       PIPE_ADC_BIT

IFDEF Table2
  VERSIONtbl         =       2
ENDIF
IFDEF Table3
  VERSIONtbl         =       3
ENDIF


include common.mac



 CODESEG
        org 100h
entry:
        jmp     startup

DRVname         equ     'Named pipes system'

TIMER_FREQ              dd      PIPE_FREQ

COPYRIGHTstring         db      '(C) D.Kokorev Samara RU',0
NAMEstring              db      'Driver for ',DRVname,0,EOT
NAMElen                 equ     $-NAMEstring-1

ErrMsgOK        db      0
ErrMsgTest      db      'Pipes support can`t detected.',0
ErrMsgTimeout   db      'Emulator can`t connected.',0


Scale           dw      0
ScaleVal        dw      0
Active          db      0
SkipTik         dd      0
dac_word        dw      NUM_DAC+1 dup(0)
BufDAC          equ     dac_word
adc_word        dw      NUM_ADC+1 dup(0)
adc_tmp         dw      NUM_ADC+1 dup(0)
BufADC          equ     adc_word
flag            db      0
use             db      0
LoopFunc        dd      0
EmulFunc        dd      0
DAC_channel     dw      NUM_DAC
ADC_channel     dw      NUM_ADC

                        dw      4096 dup('Ss')
stackbuf                dw      0
_ss                     dw      0
_sp                     dw      0

; ================[ Инициализация платы ]===============
ENABLE proc pascal far
arg DivHi:word,DivLo:word
;        pusha
        push    ds
        push    es

        mov     ax,cs
        mov     ds,ax
        mov     es,ax

        mov     ax,DRV_REINIT
        cmp     [Active],0
        je      @@ini
        jmp     @@exit

@@ini:
        lea     di,BufDAC
        xor     ax,ax
        mov     cx,[DAC_channel]
        cld
        rep stosw

        mov     ax,DivLo
        mov     bl,ah
        xor     ah,ah
        mov     DivLo,ax
        mov     al,bl
        inc     ax
        mov     [Scale],ax
        mov     [ScaleVal],ax

        PIPE_ENABLE  DivHi,DivLo,RunProc

        cmp     al,0
        je      @@next
        mov     [ErrMsg],offset ErrMsgTimeout
        mov     ax,DRV_FAILED
        jmp     @@exit
@@next:
        mov     [cs:use],0
        mov     [cs:Active],1
        mov     ax,DRV_OK
@@exit:
        pop     es
        pop     ds
;        popa
        ret
endp ENABLE


; ================[ Деинициализация платы ]==============
DISABLE proc pascal far
        pusha
        push    ds
        push    es
        cmp     [cs:Active],0
        je      @@exit

        mov     ax,cs
        mov     ds,ax
        mov     es,ax

        sti
        mov     [cs:use],1

        PIPE_DISABLE

        mov     [cs:Active],0
@@exit:
        pop     es
        pop     ds
        popa
        mov     ax,DRV_OK
        ret
endp DISABLE

; ==============================[]===============================
GETVOLADC proc pascal far
arg ADCbuf:dword
        push     ds
        push     es
        push     si
        push     di
        push     cx
        les      di,[ADCbuf]
        lea      si,BufADC
        mov      ax,cs
        mov      ds,ax
        mov      cx,NUM_ADC
        cld
        rep      movsw
        pop      cx
        pop      di
        pop      si
        pop      es
        pop      ds
        mov     ax,DRV_OK
        ret
endp GETVOLADC

; ==============================[]===============================
SETVOLDAC proc pascal far
arg DACbuf:dword
        push     ds
        push     es
        push     si
        push     di
        push     cx

        lds      si,[DACbuf]
        lea      di,BufDAC
        mov      ax,cs
        mov      es,ax
        mov      cx,NUM_DAC
        cld
        rep      movsw
        pop      cx
        pop      di
        pop      si
        pop      es
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

        push     ds
        push     es
        push     si
        push     di
        push     cx

        lds      si,[dac]
        lea      di,BufDAC
        mov      ax,cs
        mov      es,ax
        mov      cx,NUM_DAC
        cld
        rep      movsw

        sti
        mov     [cs:flag],0
@@loop:
        call    RunProc
        cmp     [cs:flag],1
        jne     @@loop

        les      di,[adc]
        lea      si,BufADC
        mov     ax,cs
        mov     ds,ax
        mov     cx,NUM_ADC
        cld
        rep     movsw
        pop      cx
        pop      di
        pop      si
        pop      es
        pop      ds

@@exit:
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
        mov     ax,DRV_FAILED
        cmp     cs:[Active],1
        jne     @@exit
        mov     cs:[flag],0
@@loop:
        call    RunProc
        cmp     cs:[flag],0
        je      @@loop
        mov     ax,DRV_OK
@@exit:
        ret
endp WAITDAC_ADC

; ==============================[]===============================
SETCHANNEL proc pascal far
arg DACnum:word,ADCnum:word
        push    cx
        push    bx
        mov     ax,DRV_ERRARG
        mov     bx,DACnum
        cmp     bx,NUM_DAC
        ja      @@error
        mov     cx,ADCnum
        cmp     cx,NUM_ADC
        ja      @@error
        mov     cs:DAC_channel,bx
        mov     cs:ADC_channel,cx
        mov     ax,DRV_OK
@@error:
        pop     cx
        pop     bx
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
        lds     bx,[DACnum]
        mov     word ptr [bx],NUM_DAC
        lds     bx,[ADCnum]
        mov     word ptr [bx],NUM_ADC
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

;  -------------------------------- <<3>>
UnitUPI struc
  Unit    dw    ?
  Slot    dw    ?
  Channel dw    ?
ends

SETUPDAC proc pascal far
arg num:word, ptr:dword
      ret
endp SETUPDAC

SETUPADC proc pascal far
arg num:word, ptr:dword
      ret
endp SETUPADC

GETDIGIN proc pascal far
arg num:word, ptr:dword
      ret
endp GETDIGIN

GETDIGOUT proc pascal far
arg num:word, ptr:dword
      ret
endp GETDIGOUT

SETDIGOUT proc pascal far
arg num:word, ptr:dword
      ret
endp SETDIGOUT

GETMAXDIG proc pascal far
arg num:word, ptr:dword
      ret
endp GETMAXDIG



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

IFDEF Table3
        dw      offset SETUPDAC,0
        dw      offset SETUPADC,0
        dw      offset GETDIGIN,0
        dw      offset SETDIGOUT,0
        dw      offset GETDIGOUT,0
        dw      offset GETMAXDIG,0
ENDIF
        dd      0



; ========================================================= ;
; ================[ Внутренние процедуры ]================= ;
; ========================================================= ;

None proc
        iret
endp None


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

        putc    '<'

        mov     ax,[ScaleVal]
        mov     [Scale],ax

        PIPE_GetADC adc_word, NUM_ADC ;,ADC_channel
        PIPE_SetDAC dac_word, NUM_DAC ;DAC_channel,List_DAC


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

        putc    '>'
        mov     [flag],1
        dec     [use]

        pop     es
        pop     ds
        popa
        ret
endp RunProc


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
@@ok:
;      xor ax,ax
;      jmp       @@done
        call    DISABLE
        xor     ax,ax
@@done:
        pop     es
        pop     ds
        pop     si
        pop     di
        ret
endp

deinit proc far
        call    DISABLE
        call    CLRLOOPFUN
        xor     ax,ax
        ret
endp

WaitProc proc near
        nop
        ret
endp


PIPE_Declare

include startup.inc

DefaultCfg   proc
             ret
DefaultCfg   endp

LoadCfg      proc
                ret
LoadCfg      endp



        end entry
