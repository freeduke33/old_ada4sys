//
//  ��᫥������� ��쥪� �ࠢ����� � "�஧�筮�" ०���
//             (��� ᯥ�. �������⢨�)
//

#ifdef Uses_mdl_Anatomy

class mdl_Anatomy_1x1: public Container_1x1
{
 public:
   mdl_Anatomy_1x1(FModel_1x1& mdl):Container_1x1(mdl) { ; }

   virtual float _HUGE Calc(float y);
   virtual void _HUGE Append(float x,float y);
   virtual int isSuccessful(char *msg);

 protected:

  virtual void store(float x,float y) =0;
  virtual void calc(void) =0;
};

#endif

