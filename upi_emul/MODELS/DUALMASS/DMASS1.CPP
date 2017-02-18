#include <dualmass.hpp>

float _HUGE DualMass::Calc(float wy1)
{
  X[2]=X[1]; X[1]=wy1;

  Psi[0]=Et[0]*Psi[1]-Et[1]*Psi[2]+X[1]-X[2];
  wy1=Lam[0]*Y[1]-Lam[1]*Y[2]+E[0]*Psi[0]-E[1]*Psi[1]+E[2]*Psi[2];

  Psi[2]=Psi[1]; Psi[1]=Psi[0];
  Y[2]=Y[1]; Y[1]=wy1;

  return wy1;
}