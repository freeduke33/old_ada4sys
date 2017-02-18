#define DATA    0x00
#define COMMAND 0x08
#define CONTROL 0x0C

typedef enum { lc_NONE,
       lc_NOBIOSFILE,
       lc_BADBIOSFILE,
       lc_NOTAIL,
       lc_NORESPONSE,
       lc_TIMEOUT,
       lc_NOTSUPPORT,
       lc_UNKNOWN
        } lcRet;


lcRet LoadBios(int,char *);
lcRet TestBios(void);
lcRet Enable(unsigned Hi,unsigned Lo);
lcRet fEnable(float Freq);
lcRet Disable(void);
typedef void interrupt (* intrProc)(void);
lcRet SetHook(intrProc );
lcRet ClrHook(void);



lcRet putCmd(int,unsigned );
lcRet putData(int,unsigned );
lcRet getData(int,unsigned *);

/*
#define inport(a)       inp(a)
#define outport(a,b)    outp(a,b)
#define getvect(a)      _dos_getvect(a)
#define setvect(a,b)    _dos_setvect(a,b)
  */
