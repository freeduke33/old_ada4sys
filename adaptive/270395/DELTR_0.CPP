#define Uses_delTr0_270395
#include <adaptive.hpp>

float delTr0_270395::calc(float val)
{
 float su;
 unsigned int i;

 // ---------------- (10)

 for(i=su=0;i<Q;i++) su+=buf[i];
 return val-su/Q;
}

