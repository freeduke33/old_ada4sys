#ifdef Uses_ExtTrendARSS_1x1

class TemplateARSS_1x1;

class ExtTrendARSS_1x1
{
 public:
   ExtTrendARSS_1x1(void) { Model=NULL; }

   inline void Connect(TemplateARSS_1x1 *mdl) { Model=mdl; }
   virtual float Calc(int l) =0;

 protected:

  TemplateARSS_1x1 *Model;
};


#endif