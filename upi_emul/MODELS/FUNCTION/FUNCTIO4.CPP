#include <math.h>
#include <function.hpp>
#include <link.hpp>

LINK_ZVENO(SqrtFunction,"����१��")



float _HUGE SqrtFunction::Calc(float val) { return k*sqrt(val); }
