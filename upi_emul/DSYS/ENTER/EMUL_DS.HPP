#ifndef __EMUL_DS_HPP__
#define __EMUL_DS_HPP__

#ifdef Uses_DefEmul
#  define Uses_TDialog
#endif


#ifdef Uses_TVision
# include <tv.h>
#endif

#ifdef Uses_Const
# include "emul_ds.h"
#endif


#ifdef Uses_MyApp

class far TEvent;
class far TStatusLine;
class far TMenuBar;
class far TRect;
class far DefEmul;

class MyApp:public TApplication
{
 public:
   MyApp(void);

   TApplication::run;

   virtual void handleEvent(TEvent& );

   static TStatusLine *initStatusLine(TRect r);
   static TMenuBar    *initMenuBar(TRect r);

   void ViewEmul(void);

  protected:

   void LoadEmul(void);
   void SaveEmul(Boolean AskFile);

   void InsChannel(void);
   void DelChannel(void);
   void XrefChannel(void);
   void EdtChannel(void*);

   void InsZveno(void);
   void DelZveno(void);
   void DefZveno(void);


   char EmulName[70]; // файл описания эмулятора
   char NikName[11]; // только имя файла описания эмулятора

   DefEmul far *Emul;
};

#endif

#ifdef Uses_DefEmul

#if !defined(__TListBoxRec)
#define __TListBoxRec

class far TCollection;

struct TListBoxRec
{
    TCollection* collection;
    short focused;
};
#endif

class far UPI_dsys;

class DefEmul:public TDialog
{
 public:
   DefEmul(char *fname,char *title);
  ~DefEmul(void);

  virtual void handleEvent(TEvent& );

 // new functions
 virtual void SaveCfg(ostream& os);
 inline int isChanged(void) { return chgFlag; }


 protected:

 UPI_dsys *DSYS;

 int chgFlag;

// virtual void LoadCfg(istream& is);


 struct dataRec  {
  TListBoxRec CHL;   //TListBox
  } D;

};

#endif

#endif
