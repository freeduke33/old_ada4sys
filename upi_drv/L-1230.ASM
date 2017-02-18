; 
.model TINY
LOCALS
P286N

RESET           =1     ;
NeedCFG         =1

include L-1230.inc ; Макросы управления LCard-1230

NUM_DAC         =       L1230_NUM_DAC
DAC_BIT         =       L1230_DAC_BIT
NUM_ADC         =       L1230_NUM_ADC
ADC_BIT         =       L1230_ADC_BIT
NUM_DIGOUT      =       L1230_NUM_OUT
NUM_DIGIN       =       L1230_NUM_IN

VERSIONtbl         =       3

include common.mac



 CODESEG
        org 100h
entry:
        jmp     startup

DRVname         equ     'LCard-1230 & Crait system'

TIMER_FREQ              dd      L1230_FREQ

COPYRIGHTstring         db      '(C) D.Kokorev Samara RU',0
NAMEstring              db      'Driver for ',DRVname,0,EOT
NAMElen                 equ     $-NAMEstring-1

ErrMsgOK        db      0
ErrMsgTest      db      'LCard-1230 can`t detected.',0


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

OldMask         db      0
IrqN            db      73h
IrqMaskBit      db      094h; 0F7h

Target_ADC      dw      NUM_ADC+1 dup(0) ; N канала и адрес модуля : 2300h+<Канал>*10h+<Слот>

Target0_DAC     dw      NUM_DAC+1 dup(0) ; адрес модуля : <Слот>*10h
Target1_DAC     dw      NUM_DAC+1 dup(0) ; N канала     : 0x8+ <Канал>


Target0_DIGIN   dw      30h,30h,30h,30h,30h,30h,30h,30h,30h,30h,NUM_DIGIN  dup(0) ; адрес модуля
Target1_DIGIN   db        0,  1,  2,  3,  4,  5,  6,  7,  8,  9,NUM_DIGIN  dup(0) ; N канала (бита)
Target0_DIGOUT  dw      40h,40h,40h,40h,40h,40h,40h,40h,40h,40h,NUM_DIGOUT dup(0) ; адрес модуля
Target1_DIGOUT  db        0,  1,  2,  3,  4,  5,  6,  7,  8,  9,NUM_DIGOUT dup(0) ; N канала (бита)
DIG_XREF        dw        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,NUM_DIGOUT dup(0) ; ссылка N канала -> индекс в буфере
DIG_WORD        dw      NUM_DIGOUT dup(0) ; буфер вых. каналов

setTimerScale   dw      10
setADSPSpeed    dw      1

include l-1230\upi_bios.hex


CtrlR           dw      30Ch
DataR           dw      300h
CmdR            dw      308h
FlagR           dw      30Ch
IrqResetR       dw      308h

OldHand         dw      2 dup(0)
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
;        rep stosw

        mov     ax,DivLo
        mov     bl,ah
        xor     ah,ah
        mov     DivLo,ax
        mov     al,bl
        inc     ax
        mov     [Scale],ax
        mov     [ScaleVal],ax

; --------------------------------- устанавливаем нужные каналы
        mov     ax, SetChannelCmd
        call    SendCommand
        mov     ax,DAC_channel
        call    PutData

;        jmp     @@DEBUG1

        lea     si,Target0_DAC
        mov     cx,DAC_channel
@@l1:
        lodsw
        call    PutData
        loop    @@l1

        lea     si,Target1_DAC
        mov     cx,DAC_channel
@@l2:
        lodsw
        call    PutData
        loop    @@l2

@@DEBUG1:
        mov     ax,ADC_channel
        call    PutData
        lea     si,Target_ADC

;        jmp     @@DEBUG2

        mov     cx,ADC_channel
@@l3:
        lodsw
        call    PutData
        loop    @@l3
@@DEBUG2:

; --------------------------------------------- пускаем управление
        L1230_ENABLE  DivHi,DivLo,RunProc

        cmp     al,0
        je      @@next
        mov     [ErrMsg],offset ErrMsgTest
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
        mov     [flag],0
@@loop: cmp     [flag],1
        jne     @@loop
        mov     [cs:use],1

        L1230_DISABLE

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
        push    ds

        mov     ax,cs
        mov     ds,ax

        mov     ax,DRV_ERRARG
        mov     bx,DACnum
        cmp     bx,NUM_DAC
        ja      @@error
        mov     cx,ADCnum
        cmp     cx,NUM_ADC
        ja      @@error
        mov     DAC_channel,bx
        mov     ADC_channel,cx

        mov     ax,DRV_OK
@@error:
        pop     ds
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

      push      ds
      push      es
      push      si
      push      di
      push      cx

      push    cs
      pop     es

      mov     bx,[num]
      add     bx,bx
      lds     si,[ptr] ; point to array[num] of UnitUPI;

      lea     di,Target1_DAC
      add     di,bx
      mov     ax,[si+Channel]
      or      ax,8h
      stosw

      lea     di,Target0_DAC
      add     di,bx
      mov     bx, [si+Unit] ; адрес модуля
      shl     bx, 10
      mov     ax, [si+Slot]
      shl     ax, 4
      or      ax, bx
      stosw

      mov     ax,0
      pop     cx
      pop     di
      pop     si
      pop     es
      pop     ds
      ret
endp SETUPDAC

SETUPADC proc pascal far
arg num:word, ptr:dword

      push      ds
      push      es
      push      si
      push      di
      push      cx

      push    cs
      pop     es

      mov     ax,[num]
      shl     ax,1
      lds     si,[ptr] ; point to UnitUPI;
      lea     di,Target_ADC
      add     di,ax

      mov     ax, [si+Unit] ; номер крейта
      shl     ax, 10
      mov     bx, [si+Channel] ; номер канала в модуле
      shl     bx, 4
      or      ax, bx
      or      ax, [si+Slot]; номер модуля
      mov     bx, 1                ; Kadrr
      shl     bx, 13
      or      ax, bx
      mov     bx, 11b              ; Usil
      shl     bx, 8
      or      ax, bx
      stosw

      mov     ax,0
      pop     cx
      pop     di
      pop     si
      pop     es
      pop     ds
      ret
endp SETUPADC

GETDIGIN proc pascal far
arg num:word, ptr:dword

      push   ds
      push   es
      push   si
      push   cx

      mov    ax,cs
      mov    ds,ax
      mov    es,ax

      mov    ax,GetDigital
      call   SendCommand

      mov    ax,[num]
      mov    si,ax
      mov    cl,[si+ offset Target1_DIGIN] ; бит
      push   cx
      add    si,ax
      mov    ax,[si+ offset Target0_DIGIN] ; адрес на плате

      call   PutData
      call   GetData
      pop    cx
;      shr    ax,cl
;      and    ax,1
      lds    si,[ptr]
      mov    [si],al

      pop    cx
      pop    si
      pop    es
      pop    ds
      ret
endp GETDIGIN

GETDIGOUT proc pascal far
arg num:word, ptr:dword

      ret
endp GETDIGOUT

SETDIGOUT proc pascal far
arg num:word, val:byte

      push      ds
      push      es
      pusha

      mov     ax,cs
      mov     ds,ax
      mov     es,ax

      mov     ax,[num] ; канал
      mov     si,ax
      mov     cl,[si+ offset Target1_DIGOUT] ; бит
      add     ax,si
      mov     si,ax
      mov     dx,[si+ offset Target0_DIGOUT] ; адрес на плате

      xor     ah,ah
      mov     al,[val]
      or      al,al
      jz      @@zero
      mov     al,1
@@zero:
      shl     ax,cl ; значащий бит
      mov     bx,ax
      mov     ax,1
      shl     ax,cl
      not     ax    ; маска

      mov     si,[si+ offset DIG_XREF]
      shl     si,1
      add     si,offset DIG_WORD
      and     ax,[si] ; текущее значение без рабочего бита
      or      ax,bx   ; новое значение
      mov     [si],ax

      push    ax
      push    dx

      mov     ax,SetDigital
      call    SendCommand ; put to digital

      pop     ax
      call    PutData    ; channel
      pop     ax
      call    PutData    ; data

      popa
      pop     es
      pop     ds
      mov     ax,0
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
        ; ------------------------------------------ <<3>>
        dw      offset SETUPDAC,0
        dw      offset SETUPADC,0
        dw      offset GETDIGIN,0
        dw      offset SETDIGOUT,0
        dw      offset GETDIGOUT,0
        dw      offset GETMAXDIG,0
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
        push    ax
        push    dx
        L1230_IRQRESET [cs:IrqResetR],0
        pop     dx
        pop     ax
        iret
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

        L1230_IRQRESET [IrqResetR],1

        L1230_GetADC adc_word, NUM_ADC ;,ADC_channel
        L1230_SetDAC dac_word, NUM_DAC ;DAC_channel,List_DAC


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
;      xor ax,ax
;      jmp       @@done

        mov     cx,[bios_cnt]
        inc     cx
        mov     si,offset bios_mem

        mov     dx,[CtrlR]
        mov     bx,[DataR]

        xor     ax,ax
        out     dx,al
        xchg    dx,bx
        lodsb
        out     dx,al
        mov     al,8
        xchg    dx,bx
        out     dx,al
        xchg    dx,bx

@@load:
        lodsb
        out     dx,al
        call    WaitProc
        call    WaitProc
        loop    @@load

        mov     cx,1000
@@ll:   add     ax,bx
        loop    @@ll

        call    TimeWaitData ; ждем завершения загрузки
        jc      @@done

        xor     ax,ax
        call    SendCommand

        call    TimeWaitData
        mov     bl,al
        call    TimeWaitData
        mov     ah,bl
        cmp     ax,6699h
        jne     @@done

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


L1230_Declare


include startup.inc

DefaultCfg   proc
             push cs
             pop  es

             mov  ax,10
             mov  [cs:setTimerScale],ax
             mov  ax,0
             mov  [cs:setADSPSpeed],ax
             jmp  @@l  ; ------------------------------------------------- !!!

             mov  cx,NUM_ADC
             lea  di,Target_ADC
             mov  ax,2301h   ; 1 крейт 1 модуль
             mov  dx,10h
@@l1:
             stosw
             add   ax,dx  ; 2,3,4... АЦП
             loop  @@l1

@@l:
             mov  cx,NUM_DAC
             lea  di,Target0_DAC
             mov  ax,0   ; 1 модуль 1 крайт
             repnz stosw

             mov  cx,NUM_DAC
             lea  di,Target1_DAC
             mov  ax,08h   ; 1 ЦАП
@@l2:
             stosw
             inc  ax       ; 2,3,4... ЦАП
             loop @@l2

             ret
DefaultCfg   endp

LoadCfg      proc
                lea     si,filebuf

                mov     cx,NUM_ADC
                ret
LoadCfg      endp



        end entry
