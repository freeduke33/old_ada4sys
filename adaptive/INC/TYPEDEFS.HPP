typedef unsigned char Byte;
typedef unsigned int Word;
typedef unsigned long DWord;


#ifdef __WATCOMC__
# define FPE_INTOVFLOW  FPE_IOVERFLOW
# define FPE_INTDIV0    0
# define FPE_STACKFAULT FPE_STACKOVERFLOW
# define M_PI 3.1416
#endif
