
#define Uses_TScreen
#define Uses_TRadioButtons
#define Uses_TMenuBox
#define Uses_TFrame
#define Uses_TIndicator
#define Uses_THistory
#define Uses_TColorSelector
#define Uses_TMonoSelector
#define Uses_TColorDialog
#define Uses_TInputLine
#define Uses_TStatusLine
#define Uses_TCheckBoxes
#define Uses_TScrollBar
#define Uses_TButton
#define Uses_TDirListBox
#define Uses_TFileEditor
#include <tv.h>


const char * near TRadioButtons::button = " ( ) ";

const char * near TMenuBox::frameChars = " \332\304\277  \300\304\331  \263 \263  \303\304\264 ";

const char near TFrame::initFrame[19] =
  "\x06\x0A\x0C\x05\x00\x05\x03\x0A\x09\x16\x1A\x1C\x15\x00\x15\x13\x1A\x19";

char near TFrame::frameChars[33] =
    "   � ��� �������   � ��� ��ϻ�� "; // for UnitedStates code page

const char * near TFrame::closeIcon = "[~\xFE~]";
const char * near TFrame::zoomIcon = "[~\x18~]";
const char * near TFrame::unZoomIcon = "[~\x12~]";
const char * near TFrame::dragIcon = "~��~";

const char near TIndicator::dragFrame = '\xCD';
const char near TIndicator::normalFrame = '\xC4';

const char * near THistory::icon = "\xDE~\x19~\xDD";

const char near TColorSelector::icon = '\xDB';

const char * near TMonoSelector::button = " ( ) ";
const char * near TMonoSelector::normal = "��ଠ���";
const char * near TMonoSelector::highlight = "�뤥����";
const char * near TMonoSelector::underline = "����ભ���";
const char * near TMonoSelector::inverse = "�������";

const char * near TColorDialog::colors = "����";
const char * near TColorDialog::groupText = "��㯯�";
const char * near TColorDialog::itemText = "�������";
const char * near TColorDialog::forText = "�㪢�";
const char * near TColorDialog::bakText = "��� �㪢";
const char * near TColorDialog::textText = "�����";
const char * near TColorDialog::colorText = "���⭮�";
const char * near TColorDialog::okText = "��������";
const char * near TColorDialog::cancelText = "��ࢠ��";

const char near TInputLine::rightArrow = '\x10';
const char near TInputLine::leftArrow = '\x11';

const char * near TStatusLine::hintSeparator = "\xB3 ";

const char * near TCheckBoxes::button = " [ ] ";

TScrollChars near TScrollBar::vChars = {30, 31, 177, 254, 178};
TScrollChars near TScrollBar::hChars = {17, 16, 177, 254, 178};

const char * near TButton::shadows = "\xDC\xDB\xDF";
const char * near TButton::markers = "[]";

const char * near TDirListBox::pathDir   = "���";
const char * near TDirListBox::firstDir  =   "���";
const char * near TDirListBox::middleDir =   " ��";
const char * near TDirListBox::lastDir   =   " ��";
const char * near TDirListBox::drives = "��᪨";
const char * near TDirListBox::graphics = "���";

const char * near TFileEditor::backupExt = ".BAK";

