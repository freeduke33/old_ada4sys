#include <function.hpp>
#include <link.hpp>

LINK_ZVENO(SingleFunction,"Линейное")


float _HUGE SingleFunction::Calc(float val) { return val*k; }
