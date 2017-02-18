.Model Large,pascal
.286
IDEAL

public _INSTALLDRIVER,_EMULERRMSG
public _ENABLEDRIVER,_DISABLEDRIVER
public _ENABLETIMER,_DISABLETIMER,_WAITTIMER


CODESEG

TIMERINT        equ     08h

ctrl            dw      0

IDstring        db      'Stend driver for DAC/ADC',0
IDlen           equ     $-offset IDstring
SubFun          db      0
inMemory        dw      0
FuncTable       dd      0
LoopFun         dd      0
_EMULERRMSG     dd      0
ErrMsg          equ     _EMULERRMSG
old2F           dd      0
oldTimer        dd      0
NameString      dd      0

TimeDivHi       dw      0
TimeDivLo       dw      0

wFlag           db      0
inUse           db      0
_ss             dw      0
_sp             dw      0
_fpuInt         dd      12 dup(0)

; ==========================[ ]=============================

isFace  db      0

macro putc ch
      push ax
      mov  al,ch
      mov  ah,0Eh
      int  10h
      pop  ax
endm



proc _ENABLEDRIVER far
arg TablPtr:dword,control:word

        push    es
        push    ds
        pusha
        push    cs
        pop     ds
        cmp     [isFace],0
        jne     @@exit
        inc     [isFace]

        push    [control]
        pop     [ctrl]
        push    [TablPtr]
        pop     [FuncTable]
        xor     ax,ax
        mov     [inMemory],ax
        call    near ptr chk_tsr
        jc      @@exit
        cmp     [inMemory],0
        jne     @@exit
        mov     ax,352fh
        int     21h
        mov     [word ptr cs:old2F+2],es
        mov     [word ptr cs:old2F+0],bx
        mov     ah,25h
        lea     dx,[face]
        int     21h
@@exit:
        popa
        mov     ax,[cs:inMemory]
        pop     ds
        pop     es
        ret
endp _ENABLEDRIVER

proc _DISABLEDRIVER far
        push    ds
        push    dx
        push    cs
        pop     ds
        cmp     [isFace],0
        je      @@exit
        dec     [isFace]

        call    far _DISABLETIMER
        mov     dx,[word ptr old2F+0]
        mov     ds,[word ptr old2F+2]
        mov     ax,252Fh
        int     21h
@@exit:
        pop     dx
        pop     ds
        ret
endp _DISABLEDRIVER

; INT 2Fh

proc face
        cmp     ah,[cs:SubFun]
        jne     @@next
                        ; **** Presentation
        or      al,al
        jne     @@f1
        mov     ax,cs
        mov     [word ptr cs:ErrMsg+2],ax
;       call    init
        mov     bx,cs
        mov     dx,offset IDstring
        mov     al,0ffh
        iret
@@f1:                   ; **** Initialize
        cmp     al,1
        jne     @@f2
        mov     al,0
        mov     [cs:inUse],0
;       call    init
        jmp     @@end
@@f2:                   ; **** Deinitialize
        cmp     al,2
        jne     @@f3
;       call    deinit
        jmp     @@end
@@f3:                   ; **** Get table of function's
        cmp     al,3
        jne     @@f4
        mov     bx,[word ptr cs:FuncTable+2]
        mov     dx,[word ptr cs:FuncTable+0]
        jmp     @@end
@@f4:
        cmp     al,4
        jne     @@f5
        mov     bx,[word ptr cs:NAMEstring+2]
        mov     dx,[word ptr cs:NAMEstring+0]
@@f5:                   ; **** Get error message
        cmp     al,5
        jne     @@end
        mov     bx,[word ptr cs:ErrMsg+2]
        mov     dx,[word ptr cs:ErrMsg+0]
@@end:
        xor     ax,ax
        iret
@@next:
        jmp     [dword ptr cs:old2F]
endp face


proc chk_tsr near
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
        mov  [SubFun],ah
        jmp  short @@next
@@test:
        mov     ds,bx
        mov     si,dx
        lea     di,[IDstring]
        cld
        mov     cx,[word ptr IDlen]
        repz    cmpsb
        jnz      @@next
        mov      [cs:inMemory],bx
        mov      [cs:SubFun],ah
        jmp      short @@ok
@@next:
        dec     ah
        cmp     ah,0C0h
        jne     @@loop
@@ok:
        pop     es
        pop     ds

        cmp     [SubFun],0
        jne     @@ret
        stc
        ret
@@ret:
        clc
        ret
endp chk_tsr

; ========================[ ]=========================
isTimer         db      0

proc _ENABLETIMER far
arg DivHi:word,DivLo:word,FuncPtr:dword

        push    ds
        push    es
        pusha
        push    cs
        pop     ds



        cmp     [isTimer],0
        jne     @@exit
        inc     [isTimer]

        push    [FuncPtr]
        pop     [LoopFun]
        push    [DivHi]
        pop     [TimeDivHi]
        push    [DivLo]
        pop     [TimeDivLo]

        cmp     [cs:ctrl],0
        je      @@exit

        mov     ax,3500h+TIMERINT
        int     21h
        mov     [word ptr oldTimer+2],es
        mov     [word ptr oldTimer+0],bx
        mov     ah,25h
        lea     dx,[timer]
        int     21h

; программирование таймера

@@exit:
        popa
        pop     es
        pop     ds
        ret
endp _ENABLETIMER

proc _DISABLETIMER far
        pusha
        push    es
        push    ds

        cmp     [cs:isTimer],0
        je      @@exit
        dec     [cs:isTimer]

; восстановление таймера в норму

        cmp     [cs:ctrl],0
        je      @@exit

        mov     dx,[word ptr cs:oldTimer+0]
        mov     ds,[word ptr cs:oldTimer+2]
        mov     ax,2500h+TIMERINT
        int     21h

@@exit:
        pop     ds
        pop     es
        popa
        ret
endp _DISABLETIMER

proc _WAITTIMER far
        cmp     [cs:inUse],0
        jne     @@exit
        inc     [cs:inUse]
        cmp     [word ptr cs:LoopFun],0
        je      @@skipfunc

        pushf
        call    [dword ptr cs:LoopFun]

@@skipfunc:
        mov     [cs:wFlag],0
        dec     [cs:inUse]
@@exit:
        xor     ax,ax
        ret


;        pusha
;        push    es
;        push    ds
;
;        mov     [cs:wFlag],1
;
;        mov     ax,3500h+TIMERINT
;        int     21h
;        cmp     bx,offset timer
;        jne     @@nowait
;        mov     ax,es
;        mov     bx,cs
;        cmp     ax,bx
;        jne     @@nowait
;
;        sti
;@@loop:
;        cmp     [cs:wFlag],0
;        jne     @@loop
;
;@@nowait:
;        pop     ds
;        pop     es
;        popa
;        mov     al,[cs:wFlag]
;        mov     ah,al
;        ret
endp _WAITTIMER


proc timer
        pushf
        call    [dword ptr cs:oldTimer]

        push    ax
        cmp     [cs:inUse],0
        jne     @@exit
        inc     [cs:inUse]
        cmp     [word ptr cs:LoopFun],0
        je      @@skipfunc
        mov     [cs:_ss],ss
        mov     [cs:_sp],sp
        mov     ax,seg stackbuf
        mov     ss,ax
        mov     sp,offset stackbuf
;        call    near swapFPU


        pushf
        call    [dword ptr cs:LoopFun]

;        call    near swapFPU
        mov     ax,[cs:_ss]
        mov     ss,ax
        mov     sp,[cs:_sp]
@@skipfunc:
        mov     [cs:wFlag],0
        dec     [cs:inUse]
@@exit:
        pop     ax
        iret
endp timer


proc swapFPU near
        cli
        push    es
        push    si
        push    di
        push    cx

        mov     cx,22
        xor     ax,ax
        mov     es,ax
        mov     di,34h*4
        lea     si,[_fpuInt]
@@loop:
        mov     ax,[es:di]
        xchg    ax,[cs:si]
        stosw
        lodsw   ; SI+=2
        loop    @@loop

        pop     cx
        pop     di
        pop     si
        pop     es
        sti
        ret
endp swapFPU

proc _INSTALLDRIVER
        cli
        push    es
        push    ds
        push    si
        push    di
        push    cx
        xor     ax,ax
        mov     ds,ax
        mov     si,34h*4
        lea     di,[_fpuInt]
        push    cs
        pop     es
        mov     cx,22
        rep     movsw
        pop     cx
        pop     di
        pop     si
        pop     ds
        pop     es
        sti
        ret
endp _INSTALLDRIVER


ends

segment dstack private 'FAR_DATA'
                dw      1024*30 dup('Ss')
stackbuf        db      0
ends dstack


end

