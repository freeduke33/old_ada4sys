//
// ��騥 �㭪樨
//

#ifndef __ADAPTIVE_GAUGES_HPP__
#define __ADAPTIVE_GAUGES_HPP__

// �᫮ ��⠭�� �� k ������⮢ �� v
inline int CC(int k,int v)
 {
 int i,R=1;
 for(i=1;i<=v;i++)
               R*=(k-i+1)/i;
 return R;
 }


#endif