; ================== �������� ��� �ନ஢���� 蠡���� ==================
struc cfgHdr
  ItemNum        dw     ?
  DisplayProc    dd     0                      ; ����� ⥪�饩 ��ப� � 䠩��
ends

struc cfgItemHdr
 iName           db     40 dup(0)                ; ������������ ��ࠬ���
 iKnown          db     0                        ; ���祭�� ��ࠬ��� ��।�����
 iType           db     ?                        ; ⨯ G,N,S,B,L
ends

struc cfgItemGroup
 iGroupLen       dw     ?
 iRunTimeOwner   dd     ?
ends

struc cfgItemNumber
 iNumberMax      dd     ?                        ;\
 iNumberMin      dd     ?                        ; > �᫮�� ���祭��
 iNumberStep     dd     ?                        ; > ��ࠬ���
 iNumberPtr      dd     ?                        ;/
ends

struc cfgItemString
 iStringLen      dw     ?
 iStringPtr      dd     ?
ends

struc cfgItemBoolean
 iBooleanPtr     dd     ?
ends

struc cfgItemList
 iLstType        db     ?
 iLstMaxLen      dw     ?
 iLstPtr         dd     ?
ends



;=============/
;===========< ������� ��� ����㧪�/��࠭���� ���䨣��樨 ��⥬�
;=============\

BUFFER_SIZE     equ     256

struc cfgParam
 ; =======> ���ᠭ�� ���䨣��樨 ��⥬�
 CfgSize      dw      ?                       ; ������ �������� ���䨣��樨
 WorkPtr      dd      ?                       ; ���祭�� ������⮢ ���䨣��樨
 DefaultPtr   dd      ?                       ; 㬠�稢���� ���祭��
 FileName     db     160 dup(0)               ; ��� 䠩�� ���䨣��樨
 Picture      dd      ?                       ; 蠡��� 䠩�� ���䨣��樨
 ; =======> ࠡ�稥 ��६����
 Buf          db      BUFFER_SIZE dup(0)      ; ���� 䠩�� ���䨣��樨
 BufPos       dw      ?                       ; ⥪��� ������ � ����
 GrpDebth     db      ?                       ; ����������� ��㯯
 GrpPtr       dd      ?                       ; ⥪��� ��㯯�
 fHandle      dw      ?                       ; ����⥫� 䠩�� ���䨣��樨
ends



