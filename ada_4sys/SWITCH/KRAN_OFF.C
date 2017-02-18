/*
  Имя файла...: kran_off.c
  Автор.......: Ключников В.А.
  Дата........: 29.01.97
  Задача......: Выключение электрокранов
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
  printf("                     ╔═══════════════════════════════╗\n");
  printf("                     ║  Не установлена плата L-1230  ║\n");
  printf("                     ║     нажмите любую клавишу     ║\n");
  printf("                     ╚═══════════════════════════════╝\n");
  getch();
              exit(1);
             }

// обнуление каналов 0...7 ЦАП (LM-301)
   for(j = 0; j < 8; j++)
    if(GET_SLOT_CODE(0,j) == 20) //поиск модулей LM-301
    { for(i = 0; i < 8; i++) PROGRAM_LM_DAC(0,j,(i == 7)?8:0,0,i);}

//поиск модуля LM-402 (цифровой выход)
  for(i=0; i<8; i++) if(GET_SLOT_CODE(0,i) == 0x1c) break;
  if(i == 8) {
  clrscr(); printf("\n\n\n\n\n\n\n\n\n\n");
  printf("                     ╔══════════════════════════╗\n");
  printf("                     ║  Не найден модуль LM402  ║\n");
  printf("                     ║  нажмите любую клавишу   ║\n");
  printf("                     ╚══════════════════════════╝\n");
  getch();
/*
              w4display("","Не найден модуль LM402",
                        "",
                        "","нажмите любую клавишу",
                       (char *) 0);
*/
              exit(1);
             }
  Slot_402 = i;
  PROGRAM_LM_TTLOUT(0,Slot_402,63); //выкл.эл.кранов

  EXIT:
  return 0 ;
}
