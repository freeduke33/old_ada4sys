#include <stdio.h>
#define Uses_delTr1_270395
#include <adaptive.hpp>

float delTr1_270395::calc(float val)
{
 float su;
 unsigned int i;

 // ---------------- (17)

 for(i=su=0;i<Q;i++) su+=(((2.0*Q-1)/3.0)-i)*buf[(cur+Q-i)%Q];

 return val-(6.0*su/((Q+1)*Q));
}

