; ================== Структуры для формирования шаблона ==================
struc cfgHdr
  ItemNum        dw     ?
  DisplayProc    dd     0                      ; печать текущей строки в файле
ends

struc cfgItemHdr
 iName           db     40 dup(0)                ; наименование параметра
 iKnown          db     0                        ; значение параметра определено
 iType           db     ?                        ; тип G,N,S,B,L
ends

struc cfgItemGroup
 iGroupLen       dw     ?
 iRunTimeOwner   dd     ?
ends

struc cfgItemNumber
 iNumberMax      dd     ?                        ;\
 iNumberMin      dd     ?                        ; > числовые значения
 iNumberStep     dd     ?                        ; > параметра
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
;===========< структура для загрузки/сохранения конфигурации системы
;=============\

BUFFER_SIZE     equ     256

struc cfgParam
 ; =======> описание конфигурации системы
 CfgSize      dw      ?                       ; длинна структуры конфигурации
 WorkPtr      dd      ?                       ; значения элементов конфигурации
 DefaultPtr   dd      ?                       ; умалчиваемые значения
 FileName     db     160 dup(0)               ; имя файла конфигурации
 Picture      dd      ?                       ; шаблон файла конфигурации
 ; =======> рабочие переменные
 Buf          db      BUFFER_SIZE dup(0)      ; буфер файла конфигурации
 BufPos       dw      ?                       ; текущая позиция в буфере
 GrpDebth     db      ?                       ; вложенность групп
 GrpPtr       dd      ?                       ; текущая группа
 fHandle      dw      ?                       ; описатель файла конфигурации
ends



