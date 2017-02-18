#define DEBUG

#define Uses_mdl_1TR031095
#include <adaptive.hpp>

#define model mdl_1TR031095
#define DMAX DMAX_031095

#define M(a) ((DMAX+(a))%DMAX)

void model::params(void)
{
 double su;
 int k;

 switch(Typ)
   {
   case 0: X[l]=x[0]; break;
   case 1: X[l]=x[0]-x[1]; x[1]=x[0]; break;
   case 2: X[l]=x[0]-2*x[1]+x[2]; x[2]=x[1]; x[1]=x[0]; break;
   }

   // ----------- (5)
#define calc_1(A,S) \
   for(k=su=0;k<Q;k++) su+=((Q-1)*(2*Q+5)-3*(Q+3)*k)*S[l-k]; \
   A[l]=2*su/(Q*(Q*Q-1));

   // ----------- (6)
#define calc_2(A,S) \
   A[l]=2*A[M(l-1)]-A[M(l-2)]+ \
        2*((Q-1)*(2*Q+5)*S[l]-(Q+1)*(2*Q+4)*S[M(l-1)]+ \
        (Q+1)*(Q+5)*S[M(l-Q)]-(Q-1)*(Q+4)*S[M(l-1-Q)])/(Q*(Q*Q-1))

   calc_1(Ax,X);
   calc_1(Ay,Y);

//   calc_2(Ax,X);
//   calc_2(Ay,Y);

 return;
}

