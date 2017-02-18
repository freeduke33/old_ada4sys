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


// вычисление времени последнего обращения
void UpdateTIMESTAMP(unsigned long *,int isMax=0);

class tabFile;
// параграф
class tabItem
{
 public:
   tabItem(void);
   virtual ~tabItem(void);

   // выделить область памяти под данные параграфа
   virtual int Init(unsigned long idx,unsigned long size);

   // считать параграф пустым (но не очистить его фактически)
   inline int SetEmpty(void) { if(!changed) empty=1; return empty; }
   // проверить на пустоту (возможность заполнения новыми данными)
   inline int isEmpty(void) { return empty; }

   // вернуть указатель на данные
   inline void* Data(void)          { return Ptr; }
   // вернуть размер данных
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
// файл с набором параграфов
class tabFile
{
 public:
   tabFile(tabIndex *offs);
   virtual ~tabFile(void) {;}

   virtual int isOk(void) =0;

   // прочитать параграф номер idx из файла
   int Read(unsigned long Idx,tabItem *);
   // переписать параграф в файл под номером idx
   int Write(tabItem *);
   // создать пустой параграф
   virtual int CreateBlank(tabItem **);

 private:
   tabIndex *Offset;

   virtual unsigned int sizeData(void) =0;
   virtual int read(tabItem *) =0;

   virtual int seek(unsigned long) =0;
   virtual unsigned long pos(void) =0;
};

// рабочая область
class workArea
{
 public:
   workArea(unsigned int size,tabFile *tfile);
  ~workArea(void);

  // взять существующий параграф
  virtual int get(unsigned long idx,tabItem **t);
  // заменить существующий параграф или добавить новый
  virtual int put(tabItem *t);

  // сохранить все измененные параграфы на диске
  int flush(void);

  inline int isOk(void) { return Size!=0 && File && File->isOk(); }

 protected:
  // размер рабочей области
  unsigned int Size;
  // указатель на рабочую область
  tabItem **Data;
  // указатель на файл данных
  tabFile *File;

  // найти пустую или ненужную ячейку в рабочей области
  int getEmpty(unsigned int *);
  // удалить рабочую область
  void dispose(void);
};



#endif
