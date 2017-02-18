; 
; Driver structure:
; -------------------
;    {BOF}
;    .code
;    org 100h
;    jmp startup         ; !!! impotant
;         ....
;      user defined TSR part
;         ....
;    include startup.inc ; !!! impotant
;         ....
;      not TSR part user defined
;         ....
;    {EOF}


;
; ******** External definitions ********
; === in TSR part ===
;      proc init                ; initialize driver
;      proc deinit              ; deinitialize driver
;
;      dd   FuncTable           ; common function
;      db   ErrMsg              ; error description
;
; === not in TSR part ===
;      db   Title_msg           ; first message when started
;
; === macroses ===
;      equ  DRVname             ; DAC/ADC technical name
;      equ  NAMEstring          ; private string for this driver
;                               ; it's mast be terminated : 0,EOT
;      equ  NAMElen             ; length of private string without EOT
;      equ  NeedCFG             ; defined only if may be load config file




; INT 2Fh
IDstring        db      'Stend driver for DAC/ADC',0
IDlen           equ     $-offset IDstring
SubFun          db      0
old2F           dd      0

face proc
        cmp     ah,cs:SubFun
        jne     @@next
                        ; **** Presentation
        or      al,al
        jne     @@f1
        mov     ax,cs
        mov     [cs:ErrMsg+2],ax
        call    init
        mov     bx,cs
        mov     dx,offset IDstring
        mov     al,0ffh
        iret
@@f1:                   ; **** Initialize
        cmp     al,1
        jne     @@f2
        call    init
        jmp     @@end
@@f2:                   ; **** Deinitialize
        cmp     al,2
        jne     @@f3
        call    deinit
        jmp     @@end
@@f3:                   ; **** Get table of function's
        cmp     al,3
        jne     @@f4
        mov     bx,cs
        mov     dx,offset FuncTable
        jmp     @@okend
@@f4:
        cmp     al,4
        jne     @@f5
        mov     bx,cs
        mov     dx,offset NAMEstring
@@f5:                   ; **** Get error message
        cmp     al,5
        jne     @@end
        mov     bx,cs:[ErrMsg+2]
        mov     dx,cs:[ErrMsg+0]
@@okend:
        xor     ax,ax
@@end:
        iret
@@next:
        jmp     dword ptr cs:[old2F]
face endp


; ==========================[ END OF TSR ]==================================

_STAY           equ     4
_INVALID        equ     3
_ALREADY        equ     2
_REMOVE         equ     1
_HELP           equ     0

startup:
        print   Title_msg
        int     11h
        test    ax,1
        jnz     @@okFCPU
        print   noFCPU_msg
        jmp     short exit_prg
@@okFCPU:
        call    chk_tsr         ; test TSR in memory
        jc      exit_prg
        call    get_swith       ; get command
        jc      help
        cmp     ah,'r'
        jne     stay
        call    del_tsr
        jmp     short exit_prg
stay:

ifdef NeedCFG
        call    load_cfg
endif

        call    set_tsr
        jc      exit_prg
        lea     dx,startup
        int     27h
help:
        print   help_msg
exit_prg:
        mov     ax,4c00h
        int     21h

; ----------------------------[ procedures ]---------------------
get_swith proc
        xor    ah,ah
        mov      si,81h
@@loop:
        lodsb
        cmp   al,' '
        je    @@loop
        cmp   al,'/'
        je    @@swith
        cmp   al,0Dh
        je    @@done
@@err:
        mov   ax,_HELP
        stc
        ret
@@swith:
        lodsb
        or    al,20h
        cmp   al,'i'
        je    @@do
        cmp   al,'r'
        je    @@do
        cmp   al,'c'
        jne   @@err
; override name of configuration file
        lodsb
        cmp   al,':'
        jne   @@err
        lea   di,[cfgFile]
@@fl:
        lodsb
        cmp   al,' '
        je    @@eof
        cmp   al,'/'
        je    @@eof
        cmp   al,0Dh
        je    @@eof
        stosb
        jmp   @@fl
@@eof:
        xor   al,al
        stosb
        dec   si
        jmp   @@loop
@@do:
        mov   ah,al
        jmp   @@loop
@@done:
        cmp   ah,0
        je    @@err
        clc
        ret
get_swith endp

chk_tsr proc
        push    ds
        push    es
        mov     ax,cs
        mov     es,ax

        mov  ah,0ffh
        xor  al,al
@@loop:
        push ax
        int  2fh
        or   al,al
        pop  ax
        jnz  @@test
        mov  SubFun,ah
        jmp  short @@next
@@test:
        mov     ds,bx
        mov     si,dx
        lea     di,IDstring
        cld
        mov     cx,IDlen
        repz    cmpsb
        jnz      @@next
        mov      cs:inMemory,bx
        mov      cs:SubFun,ah
        jmp      short @@ok
@@next:
        dec     ah
        cmp     ah,0C0h
        jne     @@loop
@@ok:
        pop     es
        pop     ds

        cmp     SubFun,0
        jne     @@ret
        print   invalid_msg
        stc
        ret
@@ret:
        clc
        ret
chk_tsr endp

set_tsr proc
        cmp  inMemory,0
        je   @@ok
        print already_msg
        stc
        ret
@@ok:
        push    es
        print   install_msg
        mov     ax,352fh
        int     21h
        mov     word ptr [old2F+2],es
        mov     word ptr [old2F+0],bx
        mov     ah,25h
        lea     dx,face
        int     21h
        pop     es
        clc
        ret
set_tsr endp

del_tsr proc
        cmp  inMemory,0
        jne   @@ok
        print none_msg
        ret
@@unk:
        print unknown_msg
        mov     ah,SubFun
        mov     al,4
        int     2fh
        mov     ds,bx
        mov     ah,9
        int     21h
        mov     ax,cs
        mov     ds,ax
        print   unk_msg_end
        ret
@@ok:
        push    es
        mov     ah,SubFun
        mov     al,4
        int     2fh
        mov     es,bx
        mov     di,dx
        lea     si,NAMEstring
        cld
        mov     cx,NAMElen
        repz    cmpsb
        pop     es
        jnz     @@unk


        print remove_msg
        push    ds

        mov     bx,inMemory
        mov     ds,bx

; deinit installed copy
        push    cs
        mov     ax,offset @@ret
        push    ax
        push    ds
        mov     ax,offset deinit
        push    ax
        retf

; restore vectors
@@ret:
        mov     dx,word ptr [old2F+0]
        mov     ds,word ptr [old2F+2]
        mov     ax,252Fh
        int     21h
        pop     ds

; deallocate memory
        push    es
        mov     bx,inMemory
        mov     es,bx
        push    es
        mov     es,es:[2Ch]
        mov     ah,49h
        push    ax
        int     21h
        pop     ax
        pop     es
        int     21h
        pop     es
        ret
del_tsr endp

ifdef NeedCFG

load_cfg proc
         lea  dx,cfgFile
         mov  ax,3D00H
         int  21h
         jc   @@default
         mov  bx,ax
         mov  ax,4202h
         int  21h
         jc   @@default
         mov  cx,ax
         mov  ah,3fh
         lea  dx,filebuf
         int  21h
         jc   @@default
         mov  ah,3eh
         int  21h
         jmp  @@parse
@@default:
         print default_msg
         call  DefaultCfg
         stc
         ret
@@parse:
         call  LoadCfg
         clc
         ret
load_cfg endp

endif

Title_msg       db      'DAC/ADC driver for ',DRVname,' (real mode)',CR,NL
                db      'Copyright (C) ',??date,' by D. Kokorev, '
                db      'Samara-"Trial".'
                db      CR,NL,EOT
inMemory        dw      0
cfgFile         db      'upi.cfg',80 dup(0)
cfgTitle        db      '[',DRVname,']',0

help_msg        db      NL,CR,'Usage: '
                db      'C:\> ',??filename,' {/I|/R} [/C:config]',NL,CR
                db      'where:',NL,CR
                db      '  /I',TAB,TAB,'- install driver',NL,CR
                db      '  /R',TAB,TAB,'- remove driver',NL,CR
                db      '  /C:config',TAB,'- define configuration file,'
                db      'default is UPI.CFG',NL,CR,EOT
install_msg     db      'Driver for ',DRVname,' installed in memory.',NL,CR,EOT
remove_msg      db      'Driver for ',DRVname,' removed from memory.',NL,CR,EOT
already_msg     db      'Aborted:',BELL,' one UPI driver already installed.',NL,CR,EOT
none_msg        db      'Driver for ',DRVname,' can`t found in memory.',NL,CR,EOT
overlap_msg     db      'Driver for ',DRVname,' can`t removing from memory.',NL,CR,EOT
invalid_msg     db      BELL,'Error in installation process.',NL,CR,EOT
unknown_msg     db      'Sorry,',BELL,' but I can`t remove "',EOT
unk_msg_end     db      '"',NL,CR,'Please use own program for it.',NL,CR,EOT
noFCPU_msg      db      'Sorry,',BELL,' but I can`t work without FCPU.',NL,CR,EOT
default_msg     db      'I can`t load configuration file, used default cofiguration.',NL,CR,EOT
filebuf         db      27h

