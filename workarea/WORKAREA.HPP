#ifndef __WORKAREA_HPP__
#define __WORKAREA_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// #define LOGGED

#ifdef LOGGED
#include <stdarg.h>
extern FILE *LOGFILE;
void LOG(char *fmt,...);
#else
inline void LOG(char *, ...) {;}
#endif


// ���᫥��� �६��� ��᫥����� ���饭��
void UpdateTIMESTAMP(unsigned long *,int isMax=0);

class tabFile;
// ��ࠣ��
class tabItem
{
 public:
   tabItem(void);
   virtual ~tabItem(void);

   // �뤥���� ������� ����� ��� ����� ��ࠣ��
   virtual int Init(unsigned long idx,unsigned long size);

   // ����� ��ࠣ�� ����� (�� �� ������ ��� 䠪��᪨)
   inline int SetEmpty(void) { if(!changed) empty=1; return empty; }
   // �஢���� �� ������ (����������� ���������� ���묨 ����묨)
   inline int isEmpty(void) { return empty; }

   // ������ 㪠��⥫� �� �����
   inline void* Data(void)          { return Ptr; }
   // ������ ࠧ��� ������
   inline unsigned long Size(void)  { return okSize; }
   inline unsigned long Index(void) { return Idx; }

   unsigned long TIMESTAMP;

 protected:

   void dispose(void);

   unsigned short empty,changed;
   unsigned long okSize,Idx;
   unsigned long ptrSize;
   void *Ptr;

   friend class tabFile;
};

class tabIndex;
// 䠩� � ����஬ ��ࠣ�䮢
class tabFile
{
 public:
   tabFile(tabIndex *offs);
   virtual ~tabFile(void) {;}

   virtual int isOk(void) =0;

   // ������ ��ࠣ�� ����� idx �� 䠩��
   int Read(unsigned long Idx,tabItem *);
   // ��९���� ��ࠣ�� � 䠩� ��� ����஬ idx
   int Write(tabItem *);
   // ᮧ���� ���⮩ ��ࠣ��
   virtual int CreateBlank(tabItem **);

 private:
   tabIndex *Offset;

   virtual unsigned int sizeData(void) =0;
   virtual int read(tabItem *) =0;

   virtual int seek(unsigned long) =0;
   virtual unsigned long pos(void) =0;
};

// ࠡ��� �������
class workArea
{
 public:
   workArea(unsigned int size,tabFile *tfile);
  ~workArea(void);

  // ����� �������騩 ��ࠣ��
  virtual int get(unsigned long idx,tabItem **t);
  // �������� �������騩 ��ࠣ�� ��� �������� ����
  virtual int put(tabItem *t);

  // ��࠭��� �� ��������� ��ࠣ��� �� ��᪥
  int flush(void);

  inline int isOk(void) { return Size!=0 && File && File->isOk(); }

 protected:
  // ࠧ��� ࠡ�祩 ������
  unsigned int Size;
  // 㪠��⥫� �� ࠡ���� �������
  tabItem **Data;
  // 㪠��⥫� �� 䠩� ������
  tabFile *File;

  // ���� ������ ��� ���㦭�� �祩�� � ࠡ�祩 ������
  int getEmpty(unsigned int *);
  // 㤠���� ࠡ���� �������
  void dispose(void);
};



#endif
