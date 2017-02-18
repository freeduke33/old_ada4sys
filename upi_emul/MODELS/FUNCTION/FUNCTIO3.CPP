#include <function.hpp>
#include <link.hpp>

LINK_ZVENO(FherdFunction,"Кубическое")



float _HUGE FherdFunction::Calc(float val) { return k*val*val*val; }
