/*
  ��� 䠩��...: kran_on.c
  ����.......: ���筨��� �.�.
  ���........: 29.01.97
  �����......: ����祭�� �����ப࠭��
*/
  #include <conio.h>

  #include  <d4basem.h>
  #include  <w4m.h>
  #include  <g4charm.h>

  #include  "dv_sys.h"
  #include  "def_v.h"
  #include  "fun1230.h"

  int main(void)
{
    int i, j, Slot_402;

  SETBASEADDRESS(0x300);

  if(PLATA_TEST()) {
  clrscr(); printf("\n\n\n\n\n\n\n\n\n\n");
  printf("                     �������������������������������ͻ\n");
  printf("                     �  �� ��⠭������ ���� L-1230  �\n");
  printf("                     �     ������ ���� �������     �\n");
  printf("                     �������������������������������ͼ\n");
  getch();
              exit(1);
             }

// ���㫥��� ������� 0...7 ��� (LM-301)
   for(j = 0; j < 8; j++)
    if(GET_SLOT_CODE(0,j) == 20) //���� ���㫥� LM-301
    { for(i = 0; i < 8; i++) PROGRAM_LM_DAC(0,j,(i == 7)?8:0,0,i);}

//���� ����� LM-402 (��஢�� ��室)
  for(i=0; i<8; i++) if(GET_SLOT_CODE(0,i) == 0x1c) break;
  if(i == 8) {
  clrscr(); printf("\n\n\n\n\n\n\n\n\n\n");
  printf("                     ��������������������������ͻ\n");
  printf("                     �  �� ������ ����� LM402  �\n");
  printf("                     �  ������ ���� �������   �\n");
  printf("                     ��������������������������ͼ\n");
  getch();
/*
              w4display("","�� ������ ����� LM402",
                        "",
                        "","������ ���� �������",
                       (char *) 0);
*/
              exit(1);
             }
  Slot_402 = i;
  PROGRAM_LM_TTLOUT(0,Slot_402,0); //���.��.�࠭��

  EXIT:
  return 0 ;
}
