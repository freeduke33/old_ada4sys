typedef struct {
 int ISO;             /* ⨯ 䨧��᪮� ����稭�  */
 float Const,Delt;    /* ��ॢ�� � 䨧��᪨� ����稭� */
 float MaxVol,MinVol; /* ࠧ�襭�� �������� */
} Tar;

// �⮡ࠦ���� �����᪮�� ���稪� �� 䨧��᪨� ���/���
// � ��஢��� �����樥���
int LoadHardwareMap(int chl,int type,int *slot,int *num,Tar *);
