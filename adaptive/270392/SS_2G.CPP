// СС
// с учетом тренда 1-го порядка
// с тек. сглаживанием
// сдвиг q
// без обратной связи

#define Uses_gss_q1TS270392
#define Uses_Buffer

#include <adaptive.hpp>

float _HUGE gss_q1TS270392::_y_(int m)
{
 return Tpl->get(m-l);
}
