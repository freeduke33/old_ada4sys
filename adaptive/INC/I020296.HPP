//
// 31 ﭢ��� - 2 䥢ࠫ� 1996
//


#ifdef Uses_ant_Model

class ant_Model: public mdl_Anatomic
{
 public:
   // q - ����稭� ������뢠���
   // N - ������ ��砫� ��楤��� �����䨪�樨
   // Q - (��ꥬ ��᫥�㥬��� ���ᨢ�) ��㡨�� ॣ��ਧ�樨
   ant_Model(int q,unsigned long N,int Q);

 private:

   virtual void store(float x,float y);
   virtual void calc();

};

#endif


