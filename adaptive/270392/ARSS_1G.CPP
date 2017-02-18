// АР-СС
// с учетом тренда 0-го порядка
// с тек. сглаживанием
// сдвиг q
// без обратной связи

#define Uses_garss_qTS270392
#define Uses_Buffer

#include <adaptive.hpp>

#define DMAX DMAX_270392
#define M(i) (((i)+DMAX)%DMAX)
#define C Betta
#define Lam C

float _HUGE garss_qTS270392::calc(void)
{
 int v;
 float su,su1;

//
// -------------------------------- (38) ?
// k+1 --> l

 for(v=1,su=0;v<=p;v++) su+=Lam[v]*X[M(l+1-v)];
 for(v=su1=0;v<=p;v++)  su1+=C[v]*Tpl->get(1+q-v);

 return Result(A+su+su1);
}
