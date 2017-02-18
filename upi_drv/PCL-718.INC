;; =============[ Макросы работы с платой УСО типа PCL-718 ]===============
;;
;; Предопределения :
;; ------------------
;;  pcl718_BASEPORT  - номер базового порта I/O для платы
;;
;; Описание макросов :
;; -------------------
;;
;; *>>> pcl718_ENABLE <<<*
;;   Аргументы:
;;      RunProc - адрес функции чтения/записи УСО
;;      SavPtr  - адрес двойного слова для хранения старого вектора прерывания 0Fh
;;      Div1    - делитель 1 таймера
;;      Div2    - делитель 2 таймера
;;   Выход:
;;      AL=0    - все нормально
;;      AL=1    - плата не обнаружена
;;      AL=2    - плата в режиме неполярного сигнала
;;   Изменяет:
;;      AX,BX,DX,ES,int 0Fh
;;
;; *>>> pcl718_DISABLE <<<*
;;   Аргументы:
;;      savPtr -  адрес двойного слова для хранения старого вектора прерывания 0Fh
;;   Изменяет:
;;      AX,DX,DS
;;
;; *>>> pcl718_SetDAC <<<*
;;   Аргументы:
;;      Buf - буфер для хранения записываемых в ЦАП данных
;;      Chl - число рабочих каналов ЦАП
;;
;; *>>> pcl718_GetADC <<<*
;;   Аргументы:
;;      Buf - буфер для хранения считывамых с АЦП данных
;;      Chl - число рабочих каналов АЦП
;;


;; Число каналов ЦАП/АЦП
pcl718_NUM_DAC          equ     2
pcl718_NUM_ADC          equ     16

;; Разрядность каналов ЦАП/АЦП
pcl718_DAC_BIT          equ     4096
pcl718_ADC_BIT          equ     4096


pcl718_FREQ             equ     989680h

pcl718_BASEPORT        equ      300h


pcl718_ADC_Lo          =       pcl718_BASEPORT+0
pcl718_ADC_Hi          =       pcl718_BASEPORT+1

pcl718_MUX             =       pcl718_BASEPORT+2          ; beg - end ADC channel

pcl718_DAC_Lo          =       pcl718_BASEPORT+4          ; 0 DAC channel
pcl718_DAC_Hi          =       pcl718_BASEPORT+5

pcl718_STATUS          =       pcl718_BASEPORT+8
pcl718__EOC            =       10000000b       ; end of convertion
pcl718__UNI            =       01000000b       ; unipolar mode
pcl718__MUX            =       00100000b       ; differential mode indicator
pcl718__INT            =       00010000b       ; interrupt status
pcl718__CHANNEL        =       00001111b       ; channel mask

pcl718_CONTROL         =       pcl718_BASEPORT+9
pcl718__INTE           =       10000000b
pcl718__DMAE           =       00000100b
pcl718__ST_soft        =       00000000b
pcl718__ST_extern      =       00000010b
pcl718__ST_pacer       =       00000011b

pcl718_PACER           =       pcl718_BASEPORT+10

pcl718_TIMER_0         =       pcl718_BASEPORT+12
pcl718_TIMER_1         =       pcl718_BASEPORT+13
pcl718_TIMER_2         =       pcl718_BASEPORT+14
pcl718_TIMER_SET       =       pcl718_BASEPORT+15

pcl718_Wait macro
      jmp short $+2
      jmp short $+2
      jmp short $+2
endm


pcl718_ENABLE macro  RunProc,savPtr,BufTim,DivCount
local @@active,@@bipolar,@@wait_act,@@exit


        mov     dx,pcl718_CONTROL
        mov     al,70h
        out     dx,al

        pcl718_Wait

        in      al,dx
        cmp     al,70h
        je      @@active
        mov     al,1
        jmp     @@exit
@@active:
        mov     dx,pcl718_STATUS
        in      al,dx
        test    al,pcl718__UNI
        jz      @@bipolar
        mov     ax,2
        jmp     @@exit
@@bipolar:
        mov     dx,pcl718_MUX
        mov     al,[ADC_channel]
        dec     al
        shl     al,4
        out     dx,al

        mov     dx,pcl718_CONTROL
        xor     al,al
        out     dx,al

        pcl718_Wait

        mov     dx,pcl718_TIMER_1
        mov     ax,-1
        out     dx,al
        mov     al,ah
        out     dx,al

        mov     dx,pcl718_TIMER_SET
        mov     al,10111100b
        out     dx,al

        mov     dx,pcl718_TIMER_2
        mov     ax,-1
        out     dx,al
        mov     al,ah
        out     dx,al

        mov     dx,pcl718_PACER
        mov     al,0
        out     dx,al

        pcl718_Wait

        mov     dx,pcl718_STATUS
        out     dx,al

        pcl718_Wait

; --------------------[ активизировать плату PCL ]----------------------
        mov     ax,350Fh
        int     21h
        mov     [word ptr savPtr+0],es
        mov     [word ptr savPtr+2],bx

        lea     dx,[RunProc]
        mov     ax,250Fh
        int     21h


        mov     dx,pcl718_TIMER_SET
        mov     al,01111100b
;                         -  binary
;                      ---   divide by N count
;                    --      Lo first, then Hi counter bytes
;                  --        counter
        out     dx,al

        mov     dx,pcl718_TIMER_1
        mov     ax,DivCount
        out     dx,al
        mov     al,ah
        out     dx,al

        mov     dx,pcl718_TIMER_SET
        mov     al,10111100b
        out     dx,al

        mov     dx,pcl718_TIMER_2
        mov     ax,BufTim
        out     dx,al
        mov     al,ah
        out     dx,al

        mov     dx,pcl718_CONTROL
        mov     al,pcl718__INTE+pcl718__ST_pacer+ 70h
        out     dx,al

        pcl718_Wait

@@wait_act:
        mov     dx,pcl718_STATUS
        in      al,dx
        test    al,pcl718__INT
        jz      @@wait_act

        mov     dx,pcl718_PACER
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

        mov     dx,pcl718_STATUS
        mov     al,1
        out     dx,al
        mov     al,0
@@exit:
endm

pcl718_DISABLE macro savPtr
        cli
        mov     dx,pcl718_CONTROL
        mov     al,0
        out     dx,al

        in      al,21h
        jmp     $+2
        or      al,80h
        out     21h,al
        sti
        mov     dx,[word ptr savPtr+2]
        mov     ds,[word ptr savPtr+0]
        mov     ax,250Fh
        int     21h
endm


pcl718_SetDAC macro dac_word,dac_channel
local @@DACloop

        lea     si,dac_word
        xor     cl,cl
@@DACloop:
        lodsw
ifdef REVERSE
;       not     ax              ; !!!!!!!!!!!!!!!!!!! inverse signal
endif
        shl     ax,4
        or      al,cl
        mov     dx,pcl718_DAC_Lo
        out     dx,al
        inc     dx ;============== pcl718_DAC_Hi
        mov     al,ah
        out     dx,al

        inc     cl
        cmp     cl,DAC_channel
        jbe     @@DACloop

endm


pcl718_GetADC macro adc_word,ADC_channel
local @@ADCloop,@@wait3

; move MUX to first channel
        mov     dx,pcl718_MUX
        mov     al,ADC_channel
        dec     al
        shl     al,4
        out     dx,al

        mov     dx,pcl718_ADC_Lo
        mov     cl,ADC_channel
        xor     ch,ch
        lea     di,adc_word
@@ADCloop:
        out     dx,al           ; software trigger
        mov     dx,pcl718_STATUS
@@wait3:
        in      al,dx
        test    al,pcl718__EOC
        jnz     @@wait3
        mov     dx,pcl718_ADC_Hi
        in      al,dx
        mov     ah,al
        dec     dx
        in      al,dx
        shr     ax,4
        stosw
        loop    @@ADCloop

        mov     dx,pcl718_STATUS
        out     dx,al
        sti

endm

;; <<EOF>>