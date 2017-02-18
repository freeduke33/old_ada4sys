; 
.model TINY
LOCALS
P286N

DT_ACTIVE       =1     ; Определенно, если активна Data Translation
RESET           =1     ;
;REVERSE         =1

include pcl-718.inc ; Макросы управления PCL-718
IFDEF DT_ACTIVE
include dt-2815.inc ; Макросы управления DT-1528
ENDIF

IFDEF DT_ACTIVE
NUM_DAC         =       dt2815_NUM_DAC
DAC_BIT         =       dt2815_DAC_BIT
ELSE
NUM_DAC         =       pcl718_NUM_DAC
DAC_BIT         =       pcl718_DAC_BIT
ENDIF

NUM_ADC         =       pcl718_NUM_ADC
ADC_BIT         =       pcl718_ADC_BIT

VERSIONtbl         =       2

include common.mac



 CODESEG
        org 100h
entry:
        jmp     startup

IFDEF DT_ACTIVE
DRVname         equ     'DT2815&PCL-718 8x16'
ELSE
DRVname         equ     'PCL-718 2x8'
ENDIF

TIMER_FREQ              dd      pcl718_FREQ

COPYRIGHTstring         db      '(C) D.Kokorev Samara RU',0
NAMEstring              db      'Driver for ',DRVname,0,EOT
NAMElen                 equ     $-NAMEstring-1

ErrMsgOK        db      0
IFDEF DT_ACTIVE
ErrMsgTimeOut   db      'Timeout in Data Translation',0
ErrMsgTest      db      'Data Translation test failed',0
ENDIF
ErrMsgUNIPOLAR  db      'PCL-718 in unipolar mode',0
ErrMsgNOPCL     db      'PCL-718 can`t installed',0






Active          db      0
savPtr          dd      0
SkipTik         dd      0
dac_word        dw      NUM_DAC+1 dup(0)
BufDAC          equ     dac_word
adc_word        dw      NUM_ADC+1 dup(0)
BufADC          equ     adc_word
flag            db      0
use             db      0
overflow        db      0
LoopFunc        dd      0
EmulFunc        dd      0
DAC_channel     db      NUM_DAC
ADC_channel     db      NUM_ADC

_ss                     dw      0
_sp                     dw      0
                        dw      4096 dup('Ss')
stackbuf                dw      0

; ================[ Инициализация платы ]===============
ENABLE proc pascal far
arg BufTim:word,DivCount:word
        push    ds
        push    es

        mov     ax,cs
        mov     ds,ax

        mov     ax,DRV_REINIT
        cmp     [Active],0
        je      @@ini
        jmp     @@exit

@@ini:
        lea     di,BufDAC
        push    cs
        pop     es
        xor     ax,ax
        mov     ch,al
        mov     cl,[DAC_channel]
        cld
        rep stosw

ifdef RESET

; ==========================================
        dt2815_ENABLE DAC_channel
        cmp     al,0
        je      @@next
        mov     [ErrMsg],offset ErrMsgTest
        cmp     al,1
        je      @@err
        mov     [ErrMsg],offset ErrMsgTimeOut
@@err:
        mov     ax,DRV_FAILED
        jmp     @@exit
@@next:

endif

        pcl718_ENABLE RunProc,savPtr,BufTim,DivCount
        cmp     al,0
        je      @@ok
        mov     [ErrMsg],offset ErrMsgNOPCL
        cmp     al,1
        je      @@t1
        mov     [ErrMsg],offset ErrMsgUNIPOLAR
@@t1:
        mov     ax,DRV_FAILED
        jmp     short @@exit
@@ok:
        mov     [Active],1
        mov     ax,DRV_OK
@@exit:
        pop     es
        pop     ds
        ret
endp ENABLE


; ================[ Деинициализация платы ]==============
DISABLE proc pascal far
        push    ds
        cmp     [cs:Active],0
        je      @@exit

        pcl718_DISABLE savPtr
        dt2815_DISABLE

        mov     [cs:Active],0
@@exit:
        pop     ds
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
        mov      [overflow],0
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
@@loop: cmp     [cs:flag],1
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
        mov     cs:DAC_channel,bl
        mov     cs:ADC_channel,cl
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
        push    ds
        push    es
        mov     ax,cs
        mov     ds,ax
        mov     es,ax

        mov     al,[overflow]
        cbw
        add     word ptr [SkipTik],ax
        adc     word ptr [SkipTik+2],0
        mov     [overflow],1

        mov     al,20h
        out     20h,al
        cld
        cli
; ========================== Data Translation ==============
IFDEF DT_ACTIVE
        dt2815_SetDAC dac_word,DAC_channel
ELSE
        pcl718_SetDAC dac_word,DAC_channel
ENDIF
        pcl718_GetADC adc_word,ADC_channel

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

        mov     [flag],1
        dec     [use]

        pop     es
        pop     ds
        popa
        iret
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
IFDEF DT_ACTIVE
        dt2815_ENABLE DAC_channel
        cmp     al,0
        je      @@exit
        mov     [ErrMsg],offset ErrMsgTest
        cmp     al,1
        je      @@err
        mov     [ErrMsg],offset ErrMsgTimeOut
@@err:
        mov     ax,DRV_FAILED
@@exit:
ELSE
        xor     ax,ax
ENDIF
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

include startup.inc


        end entry
