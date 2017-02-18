/*------------------------------------------------------------*/
/* filename -       tvtext2.cpp                               */
/*                                                            */
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/*                                                            */
/*    Turbo Vision -  Version 1.0                             */
/*                                                            */
/*                                                            */
/*    Copyright (c) 1991 by Borland International             */
/*    All Rights Reserved.                                    */
/*                                                            */
/*------------------------------------------------------------*/

#define Uses_TEditWindow
#define Uses_TFileList
#define Uses_TProgram
#define Uses_MsgBox
#define Uses_TChDirDialog
#define Uses_TFileDialog
#define Uses_TFileInfoPane
#define Uses_TSystemError
#define Uses_TDeskTop
#include <tv.h>

#if !defined( __CTYPE_H )
#include <ctype.h>
#endif  // __CTYPE_H



static const char altCodesR[] =
    "ЙЦУКЕНГШЩЗХЪ\0\0ФЫВАПРОЛДЖЭ\0\0\0ЯЧСМИТЬБЮ";
static const char altCodes1[] =
    "QWERTYUIOP\0\0\0\0ASDFGHJKL\0\0\0\0\0ZXCVBNM";

static const char altCodes2[] = "1234567890-=";

#pragma warn -rng

char toupper(char ch)
{
 if(ch & 0x80)
    {
    if((unsigned)ch>='а' && (unsigned)ch<='п') ch-=0x10;
    else if((unsigned)ch>='р' && (unsigned)ch<='я') ch-=0x30;
    }
 else if(ch>='a' && ch<='z') ch&=~0x60;
 return ch;
}

char getAltChar(ushort keyCode)
{
    if ((keyCode & 0xff) == 0)
        {
        ushort tmp = (keyCode >> 8);

        if( tmp == 2 )
            return '\xF0';      // special case to handle alt-Space

        else if( tmp >= 0x10 && tmp <= 0x32 )
            return altCodes1[tmp-0x10];     // alt-letter

        else if( tmp >= 0x78 && tmp <= 0x83 )
            return altCodes2[tmp - 0x78];   // alt-number

        }
    return 0;
}

ushort getAltCode(char c)
{
    if( c == 0 )
        return 0;

    c = toupper(c);

    if( unsigned(c) == '\xF0' )
        return 0x200;       // special case to handle alt-Space

    for( int i = 0; i < sizeof( altCodes1 ); i++)
       if( altCodes1[i] == c )
        return (i+0x10) << 8;

    for( i = 0; i < sizeof( altCodes2); i++)
        if (altCodes2[i] == c)
            return (i+0x78) << 8;

    for( i = 0; i < sizeof( altCodesR); i++)
        if (altCodesR[i] == c)
            return (i+0x78) << 8;

    return 0;
}




#pragma warn .rng

const char * near TEditWindow::clipboardTitle = "Карман";
const char * near TEditWindow::untitled = "Безимянный";

const char * near TFileList::tooManyFiles = "Слишком много файлов.";

const char * near TProgram::exitText = "~Alt-X~ Выход";

const char * near MsgBoxText::yesText = "Да";
const char * near MsgBoxText::noText = "~Нет";
const char * near MsgBoxText::okText = "Закончить";
const char * near MsgBoxText::cancelText = "Прервать";
const char * near MsgBoxText::warningText = "Warning";
const char * near MsgBoxText::errorText = "Ошибка";
const char * near MsgBoxText::informationText = "Информация";
const char * near MsgBoxText::confirmText = "Подтверждение";

const char * near TChDirDialog::changeDirTitle = "Сменить каталог";
const char * near TChDirDialog::dirNameText = "Имя каталога";
const char * near TChDirDialog::dirTreeText = "Дерево каталога";
const char * near TChDirDialog::okText = "Закончить";
const char * near TChDirDialog::chdirText = "Каталог";
const char * near TChDirDialog::revertText = "Назад";
const char * near TChDirDialog::helpText = "Помощь";
const char * near TChDirDialog::drivesText = "Диски";
const char * near TChDirDialog::invalidText = "Неверный каталог";

const char * near TFileDialog::filesText = "Файлы";
const char * near TFileDialog::openText = "Открыть";
const char * near TFileDialog::okText = "Выполнить";
const char * near TFileDialog::replaceText = "Заменить";
const char * near TFileDialog::clearText = "Очистить";
const char * near TFileDialog::cancelText = "Прервать";
const char * near TFileDialog::helpText = "Помощь";
const char * near TFileDialog::invalidDriveText = "Неверный диск или каталог";
const char * near TFileDialog::invalidFileText = "Неверное имя файла";

const char * near TFileInfoPane::pmText = "p";
const char * near TFileInfoPane::amText = "a";
const char * const near TFileInfoPane::months[] =
    {
    "","Янв","Фев","Мар","Апр","Май","Июн",
    "Июл","Авг","Сен","Окт","Ноя","Дек"
    };

const char * const near TSystemError::errorString[] =
{
/*!!!!was    "Critical disk error on drive %c",
    "Disk is write-protected in drive %c",
    "Disk is not ready in drive %c",
    "Data integrity error on drive %c",
    "Seek error on drive %c",
    "Unknown media type in drive %c",
    "Sector not found on drive %c",
    "Printer out of paper",
    "Write fault on drive %c",
    "Read fault on drive %c",
    "Hardware failure on drive %c",
    "Bad memory image of FAT detected",
    "Device access error",
    "Insert diskette in drive %c"
!!!!!was*/
//!!!!!become
  "Диск %c зашищен от записи",
  "Критичиская ошибка на диске %c",     // unknown unit
  "Диск %c не готов к работе",
  "Критическая ошибка на диске %c",     // unknown command
  "Ошибка целостности данных на диске %c",    // bad CRC
  "Критическая ошибка на диске %c",     // bad request
  "Ошибка доступа к диску %c",
  "Неизвестный формат диска %c",
  "Сектор не найден на диске %c",
  "В принтере нет бумаги",
  "Ошибка при записи на диск %c",
  "Ошибка при чтении с диска %c",
  "Общая ошибка диска %c",
  "Ошибка при чтении FAT",
//!!!!!become
};

const char * near TSystemError::sRetryOrCancel = "~Enter~-Повторить, ~Esc~-Прервать";

const char near TDeskTop::defaultBkgrnd = '\xB0';
