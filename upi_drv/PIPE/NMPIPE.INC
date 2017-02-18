PIPE_FREQ            EQU     10000000     ; 10 МГц
PIPE_NUM_DAC         EQU     32
PIPE_NUM_ADC         EQU     32
PIPE_DAC_BIT         EQU     4096
PIPE_ADC_BIT         EQU     4096


PIPE_ENABLE macro RateHi,RateLo,Vector
local done

        push    cs
        pop     ds

;; проверяем присутствие
        lea     dx,pipe_name
        mov     bx,0
        mov     cx,100
        mov     ax,5F38h
        int     21h
        jc      done

;; открываем PIPE
        lea     dx,pipe_name
        mov     ax,3d02h
        int     21h
        jc      done
        mov     pipe_handle,ax

        mov     bx,ax
        mov     ax,5334h
        mov     cx,8100h
        int     21h

        putc    '!'
        mov     ax,0
        clc
done:
endm

PIPE_DISABLE macro
local done

        push    cs
        pop     ds
        mov     bx,pipe_handle
        or      bx,bx
        jz      done

        mov     ah,3eh
        int     21h
done:
        xor     ax,ax
        mov     pipe_handle,ax
endm



PIPE_SetDAC macro dac_word,channels
local done

       mov      ax,cs
       mov      ds,ax

       mov     bx,pipe_handle
       or      bx,bx
       jz      done

       mov      ah,40h
       mov      bx,pipe_handle
       mov      cx,2*PIPE_NUM_DAC
       lea      dx,dac_word
       int      21h
done:
endm

PIPE_GetADC macro adc_word,channels
local done

       mov      ax,cs
       mov      ds,ax

       mov     bx,pipe_handle
       or      bx,bx
       jz      done

       mov      ah,3fh
       mov      bx,pipe_handle
       mov      cx,2*PIPE_NUM_ADC
       lea      dx,adc_word
       int      21h
done:
endm



PIPE_Declare macro
;;
pipe_name       db      '\PIPE\UPI_Emulator',0
pipe_handle     dw      0

;;
;;
endm


