#include <math.h>
#include <function.hpp>
#include <link.hpp>

LINK_ZVENO(SqrtFunction,"Гистерезис")



float _HUGE SqrtFunction::Calc(float val) { return k*sqrt(val); }
