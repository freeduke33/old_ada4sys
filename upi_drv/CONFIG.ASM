.model tiny
.code
IDEAL
org 100h

include "struct.inc"



errUnexpectedEof        equ     -1
errNoParentEndGrp       equ     -2
errUnknownGrp           equ     -3


sample:
  mov   ax,cs
  mov   ds,ax
  mov   es,ax

  lea   bp,[parm]
  call  LoadCfg
  mov   ax,4c00h
  int   21h

; основная процедура чтения файла конфигурации
; DS:BP = адрес структуры описания элементов конфигурации
proc LoadCfg
     mov     ax,ds
     mov     [word ptr ParamPtr],AX
     mov     [word ptr ParamPtr+0],BP

     call    SetDefaultCfg
     call    ClearKnownFlg
; открываем файл
     mov     AX,3d00h
     lea     DX,[DS:BP+cfgParam.FileName]
     int     21h
     jc      @@err
     mov     [DS:BP+cfgParam.fHandle],BX
; читаем конфигурацию
     mov     [ds:cfgParam.GrpDebth],0
     call    ParseGroup
     cmp     ax,0
     jne     @@done
     cmp     [ds:cfgParam.GrpDebth],0
     je      @@done
     mov     ax,errUnexpectedEof
@@done:
; закрываем файл
     mov    ah,3eh
     mov    bx,[DS:BP+cfgParam.fHandle]
     int    21h
     clc
@@exit:
     push    ax
     pop     ax
     ret
@@err:
     call    SetDefaultCfg
     stc
     jmp     @@exit
endp

; основная процедура записи файла конфигурации
proc SaveCfg
     ret
endp

; устанавливает у всех параметров значения по умолчанию
proc SetDefaultCfg
     push       ds
     mov        cx,[DS:BP+cfgParam.CfgSize]
     les        di,[DS:BP+cfgParam.DefaultPtr]
     lds        si,[DS:BP+cfgParam.WorkPtr]
     cld
     rep movsb
     pop        ds
     ret
endp

; сбрасывает флаг известности значения у всех элементов шаблона
proc ClearKnownFlg
     ret
endp

; заполняет с первой позиции буфер новой строкой из файла конфигурации
proc ReadLine
     push     ds
@@rescan:
     mov      cx,BUFFER_SIZE
     lea      si,[DS:BP+cfgParam.Buf]
     push     si
@@l1:
     lodsb
     cmp      al,0Ah
     je       @@shift
     cmp      al,0Dh
     je       @@shift
     loop     @@l1
     pop      si
     mov      cx,BUFFER_SIZE
     call     file2buf
     jc       @@done
     jmp      @@rescan  ; пропустим конец длинной строки
@@shift:
     pop      di
     mov      ax,ds
     mov      es,ax
@@l2:     ; перенесем остаток (начало новой строки) в начало буфера
     mov      ax,BUFFER_SIZE
     sub      ax,cx
     rep movsb
     mov      cx,ax
     call     file2buf  ; дочитаем в буфер до конца
@@done:
     pop      ds
     ret
endp

; читает из файла в DS:SI CX байт
proc file2buf
     mov      bx,[DS:BP+cfgParam.fHandle]
     mov      ah,3Fh
     mov      dx,si
     int      21h
     ret
endp

; производит разбор группы параметров из файла конфигурации
proc ParseGroup
@@loop:
     call     ReadLine
     jc       @@done
     call     isRemark
     jnc      @@loop
     call     isGroup
     jc       @@noGrp
     call     isValidGroup
     jc       @@skipGrp
     call     ParseGroup
     jc       @@done
     jmp      @@loop
@@noGrp:
     call     isEndGroup
     jc       @@item
     mov      ax,errNoParentEndGrp
     stc
     jmp      @@done
@@skipGrp:
     call     SkipGroup
     jc       @@done
@@item:
     call     ParseItem
     jnc      @@loop
@@done:
     ret
endp

; производит разбор буфера и заполнение соответствующих позиций конфигурации
proc ParseItem
     clc       ; строка опознана и разобрана
     ret
endp

; пропускает неизвестную группу если это базовая группа другого УСО
proc SkipGroup
     mov      ax,errUnknownGrp
     cmp      [ds:cfgParam.GrpDebth],0 ; если внутри группы то ошибка
     jne      @@err

@@err:
     stc
@@done:
     ret
endp


; информационные процедуры
proc isGroup

     ret
endp

proc isEndGroup
     ret
endp

proc isValidGroup
     ret
endp

proc isRemark
     call     isBlankLine
     jnc      @@done
     clc
@@done:
     ret
endp

proc isBlankLine
     clc
     ret
endp

ParamPtr        dd      ?       ; указатель на структуру описания конфигурации

; =======================================================================

struc test_cfg
 NumDac         dw      ?
 NumAdc         dw      ?
 TypeADSP       dw      ?
ends

cfg1    test_cfg        <,,>
cfg2    test_cfg        <16,16,0>

parm  cfgParam <size cfg1,cfg1,cfg2,'upi.cfg',pic,,,,,>


pic  cfgHdr          <1,>
        cfgItemHdr      <'LCARD1230','G'>
        cfgItemGroup    <3,>
           cfgItemHdr      <'NUM_DAC','N'>
           cfgItemNumber   <16,1,1,cfg1.NumDac>
           cfgItemHdr      <'NUM_ADC','N'>
           cfgItemNumber   <16,1,1,cfg1.NumAdc>
           cfgItemHdr      <'ADSP','N'>
           cfgItemNumber   <2,1,1,cfg1.TypeADSP>




end sample
