#include <function.hpp>
#include <link.hpp>

LINK_ZVENO(DoubleFunction,"Квадратическое")

float _HUGE DoubleFunction::Calc(float val) { return k*val*val; }
