// СС
// с учетом тренда 0-го порядка
// с тек. сглаживанием
// сдвиг q
// без обратной связи

#define Uses_Buffer
#define Uses_gss_qTS270392

#include <adaptive.hpp>

#define C Betta
#define Lam C

float _HUGE gss_qTS270392::calc(void)
{
 float su;
 int v;

//
// ------------------------- (58) ?
//

 for(v=su=0;v<=p;v++) su+=C[v]*Tpl->get(1+q-v);

 return Result(A+su);
}
