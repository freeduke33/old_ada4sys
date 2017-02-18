typedef struct {
 int ISO;             /* тип физической величины  */
 float Const,Delt;    /* перевод в физические величины */
 float MaxVol,MinVol; /* разрешенный диапазон */
} Tar;

// Отображение логического датчика на физические ЦАП/АЦП
// и тарировочные коэффициенты
int LoadHardwareMap(int type,int chl,int *slot,int *num,Tar *);
