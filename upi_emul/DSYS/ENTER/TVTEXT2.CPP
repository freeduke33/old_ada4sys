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
    "������������\0\0�����������\0\0\0���������";
static const char altCodes1[] =
    "QWERTYUIOP\0\0\0\0ASDFGHJKL\0\0\0\0\0ZXCVBNM";

static const char altCodes2[] = "1234567890-=";

#pragma warn -rng

char toupper(char ch)
{
 if(ch & 0x80)
    {
    if((unsigned)ch>='�' && (unsigned)ch<='�') ch-=0x10;
    else if((unsigned)ch>='�' && (unsigned)ch<='�') ch-=0x30;
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

const char * near TEditWindow::clipboardTitle = "��ଠ�";
const char * near TEditWindow::untitled = "�����ﭭ�";

const char * near TFileList::tooManyFiles = "���誮� ����� 䠩���.";

const char * near TProgram::exitText = "~Alt-X~ ��室";

const char * near MsgBoxText::yesText = "��";
const char * near MsgBoxText::noText = "~���";
const char * near MsgBoxText::okText = "��������";
const char * near MsgBoxText::cancelText = "��ࢠ��";
const char * near MsgBoxText::warningText = "Warning";
const char * near MsgBoxText::errorText = "�訡��";
const char * near MsgBoxText::informationText = "���ଠ��";
const char * near MsgBoxText::confirmText = "���⢥ত����";

const char * near TChDirDialog::changeDirTitle = "������� ��⠫��";
const char * near TChDirDialog::dirNameText = "��� ��⠫���";
const char * near TChDirDialog::dirTreeText = "��ॢ� ��⠫���";
const char * near TChDirDialog::okText = "��������";
const char * near TChDirDialog::chdirText = "��⠫��";
const char * near TChDirDialog::revertText = "�����";
const char * near TChDirDialog::helpText = "������";
const char * near TChDirDialog::drivesText = "��᪨";
const char * near TChDirDialog::invalidText = "������ ��⠫��";

const char * near TFileDialog::filesText = "�����";
const char * near TFileDialog::openText = "������";
const char * near TFileDialog::okText = "�믮�����";
const char * near TFileDialog::replaceText = "��������";
const char * near TFileDialog::clearText = "������";
const char * near TFileDialog::cancelText = "��ࢠ��";
const char * near TFileDialog::helpText = "������";
const char * near TFileDialog::invalidDriveText = "������ ��� ��� ��⠫��";
const char * near TFileDialog::invalidFileText = "����୮� ��� 䠩��";

const char * near TFileInfoPane::pmText = "p";
const char * near TFileInfoPane::amText = "a";
const char * const near TFileInfoPane::months[] =
    {
    "","���","���","���","���","���","��",
    "��","���","���","���","���","���"
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
  "��� %c ���饭 �� �����",
  "����᪠� �訡�� �� ��᪥ %c",     // unknown unit
  "��� %c �� ��⮢ � ࠡ��",
  "����᪠� �訡�� �� ��᪥ %c",     // unknown command
  "�訡�� 楫��⭮�� ������ �� ��᪥ %c",    // bad CRC
  "����᪠� �訡�� �� ��᪥ %c",     // bad request
  "�訡�� ����㯠 � ���� %c",
  "��������� �ଠ� ��᪠ %c",
  "����� �� ������ �� ��᪥ %c",
  "� �ਭ�� ��� �㬠��",
  "�訡�� �� ����� �� ��� %c",
  "�訡�� �� �⥭�� � ��᪠ %c",
  "���� �訡�� ��᪠ %c",
  "�訡�� �� �⥭�� FAT",
//!!!!!become
};

const char * near TSystemError::sRetryOrCancel = "~Enter~-�������, ~Esc~-��ࢠ��";

const char near TDeskTop::defaultBkgrnd = '\xB0';
