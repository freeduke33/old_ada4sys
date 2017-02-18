
// fw,fn - диапазон частот
// nq    - число точек характеристики
// S     - массив куда класть
// freq  - частота дискретизации
// lam   - чувствительность
// nr    - длина исходного ряда
// X     - массив исходной реализации


int CalcSpectr(float fn,float fw,int nq,double *S,double *Axis,
	       float freq,float lam,long nr,float *X);
