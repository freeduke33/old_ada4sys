#ifndef __data_glb__
#define __data_glb__

#ifndef Word
#define Word unsigned int
#endif


// ---------------[ ����樮���� ������ ��쥪� ]----------------------
//
struct _FuncModel
{
	int Type;
	float k;   /* ��� ����� , �����樥�� */
};

struct _AperModel
{
	float FV,FN;  /* ������, ������ �࠭�� */
	int p;	      /* ���冷� */
};

struct _KolbModel
{
	float Betta,Frez;  /* �⥯��� �ᯮ������, १����᭠� ���� */
};


extern struct _FuncModel FuncModel[3];
extern struct _AperModel AperModel[3];
extern struct _KolbModel KolbModel[3];
extern int Z_Type[4];
extern float imi_Const;


/*==========================[ ?????????? ]======================*/

extern double dtds;
extern int nti;
extern double pr[3][11],z[3][MI+1];
extern float yst[],yp[];
extern double pkm[EMAX+1][QMAX+10],pk[EMAX+1];
extern double c[PMAX+1],lam[PMAX+1];
extern double xs[PMAX+1],ys[PMAX+1];
extern double beta_kx[EMAX+1][PMAX+1],
	      beta_ky[EMAX+1][PMAX+1];
extern double koef[EMAX+1],
	      dk[QMAX+EMAX+10];


#endif
