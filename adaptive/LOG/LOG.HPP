#ifndef __LOG_HPP__
#define __LOG_HPP__



class Log
{
 public:
   Log();

   // �஢��� �뢮�� ���ଠ樨
   void setLevel(const char *level);
   // ࠧ���� ��ୠ�� (��������� � ���ᨬ����)
   // �ࠧ� �ਢ���� ��ୠ� � 㪠����� ࠧ��ࠬ
   void setPackSize(const unsigned long init,const unsigned long repack);


   // �뢮� ������ � ��ୠ�
   void put(char level,const char *fmt, ...);

   // ����室������ 㯠����� ��ୠ��
   virtual int isNeedPack(void) { return 0;}
   // 㯠������ ��ୠ� (����� ���ॡ����� ����⥫쭮�� �६���)
   virtual void Pack(void) {;}

 protected:
   // 䠪��᪠� ������ ������ � ��ୠ�
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