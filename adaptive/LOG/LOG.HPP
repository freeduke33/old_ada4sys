#ifndef __LOG_HPP__
#define __LOG_HPP__



class Log
{
 public:
   Log();

   // уровень вывода информации
   void setLevel(const char *level);
   // размеры журнала (минимальный и максимальный)
   // сразу приводит журнал к указанным размерам
   void setPackSize(const unsigned long init,const unsigned long repack);


   // вывод данных в журнал
   void put(char level,const char *fmt, ...);

   // необходимость упаковки журнала
   virtual int isNeedPack(void) { return 0;}
   // упаковать журнал (может потребовать значительного времени)
   virtual void Pack(void) {;}

 protected:
   // фактическая запись данных в журнал
   virtual void writeLog(const char *str)=0;

   char Level[100],buf[1024*10];
   unsigned long Ini,Repack;
};


class Log2TxtFile:public Log
{
 public:
    Log2TxtFile(const char *name);
   ~Log2TxtFile();

   virtual int isNeedPack(void);
   virtual void Pack(void);

 protected:
   virtual void writeLog(const char *str);

   int f;
};

extern Log *LOG;

#endif