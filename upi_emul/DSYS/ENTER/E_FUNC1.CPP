
struct dataRec  {
  long Field1;   //TInputFloat
  };


#include "e_func1.hpp"

TDialog::TDialog() :
       TDialog(TRect(7, 7, 56, 14), "Линейное звено"),
       TWindowInit(&TDialog::initFrame)

{
#ifdef _BEFORE_DIALOG_
 BeforeDlg(); {
#endif
 TView *control;

 control = new TInputLine(TRect(34, 2, 46, 3), 11, 0, "Коэффициент усиления сигнала :");
 insert(control);

   insert(new TLabel(TRect(2, 2, 33, 3), "Коэффициент усиления сигнала :", control));

 control = new TButton(TRect(15, 4, 28, 6), "Запомнить", cmOK, bfDefault);
 insert(control);

 control = new TButton(TRect(29, 4, 41, 6), "Прервать", cmCancel, bfNormal);
 insert(control);

 selectNext(False);
#ifdef _AFTER_DIALOG_
 AfterDlg();
#endif
#ifdef _BEFORE_DIALOG_
 }
#endif
}

