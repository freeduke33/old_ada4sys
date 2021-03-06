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

; �᭮���� ��楤�� �⥭�� 䠩�� ���䨣��樨
; DS:BP = ���� �������� ���ᠭ�� ������⮢ ���䨣��樨
proc LoadCfg
     mov     ax,ds
     mov     [word ptr ParamPtr],AX
     mov     [word ptr ParamPtr+0],BP

     call    SetDefaultCfg
     call    ClearKnownFlg
; ���뢠�� 䠩�
     mov     AX,3d00h
     lea     DX,[DS:BP+cfgParam.FileName]
     int     21h
     jc      @@err
     mov     [DS:BP+cfgParam.fHandle],BX
; �⠥� ���䨣����
     mov     [ds:cfgParam.GrpDebth],0
     call    ParseGroup
     cmp     ax,0
     jne     @@done
     cmp     [ds:cfgParam.GrpDebth],0
     je      @@done
     mov     ax,errUnexpectedEof
@@done:
; ����뢠�� 䠩�
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

; �᭮���� ��楤�� ����� 䠩�� ���䨣��樨
proc SaveCfg
     ret
endp

; ��⠭�������� � ��� ��ࠬ��஢ ���祭�� �� 㬮�砭��
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

; ���뢠�� 䫠� �����⭮�� ���祭�� � ��� ������⮢ 蠡����
proc ClearKnownFlg
     ret
endp

; �������� � ��ࢮ� ����樨 ���� ����� ��ப�� �� 䠩�� ���䨣��樨
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
     jmp      @@rescan  ; �ய��⨬ ����� ������� ��ப�
@@shift:
     pop      di
     mov      ax,ds
     mov      es,ax
@@l2:     ; ��७�ᥬ ���⮪ (��砫� ����� ��ப�) � ��砫� ����
     mov      ax,BUFFER_SIZE
     sub      ax,cx
     rep movsb
     mov      cx,ax
     call     file2buf  ; ���⠥� � ���� �� ����
@@done:
     pop      ds
     ret
endp

; �⠥� �� 䠩�� � DS:SI CX ����
proc file2buf
     mov      bx,[DS:BP+cfgParam.fHandle]
     mov      ah,3Fh
     mov      dx,si
     int      21h
     ret
endp

; �ந������ ࠧ��� ��㯯� ��ࠬ��஢ �� 䠩�� ���䨣��樨
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

; �ந������ ࠧ��� ���� � ���������� ᮮ⢥������� ����権 ���䨣��樨
proc ParseItem
     clc       ; ��ப� �������� � ࠧ��࠭�
     ret
endp

; �ய�᪠�� ���������� ��㯯� �᫨ �� ������� ��㯯� ��㣮�� ���
proc SkipGroup
     mov      ax,errUnknownGrp
     cmp      [ds:cfgParam.GrpDebth],0 ; �᫨ ����� ��㯯� � �訡��
     jne      @@err

@@err:
     stc
@@done:
     ret
endp


; ���ଠ樮��� ��楤���
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

ParamPtr        dd      ?       ; 㪠��⥫� �� �������� ���ᠭ�� ���䨣��樨

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
