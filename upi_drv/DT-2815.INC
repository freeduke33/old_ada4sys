;; ============[ Макросы для работы с платой УСО типа DT-2815 ]=============
;;
;; Описание макросов:
;; -------------------
;;
;; *>>> dt2815_ENABLE <<<*
;;    Аргументы:
;;       Chl - число активных каналов АЦП
;;    Выход:
;;       AL=0  - все нормально
;;       AL=1  - не проходят тесты платы
;;       AL=2  - таймаут при инициализации платы
;;    Изменения:
;;       AX,BX,CX,DX
;;
;; *>>> dt2815_DISABLE <<<*
;;
;; *>>> dt2815_SetDAC <<<*
;;    Аргументы:
;;       Buf - буфер для хранения записываемых в ЦАП данных
;;       Chl - число активных каналов
;;



dt2815_NUM_DAC          equ     8
dt2815_NUM_ADC          equ     0

dt2815_DAC_BIT          equ     4096

dt2815_STATUS           equ     225h
dt2815__IBF            =       00000010b
dt2815__F0             =       00000100b

dt2815_Wait macro
       jmp  $+2
       jmp  $+2
       jmp  $+2
endm

;
; 1 - нет платы
; 2 - ошибка инициализации
dt2815_ENABLE macro DAC_channel
local @@beg_init,@@ini,@@ini_loop,@@reset,@@timeout,@@init,@@exit
; ==================[ DT2815 ]==============
        mov     bx,4
        mov     dx,dt2815_STATUS
@@beg_init:
        mov     cx,-1
@@ini:
        dt2815_Wait

        in      al,dx
        test    al,dt2815__F0  ; ready to read control word
        jz      @@ini_loop

        and     al,11100001b    ; get result of test
        jz      @@init
        dec     bx
        jnz     @@reset

        mov     ax,1
        jmp     @@exit
@@ini_loop:
        loop    @@ini

        dec     bx
        jz      @@timeout
@@reset:
        out     dx,al         ; reset

        dt2815_Wait

        jmp     @@beg_init

@@timeout:
        mov     ax,2
        jmp     @@exit

@@init:
        dec     dx
        mov     al,[DAC_channel]
        dec     al
        and     al,00000111b
        or      al,00001000b
;                       ---    last channel
;                     --       output mode
;                  ---         reserved
        out     dx,al         ; init
        xor     ax,ax
@@exit:

endm

dt2815_DISABLE macro
;; disable for DT-2815 is noting
endm

dt2815_SetDAC macro dac_word,DAC_channel
local @@DACloop,@@wait,@@wait1

        lea     si,[dac_word]
        mov     ch,[DAC_channel]
        xor     cl,cl
@@DACloop:
        mov     dx,dt2815_STATUS
@@wait:
        in      al,dx
        test    al,dt2815__IBF
        jnz     @@wait

        lodsw
ifdef REVERSE
        not     ax              ; !!!!!!!!!!!!! inverse volume
endif
        shl     ax,3
        or      al,cl           ; !!!!!!!!!!!!! DAC channel
        shl     ax,1
        dec     dx
        out     dx,al

        inc     dx
@@wait1:
        in      al,dx
        test    al,dt2815__IBF
        jnz     @@wait1

        dec     dx
        mov     al,ah
        out     dx,al

        inc     cl
        cmp     cl,ch
        jbe     @@DACloop
endm

;; <<EOF>>

