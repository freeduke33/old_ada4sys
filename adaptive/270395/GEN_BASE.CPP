#define Uses_gen_270395
#define Uses_Buffer

#include <adaptive.hpp>
#define DMAX DMAX_270395
#define M(a) (((a)+DMAX)%DMAX)


float gen_270395::_y_(int m)
{
 return Tpl->get(m-l);
}

