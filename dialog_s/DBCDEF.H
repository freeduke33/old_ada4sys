/***************************************************************
 * Copyright (C) 1984, 1985   -   All Rights Reserved          *
 *                                                             *
 * Lattice, Inc.                                               *
 * Glen Ellyn, Illinois                                        *
 * U. S. A.                                                    *
 *                                                             *
 * Author: Billy Shakespeare & Company                         *
 ***************************************************************/
#ifndef _DBC_H
#include <dbc.h>
#endif

#ifndef _DBCDEF_H
/****************************
*  dbcdef.h (dBASE3 Option) *
*****************************/

/*****************************************************/
/* data file(.DBF) definition structure */
/*****************************************************/
typedef struct {
        int      _dbffn;    /* file number */
        BYTE     _dbfmode;  /* mode of open file */
        RECNUM   _alcnum;   /* number of records allocated */
	int      _dbfupd;   /* data file update indicator */
	RECLEN   _len;	    /* record length */

		/*** I/O buffer information ***/
	unsigned _bufsize;  /* I/O buffer size */
        char     *_bfptr;   /* I/O buffer location pointer */
	unsigned _maxnum;   /* maximum number of records in I/O buffer */
        RECNUM   _first;    /* number of first record in buffer */
	unsigned _bfhas;    /* number of records currently contained
			       in buffer */
	BYTE     _month;    /* file update date: month */
	BYTE     _day;      /* 			 day   */
	BYTE     _year;	    /*			 year  */
	FLDNUM   _nflds;    /* number of fields per record */
	dBFIELD  *_flds;    /* pointer to array of dBASE field
			       structure(dBFIELD) */
	unsigned _begdata;  /* beginning-of-data offset */
} DBFFILE;

#define NEXT     0  /* read next records */
#define PREVIOUS 1  /* read previous records */

typedef long      BLKNO; /* block number type */

/*****************************************/
/* index file(.NDX) definition structure */
/*****************************************/
#define VECSIZE 32
typedef struct {
        int      _ndxfn;    /* file number */
        BYTE     _ndxmode;  /* mode of open file */
	BLKNO    _orootbk;  /* original root block number */
	BLKNO    _oeofbk;   /* original end-of-file block number */
        BLKNO    _rootblk;  /* current root block number */
	BLKNO    _eofblk;   /* current end-of-file block number */
	KEYLEN   _keylen;   /* key length */
	KEYLEN   _kptrlen;  /* length of key and pointer (_keylen + ptr len) */
	BYTE     _maxknum;  /* maximum number of keys in a block */
	char	 _keytype;  /* key type: 'Character', 'Numeric' or 'Date' */

		/*** used for sequential accessing ***/
	struct _seqacc {
		 BLKNO  _nxblkno;   /* block number */
		 BYTE   _nxidno;    /* index id number */
		 char   *_nxbfptr;  /* .NDX I/O buffer pointer */
                 BYTE   _nxusage;   /* buffer usage indicator */
	} _svector[VECSIZE],  /* sequential access vector */
	  *_svptr, *_endofsv; /* pointers to sequential access vector */

} NDXFILE;
/* top-bottom bit definitions */
#define TOP    0x80
#define BOTTOM 0x40
#define TBMASK 0x3f

typedef struct {
        int      _dbtfn;    /* file number */
        BYTE     _dbtmode;  /* mode of open file */
	BLKNO    _odbteof;  /* original end-of-file block number */
	BLKNO    _dbteof;   /* current end-of-file block number */
} DBTFILE;

#define DBTBLKN  8
#define DBTBLKSZ 4608

/* constant definitions */
typedef long      NBYTES;
#define SKIPBYTES 4
#define LSKIP     8
#define PLUSKEY   4
#define LBYTES    8
#define ADDNUM    4

/* symbolic constant definitions used for
   the "char _*mode" element in DBFFILE and NDXFILE */
#define _CLOSED  0x00	/* file closed */
#define _READ	 0x01	/* file open for reading */
#define _APPEND  0x02	/* file open for appending */
#define _INSERT  0x04	/* file open for inserting */
#define _UPDATE  0x08	/* file open for updating */
#define _REMOVE  0x10   /* file open for removing */
#define _WRITE   0x1e   /* .NDX buffer used for 'write' */
/* combination of "char _*mode" bit patters */
#define _AUR	 0x1a	/* file open for append/update/remove */
#define _IUR	 0x1c	/* file open for insert/update/remove */
#define _AI	 0x06	/* file open for append/insert */
#define _UR	 0x18	/* file open for update/remove */
#define _AIR	 0x16	/* file open for append/insert/remove */
#define _RAIU    0x0f   /* file open for read/append/insert/update */
#define _AIU	 0x0e	/* file open for append/insert/update */
#define _AIUR	 0x1e   /* file open for append/insert/update/remove */

#ifndef NULL
# define NULL 0L
#endif

#define UBUFSIZ 16384  /* ultra buffer size */
#define JBUFSIZ  8192  /* jumbo buffer size */
#define BBUFSIZ  4096  /* big buffer size */
#define LBUFSIZ  2048  /* large buffer size */
#define DBUFSIZ  1024  /* double buffer size */
#define BUFSIZ    512  /* standard buffer size */
#define HBUFSIZ   256  /* half buffer size */
#define QBUFSIZ   128  /* quarter buffer size */
#define SBUFSIZ    64  /* small buffer size */
#define IBUFSIZ  4200  /* initial buffer size (to store up to 128 fields) */

/* .DBF file offset definitions */
#define FILETYPE 0 /* file type */
#define		DBF  0x03 /* .DBF file type 1 */
#define         DBFM 0x83 /* .DBF file type 2 */
#define YEAR  1  /* year */
#define MONTH 2  /* month */
#define DAY   3  /* day */
#define RNUM0 4  /* number of records allocated < 1st byte > */
#define RNUM1 5  /* number of records allocated < 2nd byte > */
#define RNUM2 6  /* number of records allocated < 3rd byte > */
#define RNUM3 7  /* number of records allocated < 4th byte > */
#define FREC0 8  /* address of first record */
#define FREC1 9  /* address of second record */
#define RLEN0 0x0a /* record length < 1st byte > */
#define RLEN1 0x0b /* record length < 2nd byte > */
#define Z0C   0x0c /* zero byte BEGIN */
#define Z1F   0x1f /* zero byte END   */
#define FSTFIELD 0x20 /* first field offset */
#define TYPE  0xb /* type offset ('C', 'N', 'L', 'M' or 'D'): */
		  /* added to field offset */
#define 	UU0C     0xc  /* unused byte BEGIN */
#define 	UU0F     0xf  /* unused byte END */
#define 	WIDTH    0x10 /* width offset: added to field offset */
#define 	DECPLACE 0x11 /* decimal point offset: added to field offset */
#define         FZ12     0x12 /* field zero byte BEGIN */
#define         FZ1F     0x1f /* field zero byte END */
#define NXTFIELD 0x20 /* add NXTFIELD to get next field offset */
#define BEGINDATA ((dbfptr)->_begdata)

/* maximum number definitions */
#define MAXFLDS    128     /* maximum number of fields */
#define MAXRECLEN 4000     /* maximum record length */

/* special character definitions in data file */
#define EOFIELD  0x0d  /* end of field marker in data file */
#define EODATA	 0x1a  /* end of data marker in data file */
#define EOBLKID  -1    /* end of block-id marker in seq. vector */


/* calling sequence defition to _update() function */
#define UPDATE  1
#define DELETE  2
#define RECALL  3

/***** definitions for index file *****/

#define NDXHDRSZ 0x200	/* index(.NDX) file header size */

/* .NDX file offset definitions */
#define ROOTBLK   0     /* root block number 4 bytes BEGIN */
#define EOFBLK    4     /* eof block number 4 bytes BEGIN */
#define NDXZ4     8     /* zero bytes (4 bytes) BEGIN */
#define AKEYLEN0  0x0c  /* key length (1st byte) */
#define AKEYLEN1  0x0d  /* key length (2nd byte) */
#define MAXKEY0   0x0e  /* maximum number of keys per block (1st byte) */
#define MAXKEY1   0x0f  /* maximum number of keys per block (2nd byte) */
#define KEYTYPE   0x10  /* key type */
#define        CTYPE    0         /* character type */
#define        NOTCTYPE 1         /* numeric or date type */
#define NDXZ11    0x11  /* zero byte */
#define KPTRLEN0  0x12  /* key+ptr length (usually (AKEYLEN*) + 8 */
#define NDXZ13    0x13  /* zero bytes (5 bytes) BEGIN */
#define KEYEXPR   0x18  /* key expression (terminated with 0) BEGIN */

#define BLKSIZ    512 /* full block size */
#define DBLKSIZ  1024 /* double block size */

/* return code from _comparek() */
#define LLTR 0	/* left key < right key */
#define LEQR 1	/* left key = right key */
#define LGTR 2	/* left key > right key */

/* global data definitions */
#define NDBFFILE 10 /* max. number of .DBF files open */
#define NNDXFILE 10 /* max. number of .NDX files open */
#define NDBTFILE 10 /* max. number of .DBT files open */


#ifdef LATTICE
/*** macros for LATTICE environment ***/

#define READDATE 0x2a00 /* DOS function specification */
			/* used by "dBcreat" and "dBclose" to read date */
#define AX reg.x.ax
#define CX reg.x.cx
#define DX reg.x.dx
#endif /* match -> #ifdef LATTICE */  

#ifdef C86
/*** macros for C86 environment ***/

#define READDATE 0x2a00 /* DOS function specification */
			/* used by "dBcreat" and "dBclose" to read date */
#define getmem malloc
#define rlsmem(p,x) free(p)
#define AX reg.ax
#define CX reg.cx
#define DX reg.dx
#define intdos sysint21
#endif /* match -> #ifdef C86 */  

#ifdef DeSmet
/*** macros for DeSmet C environment ***/

#define getmem malloc
#define rlsmem(p,x) free(p)

#endif /* match -> #ifdef DeSmet */  

#ifdef MSC 
/*** macros for Microsoft C environment ***/

#define READDATE 0x2a00 /* DOS function specification */
			/* used by "dBcreat" and "dBclose" to read date */
#define AX reg.x.ax
#define CX reg.x.cx
#define DX reg.x.dx
#define getmem malloc
#define rlsmem(p,x) free(p)

#endif /* match -> #ifdef MSC */  

#endif /* match -> #ifndef _DBCDEF_H */

#define _DBCDEF_H
