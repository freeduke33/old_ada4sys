//
// общие функции
//

#ifndef __ADAPTIVE_GAUGES_HPP__
#define __ADAPTIVE_GAUGES_HPP__

// число сочетаний из k элементов по v
inline int CC(int k,int v)
 {
 int i,R=1;
 for(i=1;i<=v;i++)
               R*=(k-i+1)/i;
 return R;
 }


#endif