//
// 31 января - 2 февраля 1996
//


#ifdef Uses_ant_Model

class ant_Model: public mdl_Anatomic
{
 public:
   // q - величина запаздывания
   // N - момент начала процедуры идентификации
   // Q - (объем исследуемого массива) глубина регуляризации
   ant_Model(int q,unsigned long N,int Q);

 private:

   virtual void store(float x,float y);
   virtual void calc();

};

#endif


